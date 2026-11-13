package graphics;

import graphics.ColorModel.RGB;
import graphics.ColorModel.HSV;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Тесты для цветовых моделей RGB и HSV
 * Модуль 1: Основы компьютерной графики
 */
public class ColorModelTest {
    private static final float EPSILON = 0.1f;

    @Test
    public void testRGBCreation() {
        // Создание чистого красного цвета
        RGB red = new RGB(255, 0, 0);
        assertEquals("Красный компонент", 255, red.r);
        assertEquals("Зелёный компонент", 0, red.g);
        assertEquals("Синий компонент", 0, red.b);
    }

    @Test
    public void testRGBClamping() {
        // Проверка ограничения значений в диапазоне 0-255
        RGB color = new RGB(300, -10, 128);
        assertEquals("Красный обрезан до 255", 255, color.r);
        assertEquals("Зелёный обрезан до 0", 0, color.g);
        assertEquals("Синий в пределах нормы", 128, color.b);
    }

    @Test
    public void testRGBToHSV_Red() {
        // Чистый красный: RGB(255, 0, 0) → HSV(0°, 100%, 100%)
        RGB red = new RGB(255, 0, 0);
        HSV hsv = red.toHSV();

        assertEquals("Оттенок красного = 0°", 0, hsv.h, EPSILON);
        assertEquals("Насыщенность = 100%", 100, hsv.s, EPSILON);
        assertEquals("Яркость = 100%", 100, hsv.v, EPSILON);
    }

    @Test
    public void testRGBToHSV_Green() {
        // Чистый зелёный: RGB(0, 255, 0) → HSV(120°, 100%, 100%)
        RGB green = new RGB(0, 255, 0);
        HSV hsv = green.toHSV();

        assertEquals("Оттенок зелёного = 120°", 120, hsv.h, EPSILON);
        assertEquals("Насыщенность = 100%", 100, hsv.s, EPSILON);
        assertEquals("Яркость = 100%", 100, hsv.v, EPSILON);
    }

    @Test
    public void testRGBToHSV_Blue() {
        // Чистый синий: RGB(0, 0, 255) → HSV(240°, 100%, 100%)
        RGB blue = new RGB(0, 0, 255);
        HSV hsv = blue.toHSV();

        assertEquals("Оттенок синего = 240°", 240, hsv.h, EPSILON);
        assertEquals("Насыщенность = 100%", 100, hsv.s, EPSILON);
        assertEquals("Яркость = 100%", 100, hsv.v, EPSILON);
    }

    @Test
    public void testRGBToHSV_Gray() {
        // Серый цвет: RGB(128, 128, 128) → HSV(0°, 0%, ~50%)
        RGB gray = new RGB(128, 128, 128);
        HSV hsv = gray.toHSV();

        assertEquals("Насыщенность серого = 0%", 0, hsv.s, EPSILON);
        assertEquals("Яркость серого ~50%", 50.2, hsv.v, EPSILON);
    }

    @Test
    public void testHSVToRGB_Red() {
        // HSV(0°, 100%, 100%) → RGB(255, 0, 0)
        HSV hsv = new HSV(0, 100, 100);
        RGB rgb = hsv.toRGB();

        assertEquals("Красный", 255, rgb.r);
        assertEquals("Зелёный", 0, rgb.g);
        assertEquals("Синий", 0, rgb.b);
    }

    @Test
    public void testHSVToRGB_Yellow() {
        // Жёлтый: HSV(60°, 100%, 100%) → RGB(255, 255, 0)
        HSV hsv = new HSV(60, 100, 100);
        RGB rgb = hsv.toRGB();

        assertEquals("Красный", 255, rgb.r);
        assertEquals("Зелёный", 255, rgb.g);
        assertEquals("Синий", 0, rgb.b);
    }

    @Test
    public void testHSVToRGB_Cyan() {
        // Циан (голубой): HSV(180°, 100%, 100%) → RGB(0, 255, 255)
        HSV hsv = new HSV(180, 100, 100);
        RGB rgb = hsv.toRGB();

        assertEquals("Красный", 0, rgb.r);
        assertEquals("Зелёный", 255, rgb.g);
        assertEquals("Синий", 255, rgb.b);
    }

