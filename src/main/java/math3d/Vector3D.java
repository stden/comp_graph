package math3d;

/**
 * 3D вектор для работы с трёхмерной графикой
 *
 * Основан на примере из JOGL NeHe Lesson 39
 * Используется для:
 * - Представления точек в 3D пространстве
 * - Направлений и скоростей
 * - Нормалей к поверхностям
 */
public class Vector3D {
    public float x;  // координата X
    public float y;  // координата Y
    public float z;  // координата Z

    /**
     * Конструктор по умолчанию - создаёт нулевой вектор (0, 0, 0)
     */
    public Vector3D() {
        this.x = 0;
        this.y = 0;
        this.z = 0;
    }

    /**
     * Конструктор копирования - создаёт копию вектора
     *
     * @param vector исходный вектор для копирования
     */
    public Vector3D(Vector3D vector) {
        this.x = vector.x;
        this.y = vector.y;
        this.z = vector.z;
    }

    /**
     * Конструктор с координатами
     *
     * @param x координата X
     * @param y координата Y
     * @param z координата Z
     */
    public Vector3D(float x, float y, float z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    /**
     * Сложение векторов (модифицирует текущий вектор)
     *
     * Пример: v1.add(v2) изменяет v1 на v1 + v2
     *
     * @param v вектор для сложения
     * @return this для цепочки вызовов
     */
    public Vector3D add(Vector3D v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return this;
    }

    /**
     * Вычитание векторов (модифицирует текущий вектор)
     *
     * Пример: v1.sub(v2) изменяет v1 на v1 - v2
     *
     * @param v вектор для вычитания
     * @return this для цепочки вызовов
     */
    public Vector3D sub(Vector3D v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return this;
    }

    /**
     * Умножение вектора на скаляр (модифицирует текущий вектор)
     *
     * Пример: v.scale(2) увеличивает длину вектора в 2 раза
     *
     * @param value множитель
     * @return this для цепочки вызовов
     */
    public Vector3D scale(float value) {
        x *= value;
        y *= value;
        z *= value;
        return this;
    }

    /**
     * Инверсия вектора (меняет направление на противоположное)
     *
     * Пример: v.inverse() превращает (1,2,3) в (-1,-2,-3)
     *
     * @return this для цепочки вызовов
     */
    public Vector3D inverse() {
        x = -x;
        y = -y;
        z = -z;
        return this;
    }

    /**
     * Вычисляет длину (модуль) вектора
     *
     * Формула: √(x² + y² + z²)
     *
     * @return длина вектора
     */
    public float length() {
        return (float) Math.sqrt(x * x + y * y + z * z);
    }

    /**
     * Нормализация вектора (приведение длины к 1)
     *
     * После нормализации направление вектора сохраняется,
     * но длина становится равна 1 (единичный вектор)
     *
     * Если вектор нулевой, ничего не делает
     */
    public void normalize() {
        float length = length();

        if (length == 0)
            return;

        x /= length;
        y /= length;
        z /= length;
    }

    /**
     * Скалярное произведение (dot product)
     *
     * Используется для:
     * - Вычисления угла между векторами
     * - Проекции одного вектора на другой
     * - Определения перпендикулярности (результат = 0)
     *
     * @param v второй вектор
     * @return скалярное произведение
     */
    public float dot(Vector3D v) {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * Векторное произведение (cross product)
     *
     * Результат - вектор, перпендикулярный обоим исходным
     * Используется для вычисления нормалей к поверхностям
     *
     * @param v второй вектор
     * @return новый вектор - результат векторного произведения
     */
    public Vector3D cross(Vector3D v) {
        return new Vector3D(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    /**
     * Вычисляет расстояние до другого вектора (точки)
     *
     * @param v вторая точка
     * @return расстояние между точками
     */
    public float distance(Vector3D v) {
        float dx = x - v.x;
        float dy = y - v.y;
        float dz = z - v.z;
        return (float) Math.sqrt(dx * dx + dy * dy + dz * dz);
    }

    @Override
    public String toString() {
        return String.format("(%.2f, %.2f, %.2f)", x, y, z);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        Vector3D vector = (Vector3D) obj;
        return Float.compare(vector.x, x) == 0 &&
               Float.compare(vector.y, y) == 0 &&
               Float.compare(vector.z, z) == 0;
    }

    @Override
    public int hashCode() {
        int result = (x != +0.0f ? Float.floatToIntBits(x) : 0);
        result = 31 * result + (y != +0.0f ? Float.floatToIntBits(y) : 0);
        result = 31 * result + (z != +0.0f ? Float.floatToIntBits(z) : 0);
        return result;
    }
}
