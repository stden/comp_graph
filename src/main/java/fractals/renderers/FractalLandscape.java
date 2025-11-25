package fractals.renderers;

import com.jogamp.opengl.GL2;
import fractals.core.Fractal;
import java.util.Random;

/**
 * Fractal Landscape - фрактальный ландшафт с использованием алгоритма Midpoint Displacement
 */
public class FractalLandscape extends Fractal {
    private static final int GRID_SIZE = 129; // 2^n + 1 для алгоритма
    private float[][] heightMap;
    private Random random;
    private float roughness = 0.7f;
    private long seed = 12345;
    private boolean needsRegeneration = true;

    public FractalLandscape() {
        super("Fractal Landscape");
        this.zoom = 1.0;
        this.offsetX = 0.0;
        this.offsetY = 0.0;
        this.iterations = 7;
        this.random = new Random(seed);
        this.heightMap = new float[GRID_SIZE][GRID_SIZE];
    }

    public void setRoughness(float r) {
        this.roughness = Math.max(0.1f, Math.min(1.5f, r));
        needsRegeneration = true;
        needsUpdate = true;
    }

    public void setSeed(long s) {
        this.seed = s;
        this.random = new Random(seed);
        needsRegeneration = true;
        needsUpdate = true;
    }

    public void regenerate() {
        needsRegeneration = true;
        needsUpdate = true;
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        if (needsRegeneration) {
            generateLandscape();
            needsRegeneration = false;
        }

        // Рендеринг 3D ландшафта с простой изометрической проекцией
        double scale = 1.5 / zoom;
        double cellSize = scale / GRID_SIZE;

        for (int x = 0; x < GRID_SIZE - 1; x++) {
            for (int z = 0; z < GRID_SIZE - 1; z++) {
                // Координаты четырёх вершин квада
                double x1 = (x - GRID_SIZE / 2.0) * cellSize + offsetX;
                double z1 = (z - GRID_SIZE / 2.0) * cellSize + offsetY;
                double y1 = heightMap[x][z] * scale * 0.3;

                double x2 = (x + 1 - GRID_SIZE / 2.0) * cellSize + offsetX;
                double z2 = (z - GRID_SIZE / 2.0) * cellSize + offsetY;
                double y2 = heightMap[x + 1][z] * scale * 0.3;

                double x3 = (x + 1 - GRID_SIZE / 2.0) * cellSize + offsetX;
                double z3 = (z + 1 - GRID_SIZE / 2.0) * cellSize + offsetY;
                double y3 = heightMap[x + 1][z + 1] * scale * 0.3;

                double x4 = (x - GRID_SIZE / 2.0) * cellSize + offsetX;
                double z4 = (z + 1 - GRID_SIZE / 2.0) * cellSize + offsetY;
                double y4 = heightMap[x][z + 1] * scale * 0.3;

                // Простая изометрическая проекция (вид сверху с высотой)
                double iso_x1 = x1 - z1 * 0.5;
                double iso_y1 = (x1 + z1) * 0.25 + y1;

                double iso_x2 = x2 - z2 * 0.5;
                double iso_y2 = (x2 + z2) * 0.25 + y2;

                double iso_x3 = x3 - z3 * 0.5;
                double iso_y3 = (x3 + z3) * 0.25 + y3;

                double iso_x4 = x4 - z4 * 0.5;
                double iso_y4 = (x4 + z4) * 0.25 + y4;

                // Цвет зависит от высоты (синий-зелёный-коричневый-белый)
                float avgHeight = (heightMap[x][z] + heightMap[x + 1][z] +
                                  heightMap[x + 1][z + 1] + heightMap[x][z + 1]) / 4.0f;

                float[] color = getHeightColor(avgHeight);
                gl.glColor3f(color[0], color[1], color[2]);

                // Рисуем два треугольника
                gl.glBegin(gl.GL_TRIANGLES);
                gl.glVertex2d(iso_x1, iso_y1);
                gl.glVertex2d(iso_x2, iso_y2);
                gl.glVertex2d(iso_x3, iso_y3);

                gl.glVertex2d(iso_x1, iso_y1);
                gl.glVertex2d(iso_x3, iso_y3);
                gl.glVertex2d(iso_x4, iso_y4);
                gl.glEnd();

                // Контур для чёткости
                gl.glColor3f(0.2f, 0.2f, 0.2f);
                gl.glBegin(gl.GL_LINE_LOOP);
                gl.glVertex2d(iso_x1, iso_y1);
                gl.glVertex2d(iso_x2, iso_y2);
                gl.glVertex2d(iso_x3, iso_y3);
                gl.glVertex2d(iso_x4, iso_y4);
                gl.glEnd();
            }
        }

        markUpdated();
    }

