package graphics;

/**
 * Матрица 4x4 для 3D трансформаций
 *
 * Модуль 3: Основы 3D-графики
 * - Проекционные матрицы (перспективная, ортографическая)
 * - View матрица (камера)
 * - Model матрица (трансформации объектов)
 */
public class Matrix4 {
    private double[][] m;

    /**
     * Создаёт единичную матрицу 4x4
     */
    public Matrix4() {
        m = new double[4][4];
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;
    }

    /**
     * Создаёт матрицу из массива
     */
    public Matrix4(double[][] data) {
        if (data.length != 4 || data[0].length != 4) {
            throw new IllegalArgumentException("Матрица должна быть 4x4");
        }
        m = new double[4][4];
        for (int i = 0; i < 4; i++) {
            System.arraycopy(data[i], 0, m[i], 0, 4);
        }
    }

    /**
     * Создаёт перспективную проекционную матрицу (OpenGL стиль)
     *
     * @param fov поле зрения (field of view) в радианах
     * @param aspect соотношение сторон (width / height)
     * @param near ближняя плоскость отсечения
     * @param far дальняя плоскость отсечения
     */
    public static Matrix4 perspective(double fov, double aspect, double near, double far) {
        double f = 1.0 / Math.tan(fov / 2.0);
        double rangeInv = 1.0 / (near - far);

        double[][] data = {
            {f / aspect, 0, 0, 0},
            {0, f, 0, 0},
            {0, 0, (near + far) * rangeInv, 2 * near * far * rangeInv},
            {0, 0, -1, 0}
        };

        return new Matrix4(data);
    }

    /**
     * Создаёт ортографическую проекционную матрицу
     *
     * @param left левая граница
     * @param right правая граница
     * @param bottom нижняя граница
     * @param top верхняя граница
     * @param near ближняя плоскость
     * @param far дальняя плоскость
     */
    public static Matrix4 orthographic(double left, double right, double bottom, double top, double near, double far) {
        double rl = 1.0 / (right - left);
        double tb = 1.0 / (top - bottom);
        double fn = 1.0 / (far - near);

        double[][] data = {
            {2 * rl, 0, 0, -(right + left) * rl},
            {0, 2 * tb, 0, -(top + bottom) * tb},
            {0, 0, -2 * fn, -(far + near) * fn},
            {0, 0, 0, 1}
        };

        return new Matrix4(data);
    }

    /**
     * Создаёт view матрицу (lookAt) - матрицу камеры
     *
     * @param eyeX позиция камеры X
     * @param eyeY позиция камеры Y
     * @param eyeZ позиция камеры Z
     * @param centerX точка, на которую смотрит камера X
     * @param centerY точка, на которую смотрит камера Y
     * @param centerZ точка, на которую смотрит камера Z
     * @param upX вектор "вверх" X
     * @param upY вектор "вверх" Y
     * @param upZ вектор "вверх" Z
     */
    public static Matrix4 lookAt(
        double eyeX, double eyeY, double eyeZ,
        double centerX, double centerY, double centerZ,
        double upX, double upY, double upZ
    ) {
        // Вычисляем направление взгляда (forward)
        double fx = centerX - eyeX;
        double fy = centerY - eyeY;
        double fz = centerZ - eyeZ;

        // Нормализуем forward
        double fLen = Math.sqrt(fx * fx + fy * fy + fz * fz);
        fx /= fLen;
        fy /= fLen;
        fz /= fLen;

        // Вычисляем right = forward × up
        double rx = fy * upZ - fz * upY;
        double ry = fz * upX - fx * upZ;
        double rz = fx * upY - fy * upX;

        // Нормализуем right
        double rLen = Math.sqrt(rx * rx + ry * ry + rz * rz);
        rx /= rLen;
        ry /= rLen;
        rz /= rLen;

        // Вычисляем новый up = right × forward
        double ux = ry * fz - rz * fy;
        double uy = rz * fx - rx * fz;
        double uz = rx * fy - ry * fx;

        double[][] data = {
            {rx, rx, rx, -(rx * eyeX + ry * eyeY + rz * eyeZ)},
            {ux, uy, uz, -(ux * eyeX + uy * eyeY + uz * eyeZ)},
            {-fx, -fy, -fz, (fx * eyeX + fy * eyeY + fz * eyeZ)},
            {0, 0, 0, 1}
        };

        return new Matrix4(data);
    }

    /**
     * Создаёт матрицу трансляции (переноса) в 3D
     */
    public static Matrix4 translate(double x, double y, double z) {
        double[][] data = {
            {1, 0, 0, x},
            {0, 1, 0, y},
            {0, 0, 1, z},
            {0, 0, 0, 1}
        };
        return new Matrix4(data);
    }

    /**
     * Создаёт матрицу масштабирования в 3D
     */
    public static Matrix4 scale(double x, double y, double z) {
        double[][] data = {
            {x, 0, 0, 0},
            {0, y, 0, 0},
            {0, 0, z, 0},
            {0, 0, 0, 1}
        };
        return new Matrix4(data);
    }

    /**
     * Создаёт матрицу вращения вокруг оси X
     *
     * @param angle угол в радианах
     */
    public static Matrix4 rotateX(double angle) {
        double c = Math.cos(angle);
        double s = Math.sin(angle);

        double[][] data = {
            {1, 0, 0, 0},
            {0, c, -s, 0},
            {0, s, c, 0},
            {0, 0, 0, 1}
        };
        return new Matrix4(data);
    }

    /**
     * Создаёт матрицу вращения вокруг оси Y
     *
     * @param angle угол в радианах
     */
    public static Matrix4 rotateY(double angle) {
        double c = Math.cos(angle);
        double s = Math.sin(angle);

        double[][] data = {
            {c, 0, s, 0},
            {0, 1, 0, 0},
            {-s, 0, c, 0},
            {0, 0, 0, 1}
        };
        return new Matrix4(data);
    }

    /**
     * Создаёт матрицу вращения вокруг оси Z
     *
     * @param angle угол в радианах
     */
    public static Matrix4 rotateZ(double angle) {
        double c = Math.cos(angle);
        double s = Math.sin(angle);

        double[][] data = {
            {c, -s, 0, 0},
            {s, c, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        };
        return new Matrix4(data);
    }

    /**
     * Умножение матриц
     */
    public Matrix4 multiply(Matrix4 other) {
        double[][] result = new double[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result[i][j] += this.m[i][k] * other.m[k][j];
                }
            }
        }

        return new Matrix4(result);
    }

    /**
     * Применяет матрицу к вектору (x, y, z, w)
     *
     * @return массив из 4 элементов [x', y', z', w']
     */
    public double[] transform(double x, double y, double z, double w) {
        double[] result = new double[4];

        result[0] = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
        result[1] = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
        result[2] = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
        result[3] = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;

        return result;
    }

    /**
     * Применяет матрицу к точке (x, y, z)
     * Использует однородные координаты с w=1
     *
     * @return массив [x', y', z'] после перспективного деления
     */
    public double[] transformPoint(double x, double y, double z) {
        double[] homogeneous = transform(x, y, z, 1.0);

        // Перспективное деление
        double w = homogeneous[3];
        return new double[] {
            homogeneous[0] / w,
            homogeneous[1] / w,
            homogeneous[2] / w
        };
    }

    /**
     * Получить элемент матрицы
     */
    public double get(int row, int col) {
        return m[row][col];
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 4; i++) {
            sb.append("| ");
            for (int j = 0; j < 4; j++) {
                sb.append(String.format("%7.3f ", m[i][j]));
            }
            sb.append("|\n");
        }
        return sb.toString();
    }
}
