package fractals.core;

import com.jogamp.opengl.GL2;

/**
 * Базовый абстрактный класс для всех фракталов
 * Обеспечивает единый интерфейс для отрисовки и управления
 */
public abstract class Fractal {
    protected String name;
    protected int iterations;
    protected double zoom;
    protected double offsetX;
    protected double offsetY;
    protected boolean needsUpdate;

    public Fractal(String name) {
        this.name = name;
        this.iterations = 100;
        this.zoom = 1.0;
        this.offsetX = 0.0;
        this.offsetY = 0.0;
        this.needsUpdate = true;
    }

    /**
     * Основной метод отрисовки фрактала
     */
    public abstract void render(GL2 gl, int width, int height);

    /**
     * Сброс параметров к начальным значениям
     */
    public abstract void reset();

    /**
     * Получить информацию о фрактале для UI
     */
    public abstract String getInfo();

    // Getters and Setters
    public String getName() {
        return name;
    }

    public int getIterations() {
        return iterations;
    }

    public void setIterations(int iterations) {
        this.iterations = Math.max(10, Math.min(1000, iterations));
        this.needsUpdate = true;
    }

    public double getZoom() {
        return zoom;
    }

    public void setZoom(double zoom) {
        this.zoom = Math.max(0.001, Math.min(1000.0, zoom));
        this.needsUpdate = true;
    }

    public void zoom(double factor) {
        setZoom(zoom * factor);
    }

    public double getOffsetX() {
        return offsetX;
    }

    public double getOffsetY() {
        return offsetY;
    }

    public void setOffset(double x, double y) {
        this.offsetX = x;
        this.offsetY = y;
        this.needsUpdate = true;
    }

    public void pan(double dx, double dy) {
        this.offsetX += dx / zoom;
        this.offsetY += dy / zoom;
        this.needsUpdate = true;
    }

    public boolean needsUpdate() {
        return needsUpdate;
    }

    public void markUpdated() {
        needsUpdate = false;
    }

    /**
     * Преобразование координат экрана в координаты фрактала
     */
    protected double screenToFractalX(int screenX, int width) {
        return (screenX - width / 2.0) / (width / 4.0 * zoom) + offsetX;
    }

    protected double screenToFractalY(int screenY, int height) {
        return -(screenY - height / 2.0) / (height / 4.0 * zoom) + offsetY;
    }

    /**
     * Получить цвет для итерации (градиент)
     */
    protected int[] getColor(int iteration, int maxIterations) {
        if (iteration == maxIterations) {
            return new int[]{0, 0, 0}; // Черный для точек множества
        }

        // Плавный градиент
        double t = (double) iteration / maxIterations;

        // HSV to RGB с вращением по цветовому кругу
        double hue = t * 360.0;
        double saturation = 1.0;
        double value = t < 0.5 ? t * 2 : 1.0;

        return hsvToRgb(hue, saturation, value);
    }

    /**
     * Конвертация HSV в RGB
     */
    private int[] hsvToRgb(double h, double s, double v) {
        double c = v * s;
        double x = c * (1 - Math.abs((h / 60.0) % 2 - 1));
        double m = v - c;

        double r = 0, g = 0, b = 0;
        if (h < 60) {
            r = c; g = x;
        } else if (h < 120) {
            r = x; g = c;
        } else if (h < 180) {
            g = c; b = x;
        } else if (h < 240) {
            g = x; b = c;
        } else if (h < 300) {
            r = x; b = c;
        } else {
            r = c; b = x;
        }

        return new int[]{
            (int)((r + m) * 255),
            (int)((g + m) * 255),
            (int)((b + m) * 255)
        };
    }
}