    @Test
    public void testRGBToHSVToRGB_RoundTrip() {
        // Проверка обратного преобразования RGB → HSV → RGB
        RGB original = new RGB(123, 234, 56);
        RGB converted = original.toHSV().toRGB();

        assertEquals("Красный совпадает", original.r, converted.r, 1);
        assertEquals("Зелёный совпадает", original.g, converted.g, 1);
        assertEquals("Синий совпадает", original.b, converted.b, 1);
    }

    @Test
    public void testGrayscale() {
        // Преобразование цветного изображения в оттенки серого
        // Используется взвешенная формула яркости
        RGB color = new RGB(100, 150, 200);
        RGB gray = color.toGrayscale();

        // Проверяем, что все компоненты равны (серый цвет)
        assertEquals("R = G", gray.r, gray.g);
        assertEquals("G = B", gray.g, gray.b);

        // Яркость: 0.2126*100 + 0.7152*150 + 0.0722*200 ≈ 143
        assertEquals("Яркость по формуле ITU-R BT.709", 143, gray.r, 1);
    }

    @Test
    public void testRGBEquality() {
        RGB color1 = new RGB(100, 150, 200);
        RGB color2 = new RGB(100, 150, 200);
        RGB color3 = new RGB(100, 150, 201);

        assertEquals("Одинаковые цвета равны", color1, color2);
        assertNotEquals("Разные цвета не равны", color1, color3);
    }

    @Test
    public void testRGBHashCode() {
        RGB color1 = new RGB(100, 150, 200);
        RGB color2 = new RGB(100, 150, 200);

        assertEquals("Одинаковые hashCode", color1.hashCode(), color2.hashCode());
    }

    @Test
    public void testHSVHueWrapping() {
        // Проверка, что оттенок циклический (0° = 360°)
        HSV hsv1 = new HSV(0, 50, 50);
        HSV hsv2 = new HSV(360, 50, 50);

        assertEquals("0° = 360°", hsv1.h, hsv2.h, EPSILON);
    }

    @Test
    public void testHSVClamping() {
        // Проверка ограничения насыщенности и яркости
        HSV hsv = new HSV(200, 150, -10);

        assertEquals("Насыщенность обрезана до 100%", 100, hsv.s, EPSILON);
        assertEquals("Яркость обрезана до 0%", 0, hsv.v, EPSILON);
    }

    @Test
    public void testRGBToString() {
        RGB color = new RGB(255, 128, 64);
        assertEquals("Формат вывода RGB", "RGB(255, 128, 64)", color.toString());
    }

    @Test
    public void testHSVToString() {
        HSV color = new HSV(180, 75.5f, 50.2f);
        assertEquals("Формат вывода HSV", "HSV(180.0°, 75.5%, 50.2%)", color.toString());
    }

    // ============ ЯРКИЕ И НЕОБЫЧНЫЕ ТЕСТЫ ============

    @Test
    public void testRainbowColors() {
        // Проверка полного спектра радуги (7 цветов)
        RGB[] rainbow = {
            new RGB(255, 0, 0),     // Красный
            new RGB(255, 127, 0),   // Оранжевый
            new RGB(255, 255, 0),   // Жёлтый
            new RGB(0, 255, 0),     // Зелёный
            new RGB(0, 0, 255),     // Синий
            new RGB(75, 0, 130),    // Индиго
            new RGB(148, 0, 211)    // Фиолетовый
        };

        float[] expectedHues = {0, 30, 60, 120, 240, 275, 282};

        for (int i = 0; i < rainbow.length; i++) {
            HSV hsv = rainbow[i].toHSV();
            assertEquals("Оттенок радуги #" + i, expectedHues[i], hsv.h, 2);
        }
    }

