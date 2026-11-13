package graphics;

import java.util.ArrayList;
import java.util.List;

/**
 * Алгоритмы Брезенхема для рисования линий и окружностей
 *
 * Модуль 2: 2D-графика и преобразования
 * - Алгоритм Брезенхема для линий (без использования float)
 * - Алгоритм Брезенхема для окружностей
 */
public class Bresenham {

    /**
     * Точка на растре (пиксель)
     */
    public static class Point {
        public final int x, y;

        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Object obj) {
            if (!(obj instanceof Point)) return false;
            Point p = (Point) obj;
            return x == p.x && y == p.y;
        }

        @Override
        public int hashCode() {
            return (x << 16) | (y & 0xFFFF);
        }

        @Override
        public String toString() {
            return "(" + x + ", " + y + ")";
        }
    }

    /**
     * Алгоритм Брезенхема для рисования линии
     *
     * Преимущества:
     * - Использует только целочисленную арифметику
     * - Быстрее алгоритмов с вещественными числами
     * - Не требует округления
     *
     * @param x0 начальная x координата
     * @param y0 начальная y координата
     * @param x1 конечная x координата
     * @param y1 конечная y координата
     * @return список точек (пикселей), составляющих линию
     */
    public static List<Point> line(int x0, int y0, int x1, int y1) {
        List<Point> points = new ArrayList<>();

        int dx = Math.abs(x1 - x0);
        int dy = Math.abs(y1 - y0);

        int sx = x0 < x1 ? 1 : -1;  // направление по x
        int sy = y0 < y1 ? 1 : -1;  // направление по y

        int err = dx - dy;
        int x = x0;
        int y = y0;

        while (true) {
            points.add(new Point(x, y));

            if (x == x1 && y == y1) {
                break;
            }

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }

            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }

        return points;
    }

    /**
     * Алгоритм Брезенхема для рисования окружности
     *
     * Рисует окружность с центром (cx, cy) и радиусом r.
     * Использует симметрию окружности - вычисляет 1/8 окружности,
     * остальные 7/8 получает отражением.
     *
     * @param cx центр окружности по x
     * @param cy центр окружности по y
     * @param r радиус окружности
     * @return список точек (пикселей), составляющих окружность
     */
    public static List<Point> circle(int cx, int cy, int r) {
        List<Point> points = new ArrayList<>();

        int x = 0;
        int y = r;
        int d = 3 - 2 * r;  // начальное значение переменной решения

        // Добавляем 8 симметричных точек
        addCirclePoints(points, cx, cy, x, y);

        while (y >= x) {
            x++;

            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }

            // Добавляем 8 симметричных точек для текущего x, y
            addCirclePoints(points, cx, cy, x, y);
        }

        return points;
    }

    /**
     * Добавляет 8 симметричных точек окружности
     */
    private static void addCirclePoints(List<Point> points, int cx, int cy, int x, int y) {
        points.add(new Point(cx + x, cy + y));
        points.add(new Point(cx - x, cy + y));
        points.add(new Point(cx + x, cy - y));
        points.add(new Point(cx - x, cy - y));
        points.add(new Point(cx + y, cy + x));
        points.add(new Point(cx - y, cy + x));
        points.add(new Point(cx + y, cy - x));
        points.add(new Point(cx - y, cy - x));
    }

    /**
     * Алгоритм заливки методом распространения (Flood Fill)
     *
     * Простая рекурсивная реализация для демонстрации концепции.
     * В реальных приложениях используется стек для избежания переполнения.
     *
     * @param canvas двумерный массив пикселей
     * @param x координата начальной точки
     * @param y координата начальной точки
     * @param targetColor цвет, который заменяем
     * @param replacementColor новый цвет
     */
    public static void floodFill(int[][] canvas, int x, int y, int targetColor, int replacementColor) {
        if (x < 0 || x >= canvas.length || y < 0 || y >= canvas[0].length) {
            return;  // вышли за границы
        }

        if (canvas[x][y] != targetColor || canvas[x][y] == replacementColor) {
            return;  // не тот цвет или уже закрашено
        }

        canvas[x][y] = replacementColor;

        // Рекурсивно заливаем соседние пиксели (4-связность)
        floodFill(canvas, x + 1, y, targetColor, replacementColor);
        floodFill(canvas, x - 1, y, targetColor, replacementColor);
        floodFill(canvas, x, y + 1, targetColor, replacementColor);
        floodFill(canvas, x, y - 1, targetColor, replacementColor);
    }
}
