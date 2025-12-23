package fractals.renderers;

import fractals.core.Fractal;
import com.jogamp.opengl.GL2;

/**
 * Множество Жюлиа
 * f(z) = z² + c, где c - константа
 *
 * Связано с Мандельбротом: каждая точка Мандельброта
 * соответствует своему множеству Жюлиа
 */
public class JuliaSet extends Fractal {
    private double cReal; // Действительная часть константы c
    private double cImag; // Мнимая часть константы c

    public JuliaSet() {
        super("Julia Set");
        this.iterations = 256;
        this.zoom = 0.5;
        this.cReal = -0.7;
        this.cImag = 0.27015;
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        gl.glBegin(GL2.GL_POINTS);

        int step = zoom > 10 ? 1 : (zoom > 1 ? 2 : 3);

        for (int screenY = 0; screenY < height; screenY += step) {
            for (int screenX = 0; screenX < width; screenX += step) {
                double zReal = screenToFractalX(screenX, width);
                double zImag = screenToFractalY(screenY, height);

                int iter = computeJulia(zReal, zImag);
                int[] color = getColor(iter, iterations);

                gl.glColor3ub((byte) color[0], (byte) color[1], (byte) color[2]);

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

    private int computeJulia(double zReal, double zImag) {
        double zr = zReal;
        double zi = zImag;
        int iteration = 0;

        while (zr * zr + zi * zi <= 4.0 && iteration < iterations) {
            double temp = zr * zr - zi * zi + cReal;
            zi = 2.0 * zr * zi + cImag;
            zr = temp;
            iteration++;
        }

        return iteration;
    }

    @Override
    public void reset() {
        this.iterations = 256;
        this.zoom = 0.5;
        this.offsetX = 0.0;
        this.offsetY = 0.0;
        this.cReal = -0.7;
        this.cImag = 0.27015;
        this.needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format(
            "%s\nIterations: %d\nZoom: %.2e\nc = %.6f + %.6fi",
            name, iterations, zoom, cReal, cImag
        );
    }

    // Управление параметром c
    public void setC(double real, double imag) {
        this.cReal = real;
        this.cImag = imag;
        this.needsUpdate = true;
    }

    public double getCReal() {
        return cReal;
    }

    public double getCImag() {
        return cImag;
    }

    /**
     * Предустановленные красивые формы Жюлиа
     */
    public void loadPreset(int index) {
        switch (index) {
            case 0: // Дендриты
                cReal = -0.7; cImag = 0.27015;
                break;
            case 1: // Кролик Дуади
                cReal = -0.123; cImag = 0.745;
                break;
            case 2: // Дракон
                cReal = -0.8; cImag = 0.156;
                break;
            case 3: // Спираль
                cReal = 0.285; cImag = 0.01;
                break;
            case 4: // Цветок
                cReal = -0.4; cImag = 0.6;
                break;
            case 5: // Молния
                cReal = -0.70176; cImag = -0.3842;
                break;
        }
        needsUpdate = true;
    }
}