    @Test
    public void testColorTemperature() {
        // Проверка тёплых и холодных цветов через насыщенность
        RGB warmRed = new RGB(255, 100, 50);     // Тёплый
        RGB coolBlue = new RGB(50, 100, 255);    // Холодный
        RGB neutral = new RGB(150, 150, 150);    // Нейтральный

        HSV warmHSV = warmRed.toHSV();
        HSV coolHSV = coolBlue.toHSV();
        HSV neutralHSV = neutral.toHSV();

        assertTrue("Тёплый цвет насыщенный", warmHSV.s > 50);
        assertTrue("Холодный цвет насыщенный", coolHSV.s > 50);
        assertTrue("Нейтральный цвет ненасыщенный", neutralHSV.s < 10);

        // Проверка оттенков: тёплый (0-60), холодный (180-300)
        assertTrue("Тёплый оттенок в красно-жёлтом диапазоне", warmHSV.h < 60);
        assertTrue("Холодный оттенок в сине-фиолетовом диапазоне",
                   coolHSV.h > 180 && coolHSV.h < 300);
    }

    @Test
    public void testPastelColors() {
        // Пастельные цвета = высокая яркость + низкая насыщенность
        RGB pastelPink = new RGB(255, 209, 220);
        RGB pastelBlue = new RGB(174, 198, 207);
        RGB pastelYellow = new RGB(253, 253, 150);

        HSV[] pastels = {
            pastelPink.toHSV(),
            pastelBlue.toHSV(),
            pastelYellow.toHSV()
        };

        for (HSV hsv : pastels) {
            assertTrue("Пастельный цвет имеет высокую яркость", hsv.v > 60);
            assertTrue("Пастельный цвет имеет низкую насыщенность", hsv.s < 60);
        }
    }

    @Test
    public void testNeonColors() {
        // Неоновые цвета = максимальная насыщенность + яркость
        RGB neonGreen = new RGB(57, 255, 20);
        RGB neonPink = new RGB(255, 16, 240);
        RGB neonOrange = new RGB(255, 95, 0);  // Чистый оранжевый

        HSV[] neons = {
            neonGreen.toHSV(),
            neonPink.toHSV(),
            neonOrange.toHSV()
        };

        for (HSV hsv : neons) {
            assertTrue("Неоновый цвет максимально насыщен", hsv.s > 90);
            assertTrue("Неоновый цвет очень яркий", hsv.v > 90);
        }
    }

    @Test
    public void testGradientSmooth() {
        // Проверка плавности градиента от чёрного к белому
        int steps = 10;
        RGB[] gradient = new RGB[steps];

        for (int i = 0; i < steps; i++) {
            int value = (255 * i) / (steps - 1);
            gradient[i] = new RGB(value, value, value);
        }

        // Проверяем монотонное возрастание яркости
        for (int i = 1; i < steps; i++) {
            HSV prev = gradient[i-1].toHSV();
            HSV curr = gradient[i].toHSV();
            assertTrue("Градиент монотонно возрастает", curr.v >= prev.v);
            assertEquals("Градиент без насыщенности", 0, curr.s, EPSILON);
        }
    }

    @Test
    public void testHueCircle360Degrees() {
        // Полный круг оттенков с шагом 30° при макс. насыщенности и яркости
        int steps = 12;
        for (int i = 0; i < steps; i++) {
            float hue = (360.0f * i) / steps;
            HSV hsv = new HSV(hue, 100, 100);
            RGB rgb = hsv.toRGB();

            // Обратное преобразование должно вернуть тот же оттенок
            HSV back = rgb.toHSV();
            assertEquals("Оттенок сохранился после круга", hue % 360, back.h, 2);
        }
    }

    @Test
    public void testComplementaryColors() {
        // Дополнительные (комплементарные) цвета: разница в оттенке = 180°
        RGB red = new RGB(255, 0, 0);
        RGB cyan = new RGB(0, 255, 255);

        HSV redHSV = red.toHSV();
        HSV cyanHSV = cyan.toHSV();

        float hueDiff = Math.abs(redHSV.h - cyanHSV.h);
        assertEquals("Комплементарные цвета отличаются на 180°", 180, hueDiff, 2);
    }