    /**
     * Генерация ландшафта с помощью алгоритма Midpoint Displacement
     */
    private void generateLandscape() {
        // Инициализация углов
        heightMap[0][0] = 0.5f;
        heightMap[0][GRID_SIZE - 1] = 0.5f;
        heightMap[GRID_SIZE - 1][0] = 0.5f;
        heightMap[GRID_SIZE - 1][GRID_SIZE - 1] = 0.5f;

        int stepSize = GRID_SIZE - 1;
        float scale = roughness;

        while (stepSize > 1) {
            int halfStep = stepSize / 2;

            // Diamond step
            for (int x = halfStep; x < GRID_SIZE; x += stepSize) {
                for (int y = halfStep; y < GRID_SIZE; y += stepSize) {
                    float avg = (heightMap[x - halfStep][y - halfStep] +
                                heightMap[x - halfStep][y + halfStep] +
                                heightMap[x + halfStep][y - halfStep] +
                                heightMap[x + halfStep][y + halfStep]) / 4.0f;

                    heightMap[x][y] = avg + (random.nextFloat() - 0.5f) * scale;
                }
            }

            // Square step
            for (int x = 0; x < GRID_SIZE; x += halfStep) {
                for (int y = (x + halfStep) % stepSize; y < GRID_SIZE; y += stepSize) {
                    float avg = 0;
                    int count = 0;

                    if (x >= halfStep) { avg += heightMap[x - halfStep][y]; count++; }
                    if (x + halfStep < GRID_SIZE) { avg += heightMap[x + halfStep][y]; count++; }
                    if (y >= halfStep) { avg += heightMap[x][y - halfStep]; count++; }
                    if (y + halfStep < GRID_SIZE) { avg += heightMap[x][y + halfStep]; count++; }

                    heightMap[x][y] = avg / count + (random.nextFloat() - 0.5f) * scale;
                }
            }

            stepSize /= 2;
            scale *= Math.pow(2.0, -roughness);
        }

        // Нормализация высот в диапазон [0, 1]
        float min = Float.MAX_VALUE;
        float max = Float.MIN_VALUE;

        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                if (heightMap[x][y] < min) min = heightMap[x][y];
                if (heightMap[x][y] > max) max = heightMap[x][y];
            }
        }

        float range = max - min;
        if (range > 0) {
            for (int x = 0; x < GRID_SIZE; x++) {
                for (int y = 0; y < GRID_SIZE; y++) {
                    heightMap[x][y] = (heightMap[x][y] - min) / range;
                }
            }
        }
    }

    /**
     * Получить цвет в зависимости от высоты
     */
    private float[] getHeightColor(float height) {
        if (height < 0.3f) {
            // Вода: синий
            return new float[]{0.0f, 0.4f, 0.8f};
        } else if (height < 0.5f) {
            // Песок/берег: жёлтый
            float t = (height - 0.3f) / 0.2f;
            return new float[]{0.8f, 0.7f, 0.3f + t * 0.2f};
        } else if (height < 0.7f) {
            // Трава: зелёный
            float t = (height - 0.5f) / 0.2f;
            return new float[]{0.2f + t * 0.3f, 0.6f - t * 0.1f, 0.2f};
        } else if (height < 0.85f) {
            // Горы: коричневый
            float t = (height - 0.7f) / 0.15f;
            return new float[]{0.5f + t * 0.2f, 0.4f + t * 0.1f, 0.3f};
        } else {
            // Снег: белый
            return new float[]{0.9f, 0.9f, 1.0f};
        }
    }

    @Override
    public void reset() {
        zoom = 1.0;
        offsetX = 0.0;
        offsetY = 0.0;
        roughness = 0.7f;
        seed = 12345;
        random = new Random(seed);
        needsRegeneration = true;
        needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format("Fractal Landscape | Roughness: %.2f | Seed: %d | Zoom: %.2f", roughness, seed, zoom);
    }

    @Override
    public void setIterations(int iter) {
        // Используем итерации для изменения шероховатости
        setRoughness(0.3f + iter * 0.05f);
    }

    @Override
    public int getIterations() {
        return (int)((roughness - 0.3f) / 0.05f);
    }
}
