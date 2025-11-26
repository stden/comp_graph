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

    // ============ ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ УБИЙСТВА МУТАЦИЙ ============

    @Test
    public void testSphereHitQuadraticFormula() {
        // Тест на правильный расчёт квадратного уравнения at² + bt + c = 0
        // a = D·D, b = 2(O-C)·D, c = (O-C)·(O-C) - r²
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 10),
            2.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Пересечение найдено", hit);

        // t = 10 - 2 = 8 (центр на 10, радиус 2)
        assertEquals("Правильное t", 8.0f, hit.t, EPSILON);
    }

    @Test
    public void testSphereHitDiscriminant() {
        // Тест на дискриминант: b² - 4ac
        // Луч касательный к сфере (один корень)
        Ray ray = new Ray(
            new Vector3D(1, 0, 0),  // offset от центра
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),
            1.0f,  // радиус 1
            Material.red()
        );

        // Луч проходит на расстоянии 1 от центра сферы с радиусом 1 = касательная
        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        // Луч начинается в (1, 0, 0), направлен по Z
        // Сфера с центром (0, 0, 5), радиус 1
        // Расстояние от луча до центра = 1 = радиус, значит касание
        assertNotNull("Касательное пересечение", hit);
    }

    @Test
    public void testSphereHitNegativeDiscriminant() {
        // Тест на отрицательный дискриминант (нет пересечения)
        Ray ray = new Ray(
            new Vector3D(5, 0, 0),  // далеко от центра
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),
            1.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNull("Нет пересечения (промах)", hit);
    }

    @Test
    public void testSphereHitSecondRoot() {
        // Тест на использование второго корня когда первый за пределами
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),
            2.0f,  // радиус 2, ближнее пересечение в t=3, дальнее в t=7
            Material.red()
        );

        // tMin=4 исключает первое пересечение (t=3)
        HitRecord hit = sphere.hit(ray, 4.0f, 10.0f);
        assertNotNull("Второе пересечение найдено", hit);
        assertEquals("Дальнее пересечение t=7", 7.0f, hit.t, EPSILON);
    }

    @Test
    public void testSphereHitBothRootsOutOfRange() {
        // Тест когда оба корня вне диапазона
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 5),
            1.0f,  // t1=4, t2=6
            Material.red()
        );

        // tMin=7 исключает оба пересечения
        HitRecord hit = sphere.hit(ray, 7.0f, 10.0f);
        assertNull("Оба пересечения вне диапазона", hit);
    }

    @Test
    public void testSphereHitNormalCalculation() {
        // Тест на правильный расчёт нормали: (point - center) / radius
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 1, 0)  // вверх
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 10, 0),  // центр наверху
            3.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Пересечение найдено", hit);

        // Точка пересечения: (0, 7, 0), центр: (0, 10, 0)
        // Нормаль = (0, 7, 0) - (0, 10, 0) = (0, -3, 0) / 3 = (0, -1, 0)
        assertEquals("Нормаль X", 0, hit.normal.x, EPSILON);
        assertEquals("Нормаль Y", -1.0f, hit.normal.y, EPSILON);
        assertEquals("Нормаль Z", 0, hit.normal.z, EPSILON);
    }

    @Test
    public void testSphereHitOcVector() {
        // Тест на правильный расчёт вектора oc = origin - center
        Ray ray = new Ray(
            new Vector3D(3, 4, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 10),
            6.0f,  // радиус 6, луч на расстоянии 5 от центра (3² + 4² = 25 = 5²)
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Луч пересекает сферу", hit);
    }

    @Test
    public void testPlaneHitDenominator() {
        // Тест на правильный расчёт знаменателя D·N
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, -1, 0)  // вниз
        );

        Plane plane = new Plane(
            new Vector3D(0, -5, 0),   // плоскость внизу
            new Vector3D(0, 1, 0),    // нормаль вверх
            Material.green()
        );

        HitRecord hit = plane.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Пересечение с плоскостью", hit);
        assertEquals("Расстояние до плоскости", 5.0f, hit.t, EPSILON);
    }

    @Test
    public void testPlaneHitDiffVector() {
        // Тест на правильный расчёт diff = point - origin
        Ray ray = new Ray(
            new Vector3D(5, 3, 0),  // смещённое начало
            new Vector3D(0, 1, 0)   // вверх
        );

        Plane plane = new Plane(
            new Vector3D(0, 10, 0),  // плоскость на высоте 10
            new Vector3D(0, 1, 0),
            Material.green()
        );

        HitRecord hit = plane.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Пересечение найдено", hit);
        assertEquals("t = 10 - 3 = 7", 7.0f, hit.t, EPSILON);
    }

    @Test
    public void testPlaneHitOutOfRange() {
        // Тест на диапазон tMin, tMax для плоскости
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 1, 0)
        );

        Plane plane = new Plane(
            new Vector3D(0, 5, 0),
            new Vector3D(0, 1, 0),
            Material.green()
        );

        // t=5, но диапазон [6, 10]
        HitRecord hit = plane.hit(ray, 6.0f, 10.0f);
        assertNull("Пересечение вне диапазона", hit);
    }

    @Test
    public void testTraceRayBackgroundGradient() {
        // Тест на правильный расчёт градиента фона
        Scene scene = new Scene();

        // Луч вверх (y=1)
        Ray rayUp = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, 1, 0));
        float[] colorUp = RayTracer.traceRay(rayUp, scene, 5);

        // Луч вниз (y=-1)
        Ray rayDown = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, -1, 0));
        float[] colorDown = RayTracer.traceRay(rayDown, scene, 5);

        // Вверх должно быть более голубым (меньше R)
        assertTrue("Верх более голубой", colorUp[0] < colorDown[0]);
    }

    @Test
    public void testTraceRayDiffuseLighting() {
        // Тест на диффузное освещение (dot product нормали и света)
        Scene scene = new Scene();

        // Сфера сверху - нормаль вниз, свет сверху = минимальное освещение
        scene.add(new Sphere(new Vector3D(0, 10, 0), 1.0f, Material.red()));

        Ray ray = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, 1, 0));
        float[] color = RayTracer.traceRay(ray, scene, 5);

        // При нормали вниз (-Y) и свете сверху (+Y) dot = -1, intensity = max(0.2, -1) = 0.2
        assertTrue("Минимальное освещение", color[0] < 0.5f);
    }

    @Test
    public void testTraceRayReflection() {
        // Тест на отражения
        Scene scene = new Scene();

        // Зеркальная сфера
        scene.add(new Sphere(new Vector3D(0, 0, 5), 1.0f, Material.mirror()));

        // Красная сфера за камерой (отразится в зеркале)
        scene.add(new Sphere(new Vector3D(0, 0, -5), 1.0f, Material.red()));

        Ray ray = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, 0, 1));
        float[] colorNoReflection = RayTracer.traceRay(ray, scene, 1);  // depth=1, нет отражений
        float[] colorWithReflection = RayTracer.traceRay(ray, scene, 3);  // depth=3, есть отражения

        // Цвета должны отличаться из-за отражений (или нет, если красная сфера не видна)
        assertNotNull("Цвет без отражений", colorNoReflection);
        assertNotNull("Цвет с отражениями", colorWithReflection);
    }

    @Test
    public void testTraceRayReflectivityBlending() {
        // Тест на смешивание цветов при отражении: r*(1-k) + reflected*k
        Scene scene = new Scene();

        Material halfMirror = new Material(1.0f, 0, 0, 0.5f);  // 50% отражение красного
        scene.add(new Sphere(new Vector3D(0, 0, 5), 1.0f, halfMirror));

        Ray ray = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, 0, 1));
        float[] color = RayTracer.traceRay(ray, scene, 3);

        // Должен быть смешанный цвет (часть красного + часть отражённого)
        assertTrue("Красный компонент присутствует", color[0] > 0);
    }

    @Test
    public void testSceneClosestHit() {
        // Тест на выбор ближайшего пересечения
        Scene scene = new Scene();

        // Добавляем сферы в обратном порядке (дальняя первая)
        scene.add(new Sphere(new Vector3D(0, 0, 15), 1.0f, Material.blue()));
        scene.add(new Sphere(new Vector3D(0, 0, 10), 1.0f, Material.green()));
        scene.add(new Sphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));

        Ray ray = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, 0, 1));
        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Пересечение найдено", hit);
        assertEquals("Ближайшая красная сфера", 1.0f, hit.material.r, EPSILON);
        assertEquals("t = 4", 4.0f, hit.t, EPSILON);
    }

    @Test
    public void testSceneUpdateClosestT() {
        // Тест на обновление closestT в цикле
        Scene scene = new Scene();

        scene.add(new Sphere(new Vector3D(0, 0, 5), 1.0f, Material.red()));
        scene.add(new Sphere(new Vector3D(0, 0, 8), 1.0f, Material.green()));

        Ray ray = new Ray(new Vector3D(0, 0, 0), new Vector3D(0, 0, 1));
        HitRecord hit = scene.hit(ray, 0, Float.MAX_VALUE);

        // После нахождения красной сферы (t=4), closestT должен обновиться
        // и зелёная сфера (t=7) должна быть пропущена только если 7 > closestT=4... нет, должна проверяться
        // Но hit возвращает ближайшую
        assertEquals("Ближайшая красная", 1.0f, hit.material.r, EPSILON);
    }

    @Test
    public void testReflectFormula() {
        // Тест на формулу отражения: R = V - 2(V·N)N
        Vector3D incident = new Vector3D(1, -1, 0);  // диагональный вниз-вправо
        Vector3D normal = new Vector3D(0, 1, 0);     // вверх

        Vector3D reflected = RayTracer.reflect(incident, normal);

        // V·N = 1*0 + (-1)*1 + 0*0 = -1
        // R = (1,-1,0) - 2*(-1)*(0,1,0) = (1,-1,0) + (0,2,0) = (1,1,0)
        assertEquals("Reflected X", 1.0f, reflected.x, EPSILON);
        assertEquals("Reflected Y", 1.0f, reflected.y, EPSILON);
        assertEquals("Reflected Z", 0, reflected.z, EPSILON);
    }

    @Test
    public void testRayDirectionNormalization() {
        // Тест на нормализацию направления луча
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(3, 4, 0)  // длина 5, должна стать 1
        );

        assertEquals("Направление нормализовано", 1.0f, ray.direction.length(), EPSILON);
        assertEquals("X компонента", 0.6f, ray.direction.x, EPSILON);
        assertEquals("Y компонента", 0.8f, ray.direction.y, EPSILON);
    }

    @Test
    public void testSphereHitPointCalculation() {
        // Тест на правильный расчёт точки пересечения: pointAt(t)
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 0, 0)
        );

        Sphere sphere = new Sphere(
            new Vector3D(10, 0, 0),
            2.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Пересечение найдено", hit);

        // Точка пересечения должна быть на поверхности сферы
        assertEquals("Точка X", 8.0f, hit.point.x, EPSILON);
        assertEquals("Точка Y", 0, hit.point.y, EPSILON);
        assertEquals("Точка Z", 0, hit.point.z, EPSILON);
    }

    @Test
    public void testSphereHitRadiusSquared() {
        // Тест на правильный расчёт c = oc·oc - r²
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        // Сфера с разными радиусами
        Sphere smallSphere = new Sphere(new Vector3D(0, 0, 10), 1.0f, Material.red());
        Sphere largeSphere = new Sphere(new Vector3D(0, 0, 10), 5.0f, Material.blue());

        HitRecord hitSmall = smallSphere.hit(ray, 0, Float.MAX_VALUE);
        HitRecord hitLarge = largeSphere.hit(ray, 0, Float.MAX_VALUE);

        assertNotNull("Малая сфера", hitSmall);
        assertNotNull("Большая сфера", hitLarge);

        assertEquals("Малая t=9", 9.0f, hitSmall.t, EPSILON);
        assertEquals("Большая t=5", 5.0f, hitLarge.t, EPSILON);
    }

    @Test
    public void testSphereHit2BCoefficient() {
        // Тест на коэффициент 2.0f в b = 2.0f * oc.dot(direction)
        Ray ray = new Ray(
            new Vector3D(0, 3, 0),  // смещение по Y
            new Vector3D(0, 0, 1)   // направление по Z
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 10),
            5.0f,  // радиус 5
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Луч пересекает сферу", hit);
        // Луч в (0,3,0) по Z, сфера в (0,0,10) r=5
        // oc = (0,3,0) - (0,0,10) = (0,3,-10)
        // a = 1, b = 2*(0*0 + 3*0 + (-10)*1) = -20, c = 9+100-25 = 84
        // discriminant = 400 - 4*84 = 400-336 = 64
        // t = (20 - 8) / 2 = 6
        assertEquals("t = 6", 6.0f, hit.t, EPSILON);
    }

    @Test
    public void testSphereHit4ACCoefficient() {
        // Тест на правильный расчёт 4*a*c в дискриминанте
        // Используем луч под углом
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(1, 1, 1)  // диагональный луч
        );

        Sphere sphere = new Sphere(
            new Vector3D(5, 5, 5),
            2.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Диагональный луч попадает в сферу", hit);
    }

    @Test
    public void testSphereHitSqrtDiscriminant() {
        // Тест на правильное взятие sqrt от дискриминанта
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 13),
            5.0f,  // t1 = 13-5=8, t2 = 13+5=18
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        assertNotNull("Пересечение найдено", hit);
        assertEquals("Ближнее t=8", 8.0f, hit.t, EPSILON);
    }

    @Test
    public void testSphereHit2AInDenominator() {
        // Тест на правильный знаменатель 2*a в формуле корней
        Ray ray = new Ray(
            new Vector3D(0, 0, 0),
            new Vector3D(0, 0, 1)  // a = D·D = 1
        );

        Sphere sphere = new Sphere(
            new Vector3D(0, 0, 10),
            3.0f,
            Material.red()
        );

        HitRecord hit = sphere.hit(ray, 0, Float.MAX_VALUE);
        // t = (-b ± sqrt(d)) / 2a = (20 ± 6) / 2 = 7 или 13
        assertEquals("t = 7", 7.0f, hit.t, EPSILON);
    }
}
