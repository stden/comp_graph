package math3d;

import org.junit.Assert;
import org.junit.Test;

/**
 * Обучающие тесты для 3D векторов
 *
 * Демонстрируют:
 * 1. Базовые операции с векторами (сложение, вычитание, масштабирование)
 * 2. Нормализацию векторов
 * 3. Скалярное и векторное произведение
 * 4. Работу с расстояниями
 */
public class Vector3DTest extends Assert {

    private static final float EPSILON = 0.0001f;

    /**
     * Вспомогательный метод для сравнения float с погрешностью
     */
    private void assertFloatEquals(String message, float expected, float actual) {
        assertTrue(message + " expected: " + expected + " but was: " + actual,
                Math.abs(expected - actual) < EPSILON);
    }

    /**
     * Тест 1: Конструктор по умолчанию создаёт нулевой вектор
     *
     * Обучающая цель: понять инициализацию векторов
     */
    @Test
    public void testDefaultConstructor() {
        Vector3D v = new Vector3D();
        assertEquals("X должен быть 0", 0.0f, v.x, EPSILON);
        assertEquals("Y должен быть 0", 0.0f, v.y, EPSILON);
        assertEquals("Z должен быть 0", 0.0f, v.z, EPSILON);
        assertEquals("Длина нулевого вектора = 0", 0.0f, v.length(), EPSILON);
    }

    /**
     * Тест 2: Конструктор с координатами
     *
     * Создание вектора с заданными значениями
     */
    @Test
    public void testConstructorWithCoordinates() {
        Vector3D v = new Vector3D(1.0f, 2.0f, 3.0f);
        assertEquals(1.0f, v.x, EPSILON);
        assertEquals(2.0f, v.y, EPSILON);
        assertEquals(3.0f, v.z, EPSILON);
    }

    /**
     * Тест 3: Конструктор копирования
     *
     * Создание независимой копии вектора
     */
    @Test
    public void testCopyConstructor() {
        Vector3D original = new Vector3D(1.0f, 2.0f, 3.0f);
        Vector3D copy = new Vector3D(original);

        assertEquals(original.x, copy.x, EPSILON);
        assertEquals(original.y, copy.y, EPSILON);
        assertEquals(original.z, copy.z, EPSILON);

        // Изменение копии не влияет на оригинал
        copy.x = 10.0f;
        assertEquals("Оригинал не изменился", 1.0f, original.x, EPSILON);
    }

    /**
     * Тест 4: Сложение векторов
     *
     * Геометрический смысл: перемещение из точки A в точку B
     * (1,2,3) + (4,5,6) = (5,7,9)
     */
    @Test
    public void testAdd() {
        Vector3D v1 = new Vector3D(1.0f, 2.0f, 3.0f);
        Vector3D v2 = new Vector3D(4.0f, 5.0f, 6.0f);

        v1.add(v2);

        assertEquals("X: 1 + 4 = 5", 5.0f, v1.x, EPSILON);
        assertEquals("Y: 2 + 5 = 7", 7.0f, v1.y, EPSILON);
        assertEquals("Z: 3 + 6 = 9", 9.0f, v1.z, EPSILON);
    }

    /**
     * Тест 5: Вычитание векторов
     *
     * Геометрический смысл: вектор из точки B в точку A
     * (5,7,9) - (1,2,3) = (4,5,6)
     */
    @Test
    public void testSubtract() {
        Vector3D v1 = new Vector3D(5.0f, 7.0f, 9.0f);
        Vector3D v2 = new Vector3D(1.0f, 2.0f, 3.0f);

        v1.sub(v2);

        assertEquals(4.0f, v1.x, EPSILON);
        assertEquals(5.0f, v1.y, EPSILON);
        assertEquals(6.0f, v1.z, EPSILON);
    }

    /**
     * Тест 6: Масштабирование вектора
     *
     * Умножение на скаляр изменяет длину, но не направление
     * (1,2,3) * 2 = (2,4,6)
     */
    @Test
    public void testScale() {
        Vector3D v = new Vector3D(1.0f, 2.0f, 3.0f);
        v.scale(2.0f);

        assertEquals(2.0f, v.x, EPSILON);
        assertEquals(4.0f, v.y, EPSILON);
        assertEquals(6.0f, v.z, EPSILON);
    }