    @Test
    public void testMonochromaticScale() {
        // Монохромная шкала одного оттенка с разной яркостью
        float baseHue = 210; // Синий
        RGB[] monochromes = new RGB[5];

        for (int i = 0; i < 5; i++) {
            float brightness = 20 + (i * 20); // 20, 40, 60, 80, 100
            monochromes[i] = new HSV(baseHue, 100, brightness).toRGB();
        }

        // Все должны иметь одинаковый оттенок
        for (RGB rgb : monochromes) {
            HSV hsv = rgb.toHSV();
            assertEquals("Монохром сохраняет оттенок", baseHue, hsv.h, 2);
        }
    }

    @Test
    public void testSkinToneRange() {
        // Реалистичные оттенки кожи человека
        RGB[] skinTones = {
            new RGB(255, 224, 189),  // Светлый
            new RGB(241, 194, 125),  // Средний
            new RGB(198, 134, 66),   // Оливковый
            new RGB(141, 85, 36),    // Тёмный
            new RGB(58, 29, 15)      // Очень тёмный
        };

        for (RGB skin : skinTones) {
            HSV hsv = skin.toHSV();
            // Оттенки кожи обычно в диапазоне 0-50° (красно-оранжевый)
            assertTrue("Оттенок кожи в красно-оранжевом спектре",
                       hsv.h >= 0 && hsv.h <= 50);
            assertTrue("Насыщенность кожи умеренная", hsv.s < 80);
        }
    }

    @Test
    public void testWebSafeColors() {
        // Web-безопасные цвета (216 цветов): RGB компоненты = 0, 51, 102, 153, 204, 255
        int[] safeValues = {0, 51, 102, 153, 204, 255};

        RGB webSafe1 = new RGB(51, 102, 153);
        RGB webSafe2 = new RGB(204, 51, 255);

        // Проверка обратного преобразования
        RGB converted1 = webSafe1.toHSV().toRGB();
        RGB converted2 = webSafe2.toHSV().toRGB();

        assertEquals("Web-safe R1", webSafe1.r, converted1.r, 2);
        assertEquals("Web-safe G1", webSafe1.g, converted1.g, 2);
        assertEquals("Web-safe B1", webSafe1.b, converted1.b, 2);
    }

    @Test
    public void testExtremeSaturation() {
        // Экстремальные значения насыщенности
        HSV noSaturation = new HSV(180, 0, 75);    // Серый
        HSV maxSaturation = new HSV(180, 100, 75); // Яркий циан

        RGB gray = noSaturation.toRGB();
        RGB vivid = maxSaturation.toRGB();

        // При S=0 все компоненты RGB должны быть равны
        assertEquals("Нулевая насыщенность -> R=G", gray.r, gray.g);
        assertEquals("Нулевая насыщенность -> G=B", gray.g, gray.b);

        // При S=100 должна быть разница в компонентах
        assertTrue("Макс. насыщенность -> разные компоненты",
                   vivid.r != vivid.g || vivid.g != vivid.b);
    }

    @Test
    public void testColorBlindnessSimulation() {
        // Имитация дальтонизма (протанопия): красный → серый
        RGB red = new RGB(255, 0, 0);
        RGB green = new RGB(0, 255, 0);

        // Преобразуем в grayscale для имитации ахроматопсии
        RGB redGray = red.toGrayscale();
        RGB greenGray = green.toGrayscale();

        // Красный должен быть темнее зелёного в grayscale
        assertTrue("Красный темнее зелёного в ч/б", redGray.r < greenGray.r);
        assertEquals("Grayscale красного ~54", 54, redGray.r, 2);
        assertEquals("Grayscale зелёного ~182", 182, greenGray.r, 2);
    }

    // ============ ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ ПОКРЫТИЯ МУТАЦИЙ ============

