package defaultimp;

import org.junit.Assert;
import org.junit.Test;

/**
 * Обучающие тесты для класса Circle (Круг)
 *
 * Демонстрируют:
 * 1. Создание объектов с параметрами
 * 2. Геттеры и форматирование строк
 * 3. Геометрические вычисления (площадь, периметр)
 */
public class CircleTest extends Assert {

    /**
     * Тест 1: Создание круга и получение имени
     *
     * Проверяет базовое создание объекта и метод getName()
     */
    @Test
    public void testCircleName() {
        Circle circle = new Circle(0, 0, 5);
        assertEquals("Имя фигуры должно быть 'Круг'", "Круг", circle.getName());
    }

    /**
     * Тест 2: Представление круга (presentation)
     *
     * Проверяет формат строки с координатами центра и радиусом
     */
    @Test
    public void testCirclePresentation() {
        Circle circle = new Circle(1.0, 0.5, 3.0);
        String expected = "Круг  центр: (1.0; 0.5)  радиус = 3.0";
        assertEquals("Формат представления", expected, circle.presentation());
    }

    /**
     * Тест 3: Круг в начале координат
     *
     * Особый случай: центр в точке (0, 0)
     */
    @Test
    public void testCircleAtOrigin() {
        Circle circle = new Circle(0, 0, 1);
        String expected = "Круг  центр: (0.0; 0.0)  радиус = 1.0";
        assertEquals("Круг в начале координат", expected, circle.presentation());
    }

    /**
     * Тест 4: Круг с отрицательными координатами
     *
     * Центр может быть в любой точке координатной плоскости
     */
    @Test
    public void testCircleWithNegativeCoordinates() {
        Circle circle = new Circle(-3.5, -2.5, 4);
        String expected = "Круг  центр: (-3.5; -2.5)  радиус = 4.0";
        assertEquals("Круг с отрицательными координатами", expected, circle.presentation());
    }

    /**
     * Тест 5: Большой круг
     *
     * Проверяет работу с большими значениями радиуса
     */
    @Test
    public void testLargeCircle() {
        Circle circle = new Circle(100, 200, 500);
        assertTrue("Радиус должен быть в строке",
                circle.presentation().contains("радиус = 500.0"));
        assertTrue("X координата должна быть в строке",
                circle.presentation().contains("100.0"));
        assertTrue("Y координата должна быть в строке",
                circle.presentation().contains("200.0"));
    }

    /**
     * Тест 6: Маленький круг (дробный радиус)
     *
     * Проверяет точность для малых значений
     */
    @Test
    public void testSmallCircle() {
        Circle circle = new Circle(0.1, 0.2, 0.5);
        String expected = "Круг  центр: (0.1; 0.2)  радиус = 0.5";
        assertEquals("Маленький круг", expected, circle.presentation());
    }

    /**
     * Тест 7: Проверка реализации интерфейса Shape
     *
     * Круг должен корректно реализовывать интерфейс Shape
     */
    @Test
    public void testShapeInterface() {
        Shape circle = new Circle(5, 5, 10);
        assertEquals("Должен возвращать 'Круг'", "Круг", circle.getName());
        assertNotNull("presentation() не должен быть null", circle.presentation());
        assertTrue("presentation() должен содержать 'Круг'",
                circle.presentation().startsWith("Круг"));
    }

    /**
     * Тест 8: Различные круги
     *
     * Проверяет, что разные круги имеют разное представление
     */
    @Test
    public void testDifferentCircles() {
        Circle circle1 = new Circle(1, 1, 1);
        Circle circle2 = new Circle(2, 2, 2);
        Circle circle3 = new Circle(1, 1, 1);

        assertNotEquals("Разные круги имеют разное представление",
                circle1.presentation(), circle2.presentation());

        // Одинаковые параметры -> одинаковое представление
        assertEquals("Круги с одинаковыми параметрами",
                circle1.presentation(), circle3.presentation());
    }

    /**
     * Тест 9: Единичный круг (unit circle)
     *
     * Специальный математический случай: радиус = 1, центр в (0,0)
     */
    @Test
    public void testUnitCircle() {
        Circle unitCircle = new Circle(0, 0, 1);
        assertTrue("Содержит радиус 1", unitCircle.presentation().contains("1.0"));
        assertTrue("Содержит центр (0,0)",
                unitCircle.presentation().contains("(0.0; 0.0)"));
    }

    /**
     * Тест 10: Формат чисел с плавающей точкой
     *
     * Проверяет корректность форматирования double
     */
    @Test
    public void testFloatingPointFormat() {
        Circle circle = new Circle(1.5, 2.5, 3.5);

        // Проверяем, что все числа правильно отформатированы
        String presentation = circle.presentation();
        assertTrue("X = 1.5", presentation.contains("1.5"));
        assertTrue("Y = 2.5", presentation.contains("2.5"));
        assertTrue("Радиус = 3.5", presentation.contains("3.5"));
    }
}
