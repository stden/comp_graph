package graphics;

import graphics.Lighting.Light;
import graphics.Lighting.Material;
import math3d.Vector3D;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Тесты для моделей освещения (TDD подход)
 * Модуль 4: Рендеринг и освещение
 */
public class LightingTest {
    private static final float EPSILON = 0.001f;

    @Test
    public void testLambertDirectLight() {
        // Свет падает перпендикулярно поверхности (макс. освещение)
        Vector3D normal = new Vector3D(0, 1, 0);  // вверх
        Vector3D lightDir = new Vector3D(0, 1, 0);  // свет сверху

        float intensity = Lighting.lambert(normal, lightDir, 1.0f, 1.0f);

        assertEquals("Максимальное освещение", 1.0f, intensity, EPSILON);
    }

    @Test
    public void testLambertAngled45() {
        // Свет под углом 45°
        Vector3D normal = new Vector3D(0, 1, 0);  // вверх
        Vector3D lightDir = new Vector3D(1, 1, 0);  // под углом
        lightDir.normalize();

        float intensity = Lighting.lambert(normal, lightDir, 1.0f, 1.0f);

        // cos(45°) ≈ 0.707
        assertEquals("Освещение под 45°", 0.707f, intensity, 0.01f);
    }

    @Test
    public void testLambertOppositeDirection() {
        // Свет с обратной стороны (нет освещения)
        Vector3D normal = new Vector3D(0, 1, 0);  // вверх
        Vector3D lightDir = new Vector3D(0, -1, 0);  // снизу

        float intensity = Lighting.lambert(normal, lightDir, 1.0f, 1.0f);

        assertEquals("Нет освещения", 0.0f, intensity, EPSILON);
    }

    @Test
    public void testLambertDiffuseCoeff() {
        // Тест коэффициента диффузного отражения
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D lightDir = new Vector3D(0, 1, 0);

        float intensity = Lighting.lambert(normal, lightDir, 1.0f, 0.5f);

        assertEquals("Коэффициент 0.5", 0.5f, intensity, EPSILON);
    }

    @Test
    public void testPhongAmbientOnly() {
        // Тест только фонового освещения
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        // Свет очень далеко (почти нет прямого освещения)
        Light light = new Light(new Vector3D(0, 1000, 0), 0.01f);
        Material material = new Material(0.2f, 0.0f, 0.0f, 1.0f);  // только ambient

        float intensity = Lighting.phong(surfacePoint, normal, viewDir, light, material, 1.0f);

        // Должно быть примерно ambient * ambientIntensity = 0.2 * 1.0
        assertTrue("Фоновое освещение ~0.2", intensity >= 0.19f && intensity <= 0.21f);
    }

    @Test
    public void testPhongDiffuse() {
        // Тест диффузного освещения
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        // Свет прямо сверху
        Light light = new Light(new Vector3D(0, 10, 0), 1.0f);
        Material material = Material.matte();  // матовый материал

        float intensity = Lighting.phong(surfacePoint, normal, viewDir, light, material, 0.0f);

        // Должно быть диффузное освещение (без зеркального)
        assertTrue("Диффузное освещение", intensity > 0.5f);
    }

    @Test
    public void testPhongSpecular() {
        // Тест зеркального отражения
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);  // смотрим сверху

        // Свет прямо сверху (идеальное отражение)
        Light light = new Light(new Vector3D(0, 10, 0), 1.0f);
        Material material = Material.metal();  // металл (сильное зеркальное отражение)

        float intensity = Lighting.phong(surfacePoint, normal, viewDir, light, material, 0.0f);

