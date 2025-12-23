# Руководство по элегантному коду

## 🎯 Принципы элегантного кода

### 1. Читаемость превыше всего
- Код должен читаться как проза
- Имена переменных должны объяснять назначение
- Комментарии объясняют "почему", а не "что"

### 2. Разделение ответственности
- Одна функция = одна задача
- Короткие функции (< 20 строк)
- Понятная иерархия

### 3. Математическая элегантность
- Используйте математические обозначения где уместно
- Выделяйте формулы в отдельные функции
- Добавляйте LaTeX-комментарии с формулами

---

## 🐍 Python: Улучшения для преподавания

### Пример 1: Множество Жюлиа - ДО

```python
def julia(z, c, max_iter=100):
    for n in range(max_iter):
        if abs(z) > 2:
            return n
        z = z*z + c
    return max_iter
```

### Пример 1: Множество Жюлиа - ПОСЛЕ

```python
def compute_julia_escape_time(z: complex, c: complex, max_iterations: int = 100) -> int:
    """
    Вычисляет время убегания точки из множества Жюлиа.

    Математика:
        z(n+1) = z(n)² + c, где c — константа

    Условие убегания: |z| > 2 (убегает в бесконечность)

    Args:
        z: Начальная точка на комплексной плоскости
        c: Константа множества Жюлиа
        max_iterations: Максимальное количество итераций

    Returns:
        Количество итераций до убегания (или max_iterations для точек множества)

    Example:
        >>> compute_julia_escape_time(0+0j, -0.7+0.27015j, 100)
        100  # Точка принадлежит множеству
    """
    ESCAPE_RADIUS = 2.0  # Радиус убегания (теоретически доказано)

    for iteration in range(max_iterations):
        # Проверка условия убегания: |z| > 2
        if abs(z) > ESCAPE_RADIUS:
            return iteration

        # Применение итерационной формулы: z ← z² + c
        z = z * z + c

    # Точка не убежала — принадлежит множеству Жюлиа
    return max_iterations
```

**Улучшения**:
1. ✅ Описательное имя функции
2. ✅ Type hints (Python 3.5+)
3. ✅ Подробный docstring
4. ✅ Математическая формула в комментарии
5. ✅ Константа вместо магического числа
6. ✅ Говорящие имена переменных
7. ✅ Комментарии объясняют математику

---

### Пример 2: HSV → RGB конвертация - ДО

```python
def hsv_to_rgb(h, s, v):
    h = h % 360
    c = v * s
    x = c * (1 - abs((h / 60) % 2 - 1))
    m = v - c
    if h < 60:
        r, g, b = c, x, 0
    elif h < 120:
        r, g, b = x, c, 0
    # ... и т.д.
    return int((r + m) * 100), int((g + m) * 100), int((b + m) * 100)
```

### Пример 2: HSV → RGB конвертация - ПОСЛЕ

