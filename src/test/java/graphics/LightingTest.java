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

    // ============ ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ МУТАЦИОННОГО ПОКРЫТИЯ ============

    @Test
    public void testLambertVariousAngles() {
        Vector3D normal = new Vector3D(0, 1, 0);

        // 0° - максимум
        Vector3D light0 = new Vector3D(0, 1, 0);
        assertEquals("0°: макс", 1.0f, Lighting.lambert(normal, light0, 1.0f, 1.0f), EPSILON);

        // 90° - ноль
        Vector3D light90 = new Vector3D(1, 0, 0);
        assertEquals("90°: ноль", 0.0f, Lighting.lambert(normal, light90, 1.0f, 1.0f), EPSILON);

        // 60° - половина
        Vector3D light60 = new Vector3D(0, 0.5f, 0.866f);
        light60.normalize();
        float intensity60 = Lighting.lambert(normal, light60, 1.0f, 1.0f);
        assertTrue("60°: половина", intensity60 > 0.4f && intensity60 < 0.6f);
    }

    @Test
    public void testPhongSpecularHighlightPosition() {
        // Зеркальное отражение зависит от позиции наблюдателя
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Light light = new Light(new Vector3D(1, 10, 0), 1.0f);
        Material material = Material.metal();

        // Наблюдатель на линии отражения
        Vector3D viewDirGood = new Vector3D(-1, 10, 0);
        viewDirGood.normalize();
        float intensityGood = Lighting.phong(surfacePoint, normal, viewDirGood, light, material, 0.0f);

        // Наблюдатель в стороне
        Vector3D viewDirBad = new Vector3D(10, 1, 0);
        viewDirBad.normalize();
        float intensityBad = Lighting.phong(surfacePoint, normal, viewDirBad, light, material, 0.0f);

        assertTrue("Отражение видно с правильной позиции", intensityGood > intensityBad);
    }

    @Test
    public void testPhongDifferentShininess() {
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0.1f, 1, 0);
        viewDir.normalize();

        Light light = new Light(new Vector3D(-0.1f, 10, 0), 1.0f);

        // Низкий блеск - широкое пятно
        Material lowShine = new Material(0, 0.5f, 0.5f, 1.0f);
        float lowIntensity = Lighting.phong(surfacePoint, normal, viewDir, light, lowShine, 0.0f);

        // Высокий блеск - узкое пятно
        Material highShine = new Material(0, 0.5f, 0.5f, 128.0f);
        float highIntensity = Lighting.phong(surfacePoint, normal, viewDir, light, highShine, 0.0f);

        assertNotEquals("Разные степени блеска", lowIntensity, highIntensity, 0.01f);
    }

    @Test
    public void testBlinnPhongNoSpecularFromBack() {
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        // Свет снизу
        Light light = new Light(new Vector3D(0, -10, 0), 1.0f);
        Material material = Material.metal();

        float intensity = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 0.1f);

        // Только фоновое освещение
        assertTrue("Только ambient от света сзади", intensity < 0.15f);
    }

    @Test
    public void testBlinnPhongHalfwayVector() {
        // Блинн-Фонг использует halfway вектор
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        Light light = new Light(new Vector3D(0, 10, 0), 1.0f);
        Material material = Material.metal();

        float intensity = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 0.0f);

        assertTrue("Яркое зеркальное отражение", intensity > 0.7f);
    }

    @Test
    public void testMaterialCustomValues() {
        Material custom = new Material(0.3f, 0.4f, 0.5f, 64.0f);

        assertEquals("Ambient", 0.3f, custom.ambient, EPSILON);
        assertEquals("Diffuse", 0.4f, custom.diffuse, EPSILON);
        assertEquals("Specular", 0.5f, custom.specular, EPSILON);
        assertEquals("Shininess", 64.0f, custom.shininess, EPSILON);
    }

    @Test
    public void testAttenuationLinear() {
        // Только линейное затухание
        float atten1 = Lighting.attenuation(1, 1.0f, 1.0f, 0.0f);
        float atten2 = Lighting.attenuation(2, 1.0f, 1.0f, 0.0f);

        // 1/(1 + 1*1) = 0.5
        assertEquals("Затухание на d=1", 0.5f, atten1, EPSILON);
        // 1/(1 + 1*2) = 0.333
        assertEquals("Затухание на d=2", 0.333f, atten2, 0.01f);
    }

    @Test
    public void testAttenuationConstant() {
        // Только константное затухание
        float atten = Lighting.attenuation(100, 2.0f, 0.0f, 0.0f);

        // 1/2 = 0.5 (расстояние не влияет)
        assertEquals("Константное затухание", 0.5f, atten, EPSILON);
    }

    @Test
    public void testLightIntensityEffect() {
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D lightDir = new Vector3D(0, 1, 0);

        float low = Lighting.lambert(normal, lightDir, 0.5f, 1.0f);
        float high = Lighting.lambert(normal, lightDir, 1.0f, 1.0f);

        assertEquals("Интенсивность 0.5", 0.5f, low, EPSILON);
        assertEquals("Интенсивность 1.0", 1.0f, high, EPSILON);
    }

    @Test
    public void testPhongLightDirection() {
        // Проверка нормализации направления света
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        // Свет на разных расстояниях должен давать похожий результат
        Light lightNear = new Light(new Vector3D(0, 5, 0), 1.0f);
        Light lightFar = new Light(new Vector3D(0, 100, 0), 1.0f);

        Material material = Material.matte();

        float intensityNear = Lighting.phong(surfacePoint, normal, viewDir, lightNear, material, 0.0f);
        float intensityFar = Lighting.phong(surfacePoint, normal, viewDir, lightFar, material, 0.0f);

        // Направление одинаковое, результат близкий
        assertEquals("Направление нормализовано", intensityNear, intensityFar, 0.1f);
    }

    @Test
    public void testBlinnPhongClampToOne() {
        Vector3D surfacePoint = new Vector3D(0, 0, 0);
        Vector3D normal = new Vector3D(0, 1, 0);
        Vector3D viewDir = new Vector3D(0, 1, 0);

        Light light = new Light(new Vector3D(0, 1, 0), 100.0f);
        Material material = new Material(1.0f, 1.0f, 1.0f, 1.0f);

        float intensity = Lighting.blinnPhong(surfacePoint, normal, viewDir, light, material, 10.0f);

        assertTrue("Интенсивность ≤ 1.0", intensity <= 1.0f);
    }

    @Test
    public void testLightPositionCopy() {
        // Проверка что позиция копируется
        Vector3D pos = new Vector3D(10, 20, 30);
        Light light = new Light(pos, 1.0f);

        // Изменяем исходную позицию
        pos.x = 999;

        // Позиция света не должна измениться
        assertEquals("Позиция скопирована", 10, light.position.x, EPSILON);
    }
}