        // Должно быть сильное зеркальное отражение
        assertTrue("Зеркальное отражение", intensity > 0.7f);
    }

    @Test
    public void testPhongNoSpecularFromBack() {
        // Нет зеркального отражения, если свет с обратной стороны
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        // Свет снизу
        Light light = new Light(new Vector3D(0, -10, 0), 1.0f);
        Material material = Material.metal();

        float intensity = Lighting.phong(surfacePoint, normal, viewDir, light, material, 0.1f);

        // Только фоновое освещение
        assertTrue("Только ambient", intensity < 0.15f);
    }

    @Test
    public void testBlinnPhongVsPhong() {
        // Сравнение Блинна-Фонга и Фонга
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        Light light = new Light(new Vector3D(0, 10, 0), 1.0f);
        Material material = Material.plastic();

        float phong = Lighting.phong(surfacePoint, normal, viewDir, light, material, 0.1f);
        float blinnPhong = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 0.1f);

        // Результаты должны быть близкими, но не обязательно одинаковыми
        assertTrue("Phong и Blinn-Phong дают схожие результаты",
            Math.abs(phong - blinnPhong) < 0.3f);
    }

    @Test
    public void testBlinnPhongDirectLight() {
        // Блинн-Фонг с прямым освещением
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        Light light = new Light(new Vector3D(0, 5, 0), 1.0f);
        Material material = Material.plastic();

        float intensity = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 0.0f);

        assertTrue("Есть освещение", intensity > 0.5f);
    }

    @Test
    public void testMaterialMatte() {
        // Матовый материал не имеет зеркального отражения
        Material matte = Material.matte();

        assertEquals("Нет зеркального отражения", 0.0f, matte.specular, EPSILON);
        assertTrue("Сильное диффузное отражение", matte.diffuse > 0.5f);
    }

    @Test
    public void testMaterialMetal() {
        // Металлический материал имеет сильное зеркальное отражение
        Material metal = Material.metal();

        assertTrue("Сильное зеркальное отражение", metal.specular > 0.5f);
        assertTrue("Высокая степень блеска", metal.shininess > 50);
    }

    @Test
    public void testMaterialPlastic() {
        // Пластиковый материал - средние значения
        Material plastic = Material.plastic();

        assertTrue("Есть диффузное отражение", plastic.diffuse > 0);
        assertTrue("Есть зеркальное отражение", plastic.specular > 0);
        assertTrue("Средняя степень блеска", plastic.shininess > 10 && plastic.shininess < 100);
    }

    @Test
    public void testAttenuationNoDistance() {
        // Затухание на расстоянии 0 (максимум)
        float atten = Lighting.attenuation(0, 1.0f, 0.09f, 0.032f);

        assertEquals("Максимальное освещение", 1.0f, atten, EPSILON);
    }

    @Test
    public void testAttenuationWithDistance() {
        // Затухание увеличивается с расстоянием
        float atten1 = Lighting.attenuation(1, 1.0f, 0.09f, 0.032f);
        float atten5 = Lighting.attenuation(5, 1.0f, 0.09f, 0.032f);
        float atten10 = Lighting.attenuation(10, 1.0f, 0.09f, 0.032f);

        assertTrue("Затухание на расстоянии 1", atten1 < 1.0f);
        assertTrue("Затухание на расстоянии 5 < расстояния 1", atten5 < atten1);
        assertTrue("Затухание на расстоянии 10 < расстояния 5", atten10 < atten5);
    }

    @Test
    public void testAttenuationQuadratic() {
        // Квадратичное затухание более реалистично
        // С квадратичным затуханием
        float quadratic = Lighting.attenuation(10, 1.0f, 0.0f, 0.1f);

        // Без квадратичного затухания
        float linear = Lighting.attenuation(10, 1.0f, 0.1f, 0.0f);

        assertTrue("Квадратичное затухание сильнее", quadratic < linear);
    }

    @Test
    public void testLightCreation() {
        // Тест создания источника света
        Vector3D pos = new Vector3D(10, 20, 30);
        Light light = new Light(pos, 0.8f);

        assertEquals("Позиция X", 10, light.position.x, EPSILON);
        assertEquals("Позиция Y", 20, light.position.y, EPSILON);
        assertEquals("Позиция Z", 30, light.position.z, EPSILON);
        assertEquals("Интенсивность", 0.8f, light.intensity, EPSILON);
    }

    @Test
    public void testPhongClampToOne() {
        // Проверка, что итоговая интенсивность не превышает 1.0
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        // Очень яркий свет
        Light light = new Light(new Vector3D(0, 1, 0), 100.0f);
        Material material = new Material(1.0f, 1.0f, 1.0f, 1.0f);

        float intensity = Lighting.phong(surfacePoint, normal, viewDir, light, material, 10.0f);

        assertTrue("Интенсивность ≤ 1.0", intensity <= 1.0f);
    }
}