```python
from dataclasses import dataclass
from typing import Tuple

@dataclass(frozen=True)
class RGBColor:
    """RGB цвет с компонентами в диапазоне [0, 100]."""
    red: int
    green: int
    blue: int

    def __post_init__(self):
        """Валидация значений цветовых компонент."""
        for component in (self.red, self.green, self.blue):
            if not 0 <= component <= 100:
                raise ValueError(f"Компонент цвета должен быть в [0, 100], получено: {component}")

    def to_latex(self) -> str:
        """Конвертация в LaTeX цветовой формат."""
        return f"{{rgb,100:{self.red},{self.green},{self.blue}}}"


def hsv_to_rgb(hue: float, saturation: float, value: float) -> RGBColor:
    """
    Конвертация HSV (Hue, Saturation, Value) → RGB (Red, Green, Blue).

    Алгоритм из:
        https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB

    Args:
        hue: Оттенок в градусах [0°, 360°]
        saturation: Насыщенность [0.0, 1.0]
        value: Яркость [0.0, 1.0]

    Returns:
        RGBColor с компонентами [0, 100]

    Example:
        >>> hsv_to_rgb(0, 1.0, 1.0)  # Чистый красный
        RGBColor(red=100, green=0, blue=0)

        >>> hsv_to_rgb(120, 1.0, 1.0)  # Чистый зелёный
        RGBColor(red=0, green=100, blue=0)
    """
    # Нормализация hue к диапазону [0, 360)
    hue = hue % 360

    # Вычисление chroma (цветность)
    chroma = value * saturation

    # Промежуточное значение для цветовых сегментов
    hue_segment = (hue / 60) % 2 - 1
    intermediate = chroma * (1 - abs(hue_segment))

    # Базовые компоненты без яркости (match по сегментам hue)
    if 0 <= hue < 60:
        r_prime, g_prime, b_prime = chroma, intermediate, 0
    elif 60 <= hue < 120:
        r_prime, g_prime, b_prime = intermediate, chroma, 0
    elif 120 <= hue < 180:
        r_prime, g_prime, b_prime = 0, chroma, intermediate
    elif 180 <= hue < 240:
        r_prime, g_prime, b_prime = 0, intermediate, chroma
    elif 240 <= hue < 300:
        r_prime, g_prime, b_prime = intermediate, 0, chroma
    else:  # 300 <= hue < 360
        r_prime, g_prime, b_prime = chroma, 0, intermediate

    # Добавление яркости (lightness matching)
    lightness_offset = value - chroma

    # Конвертация в [0, 100] диапазон
    red = int((r_prime + lightness_offset) * 100)
    green = int((g_prime + lightness_offset) * 100)
    blue = int((b_prime + lightness_offset) * 100)

    return RGBColor(red, green, blue)
```

**Улучшения**:
1. ✅ Dataclass для инкапсуляции RGB
2. ✅ Type hints везде
3. ✅ Валидация через `__post_init__`
4. ✅ Метод `to_latex()` для генерации LaTeX кода
5. ✅ Подробные комментарии для каждого шага
6. ✅ Ссылка на источник алгоритма
7. ✅ Примеры использования в docstring

---

### Пример 3: Midpoint Displacement - улучшенная версия

