package fractals.renderers;

import com.jogamp.opengl.GL2;
import fractals.core.Fractal;

/**
 * Lyapunov Fractal - фрактал на основе показателя Ляпунова для логистического отображения
 * Использует последовательность A/B для переключения параметра r
 */
public class LyapunovFractal extends Fractal {
    private String sequence = "AB"; // Последовательность для переключения между rA и rB
    private double minR = 2.0;
    private double maxR = 4.0;
    private int warmupIterations = 100; // Итерации для стабилизации
    private int computeIterations = 100; // Итерации для вычисления показателя

    public LyapunovFractal() {
        super("Lyapunov Fractal");
        this.iterations = 200;
        this.zoom = 1.0;
        this.offsetX = 3.0; // Центр в интересной области
        this.offsetY = 3.0;
    }

    public void setSequence(String seq) {
        if (seq != null && seq.length() > 0) {
            this.sequence = seq.toUpperCase();
            needsUpdate = true;
        }
    }

    public String getSequence() {
        return sequence;
    }

    public void loadPreset(int index) {
        switch (index) {
            case 0:
                sequence = "AB";
                minR = 2.0;
                maxR = 4.0;
                break;
            case 1:
                sequence = "AABB";
                minR = 2.0;
                maxR = 4.0;
                break;
            case 2:
                sequence = "AAAB";
                minR = 2.5;
                maxR = 3.8;
                break;
            case 3:
                sequence = "BBBBBBAAAAAA";
                minR = 2.0;
                maxR = 4.0;
                break;
            case 4:
                sequence = "AABAB";
                minR = 2.0;
                maxR = 4.0;
                break;
        }
        needsUpdate = true;
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        double scale = 2.0 / zoom;
        int step = Math.max(1, (int)(4.0 / zoom)); // Адаптивная детализация

        for (int px = 0; px < width; px += step) {
            for (int py = 0; py < height; py += step) {
                // Преобразование пикселей в параметры rA и rB
                double rA = screenToFractalX(px, width);
                double rB = screenToFractalY(py, height);

                // Ограничиваем диапазон r для логистического отображения
                if (rA < minR || rA > maxR || rB < minR || rB > maxR) {
                    gl.glColor3f(0.0f, 0.0f, 0.0f);
                } else {
                    double lyapunov = computeLyapunov(rA, rB);

                    int[] color = getLyapunovColor(lyapunov);
                    gl.glColor3ub((byte) color[0], (byte) color[1], (byte) color[2]);
                }

                // Рисуем квадрат пикселей
                gl.glBegin(gl.GL_QUADS);
                double x1 = 2.0 * px / width - 1.0;
                double y1 = 1.0 - 2.0 * py / height;
                double x2 = 2.0 * (px + step) / width - 1.0;
                double y2 = 1.0 - 2.0 * (py + step) / height;

                gl.glVertex2d(x1, y1);
                gl.glVertex2d(x2, y1);
                gl.glVertex2d(x2, y2);
                gl.glVertex2d(x1, y2);
                gl.glEnd();
            }
        }

        markUpdated();
    }

    /**
     * Вычисление показателя Ляпунова для заданных rA и rB
     */
    private double computeLyapunov(double rA, double rB) {
        double x = 0.5; // Начальное значение
        double lyapunovSum = 0.0;

        // Warmup - стабилизация орбиты
        for (int i = 0; i < warmupIterations; i++) {
            char c = sequence.charAt(i % sequence.length());
            double r = (c == 'A') ? rA : rB;
            x = r * x * (1.0 - x);

            // Проверка на выход из области определения
            if (x <= 0.0 || x >= 1.0 || Double.isNaN(x)) {
                return Double.NEGATIVE_INFINITY; // Неустойчивая орбита
            }
        }

        // Вычисление показателя Ляпунова
        for (int i = 0; i < computeIterations; i++) {
            char c = sequence.charAt(i % sequence.length());
            double r = (c == 'A') ? rA : rB;

            // Логистическое отображение
            x = r * x * (1.0 - x);

            // Проверка на выход из области
            if (x <= 0.0 || x >= 1.0 || Double.isNaN(x)) {
                return Double.NEGATIVE_INFINITY;
            }

            // Производная логистического отображения: r * (1 - 2x)
            double derivative = Math.abs(r * (1.0 - 2.0 * x));

            if (derivative > 0) {
                lyapunovSum += Math.log(derivative);
            }
        }

        // Усреднённый показатель Ляпунова
        return lyapunovSum / computeIterations;
    }

    /**
     * Цветовая схема для показателя Ляпунова
     * Отрицательные (синий) - устойчивые, положительные (красный) - хаотичные
     */
    private int[] getLyapunovColor(double lyapunov) {
        if (Double.isInfinite(lyapunov) && lyapunov < 0) {
            return new int[]{0, 0, 0}; // Чёрный для неустойчивых орбит
        }

        // Нормализация показателя
        double normalized = Math.max(-1.0, Math.min(1.0, lyapunov));

        if (normalized < 0) {
            // Отрицательный показатель - устойчивость (синий градиент)
            double t = Math.abs(normalized);
            int blue = (int)(50 + t * 205);
            int green = (int)(t * 100);
            return new int[]{0, green, blue};
        } else {
            // Положительный показатель - хаос (красно-жёлтый градиент)
            double t = normalized;
            int red = (int)(200 + t * 55);
            int green = (int)(t * 200);
            return new int[]{red, green, 0};
        }
    }

    @Override
    public void reset() {
        zoom = 1.0;
        offsetX = 3.0;
        offsetY = 3.0;
        sequence = "AB";
        minR = 2.0;
        maxR = 4.0;
        needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format("Lyapunov Fractal | Sequence: %s | Range: [%.1f, %.1f] | Zoom: %.2f",
                sequence, minR, maxR, zoom);
    }

    @Override
    public void setIterations(int iter) {
        this.iterations = Math.max(50, Math.min(500, iter));
        this.warmupIterations = iterations / 2;
        this.computeIterations = iterations / 2;
        needsUpdate = true;
    }
}