    @Test
    public void testHSVToRGBAllHueSectors() {
        // Тест всех 6 секторов оттенков в HSV
        // Сектор 0: 0-60 (красный → жёлтый)
        HSV sector0 = new HSV(30, 100, 100);
        RGB rgb0 = sector0.toRGB();
        assertEquals("Сектор 0: R = 255", 255, rgb0.r);
        assertTrue("Сектор 0: G > 0", rgb0.g > 0);
        assertEquals("Сектор 0: B = 0", 0, rgb0.b);

        // Сектор 1: 60-120 (жёлтый → зелёный)
        HSV sector1 = new HSV(90, 100, 100);
        RGB rgb1 = sector1.toRGB();
        assertTrue("Сектор 1: R > 0", rgb1.r > 0);
        assertEquals("Сектор 1: G = 255", 255, rgb1.g);
        assertEquals("Сектор 1: B = 0", 0, rgb1.b);

        // Сектор 2: 120-180 (зелёный → циан)
        HSV sector2 = new HSV(150, 100, 100);
        RGB rgb2 = sector2.toRGB();
        assertEquals("Сектор 2: R = 0", 0, rgb2.r);
        assertEquals("Сектор 2: G = 255", 255, rgb2.g);
        assertTrue("Сектор 2: B > 0", rgb2.b > 0);

        // Сектор 3: 180-240 (циан → синий)
        HSV sector3 = new HSV(210, 100, 100);
        RGB rgb3 = sector3.toRGB();
        assertEquals("Сектор 3: R = 0", 0, rgb3.r);
        assertTrue("Сектор 3: G > 0", rgb3.g > 0);
        assertEquals("Сектор 3: B = 255", 255, rgb3.b);

        // Сектор 4: 240-300 (синий → пурпурный)
        HSV sector4 = new HSV(270, 100, 100);
        RGB rgb4 = sector4.toRGB();
        assertTrue("Сектор 4: R > 0", rgb4.r > 0);
        assertEquals("Сектор 4: G = 0", 0, rgb4.g);
        assertEquals("Сектор 4: B = 255", 255, rgb4.b);

        // Сектор 5: 300-360 (пурпурный → красный)
        HSV sector5 = new HSV(330, 100, 100);
        RGB rgb5 = sector5.toRGB();
        assertEquals("Сектор 5: R = 255", 255, rgb5.r);
        assertEquals("Сектор 5: G = 0", 0, rgb5.g);
        assertTrue("Сектор 5: B > 0", rgb5.b > 0);
    }

    @Test
    public void testHSVToRGBEdgeCases() {
        // Граничные значения оттенков
        HSV h0 = new HSV(0, 100, 100);
        HSV h60 = new HSV(60, 100, 100);
        HSV h120 = new HSV(120, 100, 100);
        HSV h180 = new HSV(180, 100, 100);
        HSV h240 = new HSV(240, 100, 100);
        HSV h300 = new HSV(300, 100, 100);
        HSV h359 = new HSV(359, 100, 100);

        assertNotNull("H=0", h0.toRGB());
        assertNotNull("H=60", h60.toRGB());
        assertNotNull("H=120", h120.toRGB());
        assertNotNull("H=180", h180.toRGB());
        assertNotNull("H=240", h240.toRGB());
        assertNotNull("H=300", h300.toRGB());
        assertNotNull("H=359", h359.toRGB());
    }

    @Test
    public void testHSVEquality() {
        HSV hsv1 = new HSV(180, 50, 75);
        HSV hsv2 = new HSV(180, 50, 75);
        HSV hsv3 = new HSV(181, 50, 75);

        assertEquals("Одинаковые HSV равны", hsv1, hsv2);
        assertNotEquals("Разные HSV не равны", hsv1, hsv3);
    }

    @Test
    public void testHSVEqualityWithNull() {
        HSV hsv = new HSV(180, 50, 75);
        assertFalse("HSV не равен null", hsv.equals(null));
        assertFalse("HSV не равен строке", hsv.equals("HSV"));
    }

    @Test
    public void testRGBEqualityWithNull() {
        RGB rgb = new RGB(100, 150, 200);
        assertFalse("RGB не равен null", rgb.equals(null));
        assertFalse("RGB не равен строке", rgb.equals("RGB"));
    }

    @Test
    public void testRGBToHSVBlack() {
        // Чёрный цвет: RGB(0, 0, 0) → HSV(0°, 0%, 0%)
        RGB black = new RGB(0, 0, 0);
        HSV hsv = black.toHSV();

        assertEquals("Чёрный: насыщенность = 0%", 0, hsv.s, EPSILON);
        assertEquals("Чёрный: яркость = 0%", 0, hsv.v, EPSILON);
    }