```python
from typing import List
import random
import math

class FractalLandscape:
    """
    Генератор фрактального ландшафта с использованием алгоритма Midpoint Displacement.

    Алгоритм также известен как Diamond-Square или Cloud Midpoint Displacement.

    Ссылки:
        - https://en.wikipedia.org/wiki/Diamond-square_algorithm
        - Fournier, Fussell, Carpenter (1982) "Computer Rendering of Stochastic Models"
    """

    def __init__(self, size: int, roughness: float = 0.7, seed: int = None):
        """
        Args:
            size: Размер сетки (должен быть 2^n + 1, например 33, 65, 129)
            roughness: Шероховатость ландшафта [0.0 = гладкий, 1.5 = очень грубый]
            seed: Seed для генератора случайных чисел (для воспроизводимости)
        """
        self.size = self._validate_size(size)
        self.roughness = roughness
        self.heightmap: List[List[float]] = []

        if seed is not None:
            random.seed(seed)

    @staticmethod
    def _validate_size(size: int) -> int:
        """Проверка что size = 2^n + 1."""
        n = math.log2(size - 1)
        if not n.is_integer():
            raise ValueError(f"Размер должен быть 2^n + 1, получено: {size}")
        return size

    def generate(self) -> List[List[float]]:
        """
        Генерация карты высот методом Midpoint Displacement.

        Returns:
            2D массив высот, нормализованный к диапазону [0, 1]
        """
        # Инициализация пустой карты
        self.heightmap = [[0.0] * self.size for _ in range(self.size)]

        # Установка высот в углах
        self._initialize_corners()

        # Основной алгоритм: Diamond-Square шаги
        self._diamond_square_algorithm()

        # Нормализация высот к [0, 1]
        self._normalize_heights()

        return self.heightmap

    def _initialize_corners(self, initial_height: float = 0.5):
        """Инициализация угловых точек начальной высотой."""
        corners = [
            (0, 0),
            (0, self.size - 1),
            (self.size - 1, 0),
            (self.size - 1, self.size - 1)
        ]

        for x, y in corners:
            self.heightmap[x][y] = initial_height

    def _diamond_square_algorithm(self):
        """
        Рекурсивный алгоритм Diamond-Square.

        На каждой итерации:
        1. Diamond step: вычисление центров квадратов
        2. Square step: вычисление центров ромбов
        3. Уменьшение шага вдвое
        4. Уменьшение амплитуды возмущений
        """
        step_size = self.size - 1
        scale = self.roughness

        while step_size > 1:
            half_step = step_size // 2

            # Diamond step
            self._diamond_step(step_size, half_step, scale)

            # Square step
            self._square_step(step_size, half_step, scale)

            # Уменьшение параметров для следующей итерации
            step_size //= 2
            scale *= 2.0 ** (-self.roughness)

    def _diamond_step(self, step_size: int, half_step: int, scale: float):
        """
        Diamond step: вычисление высоты в центре квадрата.

        Высота = среднее арифметическое углов + случайное возмущение
        """
        for x in range(half_step, self.size, step_size):
            for y in range(half_step, self.size, step_size):
                # Среднее арифметическое 4 углов
                average = (
                    self.heightmap[x - half_step][y - half_step] +
                    self.heightmap[x - half_step][y + half_step] +
                    self.heightmap[x + half_step][y - half_step] +
                    self.heightmap[x + half_step][y + half_step]
                ) / 4.0

                # Случайное возмущение ±scale/2
                perturbation = (random.random() - 0.5) * scale

                self.heightmap[x][y] = average + perturbation

    def _square_step(self, step_size: int, half_step: int, scale: float):
        """
        Square step: вычисление высоты в центре ромба.

        Высота = среднее арифметическое соседей + случайное возмущение
        """
        for x in range(0, self.size, half_step):
            for y in range((x + half_step) % step_size, self.size, step_size):
                # Сбор соседей (учитываем границы)
                neighbors = []

                if x >= half_step:
                    neighbors.append(self.heightmap[x - half_step][y])
                if x + half_step < self.size:
                    neighbors.append(self.heightmap[x + half_step][y])
                if y >= half_step:
                    neighbors.append(self.heightmap[x][y - half_step])
                if y + half_step < self.size:
                    neighbors.append(self.heightmap[x][y + half_step])

                # Среднее арифметическое соседей
                average = sum(neighbors) / len(neighbors)

                # Случайное возмущение
                perturbation = (random.random() - 0.5) * scale

                self.heightmap[x][y] = average + perturbation

    def _normalize_heights(self):
        """Нормализация всех высот к диапазону [0, 1]."""
        # Поиск минимума и максимума
        flat = [h for row in self.heightmap for h in row]
        min_height = min(flat)
        max_height = max(flat)

        # Избежание деления на ноль
        height_range = max_height - min_height
        if height_range == 0:
            return

        # Нормализация каждой точки
        for x in range(self.size):
            for y in range(self.size):
                self.heightmap[x][y] = (
                    (self.heightmap[x][y] - min_height) / height_range
                )

    def get_height_at(self, x: int, y: int) -> float:
        """Получить высоту в точке (x, y) с проверкой границ."""
        if not (0 <= x < self.size and 0 <= y < self.size):
            raise IndexError(f"Координаты ({x}, {y}) вне границ [{0}, {self.size})")

        return self.heightmap[x][y]


# Пример использования
if __name__ == "__main__":
    # Создание ландшафта 65×65
    landscape = FractalLandscape(size=65, roughness=0.7, seed=12345)

    # Генерация карты высот
    heightmap = landscape.generate()

    # Получение высоты в точке
    height = landscape.get_height_at(32, 32)
    print(f"Высота в центре: {height:.3f}")
```

**Улучшения**:
1. ✅ Объектно-ориентированный дизайн (класс)
2. ✅ Разделение на методы по ответственности
3. ✅ Валидация входных данных
4. ✅ Подробные docstrings с ссылками на литературу
5. ✅ Говорящие имена методов (_diamond_step, _square_step)
6. ✅ Комментарии объясняют математику
7. ✅ Пример использования в main
8. ✅ Type hints везде

