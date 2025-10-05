package bdd;

import graphics.RayTracer;
import graphics.RayTracer.*;
import math3d.Vector3D;
import io.cucumber.java.ru.*;
import static org.junit.Assert.*;

/**
 * Step definitions для BDD тестов трассировки лучей
 */
public class RayTracingSteps {
    private Ray ray;
    private Sphere sphere;
    private Plane plane;
    private HitRecord hitRecord;
    private float tMin = 0.1f;
    private float tMax = 1000.0f;
    private Scene scene;
    private static final float EPSILON = 0.01f;

    @Дано("луч с началом \\({float}, {float}, {float}) и направлением \\({float}, {float}, {float})")
    public void луч_с_началом_и_направлением(float ox, float oy, float oz, float dx, float dy, float dz) {
        Vector3D origin = new Vector3D(ox, oy, oz);
        Vector3D direction = new Vector3D(dx, dy, dz);
        direction.normalize();
        ray = new Ray(origin, direction);
    }

    @Дано("сфера с центром \\({float}, {float}, {float}) и радиусом {float}")
    public void сфера_с_центром_и_радиусом(float x, float y, float z, float radius) {
        Vector3D center = new Vector3D(x, y, z);
        Material material = new Material(0.1f, 0.8f, 0.2f, 32.0f);
        sphere = new Sphere(center, radius, material);
    }

    @Когда("проверяю пересечение луча со сферой")
    public void проверяю_пересечение_луча_со_сферой() {
        hitRecord = sphere.hit(ray, tMin, tMax);
    }

    @Тогда("луч пересекает сферу в точке \\({float}, {float}, {float})")
    public void луч_пересекает_сферу_в_точке(float x, float y, float z) {
        assertNotNull("Пересечение найдено", hitRecord);
        assertEquals("X точки пересечения", x, hitRecord.point.x, EPSILON);
        assertEquals("Y точки пересечения", y, hitRecord.point.y, EPSILON);
        assertEquals("Z точки пересечения", z, hitRecord.point.z, EPSILON);
    }

    @Тогда("нормаль в точке пересечения \\({float}, {float}, {float})")
    public void нормаль_в_точке_пересечения(float nx, float ny, float nz) {
        assertNotNull("Пересечение найдено", hitRecord);
        assertEquals("X нормали", nx, hitRecord.normal.x, EPSILON);
        assertEquals("Y нормали", ny, hitRecord.normal.y, EPSILON);
        assertEquals("Z нормали", nz, hitRecord.normal.z, EPSILON);
    }

    @Тогда("пересечений не найдено")
    public void пересечений_не_найдено() {
        assertNull("Пересечение не найдено", hitRecord);
    }

    @Дано("плоскость Y={float} с нормалью \\({float}, {float}, {float})")
    public void плоскость_с_нормалью(float planeY, float nx, float ny, float nz) {
        Vector3D point = new Vector3D(0, planeY, 0);
        Vector3D normal = new Vector3D(nx, ny, nz);
        Material material = new Material(0.1f, 0.8f, 0.2f, 32.0f);
        plane = new Plane(point, normal, material);
    }

    @Когда("проверяю пересечение луча с плоскостью")
    public void проверяю_пересечение_луча_с_плоскостью() {
        hitRecord = plane.hit(ray, tMin, tMax);
    }

    @Тогда("луч пересекает плоскость в точке \\({float}, {float}, {float})")
    public void луч_пересекает_плоскость_в_точке(float x, float y, float z) {
        assertNotNull("Пересечение с плоскостью найдено", hitRecord);
        assertEquals("X точки пересечения", x, hitRecord.point.x, EPSILON);
        assertEquals("Y точки пересечения", y, hitRecord.point.y, EPSILON);
        assertEquals("Z точки пересечения", z, hitRecord.point.z, EPSILON);
    }

    @Дано("диапазон расстояний от {float} до {float}")
    public void диапазон_расстояний(float min, float max) {
        tMin = min;
        tMax = max;
    }

    @Когда("проверяю пересечение в диапазоне")
    public void проверяю_пересечение_в_диапазоне() {
        hitRecord = sphere.hit(ray, tMin, tMax);
    }

    @Тогда("пересечение найдено на расстоянии {float}")
    public void пересечение_найдено_на_расстоянии(float expectedT) {
        assertNotNull("Пересечение в диапазоне найдено", hitRecord);
        assertEquals("Расстояние до пересечения", expectedT, hitRecord.t, EPSILON);
    }

    @Дано("сцена содержит сферу в \\({float}, {float}, {float})")
    public void сцена_содержит_сферу(float x, float y, float z) {
        if (scene == null) {
            scene = new Scene();
        }
        Material material = new Material(0.1f, 0.8f, 0.2f, 32.0f);
        scene.add(new Sphere(new Vector3D(x, y, z), 1.0f, material));
    }

    @Дано("сцена содержит плоскость Y={float}")
    public void сцена_содержит_плоскость(float y) {
        if (scene == null) {
            scene = new Scene();
        }
        Material material = new Material(0.1f, 0.8f, 0.2f, 32.0f);
        scene.add(new Plane(new Vector3D(0, y, 0), new Vector3D(0, 1, 0), material));
    }

    @Когда("пускаю луч \\({float}, {float}, {float}) в направлении \\({float}, {float}, {float})")
    public void пускаю_луч(float ox, float oy, float oz, float dx, float dy, float dz) {
        Vector3D origin = new Vector3D(ox, oy, oz);
        Vector3D direction = new Vector3D(dx, dy, dz);
        direction.normalize();
        ray = new Ray(origin, direction);
        hitRecord = scene.hit(ray, 0.1f, 1000.0f);
    }

    @Тогда("луч попадает в ближайшую сферу на расстоянии {float}")
    public void луч_попадает_в_ближайшую_сферу(float expectedT) {
        assertNotNull("Попадание в сферу", hitRecord);
        assertEquals("Расстояние до ближайшей сферы", expectedT, hitRecord.t, EPSILON);
    }

    @Когда("проверяю пересечение")
    public void проверяю_пересечение() {
        hitRecord = sphere.hit(ray, tMin, tMax);
    }

    @Тогда("результат {string}")
    public void результат(String expectedResult) {
        if (expectedResult.equals("пересекает")) {
            assertNotNull("Пересечение найдено", hitRecord);
        } else if (expectedResult.equals("не пересекает")) {
            assertNull("Пересечение не найдено", hitRecord);
        }
    }
}
