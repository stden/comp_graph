package lambda;

import static java.lang.System.out;

/**
 * Лямбда-выражения
 * ----------------
 */
public class LambdaDemo {

    public static void main(String[] args) {
        /*
        new Operation() {
            @Override
            public int apply(int x, int y) {
                return x + y;
            }
        };
         */
        // Умножение до Java8
        Operation multiply = new Operation() {
            @Override
            public int apply(int a, int b) {
                return a * b;
            }
        };
        Operation multiply2 = (x, y) -> x * y;
        System.out.println("multiply2.apply(2, 3) = " +
                multiply2.apply(2, 3));

        // Исходные данные для тестирования
        int[] data = {1, 2, 4};

        out.println("Перемножить все числа: " + map(data, multiply));

        Operation printAll = (x, y) -> {
            System.out.print(x + " " + y + " ");
            return 0;
        };

        Operation sumOp = (x, y) -> x + y;
        out.println("Сумма: " + map(data, sumOp));

        out.println("Mul: " + map(data, (x, y) -> x * y));
        out.println("Максимум: " + map(data, (x, y) -> (x > y) ? x : y));
        out.println("Минимум: " + map(data, (x, y) -> (x < y) ? x : y));

        out.println("Max (Math::max): " + map(data, Math::max));
        out.println("Mul2: " + map(data, LambdaDemo::mul));
    }

    private static int mul(int a, int b) {
        return a * b;
    }

    private static int map(int[] arr, Operation op) {
        if (arr.length < 1)
            throw new IllegalArgumentException("Массив должен содержать хотя бы один элемент");
        int res = arr[0];
        for (int i = 1; i < arr.length; i++)
            res = op.apply(res, arr[i]);
        return res;
    }
}
