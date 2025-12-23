package graphics;

import math3d.Vector3D;

/**
 * Модели освещения для 3D графики
 *
 * Модуль 4: Рендеринг и освещение
 * - Модель Ламберта (диффузное освещение)
 * - Модель Фонга (ambient + diffuse + specular)
 * - Модель Блинна-Фонга (оптимизированная версия)
 */
public class Lighting {

    /**
     * Точечный источник света
     */
    public static class Light {
        public Vector3D position;
        public float intensity;  // яркость (0-1)

        public Light(Vector3D position, float intensity) {
            this.position = new Vector3D(position);
            this.intensity = intensity;
        }
    }

    /**
     * Материал поверхности
     */
    public static class Material {
        public float ambient;     // коэффициент фонового освещения (0-1)
        public float diffuse;     // коэффициент диффузного отражения (0-1)
        public float specular;    // коэффициент зеркального отражения (0-1)
        public float shininess;   // степень блеска (1-128, чем больше - тем меньше пятно)

        public Material(float ambient, float diffuse, float specular, float shininess) {
            this.ambient = ambient;
            this.diffuse = diffuse;
            this.specular = specular;
            this.shininess = shininess;
        }

        /**
         * Матовый материал (только диффузное отражение)
         */
        public static Material matte() {
            return new Material(0.1f, 0.9f, 0.0f, 1.0f);
        }

        /**
         * Пластиковый материал
         */
        public static Material plastic() {
            return new Material(0.1f, 0.6f, 0.3f, 32.0f);
        }

        /**
         * Металлический материал (сильное зеркальное отражение)
         */
        public static Material metal() {
            return new Material(0.1f, 0.3f, 0.8f, 128.0f);
        }
    }

    /**
     * Модель Ламберта (только диффузное освещение)
     *
     * Интенсивность = I * kd * (N · L)
     * где:
     * - I: интенсивность света
     * - kd: коэффициент диффузного отражения
     * - N: нормаль поверхности
     * - L: направление на источник света
     *
     * @param normal нормаль поверхности (должна быть нормализована)
     * @param lightDir направление на источник света (должно быть нормализовано)
     * @param lightIntensity интенсивность света (0-1)
     * @param diffuseCoeff коэффициент диффузного отражения материала (0-1)
     * @return интенсивность освещения (0-1)
     */
    public static float lambert(Vector3D normal, Vector3D lightDir, float lightIntensity, float diffuseCoeff) {
        // Скалярное произведение нормали и направления света
        float dotNL = Math.max(0.0f, normal.dot(lightDir));

        return lightIntensity * diffuseCoeff * dotNL;
    }

    /**
     * Модель Фонга (ambient + diffuse + specular)
     *
     * Итоговая интенсивность = Ia * ka + Id * kd * (N · L) + Is * ks * (R · V)^n
     * где:
     * - Ia: интенсивность фонового освещения
     * - ka: коэффициент фонового отражения
     * - Id: интенсивность диффузного света
     * - kd: коэффициент диффузного отражения
     * - N: нормаль
     * - L: направление на свет
     * - Is: интенсивность зеркального света
     * - ks: коэффициент зеркального отражения
     * - R: отражённый луч света
     * - V: направление на наблюдателя
     * - n: степень блеска
     *
     * @param surfacePoint точка на поверхности
     * @param normal нормаль поверхности (нормализованная)
     * @param viewDir направление на камеру (нормализованное)
     * @param light источник света
     * @param material материал поверхности
     * @param ambientIntensity интенсивность фонового освещения
     * @return интенсивность освещения (0-1)
     */
    public static float phong(
        Vector3D surfacePoint,
        Vector3D normal,
        Vector3D viewDir,
        Light light,
        Material material,
        float ambientIntensity
    ) {
        // Ambient (фоновое освещение)
        float ambient = ambientIntensity * material.ambient;

        // Направление на источник света
        Vector3D lightDir = new Vector3D(light.position).sub(surfacePoint);
        lightDir.normalize();

        // Diffuse (диффузное отражение)
        float dotNL = Math.max(0.0f, normal.dot(lightDir));
        float diffuse = light.intensity * material.diffuse * dotNL;

        // Specular (зеркальное отражение)
        float specular = 0.0f;
        if (dotNL > 0) {
            // Вычисляем отражённый луч: R = 2(N·L)N - L
            Vector3D reflected = new Vector3D(normal)
                .scale(2.0f * dotNL)
                .sub(lightDir);
            reflected.normalize();

            // Интенсивность зеркального отражения
            float dotRV = Math.max(0.0f, reflected.dot(viewDir));
            specular = light.intensity * material.specular * (float)Math.pow(dotRV, material.shininess);
        }

        return Math.min(1.0f, ambient + diffuse + specular);
    }

    /**
     * Модель Блинна-Фонга (оптимизированная версия Фонга)
     *
     * Использует halfway вектор вместо отражённого луча:
     * H = normalize(L + V)
     * Specular = (N · H)^n
     *
     * Преимущества:
     * - Быстрее вычисляется (не нужно вычислять отражённый луч)
     * - Более реалистичные блики
     *
     * @param surfacePoint точка на поверхности
     * @param normal нормаль поверхности (нормализованная)
     * @param viewDir направление на камеру (нормализованное)
     * @param light источник света
     * @param material материал поверхности
     * @param ambientIntensity интенсивность фонового освещения
     * @return интенсивность освещения (0-1)
     */
    public static float blinnPhong(
        Vector3D surfacePoint,
        Vector3D normal,
        Vector3D viewDir,
        Light light,
        Material material,
        float ambientIntensity
    ) {
        // Ambient
        float ambient = ambientIntensity * material.ambient;

        // Направление на источник света
        Vector3D lightDir = new Vector3D(light.position).sub(surfacePoint);
        lightDir.normalize();

        // Diffuse
        float dotNL = Math.max(0.0f, normal.dot(lightDir));
        float diffuse = light.intensity * material.diffuse * dotNL;

        // Specular с halfway вектором
        float specular = 0.0f;
        if (dotNL > 0) {
            // Halfway вектор: H = normalize(L + V)
            Vector3D halfway = new Vector3D(lightDir).add(viewDir);
            halfway.normalize();

            // Интенсивность зеркального отражения
            float dotNH = Math.max(0.0f, normal.dot(halfway));
            specular = light.intensity * material.specular * (float)Math.pow(dotNH, material.shininess);
        }

        return Math.min(1.0f, ambient + diffuse + specular);
    }

    /**
     * Вычисляет затухание света с расстоянием (attenuation)
     *
     * Формула: 1 / (constant + linear * d + quadratic * d^2)
     * где d - расстояние до источника света
     *
     * @param distance расстояние до источника света
     * @param constant константное затухание (обычно 1.0)
     * @param linear линейное затухание (обычно 0.09)
     * @param quadratic квадратичное затухание (обычно 0.032)
     * @return коэффициент затухания (0-1)
     */
    public static float attenuation(float distance, float constant, float linear, float quadratic) {
        return 1.0f / (constant + linear * distance + quadratic * distance * distance);
    }
}
