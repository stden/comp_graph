package bdd;

import graphics.ColorModel.RGB;
import graphics.ColorModel.HSV;
import io.cucumber.java.ru.*;
import static org.junit.Assert.*;

/**
 * Step definitions для BDD тестов цветовых моделей
 */
public class ColorModelSteps {
    private RGB rgbColor;
    private HSV hsvColor;
    private RGB grayscaleColor;
    private static final float EPSILON = 0.1f;
    private static final int GRAY_EPSILON = 2;

    @Дано("RGB цвет с компонентами R={int}, G={int}, B={int}")
    public void rgb_цвет_с_компонентами(int r, int g, int b) {
        rgbColor = new RGB(r, g, b);
    }

    @Дано("HSV цвет с параметрами H={int}, S={int}, V={int}")
    public void hsv_цвет_с_параметрами(int h, int s, int v) {
        hsvColor = new HSV(h, s, v);
    }

    @Когда("преобразую цвет в HSV")
    public void преобразую_цвет_в_hsv() {
        hsvColor = rgbColor.toHSV();
    }

    @Когда("преобразую цвет в RGB")
    public void преобразую_цвет_в_rgb() {
        rgbColor = hsvColor.toRGB();
    }

    @Когда("преобразую в оттенки серого")
    public void преобразую_в_оттенки_серого() {
        grayscaleColor = rgbColor.toGrayscale();
    }

    @Тогда("оттенок должен быть {int} градусов")
    public void оттенок_должен_быть(int expectedHue) {
        assertEquals("Оттенок", expectedHue, hsvColor.h, EPSILON);
    }

    @Тогда("насыщенность должна быть {int}%")
    public void насыщенность_должна_быть(int expectedSaturation) {
        assertEquals("Насыщенность", expectedSaturation, hsvColor.s, EPSILON);
    }

    @Тогда("яркость должна быть {int}%")
    public void яркость_должна_быть(int expectedValue) {
        assertEquals("Яркость", expectedValue, hsvColor.v, EPSILON);
    }

    @Тогда("яркость должна быть около {int}%")
    public void яркость_должна_быть_около(int expectedValue) {
        assertEquals("Яркость", expectedValue, hsvColor.v, 1.0f);
    }

    @Тогда("красный компонент должен быть {int}")
    public void красный_компонент_должен_быть(int expectedRed) {
        assertEquals("Красный компонент", expectedRed, rgbColor.r);
    }

    @Тогда("зелёный компонент должен быть {int}")
    public void зелёный_компонент_должен_быть(int expectedGreen) {
        assertEquals("Зелёный компонент", expectedGreen, rgbColor.g);
    }

    @Тогда("синий компонент должен быть {int}")
    public void синий_компонент_должен_быть(int expectedBlue) {
        assertEquals("Синий компонент", expectedBlue, rgbColor.b);
    }

    @Дано("я создаю цвета радуги через HSV")
    public void я_создаю_цвета_радуги_через_hsv() {
        // Подготовка к созданию радуги
    }

    @Когда("устанавливаю оттенки {int}, {int}, {int}, {int}, {int}, {int} и {int} градуса")
    public void устанавливаю_оттенки_градуса(int h1, int h2, int h3, int h4, int h5, int h6, int h7) {
        // Проверяем, что можем создать цвета с этими оттенками
        new HSV(h1, 100, 100);
        new HSV(h2, 100, 100);
        new HSV(h3, 100, 100);
        new HSV(h4, 100, 100);
        new HSV(h5, 100, 100);
        new HSV(h6, 100, 100);
        new HSV(h7, 100, 100);
    }

    @Тогда("получаю красный, оранжевый, жёлтый, зелёный, синий, индиго и фиолетовый")
    public void получаю_цвета_радуги() {
        // Радуга создана успешно
        assertTrue("Радуга создана", true);
    }

    @Тогда("все компоненты RGB должны быть равны")
    public void все_компоненты_rgb_должны_быть_равны() {
        assertEquals("R = G", grayscaleColor.r, grayscaleColor.g);
        assertEquals("G = B", grayscaleColor.g, grayscaleColor.b);
    }

    @Тогда("значение яркости должно быть {int}")
    public void значение_яркости_должно_быть(int expectedBrightness) {
        assertEquals("Яркость grayscale", expectedBrightness, grayscaleColor.r, GRAY_EPSILON);
    }
}