    /**
     * Тест 7: Инверсия вектора
     *
     * Меняет направление на противоположное
     * (1,2,3) → (-1,-2,-3)
     */
    @Test
    public void testInverse() {
        Vector3D v = new Vector3D(1.0f, 2.0f, 3.0f);
        v.inverse();

        assertEquals(-1.0f, v.x, EPSILON);
        assertEquals(-2.0f, v.y, EPSILON);
        assertEquals(-3.0f, v.z, EPSILON);
    }

    /**
     * Тест 8: Вычисление длины вектора
     *
     * Длина вектора (3,4,0) = √(3² + 4² + 0²) = √25 = 5
     * Это трёхмерный вариант теоремы Пифагора
     */
    @Test
    public void testLength() {
        Vector3D v = new Vector3D(3.0f, 4.0f, 0.0f);
        float length = v.length();
        assertEquals("√(9+16+0) = 5", 5.0f, length, EPSILON);
    }

    /**
     * Тест 9: Длина вектора в 3D
     *
     * Вектор (1,2,2): длина = √(1² + 2² + 2²) = √9 = 3
     */
    @Test
    public void testLength3D() {
        Vector3D v = new Vector3D(1.0f, 2.0f, 2.0f);
        float length = v.length();
        assertEquals("√(1+4+4) = 3", 3.0f, length, EPSILON);
    }

    /**
     * Тест 10: Нормализация вектора
     *
     * Нормализация приводит вектор к единичной длине (1.0)
     * при сохранении направления
     */
    @Test
    public void testNormalize() {
        Vector3D v = new Vector3D(3.0f, 4.0f, 0.0f);
        v.normalize();

        // Длина должна стать равна 1
        assertFloatEquals("Длина после нормализации = 1", 1.0f, v.length());

        // Направление сохраняется (пропорции)
        assertFloatEquals("X компонента", 0.6f, v.x);  // 3/5 = 0.6
        assertFloatEquals("Y компонента", 0.8f, v.y);  // 4/5 = 0.8
        assertFloatEquals("Z компонента", 0.0f, v.z);
    }

    /**
     * Тест 11: Нормализация нулевого вектора
     *
     * Нулевой вектор нельзя нормализовать - он остаётся нулевым
     */
    @Test
    public void testNormalizeZeroVector() {
        Vector3D v = new Vector3D(0.0f, 0.0f, 0.0f);
        v.normalize();

        assertEquals(0.0f, v.x, EPSILON);
        assertEquals(0.0f, v.y, EPSILON);
        assertEquals(0.0f, v.z, EPSILON);
    }

    /**
     * Тест 12: Скалярное произведение (dot product)
     *
     * dot((1,0,0), (1,0,0)) = 1 (одинаковое направление)
     * dot((1,0,0), (0,1,0)) = 0 (перпендикулярны)
     * dot((1,0,0), (-1,0,0)) = -1 (противоположное направление)
     */
    @Test
    public void testDotProduct() {
        Vector3D v1 = new Vector3D(1.0f, 0.0f, 0.0f);
        Vector3D v2 = new Vector3D(1.0f, 0.0f, 0.0f);
        assertEquals("Параллельные векторы", 1.0f, v1.dot(v2), EPSILON);

        Vector3D v3 = new Vector3D(0.0f, 1.0f, 0.0f);
        assertEquals("Перпендикулярные векторы", 0.0f, v1.dot(v3), EPSILON);

        Vector3D v4 = new Vector3D(-1.0f, 0.0f, 0.0f);
        assertEquals("Противоположные векторы", -1.0f, v1.dot(v4), EPSILON);
    }

    /**
     * Тест 13: Скалярное произведение для вычисления угла
     *
     * cos(θ) = (a · b) / (|a| * |b|)
     * Для единичных векторов: cos(θ) = a · b
     */
    @Test
    public void testDotProductAngle() {
        // Угол 90° между осями X и Y
        Vector3D x = new Vector3D(1.0f, 0.0f, 0.0f);
        Vector3D y = new Vector3D(0.0f, 1.0f, 0.0f);

        float dot = x.dot(y);
        assertEquals("cos(90°) = 0", 0.0f, dot, EPSILON);
    }