    @Test
    public void testRGBToHSVWhite() {
        // Белый цвет: RGB(255, 255, 255) → HSV(0°, 0%, 100%)
        RGB white = new RGB(255, 255, 255);
        HSV hsv = white.toHSV();

        assertEquals("Белый: насыщенность = 0%", 0, hsv.s, EPSILON);
        assertEquals("Белый: яркость = 100%", 100, hsv.v, EPSILON);
    }

    @Test
    public void testRGBToHSVMagenta() {
        // Пурпурный: RGB(255, 0, 255) → HSV(300°, 100%, 100%)
        RGB magenta = new RGB(255, 0, 255);
        HSV hsv = magenta.toHSV();

        assertEquals("Пурпурный: оттенок = 300°", 300, hsv.h, EPSILON);
        assertEquals("Пурпурный: насыщенность = 100%", 100, hsv.s, EPSILON);
    }

    @Test
    public void testHSVToRGBLowSaturation() {
        // Низкая насыщенность - почти серый
        HSV lowSat = new HSV(120, 10, 80);
        RGB rgb = lowSat.toRGB();

        // При низкой насыщенности R, G, B должны быть близки друг к другу
        int maxDiff = Math.max(Math.abs(rgb.r - rgb.g),
                      Math.max(Math.abs(rgb.g - rgb.b), Math.abs(rgb.r - rgb.b)));
        assertTrue("Низкая насыщенность - близкие значения RGB", maxDiff < 50);
    }

    @Test
    public void testHSVToRGBLowBrightness() {
        // Низкая яркость - тёмный цвет
        HSV lowVal = new HSV(240, 100, 20);
        RGB rgb = lowVal.toRGB();

        assertTrue("Низкая яркость - все компоненты < 60", rgb.r < 60 && rgb.g < 60 && rgb.b < 60);
    }

    @Test
    public void testRGBToHSVYellow() {
        // Жёлтый: RGB(255, 255, 0) → HSV(60°, 100%, 100%)
        RGB yellow = new RGB(255, 255, 0);
        HSV hsv = yellow.toHSV();

        assertEquals("Жёлтый: оттенок = 60°", 60, hsv.h, EPSILON);
    }

    @Test
    public void testRGBToHSVCyan() {
        // Циан: RGB(0, 255, 255) → HSV(180°, 100%, 100%)
        RGB cyan = new RGB(0, 255, 255);
        HSV hsv = cyan.toHSV();

        assertEquals("Циан: оттенок = 180°", 180, hsv.h, EPSILON);
    }

    @Test
    public void testRGBToHSVOrange() {
        // Оранжевый: проверка вычисления оттенка когда max = R
        RGB orange = new RGB(255, 128, 0);
        HSV hsv = orange.toHSV();

        assertTrue("Оранжевый: оттенок между 0 и 60", hsv.h > 0 && hsv.h < 60);
    }

    @Test
    public void testRGBToHSVLime() {
        // Лайм: проверка вычисления оттенка когда max = G
        RGB lime = new RGB(128, 255, 0);
        HSV hsv = lime.toHSV();

        assertTrue("Лайм: оттенок между 60 и 120", hsv.h > 60 && hsv.h < 120);
    }

    @Test
    public void testRGBToHSVPurple() {
        // Фиолетовый: проверка вычисления оттенка когда max = B
        RGB purple = new RGB(128, 0, 255);
        HSV hsv = purple.toHSV();

        assertTrue("Фиолетовый: оттенок между 240 и 300", hsv.h > 240 && hsv.h < 300);
    }

    @Test
    public void testGrayscalePreservesGray() {
        // Серый цвет должен остаться серым
        RGB gray = new RGB(128, 128, 128);
        RGB result = gray.toGrayscale();

        assertEquals("Серый остаётся серым", gray.r, result.r);
    }

    @Test
    public void testHSVToRGBRoundTrip() {
        // Полный круг HSV → RGB → HSV
        float[] hues = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330};

        for (float hue : hues) {
            HSV original = new HSV(hue, 80, 90);
            RGB rgb = original.toRGB();
            HSV back = rgb.toHSV();

            assertEquals("Оттенок сохранён для H=" + hue, hue, back.h, 2);
        }
    }
}
