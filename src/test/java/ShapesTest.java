import defaultimp.Circle;
import defaultimp.Square;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * Демонстрация работы с фигурами
 */
public class ShapesTest {

    /**
     * Проверка метода name
     */
    @Test
    public void testName() {
        Square square = new Square();
        assertEquals("Квадрат", square.getName());
        System.out.println("square.presentation() = " + square.presentation());
        Circle circle = new Circle(1.0, 0.5, 3);
        assertEquals("Круг", circle.getName());
        assertEquals("Круг  центр: (1.0; 0.5)  радиус = 3.0", circle.presentation());
        System.out.println("circle.presentation() = " + circle.presentation());
    }
}
