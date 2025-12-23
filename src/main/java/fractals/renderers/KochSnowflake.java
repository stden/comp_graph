package fractals.renderers;

import fractals.core.Fractal;
import com.jogamp.opengl.GL2;
import java.util.ArrayList;
import java.util.List;

/**
 * Кривая Коха (Снежинка Коха)
 * Классический фрактал на основе рекурсивного деления отрезков
 */
public class KochSnowflake extends Fractal {
    private int recursionLevel;
    private List<double[]> points;

    public KochSnowflake() {
        super("Koch Snowflake");
        this.recursionLevel = 5;
        this.zoom = 1.5;
        this.points = new ArrayList<>();
    }

    @Override
    public void render(GL2 gl, int width, int height) {
        if (needsUpdate) {
            generateSnowflake();
        }

        gl.glMatrixMode(GL2.GL_PROJECTION);
        gl.glLoadIdentity();
        double aspect = (double) width / height;
        gl.glOrtho(-aspect * zoom, aspect * zoom, -zoom, zoom, -1, 1);

        gl.glMatrixMode(GL2.GL_MODELVIEW);
        gl.glLoadIdentity();

        // Рисуем снежинку
        gl.glColor3d(0.2, 0.6, 1.0);
        gl.glLineWidth(1.5f);
        gl.glBegin(GL2.GL_LINE_STRIP);
        for (double[] point : points) {
            gl.glVertex2d(point[0], point[1]);
        }
        gl.glEnd();

        markUpdated();
    }

    private void generateSnowflake() {
        points.clear();

        // Начальный треугольник
        double size = 1.0;
        double h = size * Math.sqrt(3) / 2;

        double[] p1 = {0, h * 2/3};
        double[] p2 = {-size/2, -h/3};
        double[] p3 = {size/2, -h/3};

        // Три стороны снежинки
        kochCurve(p1, p2, recursionLevel);
        kochCurve(p2, p3, recursionLevel);
        kochCurve(p3, p1, recursionLevel);

        // Замыкаем
        if (!points.isEmpty()) {
            points.add(points.get(0));
        }
    }

    private void kochCurve(double[] start, double[] end, int level) {
        points.add(start);

        if (level == 0) {
            return;
        }

        // Делим отрезок на 3 части
        double dx = (end[0] - start[0]) / 3;
        double dy = (end[1] - start[1]) / 3;

        double[] p1 = {start[0] + dx, start[1] + dy};
        double[] p2 = {start[0] + 2*dx, start[1] + 2*dy};

        // Вершина треугольника
        double angle = Math.PI / 3;
        double cos = Math.cos(angle);
        double sin = Math.sin(angle);

        double px = p1[0] - start[0];
        double py = p1[1] - start[1];

        double[] peak = {
            start[0] + px * cos - py * sin + dx,
            start[1] + px * sin + py * cos + dy
        };

        // Рекурсивные вызовы
        kochCurve(start, p1, level - 1);
        kochCurve(p1, peak, level - 1);
        kochCurve(peak, p2, level - 1);
        kochCurve(p2, end, level - 1);
    }

    @Override
    public void reset() {
        this.recursionLevel = 5;
        this.zoom = 1.5;
        this.needsUpdate = true;
    }

    @Override
    public String getInfo() {
        return String.format("%s\nLevel: %d\nSegments: %d",
            name, recursionLevel, points.size());
    }

    public void setLevel(int level) {
        this.recursionLevel = Math.max(0, Math.min(7, level));
        this.needsUpdate = true;
    }
}
