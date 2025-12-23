package graphics;

/**
 * 2D трансформации с использованием матриц 3x3
 *
 * Модуль 2: 2D-графика и преобразования
 * - Трансляция (перемещение)
 * - Масштабирование
 * - Вращение
 * - Композиция трансформаций
 *
 * Использует однородные координаты (x, y, 1) для представления 2D точек
 */
public class Transform2D {

    /**
     * Матрица 3x3 для 2D трансформаций
     * Формат:
     * | m00 m01 m02 |
     * | m10 m11 m12 |
     * | m20 m21 m22 |
     */
    private double[][] matrix;

    /**
     * Создаёт единичную матрицу (identity)
     */
    public Transform2D() {
        matrix = new double[3][3];
        matrix[0][0] = 1;
        matrix[1][1] = 1;
        matrix[2][2] = 1;
    }

    /**
     * Создаёт трансформацию из матрицы
     */
    public Transform2D(double[][] m) {
        if (m.length != 3 || m[0].length != 3) {
            throw new IllegalArgumentException("Матрица должна быть 3x3");
        }
        this.matrix = new double[3][3];
        for (int i = 0; i < 3; i++) {
            System.arraycopy(m[i], 0, this.matrix[i], 0, 3);
        }
    }

    /**
     * Создаёт трансформацию трансляции (переноса)
     * Сдвигает точку на (dx, dy)
     *
     * Матрица:
     * | 1  0  dx |
     * | 0  1  dy |
     * | 0  0   1 |
     */
    public static Transform2D translate(double dx, double dy) {
        double[][] m = {
            {1, 0, dx},
            {0, 1, dy},
            {0, 0, 1}
        };
        return new Transform2D(m);
    }

    /**
     * Создаёт трансформацию масштабирования
     * Масштабирует по X в sx раз, по Y в sy раз
     *
     * Матрица:
     * | sx  0  0 |
     * |  0 sy  0 |
     * |  0  0  1 |
     */
    public static Transform2D scale(double sx, double sy) {
        double[][] m = {
            {sx, 0, 0},
            {0, sy, 0},
            {0, 0, 1}
        };
        return new Transform2D(m);
    }

    /**
     * Создаёт трансформацию вращения вокруг начала координат
     * Поворачивает на угол angle (в радианах) против часовой стрелки
     *
     * Матрица:
     * | cos(θ) -sin(θ)  0 |
     * | sin(θ)  cos(θ)  0 |
     * |   0       0     1 |
     */
    public static Transform2D rotate(double angle) {
        double cos = Math.cos(angle);
        double sin = Math.sin(angle);
        double[][] m = {
            {cos, -sin, 0},
            {sin, cos, 0},
            {0, 0, 1}
        };
        return new Transform2D(m);
    }

    /**
     * Создаёт трансформацию вращения вокруг точки (cx, cy)
     * 1. Переносим центр вращения в начало координат
     * 2. Поворачиваем
     * 3. Переносим обратно
     */
    public static Transform2D rotateAround(double cx, double cy, double angle) {
        return translate(cx, cy)
            .multiply(rotate(angle))
            .multiply(translate(-cx, -cy));
    }

    /**
     * Умножение матриц (композиция трансформаций)
     * Применяется СПРАВА НАЛЕВО: result = this * other
     */
    public Transform2D multiply(Transform2D other) {
        double[][] result = new double[3][3];

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result[i][j] = 0;
                for (int k = 0; k < 3; k++) {
                    result[i][j] += this.matrix[i][k] * other.matrix[k][j];
                }
            }
        }

        return new Transform2D(result);
    }

    /**
     * Применяет трансформацию к точке (x, y)
     * Использует однородные координаты: [x, y, 1]
     *
     * @return новая точка [x', y']
     */
    public double[] transform(double x, double y) {
        double[] point = {x, y, 1};
        double[] result = new double[2];

        // Умножаем матрицу на вектор
        double x_new = matrix[0][0] * point[0] + matrix[0][1] * point[1] + matrix[0][2] * point[2];
        double y_new = matrix[1][0] * point[0] + matrix[1][1] * point[1] + matrix[1][2] * point[2];
        double w = matrix[2][0] * point[0] + matrix[2][1] * point[1] + matrix[2][2] * point[2];

        // Нормализуем однородные координаты
        result[0] = x_new / w;
        result[1] = y_new / w;

        return result;
    }

    /**
     * Получить элемент матрицы
     */
    public double get(int row, int col) {
        return matrix[row][col];
    }

    /**
     * Создаёт трансформацию отражения по оси X
     * Матрица:
     * | 1   0  0 |
     * | 0  -1  0 |
     * | 0   0  1 |
     */
    public static Transform2D flipX() {
        return scale(1, -1);
    }

    /**
     * Создаёт трансформацию отражения по оси Y
     * Матрица:
     * | -1  0  0 |
     * |  0  1  0 |
     * |  0  0  1 |
     */
    public static Transform2D flipY() {
        return scale(-1, 1);
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 3; i++) {
            sb.append("| ");
            for (int j = 0; j < 3; j++) {
                sb.append(String.format("%7.2f ", matrix[i][j]));
            }
            sb.append("|\n");
        }
        return sb.toString();
    }
}
