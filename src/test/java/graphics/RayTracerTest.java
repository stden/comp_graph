package graphics;

import graphics.RayTracer.*;
import math3d.Vector3D;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Тесты для ray tracer (TDD подход)
 * Модуль 5: Продвинутые темы
 */
public class RayTracerTest {
    private static final float EPSILON = 0.001f;

    @Test
    public void testRayCreation() {
        Vector3D origin = new Vector3D(0, 0, 0);
        Vector3D direction = new Vector3D(1, 0, 0);

        Ray ray = new Ray(origin, direction);

        assertEquals("Origin X", 0, ray.origin.x, EPSILON);
        assertEquals("Direction нормализовано", 1.0f, ray.direction.length(), EPSILON);
    }

    @Test
    public void testRayPointAt() {
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)
        );

        Vector3D point = ray.pointAt(5.0f);

        assertEquals("Точка на расстоянии 5", 5.0f, point.x, EPSILON);
        assertEquals("Y = 0", 0, point.y, EPSILON);
        assertEquals("Z = 0", 0, point.z, EPSILON);
    }

    @Test
    public void testSphereHitDirect() {
        // Луч напрямую в центр сферы
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),  // центр на расстоянии 5
            1.0f,                    // радиус 1
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Есть пересечение", hit);
        assertEquals("Расстояние до пересечения", 4.0f, hit.t, EPSILON);  // 5 - 1 = 4
    }

    @Test
    public void testSphereNoHit() {
        // Луч мимо сферы
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)  // луч вправо
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),  // сфера впереди
            1.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);

        assertNull("Нет пересечения", hit);
    }

    @Test
    public void testSphereHitTwoPoints() {
        // Луч проходит через сферу (2 пересечения)
        // Должно вернуть ближайшее
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 10),
            3.0f,  // большая сфера
            Material.green()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Есть пересечение", hit);
        // Ближайшее пересечение: 10 - 3 = 7
        assertEquals("Ближайшее пересечение", 7.0f, hit.t, EPSILON);
    }

    @Test
    public void testSphereNormal() {
        // Проверка нормали в точке пересечения
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)
        );

        Sphere sphere = new Sphere(
            new Vector3D(5, 0, 0),
            2.0f,
            Material.blue()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Есть пересечение", hit);

        // Нормаль должна указывать от центра сферы
        // В точке (3, 0, 0) нормаль = (-1, 0, 0)
        assertEquals("Нормаль X", -1.0f, hit.normal.x, EPSILON);
        assertEquals("Нормаль Y", 0, hit.normal.y, EPSILON);
        assertEquals("Нормаль Z", 0, hit.normal.z, EPSILON);
    }

    @Test
    public void testPlaneHit() {
        // Луч пересекает плоскость
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 1, 0)  // вверх
        );

        Plane plane = new Plane(
            new Vector3D(0, 5, 0),   // плоскость на высоте 5
            new Vector3D(0, 1, 0),   // нормаль вверх
            Material.green()
        );

        HitRecord hit = plane.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Есть пересечение", hit);
        assertEquals("Расстояние до плоскости", 5.0f, hit.t, EPSILON);
    }

    @Test
    public void testPlaneParallel() {
        // Луч параллелен плоскости
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)  // вправо
        );

        Plane plane = new Plane(
            new Vector3D(0, 5, 0),   // горизонтальная плоскость
            new Vector3D(0, 1, 0),   // нормаль вверх
            Material.red()
        );

        HitRecord hit = plane.hit(ray, 0, Float.MAX_VALUE);

        assertNull("Нет пересечения (параллель)", hit);
    }

    @Test
    public void testSceneEmpty() {
        Scene scene = new Scene();
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)
        );

        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);

        assertNull("Пустая сцена", hit);
    }

    @Test
    public void testSceneMultipleObjects() {
        Scene scene = new Scene();

        // Ближняя сфера
        scene.add(new Sphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        // Дальняя сфера
        scene.add(new Sphere(new Vector3D(0, 0, 10), 1.0f, Material.blue()));

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Есть пересечение", hit);
        // Должно вернуть ближайшую сферу
        assertEquals("Ближайший объект (красный)", 1.0f, hit.material.r, EPSILON);
        assertEquals("Расстояние", 4.0f, hit.t, EPSILON);
    }

    @Test
    public void testReflect() {
        // Отражение вектора от горизонтальной поверхности
        Vector3D incident = new Vector3D(1, -1, 0);  // под углом 45° вниз
        incident.normalize();

        Vector3D normal = new Vector3D(0, 1, 0);  // вверх

        Vector3D reflected = RayTracer.reflect(incident, normal);

        // Отражённый луч должен идти вверх под тем же углом
        assertTrue("Отражённый X > 0", reflected.x > 0);
        assertTrue("Отражённый Y > 0", reflected.y > 0);
        assertEquals("Симметричное отражение", incident.x, reflected.x, EPSILON);
        assertEquals("Y инвертирован", -incident.y, reflected.y, EPSILON);
    }

    @Test
    public void testTraceRayBackground() {
        // Луч в пустую сцену - должен вернуть цвет фона
        Scene scene = new Scene();
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 1, 0)  // вверх
        );

        float[] color = RayTracer.traceRay(ray, scene, 5);

        // Фон должен быть светлым (небо)
        assertTrue("Фон светлый", color[0] > 0.5f || color[1] > 0.5f || color[2] > 0.5f);
    }

    @Test
    public void testTraceRayHitObject() {
        // Луч попадает в красную сферу
        Scene scene = new Scene();
        scene.add(new Sphere(
            new Vector3D(0, 0, 5),
            1.0f,
            Material.red()
        ));

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        float[] color = RayTracer.traceRay(ray, scene, 5);

        // Цвет должен быть красноватым
        assertTrue("Красный компонент преобладает", color[0] > color[1]);
        assertTrue("Красный компонент преобладает", color[0] > color[2]);
    }

    @Test
    public void testTraceRayDepthLimit() {
        // Проверка ограничения глубины рекурсии
        Scene scene = new Scene();
        scene.add(new Sphere(
            new Vector3D(0, 0, 5),
            1.0f,
            Material.mirror()  // зеркальная поверхность
        ));

        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        // С глубиной 0 должен вернуть чёрный
        float[] color = RayTracer.traceRay(ray, scene, 0);

        assertEquals("Чёрный R", 0, color[0], EPSILON);
        assertEquals("Чёрный G", 0, color[1], EPSILON);
        assertEquals("Чёрный B", 0, color[2], EPSILON);
    }

    @Test
    public void testMaterialPresets() {
        Material red = Material.red();
        Material green = Material.green();
        Material blue = Material.blue();
        Material mirror = Material.mirror();

        assertEquals("Красный цвет", 1.0f, red.r, EPSILON);
        assertEquals("Зелёный цвет", 1.0f, green.g, EPSILON);
        assertEquals("Синий цвет", 1.0f, blue.b, EPSILON);
        assertTrue("Зеркало отражает", mirror.reflectivity > 0.5f);
    }

    @Test
    public void testHitRecordCreation() {
        Vector3D point = new Vector3D(1, 2, 3);
        Vector3D normal = new Vector3D(0, 1, 0);
        Material mat = Material.red();

        HitRecord hit = new HitRecord(5.0f, point, normal, mat);

        assertEquals("Расстояние", 5.0f, hit.t, EPSILON);
        assertEquals("Точка X", 1, hit.point.x, EPSILON);
        assertEquals("Нормаль Y", 1, hit.normal.y, EPSILON);
        assertEquals("Материал", mat, hit.material);
    }

    @Test
    public void testSphereHitRange() {
        // Проверка диапазона tMin, tMax
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),
            1.0f,
            Material.red()
        );

        // Ближнее пересечение t=4, дальнее t=6
        // С tMin=7 оба пересечения вне диапазона
        HitRecord hit1 = sphere.hit(ray, 7.0f, 10.0f);
        assertNull("Оба пересечения вне диапазона", hit1);

        // Пересечение в диапазоне
        HitRecord hit2 = sphere.hit(ray, 0, 10.0f);
        assertNotNull("Пересечение в диапазоне", hit2);
        assertEquals("Ближнее пересечение t=4", 4.0f, hit2.t, EPSILON);
    }
}