---

## ☕ Java: Улучшения для преподавания

### Пример 1: Множество Мандельброта - ПОСЛЕ

```java
package fractals.math;

/**
 * Вычисление принадлежности точек множеству Мандельброта.
 *
 * <p>Множество Мандельброта — это множество точек c на комплексной плоскости,
 * для которых рекуррентная последовательность z(n+1) = z(n)² + c,
 * начинающаяся с z(0) = 0, остаётся ограниченной.</p>
 *
 * <p><b>Математическое определение:</b><br>
 * M = {c ∈ ℂ : lim(n→∞) |z_n| ≠ ∞, где z_{n+1} = z_n² + c, z_0 = 0}</p>
 *
 * <p><b>Условие убегания:</b><br>
 * Если |z_n| > 2, то последовательность убегает в бесконечность (доказано).</p>
 *
 * @author Comp Graph Course
 * @version 1.0
 * @see <a href="https://en.wikipedia.org/wiki/Mandelbrot_set">Википедия: Множество Мандельброта</a>
 */
public final class MandelbrotSet {

    /**
     * Радиус убегания (escape radius).
     * Теоретически доказано, что если |z| > 2, последовательность убегает.
     */
    private static final double ESCAPE_RADIUS = 2.0;

    /**
     * Квадрат радиуса убегания для оптимизации (избегаем sqrt).
     */
    private static final double ESCAPE_RADIUS_SQUARED = ESCAPE_RADIUS * ESCAPE_RADIUS;

    /**
     * Приватный конструктор — утилитный класс не должен создавать экземпляры.
     */
    private MandelbrotSet() {
        throw new AssertionError("Утилитный класс не должен создавать экземпляры");
    }

    /**
     * Вычисляет время убегания (escape time) точки из множества Мандельброта.
     *
     * <p>Алгоритм:</p>
     * <ol>
     *   <li>Начинаем с z = 0</li>
     *   <li>Итеративно вычисляем z ← z² + c</li>
     *   <li>Проверяем условие убегания: |z|² > 4</li>
     *   <li>Если убежала — возвращаем номер итерации</li>
     *   <li>Если не убежала за maxIterations — точка в множестве</li>
     * </ol>
     *
     * @param real действительная часть точки c
     * @param imaginary мнимая часть точки c
     * @param maxIterations максимальное количество итераций
     * @return количество итераций до убегания, или maxIterations если точка в множестве
     *
     * @throws IllegalArgumentException если maxIterations < 1
     *
     * @example
     * <pre>
     * // Точка c = 0 + 0i принадлежит множеству
     * int iterations = MandelbrotSet.computeEscapeTime(0.0, 0.0, 1000);
     * assert iterations == 1000;
     *
     * // Точка c = 2 + 0i быстро убегает
     * iterations = MandelbrotSet.computeEscapeTime(2.0, 0.0, 1000);
     * assert iterations < 10;
     * </pre>
     */
    public static int computeEscapeTime(
            double real,
            double imaginary,
            int maxIterations
    ) {
        validateMaxIterations(maxIterations);

        // Начальное значение z = 0 + 0i
        double zReal = 0.0;
        double zImaginary = 0.0;

        for (int iteration = 0; iteration < maxIterations; iteration++) {
            // Вычисление |z|² = zReal² + zImaginary²
            double magnitudeSquared = zReal * zReal + zImaginary * zImaginary;

            // Проверка условия убегания: |z| > 2 ⟺ |z|² > 4
            if (magnitudeSquared > ESCAPE_RADIUS_SQUARED) {
                return iteration;
            }

            // Итерация: z ← z² + c
            // (a + bi)² = (a² - b²) + 2abi
            double newReal = zReal * zReal - zImaginary * zImaginary + real;
            double newImaginary = 2.0 * zReal * zImaginary + imaginary;

            zReal = newReal;
            zImaginary = newImaginary;
        }

        // Точка не убежала — принадлежит множеству Мандельброта
        return maxIterations;
    }

    /**
     * Проверяет, принадлежит ли точка множеству Мандельброта.
     *
     * @param real действительная часть точки c
     * @param imaginary мнимая часть точки c
     * @param maxIterations максимальное количество итераций
     * @return true если точка вероятно принадлежит множеству
     */
    public static boolean isInSet(double real, double imaginary, int maxIterations) {
        return computeEscapeTime(real, imaginary, maxIterations) == maxIterations;
    }

    /**
     * Валидация параметра maxIterations.
     *
     * @param maxIterations количество итераций для проверки
     * @throws IllegalArgumentException если maxIterations < 1
     */
    private static void validateMaxIterations(int maxIterations) {
        if (maxIterations < 1) {
            throw new IllegalArgumentException(
                "Количество итераций должно быть >= 1, получено: " + maxIterations
            );
        }
    }
}
```

