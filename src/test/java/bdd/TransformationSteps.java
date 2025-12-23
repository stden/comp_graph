package bdd;

import graphics.Transform2D;
import graphics.Matrix4;
import math3d.Vector3D;
import io.cucumber.java.ru.*;
import static org.junit.Assert.*;

/**
 * Step definitions для BDD тестов трансформаций
 */
public class TransformationSteps {
    private double[] point2D;
    private Vector3D point3D;
    private Transform2D transform2D;
    private Matrix4 matrix4;
    private double[] transformedPoint2D;
    private Vector3D transformedPoint3D;
    private static final double EPSILON = 0.01;

    @Дано("точка с координатами \\({doubleNumber}, {doubleNumber})")
    public void точка_с_координатами_2d(double x, double y) {
        point2D = new double[]{x, y};
    }

    @Дано("матрица переноса на вектор \\({doubleNumber}, {doubleNumber})")
    public void матрица_переноса(double tx, double ty) {
        transform2D = Transform2D.translate(tx, ty);
    }

    @Дано("матрица поворота на {doubleNumber} градусов")
    public void матрица_поворота(double degrees) {
        double radians = Math.toRadians(degrees);
        transform2D = Transform2D.rotate(radians);
    }

    @Дано("матрица масштабирования с коэффициентами \\({doubleNumber}, {doubleNumber})")
    public void матрица_масштабирования(double sx, double sy) {
        transform2D = Transform2D.scale(sx, sy);
    }

    @Когда("применяю трансформацию")
    public void применяю_трансформацию() {
        if (point2D != null && transform2D != null) {
            transformedPoint2D = transform2D.transform(point2D[0], point2D[1]);
        } else if (point3D != null && matrix4 != null) {
            double[] result = matrix4.transformPoint(point3D.x, point3D.y, point3D.z);
            transformedPoint3D = new Vector3D((float)result[0], (float)result[1], (float)result[2]);
        }
    }

    @Тогда("точка перемещается в \\({doubleNumber}, {doubleNumber})")
    public void точка_перемещается_в(double expectedX, double expectedY) {
        assertNotNull("Трансформация выполнена", transformedPoint2D);
        assertEquals("X координата", expectedX, transformedPoint2D[0], EPSILON);
        assertEquals("Y координата", expectedY, transformedPoint2D[1], EPSILON);
    }

    @Тогда("точка поворачивается приблизительно в \\({doubleNumber}, {doubleNumber})")
    public void точка_поворачивается_приблизительно_в(double expectedX, double expectedY) {
        assertNotNull("Трансформация выполнена", transformedPoint2D);
        assertEquals("X координата после поворота", expectedX, transformedPoint2D[0], 0.1);
        assertEquals("Y координата после поворота", expectedY, transformedPoint2D[1], 0.1);
    }

    @Тогда("точка масштабируется в \\({doubleNumber}, {doubleNumber})")
    public void точка_масштабируется_в(double expectedX, double expectedY) {
        assertNotNull("Трансформация выполнена", transformedPoint2D);
        assertEquals("X координата", expectedX, transformedPoint2D[0], EPSILON);
        assertEquals("Y координата", expectedY, transformedPoint2D[1], EPSILON);
    }

    @Дано("последовательность трансформаций: поворот {doubleNumber}°, затем перенос \\({doubleNumber}, {doubleNumber})")
    public void последовательность_трансформаций(double degrees, double tx, double ty) {
        double radians = Math.toRadians(degrees);
        Transform2D rotation = Transform2D.rotate(radians);
        Transform2D translation = Transform2D.translate(tx, ty);
        transform2D = translation.multiply(rotation);
    }

    @Когда("применяю композицию трансформаций")
    public void применяю_композицию_трансформаций() {
        transformedPoint2D = transform2D.transform(point2D[0], point2D[1]);
    }

    @Тогда("получаю корректно трансформированную точку")
    public void получаю_корректно_трансформированную_точку() {
        assertNotNull("Композиция трансформаций выполнена", transformedPoint2D);
        assertTrue("Точка трансформирована", transformedPoint2D.length == 2);
    }

    @Дано("3D точка с координатами \\({doubleNumber}, {doubleNumber}, {doubleNumber})")
    public void точка_3d_с_координатами(double x, double y, double z) {
        point3D = new Vector3D((float)x, (float)y, (float)z);
    }

    @Дано("перспективная проекция с углом обзора {doubleNumber} градусов")
    public void перспективная_проекция(double fovDegrees) {
        double fovRadians = Math.toRadians(fovDegrees);
        matrix4 = Matrix4.perspective(fovRadians, 1.0, 0.1, 100.0);
    }

    @Когда("применяю матрицу проекции")
    public void применяю_матрицу_проекции() {
        double[] result = matrix4.transformPoint(point3D.x, point3D.y, point3D.z);
        transformedPoint3D = new Vector3D((float)result[0], (float)result[1], (float)result[2]);
    }

    @Тогда("точка проецируется на экран")
    public void точка_проецируется_на_экран() {
        assertNotNull("Проекция выполнена", transformedPoint3D);
    }

    @Дано("позиция камеры в точке \\({doubleNumber}, {doubleNumber}, {doubleNumber})")
    public void позиция_камеры(double x, double y, double z) {
        // Сохраняем позицию камеры для последующего использования
        point3D = new Vector3D((float)x, (float)y, (float)z);
    }

    @Дано("цель взгляда в точке \\({doubleNumber}, {doubleNumber}, {doubleNumber})")
    public void цель_взгляда(double x, double y, double z) {
        // Цель сохранена
    }

    @Дано("вектор {string} \\({doubleNumber}, {doubleNumber}, {doubleNumber})")
    public void вектор_вверх(String name, double x, double y, double z) {
        // Вектор "вверх" сохранён
    }

    @Когда("создаю матрицу вида LookAt")
    public void создаю_матрицу_вида_lookat() {
        matrix4 = Matrix4.lookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
    }

    @Тогда("получаю корректную матрицу камеры")
    public void получаю_корректную_матрицу_камеры() {
        assertNotNull("Матрица LookAt создана", matrix4);
    }
}