    /**
     * Тест 14: Векторное произведение (cross product)
     *
     * Результат - вектор, перпендикулярный обоим исходным
     * X × Y = Z, Y × Z = X, Z × X = Y (правая система координат)
     */
    @Test
    public void testCrossProduct() {
        Vector3D x = new Vector3D(1.0f, 0.0f, 0.0f);
        Vector3D y = new Vector3D(0.0f, 1.0f, 0.0f);
        Vector3D z = x.cross(y);

        // X × Y = Z
        assertEquals(0.0f, z.x, EPSILON);
        assertEquals(0.0f, z.y, EPSILON);
        assertEquals(1.0f, z.z, EPSILON);

        // Результат перпендикулярен исходным векторам
        assertEquals("Перпендикулярен X", 0.0f, z.dot(x), EPSILON);
        assertEquals("Перпендикулярен Y", 0.0f, z.dot(y), EPSILON);
    }

    /**
     * Тест 15: Векторное произведение антикоммутативно
     *
     * A × B = -(B × A)
     */
    @Test
    public void testCrossProductAnticommutative() {
        Vector3D a = new Vector3D(1.0f, 2.0f, 3.0f);
        Vector3D b = new Vector3D(4.0f, 5.0f, 6.0f);

        Vector3D axb = a.cross(b);
        Vector3D bxa = b.cross(a);

        assertEquals(-axb.x, bxa.x, EPSILON);
        assertEquals(-axb.y, bxa.y, EPSILON);
        assertEquals(-axb.z, bxa.z, EPSILON);
    }

    /**
     * Тест 16: Расстояние между точками
     *
     * Расстояние от (0,0,0) до (3,4,0) = 5
     */
    @Test
    public void testDistance() {
        Vector3D origin = new Vector3D(0.0f, 0.0f, 0.0f);
        Vector3D point = new Vector3D(3.0f, 4.0f, 0.0f);

        float distance = origin.distance(point);
        assertEquals("Расстояние = 5", 5.0f, distance, EPSILON);
    }

    /**
     * Тест 17: Расстояние в 3D
     *
     * От (1,2,3) до (4,6,3) = √((4-1)² + (6-2)² + (3-3)²) = √(9+16+0) = 5
     */
    @Test
    public void testDistance3D() {
        Vector3D p1 = new Vector3D(1.0f, 2.0f, 3.0f);
        Vector3D p2 = new Vector3D(4.0f, 6.0f, 3.0f);

        float distance = p1.distance(p2);
        assertEquals("Расстояние = 5", 5.0f, distance, EPSILON);
    }

    /**
     * Тест 18: Цепочка операций
     *
     * Демонстрирует fluent interface (каждый метод возвращает this)
     */
    @Test
    public void testMethodChaining() {
        Vector3D v = new Vector3D(1.0f, 2.0f, 3.0f);

        // Цепочка: добавить (1,1,1), затем умножить на 2, затем инвертировать
        v.add(new Vector3D(1.0f, 1.0f, 1.0f))
         .scale(2.0f)
         .inverse();

        assertEquals(-4.0f, v.x, EPSILON);  // (1+1)*2 = 4, затем -4
        assertEquals(-6.0f, v.y, EPSILON);  // (2+1)*2 = 6, затем -6
        assertEquals(-8.0f, v.z, EPSILON);  // (3+1)*2 = 8, затем -8
    }

    /**
     * Тест 19: toString для отладки
     *
     * Проверяет читаемое текстовое представление
     */
    @Test
    public void testToString() {
        Vector3D v = new Vector3D(1.5f, 2.5f, 3.5f);
        String str = v.toString();

        assertTrue("Содержит X", str.contains("1.50"));
        assertTrue("Содержит Y", str.contains("2.50"));
        assertTrue("Содержит Z", str.contains("3.50"));
    }

    /**
     * Тест 20: equals и hashCode
     *
     * Проверяет корректность сравнения векторов
     */
    @Test
    public void testEqualsAndHashCode() {
        Vector3D v1 = new Vector3D(1.0f, 2.0f, 3.0f);
        Vector3D v2 = new Vector3D(1.0f, 2.0f, 3.0f);
        Vector3D v3 = new Vector3D(4.0f, 5.0f, 6.0f);

        // Рефлексивность
        assertEquals("Вектор равен самому себе", v1, v1);

        // Симметричность
        assertEquals("v1 == v2", v1, v2);
        assertEquals("v2 == v1", v2, v1);

        // Одинаковые векторы имеют одинаковый hash
        assertEquals("Одинаковый hashCode", v1.hashCode(), v2.hashCode());

        // Разные векторы не равны
        assertNotEquals("v1 != v3", v1, v3);
    }
}
