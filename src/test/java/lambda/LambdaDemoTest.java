package lambda;

import org.junit.Assert;
import org.junit.Test;

/**
 * Обучающие тесты для лямбда-выражений
 *
 * Демонстрируют:
 * 1. Функциональные интерфейсы
 * 2. Лямбда-выражения
 * 3. Ссылки на методы
 * 4. Операции высшего порядка (map/reduce)
 */
public class LambdaDemoTest extends Assert {

    /**
     * Тест 1: Анонимный класс (старый способ до Java 8)
     *
     * Показывает многословный синтаксис создания функционального объекта
     * До Java 8 нужно было создавать анонимный класс с методом apply
     */
    @Test
    public void testAnonymousClass() {
        // Создаём операцию умножения через анонимный класс
        Operation multiply = new Operation() {
            @Override
            public int apply(int a, int b) {
                return a * b;
            }
        };

        assertEquals("2 * 3 = 6", 6, multiply.apply(2, 3));
        assertEquals("5 * 4 = 20", 20, multiply.apply(5, 4));
    }

    /**
     * Тест 2: Лямбда-выражение (новый способ Java 8+)
     *
     * Лямбда: (параметры) -> выражение
     * Компилятор сам определяет типы из контекста
     */
    @Test
    public void testLambdaExpression() {
        // Краткая запись той же операции умножения
        Operation multiply = (x, y) -> x * y;

        assertEquals("Лямбда: 2 * 3 = 6", 6, multiply.apply(2, 3));
        assertEquals("Лямбда: 5 * 4 = 20", 20, multiply.apply(5, 4));
    }

    /**
     * Тест 3: Разные лямбда-выражения
     *
     * Демонстрирует различные операции через лямбды
     */
    @Test
    public void testDifferentLambdas() {
        // Сложение
        Operation sum = (a, b) -> a + b;
        assertEquals("Сложение: 5 + 3 = 8", 8, sum.apply(5, 3));

        // Вычитание
        Operation subtract = (a, b) -> a - b;
        assertEquals("Вычитание: 10 - 3 = 7", 7, subtract.apply(10, 3));

        // Максимум (тернарный оператор)
        Operation max = (a, b) -> (a > b) ? a : b;
        assertEquals("Максимум: max(7, 3) = 7", 7, max.apply(7, 3));
        assertEquals("Максимум: max(2, 9) = 9", 9, max.apply(2, 9));

        // Минимум
        Operation min = (a, b) -> (a < b) ? a : b;
        assertEquals("Минимум: min(7, 3) = 3", 3, min.apply(7, 3));
        assertEquals("Минимум: min(2, 9) = 2", 2, min.apply(2, 9));
    }

    /**
     * Тест 4: Лямбда с блоком кода
     *
     * Если нужно несколько строк, используем { }
     * Обязательно указываем return
     */
    @Test
    public void testLambdaWithBlock() {
        Operation power = (a, b) -> {
            int result = 1;
            for (int i = 0; i < b; i++) {
                result *= a;
            }
            return result;
        };

        assertEquals("2^3 = 8", 8, power.apply(2, 3));
        assertEquals("3^2 = 9", 9, power.apply(3, 2));
        assertEquals("5^0 = 1", 1, power.apply(5, 0));
    }

    /**
     * Тест 5: Ссылки на методы (Method References)
     *
     * Если лямбда просто вызывает метод, можно использовать ::
     * Math::max эквивалентно (a, b) -> Math.max(a, b)
     */
    @Test
    public void testMethodReference() {
        // Ссылка на статический метод
        Operation maxRef = Math::max;
        assertEquals("Ссылка на Math.max: max(5, 3) = 5", 5, maxRef.apply(5, 3));

        Operation minRef = Math::min;
        assertEquals("Ссылка на Math.min: min(5, 3) = 3", 3, minRef.apply(5, 3));
    }

    /**
     * Тест 6: Операция map (свёртка массива)
     *
     * Применяет операцию ко всем элементам массива последовательно
     * Например: map([1,2,3], +) = ((1+2)+3) = 6
     */
    @Test
    public void testMapOperation() {
        int[] data = {2, 3, 4};

        // Произведение: 2 * 3 * 4 = 24
        int product = map(data, (x, y) -> x * y);
        assertEquals("Произведение [2,3,4] = 24", 24, product);

        // Сумма: 2 + 3 + 4 = 9
        int sum = map(data, (x, y) -> x + y);
        assertEquals("Сумма [2,3,4] = 9", 9, sum);

        // Максимум: max(max(2, 3), 4) = 4
        int max = map(data, Math::max);
        assertEquals("Максимум [2,3,4] = 4", 4, max);
    }

    /**
     * Тест 7: Граничные случаи
     *
     * Проверяет работу с пустыми и одноэлементными массивами
     */
    @Test
    public void testEdgeCases() {
        // Одноэлементный массив
        int[] single = {42};
        assertEquals("Одно число возвращается как есть", 42, map(single, (a, b) -> a + b));

        // Пустой массив должен вызвать исключение
        int[] empty = {};
        try {
            map(empty, (a, b) -> a + b);
            fail("Должно быть исключение для пустого массива");
        } catch (IllegalArgumentException e) {
            assertEquals("Сообщение об ошибке",
                    "Массив должен содержать хотя бы один элемент",
                    e.getMessage());
        }
    }

    /**
     * Тест 8: Композиция операций
     *
     * Демонстрирует, как операции могут комбинироваться
     */
    @Test
    public void testOperationComposition() {
        int[] data = {1, 2, 3, 4, 5};

        // Сумма всех элементов: 1+2+3+4+5 = 15
        Operation sum = (a, b) -> a + b;
        assertEquals("Сумма 1..5 = 15", 15, map(data, sum));

        // Произведение: 1*2*3*4*5 = 120 (факториал 5)
        Operation multiply = (a, b) -> a * b;
        assertEquals("Произведение 1..5 = 120", 120, map(data, multiply));
    }

    /**
     * Вспомогательный метод map (свёртка)
     *
     * Применяет операцию op последовательно:
     * map([a,b,c], op) = op(op(a,b), c)
     *
     * @param arr массив чисел
     * @param op операция для применения
     * @return результат свёртки
     */
    private int map(int[] arr, Operation op) {
        if (arr.length < 1)
            throw new IllegalArgumentException("Массив должен содержать хотя бы один элемент");

        int result = arr[0];
        for (int i = 1; i < arr.length; i++) {
            result = op.apply(result, arr[i]);
        }
        return result;
    }
}
