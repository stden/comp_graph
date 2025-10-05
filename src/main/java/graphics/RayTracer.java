package graphics;

import math3d.Vector3D;
import java.util.ArrayList;
import java.util.List;

/**
 * Базовый Ray Tracer (трассировщик лучей)
 *
 * Модуль 5: Продвинутые темы
 * - Пересечение луча со сферой
 * - Пересечение луча с плоскостью
 * - Простое затенение
 *
 * Концепция ray tracing:
 * 1. Для каждого пикселя экрана пускаем луч из камеры
 * 2. Находим ближайшее пересечение луча с объектами сцены
 * 3. Вычисляем цвет в точке пересечения (освещение, тени, отражения)
 */
public class RayTracer {

    /**
     * Луч в пространстве
     * Определяется как: P(t) = origin + t * direction
     */
    public static class Ray {
        public Vector3D origin;     // начальная точка
        public Vector3D direction;  // направление (должно быть нормализовано)

        public Ray(Vector3D origin, Vector3D direction) {
            this.origin = new Vector3D(origin);
            this.direction = new Vector3D(direction);
            this.direction.normalize();
        }

        /**
         * Вычисляет точку на луче на расстоянии t
         */
        public Vector3D pointAt(float t) {
            return new Vector3D(direction).scale(t).add(origin);
        }
    }

    /**
     * Результат пересечения луча с объектом
     */
    public static class HitRecord {
        public float t;              // расстояние от начала луча до точки пересечения
        public Vector3D point;       // точка пересечения
        public Vector3D normal;      // нормаль в точке пересечения
        public Material material;    // материал поверхности

        public HitRecord(float t, Vector3D point, Vector3D normal, Material material) {
            this.t = t;
            this.point = new Vector3D(point);
            this.normal = new Vector3D(normal);
            this.material = material;
        }
    }

    /**
     * Материал объекта (упрощённый)
     */
    public static class Material {
        public float r, g, b;        // цвет (0-1)
        public float reflectivity;   // коэффициент отражения (0-1)

        public Material(float r, float g, float b, float reflectivity) {
            this.r = r;
            this.g = g;
            this.b = b;
            this.reflectivity = reflectivity;
        }

        public static Material red() {
            return new Material(1.0f, 0.0f, 0.0f, 0.0f);
        }

        public static Material green() {
            return new Material(0.0f, 1.0f, 0.0f, 0.0f);
        }

        public static Material blue() {
            return new Material(0.0f, 0.0f, 1.0f, 0.0f);
        }

        public static Material mirror() {
            return new Material(0.9f, 0.9f, 0.9f, 0.9f);
        }
    }

    /**
     * Абстрактный объект сцены
     */
    public interface Hittable {
        HitRecord hit(Ray ray, float tMin, float tMax);
    }

    /**
     * Сфера в пространстве
     */
    public static class Sphere implements Hittable {
        public Vector3D center;
        public float radius;
        public Material material;

        public Sphere(Vector3D center, float radius, Material material) {
            this.center = new Vector3D(center);
            this.radius = radius;
            this.material = material;
        }

        /**
         * Пересечение луча со сферой
         *
         * Решаем уравнение: |P(t) - C|² = r²
         * где P(t) = O + t*D (луч), C - центр сферы, r - радиус
         *
         * Получаем квадратное уравнение: at² + bt + c = 0
         * a = D·D
         * b = 2(O-C)·D
         * c = (O-C)·(O-C) - r²
         *
         * Дискриминант: Δ = b² - 4ac
         * Если Δ < 0: нет пересечений
         * Если Δ >= 0: t = (-b ± √Δ) / 2a
         */
        @Override
        public HitRecord hit(Ray ray, float tMin, float tMax) {
            // Вектор от центра сферы к началу луча
            Vector3D oc = new Vector3D(ray.origin).sub(center);

            float a = ray.direction.dot(ray.direction);
            float b = 2.0f * oc.dot(ray.direction);
            float c = oc.dot(oc) - radius * radius;

            float discriminant = b * b - 4 * a * c;

            if (discriminant < 0) {
                return null;  // нет пересечения
            }

            // Находим ближайшее пересечение
            float sqrtD = (float)Math.sqrt(discriminant);
            float t = (-b - sqrtD) / (2.0f * a);

            // Если первое пересечение за пределами диапазона, проверяем второе
            if (t < tMin || t > tMax) {
                t = (-b + sqrtD) / (2.0f * a);
                if (t < tMin || t > tMax) {
                    return null;
                }
            }

            // Вычисляем точку и нормаль в точке пересечения
            Vector3D point = ray.pointAt(t);
            Vector3D normal = new Vector3D(point).sub(center);
            normal.scale(1.0f / radius);  // нормализуем через деление на радиус

            return new HitRecord(t, point, normal, material);
        }
    }

