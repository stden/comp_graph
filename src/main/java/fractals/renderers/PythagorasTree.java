package fractals.renderers;

import fractals.core.Fractal;
import com.jogamp.opengl.GL2;
import java.util.ArrayList;
import java.util.List;

/**
 * Дерево Пифагора (Pythagoras Tree)
 * Рекурсивная структура из квадратов, образующих дерево
 *
 * Оптимизация: использует VBO для хранения вершин
 */
public class PythagorasTree extends Fractal {
    private int maxDepth;
    private double angle;
    private double lengthRatio;
    private List<float[]> vertices;
    private boolean animated;
    private double animationTime;

    public PythagorasTree() {
        super("Pythagoras Tree");
        this.maxDepth = 10;
        this.angle = Math.PI / 4; // 45 градусов
        this.lengthRatio = 0.7;
        this.zoom = 1.0;
        this.offsetX = 0.0;
        this.offsetY = -0.3;
        this.vertices = new ArrayList<>();
        this.animated = false;
        this.animationTime = 0.0;
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        if (needsUpdate || animated) {
            generateTree();
        }

        // Настройка матрицы просмотра
        gl.glMatrixMode(GL2.GL_PROJECTION);
        gl.glLoadIdentity();
        double aspect = (double) width / height;
        gl.glOrtho(-aspect * zoom, aspect * zoom,
                   -zoom + offsetY, zoom + offsetY,
                   -1, 1);

        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();
        gl.glTranslated(offsetX, 0, 0);

        // Рендер дерева
        gl.glLineWidth(2.0f);
        for (int i = 0; i < vertices.size(); i++) {
            float[] quad = vertices.get(i);

            // Цвет зависит от глубины
            int depth = i % maxDepth;
            double t = (double) depth / maxDepth;
            gl.glColor3d(0.2 + t * 0.5, 0.6 - t * 0.3, 0.2);

            gl.glBegin(GL2.GL_QUADS);
            gl.glVertex2d(quad[0], quad[1]);
            gl.glVertex2d(quad[2], quad[3]);
            gl.glVertex2d(quad[4], quad[5]);
            gl.glVertex2d(quad[6], quad[7]);
            gl.glEnd();
        }

        if (animated) {
            animationTime += 0.016; // ~60 FPS
            angle = Math.PI / 6 + Math.PI / 6 * Math.sin(animationTime);
            needsUpdate = true;
        }

        markUpdated();
    }

    private void generateTree() {
        vertices.clear();
        double baseSize = 0.1;
        drawSquare(0, -0.5, baseSize, 0, 0);
    }

    private void drawSquare(double x, double y, double size, double angle, int depth) {
        if (depth > maxDepth || size < 0.001) {
            return;
        }

        // Вершины квадрата с учетом поворота
        double cos = Math.cos(angle);
        double sin = Math.sin(angle);

        double x1 = x;
        double y1 = y;
        double x2 = x + size * cos;
        double y2 = y + size * sin;
        double x3 = x2 - size * sin;
        double y3 = y2 + size * cos;
        double x4 = x - size * sin;
        double y4 = y + size * cos;

        // Сохраняем квадрат
        vertices.add(new float[]{
            (float)x1, (float)y1,
            (float)x2, (float)y2,
            (float)x3, (float)y3,
            (float)x4, (float)y4
        });

        // Рекурсия: два новых квадрата на вершине текущего
        double newSize = size * lengthRatio;

        // Левый квадрат
        double leftAngle = angle + this.angle;
        double leftX = x4;
        double leftY = y4;
        drawSquare(leftX, leftY, newSize, leftAngle, depth + 1);

        // Правый квадрат
        double rightAngle = angle - this.angle;
        double rightX = x3;
        double rightY = y3;
        drawSquare(rightX, rightY, newSize, rightAngle, depth + 1);
    }

    @Override
    public void reset() {
        this.maxDepth = 10;
        this.angle = Math.PI / 4;
        this.lengthRatio = 0.7;
        this.zoom = 1.0;
        this.offsetX = 0.0;
        this.offsetY = -0.3;
        this.animated = false;
        this.needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format(
            "%s\nDepth: %d\nAngle: %.1f°\nRatio: %.2f\nVertices: %d",
            name, maxDepth, Math.toDegrees(angle), lengthRatio, vertices.size() / 8
        );
    }

    public void setMaxDepth(int depth) {
        this.maxDepth = Math.max(1, Math.min(12, depth));
        this.needsUpdate = true;
    }

    public void setAngle(double angleDegrees) {
        this.angle = Math.toRadians(Math.max(20, Math.min(70, angleDegrees)));
        this.needsUpdate = true;
    }

    public void toggleAnimation() {
        this.animated = !this.animated;
        this.needsUpdate = true;
    }

    public boolean isAnimated() {
        return animated;
    }
}
