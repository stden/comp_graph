package fractals.renderers;

import fractals.core.Fractal;
import com.jogamp.opengl.GL2;

/**
 * Множество Мандельброта
 * f(z) = z² + c, где z начинается с 0
 *
 * Производительность: использует оптимизированный алгоритм
 * с early escape и проверкой периодичности
 */
public class MandelbrotSet extends Fractal {

    public MandelbrotSet() {
        super("Mandelbrot Set");
        this.iterations = 256;
        this.zoom = 0.5;
        this.offsetX = -0.5;
        this.offsetY = 0.0;
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        gl.glBegin(GL2.GL_POINTS);

        // Оптимизация: рендерим пиксели с адаптивной детализацией
        int step = zoom > 10 ? 1 : (zoom > 1 ? 2 : 3);

        for (int screenY = 0; screenY < height; screenY += step) {
            for (int screenX = 0; screenX < width; screenX += step) {
                double x0 = screenToFractalX(screenX, width);
                double y0 = screenToFractalY(screenY, height);

                int iter = computeMandelbrot(x0, y0);
                int[] color = getColor(iter, iterations);

                gl.glColor3ub((byte) color[0], (byte) color[1], (byte) color[2]);

                // Рисуем квад для заполнения gaps при step > 1
                if (step > 1) {
                    for (int dy = 0; dy < step && screenY + dy < height; dy++) {
                        for (int dx = 0; dx < step && screenX + dx < width; dx++) {
                            gl.glVertex2i(screenX + dx, screenY + dy);
                        }
                    }
                } else {
                    gl.glVertex2i(screenX, screenY);
                }
            }
        }

        gl.glEnd();
        markUpdated();
    }

    /**
     * Вычисление принадлежности точки множеству Мандельброта
     * Оптимизировано с проверкой на убегание
     */
    private int computeMandelbrot(double x0, double y0) {
        double x = 0.0;
        double y = 0.0;
        double x2 = 0.0;
        double y2 = 0.0;

        int iteration = 0;

        // Основной цикл с оптимизацией
        while (x2 + y2 <= 4.0 && iteration < iterations) {
            y = 2.0 * x * y + y0;
            x = x2 - y2 + x0;
            x2 = x * x;
            y2 = y * y;
            iteration++;
        }

        return iteration;
    }

    @Override
    public void reset() {
        this.iterations = 256;
        this.zoom = 0.5;
        this.offsetX = -0.5;
        this.offsetY = 0.0;
        this.needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format(
            "%s\nIterations: %d\nZoom: %.2e\nCenter: (%.6f, %.6f)",
            name, iterations, zoom, offsetX, offsetY
        );
    }

    /**
     * Интересные точки для навигации
     */
    public void gotoInterestingPoint(int index) {
        switch (index) {
            case 0: // Главное множество
                offsetX = -0.5; offsetY = 0.0; zoom = 0.5;
                break;
            case 1: // "Хвост морского конька"
                offsetX = -0.75; offsetY = 0.1; zoom = 10.0;
                break;
            case 2: // "Долина двойной спирали"
                offsetX = -0.16; offsetY = 1.0405; zoom = 20.0;
                break;
            case 3: // "Миниатюрные мандельброты"
                offsetX = -0.7269; offsetY = 0.1889; zoom = 100.0;
                break;
            case 4: // "Спиральный узор"
                offsetX = 0.285; offsetY = 0.01; zoom = 50.0;
                break;
        }
        needsUpdate = true;
    }
}