    /**
     * Плоскость в пространстве
     * Определяется точкой и нормалью: (P - P0) · N = 0
     */
    public static class Plane implements Hittable {
        public Vector3D point;    // точка на плоскости
        public Vector3D normal;   // нормаль плоскости
        public Material material;

        public Plane(Vector3D point, Vector3D normal, Material material) {
            this.point = new Vector3D(point);
            this.normal = new Vector3D(normal);
            this.normal.normalize();
            this.material = material;
        }

        /**
         * Пересечение луча с плоскостью
         *
         * Луч: P(t) = O + t*D
         * Плоскость: (P - P0) · N = 0
         *
         * Подставляем луч в уравнение плоскости:
         * (O + t*D - P0) · N = 0
         * t*D·N = (P0 - O)·N
         * t = (P0 - O)·N / (D·N)
         *
         * Если D·N = 0, луч параллелен плоскости
         */
        @Override
        public HitRecord hit(Ray ray, float tMin, float tMax) {
            float denominator = ray.direction.dot(normal);

            if (Math.abs(denominator) < 0.0001f) {
                return null;  // луч параллелен плоскости
            }

            Vector3D diff = new Vector3D(point).sub(ray.origin);
            float t = diff.dot(normal) / denominator;

            if (t < tMin || t > tMax) {
                return null;  // пересечение за пределами диапазона
            }

            Vector3D hitPoint = ray.pointAt(t);
            return new HitRecord(t, hitPoint, normal, material);
        }
    }

    /**
     * Сцена с несколькими объектами
     */
    public static class Scene {
        public List<Hittable> objects;

        public Scene() {
            this.objects = new ArrayList<>();
        }

        public void add(Hittable object) {
            objects.add(object);
        }

        /**
         * Находит ближайшее пересечение луча с объектами сцены
         */
        public HitRecord hit(Ray ray, float tMin, float tMax) {
            HitRecord closest = null;
            float closestT = tMax;

            for (Hittable object : objects) {
                HitRecord hit = object.hit(ray, tMin, closestT);
                if (hit != null && hit.t < closestT) {
                    closest = hit;
                    closestT = hit.t;
                }
            }

            return closest;
        }
    }

    /**
     * Вычисляет цвет для луча (упрощённая модель)
     *
     * @param ray луч
     * @param scene сцена
     * @param depth глубина рекурсии (для отражений)
     * @return цвет [r, g, b] в диапазоне 0-1
     */
    public static float[] traceRay(Ray ray, Scene scene, int depth) {
        // Ограничиваем глубину рекурсии
        if (depth <= 0) {
            return new float[]{0, 0, 0};  // чёрный
        }

        HitRecord hit = scene.hit(ray, 0.001f, Float.MAX_VALUE);

        if (hit == null) {
            // Фон (небо): градиент от белого к голубому
            float t = 0.5f * (ray.direction.y + 1.0f);
            float r = 1.0f - 0.5f * t;
            float g = 1.0f - 0.3f * t;
            float b = 1.0f;
            return new float[]{r, g, b};
        }

        // Простое диффузное освещение (Ламберта)
        // Предполагаем свет сверху
        Vector3D lightDir = new Vector3D(0, 1, 0);
        float intensity = Math.max(0.2f, hit.normal.dot(lightDir));

        float r = hit.material.r * intensity;
        float g = hit.material.g * intensity;
        float b = hit.material.b * intensity;

        // Отражения
        if (hit.material.reflectivity > 0 && depth > 1) {
            Vector3D reflected = reflect(ray.direction, hit.normal);
            Ray reflectedRay = new Ray(hit.point, reflected);
            float[] reflectedColor = traceRay(reflectedRay, scene, depth - 1);

            float k = hit.material.reflectivity;
            r = r * (1 - k) + reflectedColor[0] * k;
            g = g * (1 - k) + reflectedColor[1] * k;
            b = b * (1 - k) + reflectedColor[2] * k;
        }

        return new float[]{r, g, b};
    }

    /**
     * Вычисляет отражённый вектор
     * R = V - 2(V·N)N
     *
     * @param incident входящий вектор
     * @param normal нормаль
     * @return отражённый вектор
     */
    public static Vector3D reflect(Vector3D incident, Vector3D normal) {
        float dotVN = incident.dot(normal);
        return new Vector3D(incident).sub(
            new Vector3D(normal).scale(2 * dotVN)
        );
    }
}