**Улучшения для Java**:
1. ✅ Javadoc с HTML форматированием
2. ✅ Математические формулы в комментариях
3. ✅ Константы вместо магических чисел
4. ✅ Приватный конструктор для утилитного класса
5. ✅ Валидация параметров
6. ✅ @example в Javadoc
7. ✅ Ссылки на литературу через @see
8. ✅ final класс (не наследуется)

---

## 🎓 Рекомендации для преподавания

### 1. Добавьте визуальные комментарии

```python
def generate_pythagoras_tree(depth: int) -> None:
    """
    Генерация дерева Пифагора.

    Визуализация структуры:

        ╱╲              ← Глубина 2: 2 квадрата
       ╱  ╲
      ╱    ╲
     ╱______╲          ← Глубина 1: 2 квадрата
    |        |
    |   Base |         ← Глубина 0: 1 квадрат
    |________|

    Количество квадратов: 2^(depth+1) - 1
    """
```

### 2. Используйте type aliases

```python
from typing import NewType, Tuple

# Создаем семантические типы
ComplexNumber = complex
RGBColor = Tuple[int, int, int]  # (red, green, blue) в [0, 255]
Pixel = Tuple[int, int]  # (x, y) координаты
HeightMap = List[List[float]]  # 2D массив высот

def generate_julia(z: ComplexNumber, c: ComplexNumber) -> int:
    """Теперь понятно, что z и c — комплексные числа."""
    pass
```

### 3. Добавьте утверждения (assertions) для учебных целей

```python
def diamond_step(self, x: int, y: int) -> float:
    """Diamond step в Midpoint Displacement."""

    # Утверждения помогают студентам понять инварианты
    assert 0 <= x < self.size, "x должен быть в границах сетки"
    assert 0 <= y < self.size, "y должен быть в границах сетки"

    average = self._compute_average_of_corners(x, y)

    # Математический инвариант
    assert 0.0 <= average <= 1.0, "Среднее должно быть нормализовано"

    return average
```

### 4. Создайте примеры использования

```python
if __name__ == "__main__":
    """
    Примеры использования для студентов.
    """

    # Пример 1: Простой случай
    print("=== Пример 1: Точка в множестве ===")
    iterations = compute_julia_escape_time(z=0+0j, c=-0.7+0.27015j)
    print(f"Итераций: {iterations}")

    # Пример 2: Точка вне множества
    print("\n=== Пример 2: Точка вне множества ===")
    iterations = compute_julia_escape_time(z=2+2j, c=-0.7+0.27015j)
    print(f"Итераций: {iterations}")

    # Пример 3: Визуализация
    print("\n=== Пример 3: Генерация изображения ===")
    # ... код генерации
```

---

## 📚 Дополнительные ресурсы

### Книги по элегантному коду:
- "Clean Code" by Robert C. Martin
- "The Art of Readable Code" by Dustin Boswell
- "Effective Java" by Joshua Bloch (для Java)
- "Fluent Python" by Luciano Ramalho (для Python)

### Стиль кода:
- **Python**: PEP 8 (https://pep8.org)
- **Java**: Google Java Style Guide

---

**Создано**: 2025-10-05
**Для**: Преподавания компьютерной графики и фракталов
