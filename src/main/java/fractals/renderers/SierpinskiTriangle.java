package fractals.renderers;

import com.jogamp.opengl.GL2;
import fractals.core.Fractal;

/**
 * Sierpinski Triangle - рекурсивный фрактал из треугольников
 * Создаётся путём рекурсивного вычитания центрального треугольника
 */
public class SierpinskiTriangle extends Fractal {
    private int maxDepth = 7;

    public SierpinskiTriangle() {
        super("Sierpinski Triangle");
        this.zoom = 1.0;
        this.offsetX = 0.0;
        this.offsetY = -0.2;
        this.iterations = 7;
    }

    public void setMaxDepth(int depth) {
        this.maxDepth = Math.max(0, Math.min(8, depth));
        needsUpdate = true;
    }

    public int getMaxDepth() {
        return maxDepth;
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        gl.glColor3f(1.0f, 0.4f, 0.0f); // Оранжевый цвет

        // Начальный треугольник (равносторонний)
        double size = 1.5 / zoom;
        double height_ratio = Math.sqrt(3.0) / 2.0;

        double x1 = offsetX - size / 2.0;
        double y1 = offsetY - size * height_ratio / 2.0;

        double x2 = offsetX + size / 2.0;
        double y2 = offsetY - size * height_ratio / 2.0;

        double x3 = offsetX;
        double y3 = offsetY + size * height_ratio / 2.0;

        drawSierpinski(gl, x1, y1, x2, y2, x3, y3, maxDepth);

        markUpdated();
    }

    /**
     * Рекурсивное рисование треугольника Серпинского
     */
    private void drawSierpinski(GL2 gl, double x1, double y1, double x2, double y2, double x3, double y3, int depth) {
        if (depth == 0) {
            // Базовый случай - рисуем заполненный треугольник
            gl.glBegin(gl.GL_TRIANGLES);
            gl.glVertex2d(x1, y1);
            gl.glVertex2d(x2, y2);
            gl.glVertex2d(x3, y3);
            gl.glEnd();
            return;
        }

        // Находим середины сторон
        double mid1x = (x1 + x2) / 2.0;
        double mid1y = (y1 + y2) / 2.0;

        double mid2x = (x2 + x3) / 2.0;
        double mid2y = (y2 + y3) / 2.0;

        double mid3x = (x3 + x1) / 2.0;
        double mid3y = (y3 + y1) / 2.0;

        // Рекурсивно рисуем три меньших треугольника
        drawSierpinski(gl, x1, y1, mid1x, mid1y, mid3x, mid3y, depth - 1);
        drawSierpinski(gl, mid1x, mid1y, x2, y2, mid2x, mid2y, depth - 1);
        drawSierpinski(gl, mid3x, mid3y, mid2x, mid2y, x3, y3, depth - 1);
    }

    @Override
    public void reset() {
        zoom = 1.0;
        offsetX = 0.0;
        offsetY = -0.2;
        maxDepth = 7;
        needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format("Sierpinski Triangle | Depth: %d | Zoom: %.2f", maxDepth, zoom);
    }

    @Override
    public void setIterations(int iter) {
        setMaxDepth(iter);
    }

    @Override
    public int getIterations() {
        return maxDepth;
    }
}
