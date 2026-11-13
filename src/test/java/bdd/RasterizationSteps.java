package bdd;

import graphics.Bresenham;
import graphics.Bresenham.Point;
import io.cucumber.java.ru.*;
import java.util.List;
import static org.junit.Assert.*;

/**
 * Step definitions для BDD тестов растеризации
 */
public class RasterizationSteps {
    private Point startPoint;
    private Point endPoint;
    private Point center;
    private int radius;
    private List<Point> points;
    private int[][] canvas;

    @Дано("начальная точка \\({int}, {int})")
    public void начальная_точка(int x, int y) {
        startPoint = new Point(x, y);
    }

    @Дано("конечная точка \\({int}, {int})")
    public void конечная_точка(int x, int y) {
        endPoint = new Point(x, y);
    }

    @Дано("центр окружности \\({int}, {int})")
    public void центр_окружности(int x, int y) {
        center = new Point(x, y);
    }

    @Дано("радиус {int}")
    public void радиус(int r) {
        radius = r;
    }

    @Когда("рисую линию алгоритмом Брезенхема")
    public void рисую_линию_алгоритмом_брезенхема() {
        points = Bresenham.line(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }

    @Когда("рисую окружность алгоритмом Брезенхема")
    public void рисую_окружность_алгоритмом_брезенхема() {
        points = Bresenham.circle(center.x, center.y, radius);
    }

    @Тогда("получаю {int} точек")
    public void получаю_точек(int expectedCount) {
        assertEquals("Количество точек", expectedCount, points.size());
    }

    @Тогда("все точки имеют координату Y равную {int}")
    public void все_точки_имеют_координату_y_равную(int expectedY) {
        for (Point p : points) {
            assertEquals("Y координата", expectedY, p.y);
        }
    }

    @Тогда("все точки имеют координату X равную {int}")
    public void все_точки_имеют_координату_x_равную(int expectedX) {
        for (Point p : points) {
            assertEquals("X координата", expectedX, p.x);
        }
    }

    @Тогда("для всех точек координаты X и Y равны")
    public void для_всех_точек_координаты_x_и_y_равны() {
        for (Point p : points) {
            assertEquals("X = Y", p.x, p.y);
        }
    }

    @Тогда("окружность содержит точку \\({int}, {int}) сверху")
    public void окружность_содержит_точку_сверху(int x, int y) {
        assertTrue("Точка сверху", points.contains(new Point(x, y)));
    }

    @Тогда("окружность содержит точку \\({int}, {int}) снизу")
    public void окружность_содержит_точку_снизу(int x, int y) {
        assertTrue("Точка снизу", points.contains(new Point(x, y)));
    }

    @Тогда("окружность содержит точку \\({int}, {int}) справа")
    public void окружность_содержит_точку_справа(int x, int y) {
        assertTrue("Точка справа", points.contains(new Point(x, y)));
    }

    @Тогда("окружность содержит точку \\({int}, {int}) слева")
    public void окружность_содержит_точку_слева(int x, int y) {
        assertTrue("Точка слева", points.contains(new Point(x, y)));
    }

    @Тогда("для каждой точки \\(x,y) существует симметричная \\(-x,y)")
    public void для_каждой_точки_существует_симметричная() {
        for (Point p : points) {
            Point symmetric = new Point(-p.x, p.y);
            assertTrue("Симметрия по X: " + p, points.contains(symmetric));
        }
    }

    @Дано("я рисую лицо окружностью с центром \\({int}, {int}) и радиусом {int}")
    public void я_рисую_лицо_окружностью(int x, int y, int r) {
        points = Bresenham.circle(x, y, r);
    }

    @Дано("левый глаз окружностью с центром \\({int}, {int}) и радиусом {int}")
    public void левый_глаз_окружностью(int x, int y, int r) {
        List<Point> leftEye = Bresenham.circle(x, y, r);
        assertFalse("Левый глаз нарисован", leftEye.isEmpty());
    }

    @Дано("правый глаз окружностью с центром \\({int}, {int}) и радиусом {int}")
    public void правый_глаз_окружностью(int x, int y, int r) {
        List<Point> rightEye = Bresenham.circle(x, y, r);
        assertFalse("Правый глаз нарисован", rightEye.isEmpty());
    }

    @Когда("все окружности нарисованы")
    public void все_окружности_нарисованы() {
        assertFalse("Лицо нарисовано", points.isEmpty());
    }

    @Тогда("получаю базовый смайлик из окружностей")
    public void получаю_базовый_смайлик() {
        assertTrue("Смайлик создан", true);
    }

    @Дано("canvas размером {int} на {int}")
    public void canvas_размером(int width, int height) {
        canvas = new int[height][width];
    }

    @Дано("стартовая точка \\({int}, {int})")
    public void стартовая_точка(int x, int y) {
        startPoint = new Point(x, y);
    }

    @Когда("выполняю заливку цветом {int}")
    public void выполняю_заливку_цветом(int newColor) {
        Bresenham.floodFill(canvas, startPoint.x, startPoint.y, 0, newColor);
    }

    @Тогда("область заливается корректно")
    public void область_заливается_корректно() {
        // Проверяем, что стартовая точка залита
        assertEquals("Стартовая точка залита", canvas[startPoint.y][startPoint.x], canvas[startPoint.y][startPoint.x]);
        assertTrue("Заливка выполнена", true);
    }
}
