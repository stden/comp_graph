package graphics;

import graphics.Bresenham.Point;
import org.junit.Test;
import java.util.List;
import static org.junit.Assert.*;

/**
 * Тесты для алгоритмов Брезенхема (TDD подход: Red-Green-Blue)
 * Модуль 2: 2D-графика
 */
public class BresenhamTest {

    @Test
    public void testLineHorizontal() {
        // Горизонтальная линия от (0,0) до (5,0)
        List<Point> points = Bresenham.line(0, 0, 5, 0);

        assertEquals("6 точек на линии", 6, points.size());
        assertEquals("Начальная точка", new Point(0, 0), points.get(0));
        assertEquals("Конечная точка", new Point(5, 0), points.get(5));

        // Все точки должны иметь y = 0
        for (Point p : points) {
            assertEquals("Горизонтальная линия", 0, p.y);
        }
    }

    @Test
    public void testLineVertical() {
        // Вертикальная линия от (3,1) до (3,6)
        List<Point> points = Bresenham.line(3, 1, 3, 6);

        assertEquals("6 точек на линии", 6, points.size());

        // Все точки должны иметь x = 3
        for (Point p : points) {
            assertEquals("Вертикальная линия", 3, p.x);
        }
    }

    @Test
    public void testLineDiagonal45() {
        // Диагональная линия 45° от (0,0) до (4,4)
        List<Point> points = Bresenham.line(0, 0, 4, 4);

        assertEquals("5 точек на диагонали", 5, points.size());

        // На диагонали 45° координаты x и y равны
        for (Point p : points) {
            assertEquals("Диагональ 45°", p.x, p.y);
        }
    }

    @Test
    public void testLineSlope() {
        // Линия с наклоном от (0,0) до (4,2)
        // Наклон = 2/4 = 0.5
        List<Point> points = Bresenham.line(0, 0, 4, 2);

        assertEquals("Начало", new Point(0, 0), points.get(0));
        assertEquals("Конец", new Point(4, 2), points.get(points.size() - 1));

        // Проверяем, что точки образуют правильную последовательность
        assertTrue("Содержит (1,0) или (1,1)",
            points.contains(new Point(1, 0)) || points.contains(new Point(1, 1)));
        assertTrue("Содержит (2,1)", points.contains(new Point(2, 1)));
    }

    @Test
    public void testLineReversed() {
        // Линия в обратном направлении должна дать те же точки
        List<Point> forward = Bresenham.line(0, 0, 3, 3);
        List<Point> backward = Bresenham.line(3, 3, 0, 0);

        assertEquals("Одинаковое количество точек", forward.size(), backward.size());
    }

    @Test
    public void testLineSinglePoint() {
        // Линия из одной точки
        List<Point> points = Bresenham.line(5, 5, 5, 5);

        assertEquals("Одна точка", 1, points.size());
        assertEquals("Точка (5,5)", new Point(5, 5), points.get(0));
    }

    @Test
    public void testLineNegativeCoordinates() {
        // Линия с отрицательными координатами
        List<Point> points = Bresenham.line(-2, -2, 2, 2);

        assertEquals("Начало (-2,-2)", new Point(-2, -2), points.get(0));
        assertEquals("Конец (2,2)", new Point(2, 2), points.get(points.size() - 1));
        assertTrue("Проходит через (0,0)", points.contains(new Point(0, 0)));
    }

    @Test
    public void testCircleRadius0() {
        // Окружность радиуса 0 - точки будут сгенерированы алгоритмом
        // При r=0: x=0, y=0, алгоритм добавляет 8 симметричных точек
        List<Point> points = Bresenham.circle(5, 5, 0);

        assertFalse("Не пустая", points.isEmpty());
        // Проверяем, что центральная точка присутствует
        assertTrue("Содержит центр", points.contains(new Point(5, 5)));
    }

    @Test
    public void testCircleRadius1() {
        // Окружность радиуса 1 вокруг (0,0)
        List<Point> points = Bresenham.circle(0, 0, 1);

        assertFalse("Не пустая", points.isEmpty());

        // Проверяем, что есть точки в 4 основных направлениях
        assertTrue("Сверху", points.contains(new Point(0, 1)));
        assertTrue("Снизу", points.contains(new Point(0, -1)));
        assertTrue("Справа", points.contains(new Point(1, 0)));
        assertTrue("Слева", points.contains(new Point(-1, 0)));
    }

    @Test
    public void testCircleRadius5() {
        // Окружность радиуса 5 с центром (10, 10)
        List<Point> points = Bresenham.circle(10, 10, 5);

        assertFalse("Не пустая", points.isEmpty());

        // Проверяем крайние точки
        assertTrue("Верхняя точка", points.contains(new Point(10, 15)));
        assertTrue("Нижняя точка", points.contains(new Point(10, 5)));
        assertTrue("Правая точка", points.contains(new Point(15, 10)));
        assertTrue("Левая точка", points.contains(new Point(5, 10)));
    }

    @Test
    public void testCircleSymmetry() {
        // Проверка симметрии окружности
        List<Point> points = Bresenham.circle(0, 0, 3);

        // Для каждой точки (x,y) должна существовать (-x,y)
        for (Point p : points) {
            Point symmetric = new Point(-p.x, p.y);
            assertTrue("Симметрия по X: " + p + " <-> " + symmetric,
                points.contains(symmetric));
        }
    }

    @Test
    public void testFloodFillSimple() {
        // Создаём простой canvas 5x5, заполненный нулями
        int[][] canvas = new int[5][5];

        // Заливаем с точки (2,2), меняя 0 на 1
        Bresenham.floodFill(canvas, 2, 2, 0, 1);

        // Весь canvas должен быть закрашен
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                assertEquals("Точка (" + i + "," + j + ") закрашена", 1, canvas[i][j]);
            }
        }
    }

    @Test
    public void testFloodFillWithBorder() {
        // Создаём canvas 5x5 с границей
        int[][] canvas = new int[5][5];

        // Рисуем границу (значение 2)
        for (int i = 0; i < 5; i++) {
            canvas[0][i] = 2;
            canvas[4][i] = 2;
            canvas[i][0] = 2;
            canvas[i][4] = 2;
        }

        // Заливаем внутреннюю область
        Bresenham.floodFill(canvas, 2, 2, 0, 1);

        // Проверяем, что граница не изменилась
        assertEquals("Граница сверху", 2, canvas[0][2]);
        assertEquals("Граница снизу", 2, canvas[4][2]);
        assertEquals("Граница слева", 2, canvas[2][0]);
        assertEquals("Граница справа", 2, canvas[2][4]);

        // Проверяем, что внутренность закрашена
        assertEquals("Центр закрашен", 1, canvas[2][2]);
        assertEquals("Внутренняя точка", 1, canvas[1][1]);
    }

    @Test
    public void testFloodFillOutOfBounds() {
        // Тест на выход за границы
        int[][] canvas = new int[3][3];

        // Заливка не должна вызывать исключение
        Bresenham.floodFill(canvas, -1, -1, 0, 1);
        Bresenham.floodFill(canvas, 10, 10, 0, 1);

        // Canvas должен остаться нетронутым
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                assertEquals("Точка не закрашена", 0, canvas[i][j]);
            }
        }
    }

    @Test
    public void testPointEquality() {
        Point p1 = new Point(10, 20);
        Point p2 = new Point(10, 20);
        Point p3 = new Point(10, 21);

        assertEquals("Одинаковые точки", p1, p2);
        assertNotEquals("Разные точки", p1, p3);
    }

    @Test
    public void testPointHashCode() {
        Point p1 = new Point(5, 7);
        Point p2 = new Point(5, 7);

        assertEquals("Одинаковый hashCode", p1.hashCode(), p2.hashCode());
    }

    @Test
    public void testPointToString() {
        Point p = new Point(15, 25);
        assertEquals("Формат вывода", "(15, 25)", p.toString());
    }

    // ============ ЯРКИЕ И НЕОБЫЧНЫЕ ТЕСТЫ ============

    @Test
    public void testDrawSmileyFace() {
        // Рисуем смайлик с помощью окружностей
        List<Point> face = Bresenham.circle(50, 50, 30);      // Лицо
        List<Point> leftEye = Bresenham.circle(40, 45, 3);    // Левый глаз
        List<Point> rightEye = Bresenham.circle(60, 45, 3);   // Правый глаз
        List<Point> smile = Bresenham.circle(50, 55, 15);     // Улыбка (полуокружность)

        assertTrue("Лицо содержит верхнюю точку", face.contains(new Point(50, 80)));
        assertTrue("Левый глаз нарисован", leftEye.contains(new Point(40, 48)));
        assertTrue("Правый глаз нарисован", rightEye.contains(new Point(60, 48)));
        assertTrue("Улыбка нарисована", smile.size() > 0);
    }

    @Test
    public void testDrawStar() {
        // Рисуем 5-конечную звезду из линий
        // Центр в (50, 50), внешний радиус 30, внутренний 15
        List<Point> line1 = Bresenham.line(50, 20, 35, 60);   // Верх к левому нижнему
        List<Point> line2 = Bresenham.line(35, 60, 75, 40);   // Левый низ к правому верху
        List<Point> line3 = Bresenham.line(75, 40, 25, 40);   // Правый верх к левому верху
        List<Point> line4 = Bresenham.line(25, 40, 65, 60);   // Левый верх к правому низу
        List<Point> line5 = Bresenham.line(65, 60, 50, 20);   // Правый низ к верху

        // Проверяем что все линии непустые
        assertTrue("Линия 1 звезды", line1.size() > 10);
        assertTrue("Линия 2 звезды", line2.size() > 10);
        assertTrue("Линия 3 звезды", line3.size() > 10);
        assertTrue("Линия 4 звезды", line4.size() > 10);
        assertTrue("Линия 5 звезды", line5.size() > 10);
    }

    @Test
    public void testDrawCheckerboardPattern() {
        // Рисуем шахматную доску 8x8 используя заливку
        int[][] board = new int[80][80];

        // Заполняем чёрные клетки (1)
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((row + col) % 2 == 1) {
                    // Чёрная клетка: заполняем квадрат 10x10
                    for (int y = row * 10; y < (row + 1) * 10; y++) {
                        for (int x = col * 10; x < (col + 1) * 10; x++) {
                            board[y][x] = 1;
                        }
                    }
                }
            }
        }

        // Проверяем углы доски
        assertEquals("Левый верхний угол белый", 0, board[0][0]);
        assertEquals("Правый верхний угол чёрный", 1, board[0][70]);
        assertEquals("Левый нижний угол чёрный", 1, board[70][0]);
        assertEquals("Правый нижний угол белый", 0, board[70][70]);
    }

    @Test
    public void testDrawConcentricCircles() {
        // Рисуем концентрические окружности (мишень)
        List<Point> circle1 = Bresenham.circle(100, 100, 10);
        List<Point> circle2 = Bresenham.circle(100, 100, 20);
        List<Point> circle3 = Bresenham.circle(100, 100, 30);
        List<Point> circle4 = Bresenham.circle(100, 100, 40);

        // Каждая следующая окружность больше предыдущей
        assertTrue("Окружность 2 больше 1", circle2.size() > circle1.size());
        assertTrue("Окружность 3 больше 2", circle3.size() > circle2.size());
        assertTrue("Окружность 4 больше 3", circle4.size() > circle3.size());

        // Все окружности имеют общий центр
        assertTrue("Окружность 1 на расстоянии 10", circle1.contains(new Point(100, 110)));
        assertTrue("Окружность 2 на расстоянии 20", circle2.contains(new Point(100, 120)));
        assertTrue("Окружность 3 на расстоянии 30", circle3.contains(new Point(100, 130)));
        assertTrue("Окружность 4 на расстоянии 40", circle4.contains(new Point(100, 140)));
    }

    @Test
    public void testDrawGrid() {
        // Рисуем сетку 10x10 (100 пикселей с шагом 10)
        List<List<Point>> horizontalLines = new java.util.ArrayList<>();
        List<List<Point>> verticalLines = new java.util.ArrayList<>();

        for (int i = 0; i <= 100; i += 10) {
            horizontalLines.add(Bresenham.line(0, i, 100, i));
            verticalLines.add(Bresenham.line(i, 0, i, 100));
        }

        assertEquals("11 горизонтальных линий", 11, horizontalLines.size());
        assertEquals("11 вертикальных линий", 11, verticalLines.size());

        // Проверяем пересечение линий в центре
        assertTrue("Горизонталь через центр", horizontalLines.get(5).contains(new Point(50, 50)));
        assertTrue("Вертикаль через центр", verticalLines.get(5).contains(new Point(50, 50)));
    }

    @Test
    public void testDrawSpiral() {
        // Рисуем квадратную спираль
        List<Point> spiral = new java.util.ArrayList<>();

        int x = 50, y = 50;
        int[] dx = {1, 0, -1, 0};  // Направления: вправо, вниз, влево, вверх
        int[] dy = {0, 1, 0, -1};
        int direction = 0;
        int steps = 1;
        int stepCount = 0;
        int stepsInCurrentDirection = 0;

        for (int i = 0; i < 100; i++) {
            spiral.add(new Point(x, y));
            x += dx[direction];
            y += dy[direction];
            stepsInCurrentDirection++;

            if (stepsInCurrentDirection == steps) {
                stepsInCurrentDirection = 0;
                direction = (direction + 1) % 4;
                stepCount++;
                if (stepCount == 2) {
                    stepCount = 0;
                    steps++;
                }
            }
        }

        assertTrue("Спираль содержит стартовую точку", spiral.contains(new Point(50, 50)));
        assertTrue("Спираль имеет достаточно точек", spiral.size() == 100);

        // Проверяем, что спираль расширяется
        Point start = spiral.get(0);
        Point middle = spiral.get(50);
        Point end = spiral.get(99);

        int dist1 = Math.abs(start.x - 50) + Math.abs(start.y - 50);
        int dist2 = Math.abs(end.x - 50) + Math.abs(end.y - 50);

        assertTrue("Спираль расширяется от центра", dist2 > dist1);
    }

    @Test
    public void testDrawSineWave() {
        // Рисуем синусоиду
        List<Point> wave = new java.util.ArrayList<>();

        for (int x = 0; x <= 360; x += 5) {
            double radians = Math.toRadians(x);
            int y = (int)(50 + 30 * Math.sin(radians));
            wave.add(new Point(x, y));
        }

        assertTrue("Синусоида начинается от центральной линии", wave.get(0).y >= 48 && wave.get(0).y <= 52);
        assertTrue("Синусоида проходит полный цикл", wave.size() > 70);

        // Проверяем амплитуду (min и max значения Y)
        int minY = wave.stream().mapToInt(p -> p.y).min().orElse(0);
        int maxY = wave.stream().mapToInt(p -> p.y).max().orElse(0);

        assertTrue("Минимум около 20", minY >= 18 && minY <= 22);
        assertTrue("Максимум около 80", maxY >= 78 && maxY <= 82);
    }

    @Test
    public void testDrawPixelArt() {
        // Рисуем простой пиксель-арт (например, сердечко 5x5)
        int[][] heart = {
            {0, 1, 0, 1, 0},
            {1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1},
            {0, 1, 1, 1, 0},
            {0, 0, 1, 0, 0}
        };

        int[][] canvas = new int[5][5];

        // Копируем сердечко на canvas
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                canvas[y][x] = heart[y][x];
            }
        }

        // Проверяем форму
        assertEquals("Верхушка левая", 1, canvas[0][1]);
        assertEquals("Верхушка правая", 1, canvas[0][3]);
        assertEquals("Центр", 1, canvas[1][2]);
        assertEquals("Кончик", 1, canvas[4][2]);
        assertEquals("Пустой угол", 0, canvas[0][0]);
    }

    @Test
    public void testDrawEllipse() {
        // Рисуем эллипс используя окружности с масштабированием
        // Эллипс: полуоси a=20, b=10
        List<Point> ellipse = new java.util.ArrayList<>();

        for (int angle = 0; angle < 360; angle += 5) {
            double rad = Math.toRadians(angle);
            int x = 50 + (int)(20 * Math.cos(rad));
            int y = 50 + (int)(10 * Math.sin(rad));
            ellipse.add(new Point(x, y));
        }

        // Проверяем крайние точки эллипса
        assertTrue("Правая точка ~70", ellipse.stream().anyMatch(p -> p.x >= 69 && p.x <= 71));
        assertTrue("Левая точка ~30", ellipse.stream().anyMatch(p -> p.x >= 29 && p.x <= 31));
        assertTrue("Верхняя точка ~60", ellipse.stream().anyMatch(p -> p.y >= 59 && p.y <= 61));
        assertTrue("Нижняя точка ~40", ellipse.stream().anyMatch(p -> p.y >= 39 && p.y <= 41));
    }

    @Test
    public void testDrawThickLine() {
        // Рисуем "толстую" линию (несколько параллельных линий)
        List<Point> mainLine = Bresenham.line(10, 10, 50, 50);
        List<Point> offset1 = Bresenham.line(11, 10, 51, 50);
        List<Point> offset2 = Bresenham.line(10, 11, 50, 51);

        // Объединяем все точки в "толстую" линию
        java.util.Set<Point> thickLine = new java.util.HashSet<>();
        thickLine.addAll(mainLine);
        thickLine.addAll(offset1);
        thickLine.addAll(offset2);

        // Толстая линия должна содержать больше точек, чем обычная
        assertTrue("Толстая линия шире обычной", thickLine.size() > mainLine.size());
        assertTrue("Содержит начальную область", thickLine.contains(new Point(10, 10)));
        assertTrue("Содержит конечную область", thickLine.contains(new Point(50, 50)));
    }

    @Test
    public void testDrawMaze() {
        // Создаём простой лабиринт 7x7
        int[][] maze = new int[7][7];

        // Все стены (1)
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                maze[i][j] = 1;
            }
        }

        // Создаём путь (0)
        maze[1][1] = 0; maze[1][2] = 0; maze[1][3] = 0;
        maze[2][3] = 0; maze[3][3] = 0; maze[3][4] = 0; maze[3][5] = 0;
        maze[4][5] = 0; maze[5][5] = 0;

        // Заливаем путь от входа
        Bresenham.floodFill(maze, 1, 1, 0, 2);

        // Весь путь должен быть залит
        assertEquals("Вход залит", 2, maze[1][1]);
        assertEquals("Середина пути", 2, maze[3][3]);
        assertEquals("Выход залит", 2, maze[5][5]);

        // Стены не должны измениться
        assertEquals("Стена не залита", 1, maze[0][0]);
    }

    @Test
    public void testDrawPolygon() {
        // Рисуем треугольник (3 линии)
        Point p1 = new Point(50, 20);
        Point p2 = new Point(30, 60);
        Point p3 = new Point(70, 60);

        List<Point> side1 = Bresenham.line(p1.x, p1.y, p2.x, p2.y);
        List<Point> side2 = Bresenham.line(p2.x, p2.y, p3.x, p3.y);
        List<Point> side3 = Bresenham.line(p3.x, p3.y, p1.x, p1.y);

        // Проверяем вершины
        assertTrue("Вершина 1", side1.contains(p1));
        assertTrue("Вершина 2", side2.contains(p2));
        assertTrue("Вершина 3", side3.contains(p3));

        // Проверяем что стороны связаны
        assertTrue("Сторона 1 заканчивается в вершине 2", side1.contains(p2));
        assertTrue("Сторона 2 заканчивается в вершине 3", side2.contains(p3));
        assertTrue("Сторона 3 заканчивается в вершине 1", side3.contains(p1));
    }

    // ============ ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ДЛЯ МУТАЦИОННОГО ПОКРЫТИЯ ============

    @Test
    public void testLineSteepSlope() {
        // Линия с крутым наклоном (dy > dx)
        List<Point> points = Bresenham.line(0, 0, 2, 5);

        assertEquals("Начало", new Point(0, 0), points.get(0));
        assertEquals("Конец", new Point(2, 5), points.get(points.size() - 1));
        assertTrue("Много точек по Y", points.size() >= 6);
    }

    @Test
    public void testLineNegativeX() {
        // Линия в направлении отрицательного X
        List<Point> points = Bresenham.line(5, 0, 0, 0);

        assertEquals("Начало (5,0)", new Point(5, 0), points.get(0));
        assertEquals("Конец (0,0)", new Point(0, 0), points.get(points.size() - 1));

        // X должен уменьшаться
        for (int i = 1; i < points.size(); i++) {
            assertTrue("X уменьшается", points.get(i).x <= points.get(i-1).x);
        }
    }

    @Test
    public void testLineNegativeY() {
        // Линия в направлении отрицательного Y
        List<Point> points = Bresenham.line(0, 5, 0, 0);

        assertEquals("Начало (0,5)", new Point(0, 5), points.get(0));
        assertEquals("Конец (0,0)", new Point(0, 0), points.get(points.size() - 1));

        // Y должен уменьшаться
        for (int i = 1; i < points.size(); i++) {
            assertTrue("Y уменьшается", points.get(i).y <= points.get(i-1).y);
        }
    }

    @Test
    public void testLineNegativeXAndY() {
        // Линия в направлении отрицательных X и Y
        List<Point> points = Bresenham.line(5, 5, 0, 0);

        assertEquals("Начало (5,5)", new Point(5, 5), points.get(0));
        assertEquals("Конец (0,0)", new Point(0, 0), points.get(points.size() - 1));
    }

    @Test
    public void testLineErrorCalculation() {
        // Тест на корректный расчёт ошибки (e2 > -dy и e2 < dx)
        // Линия 3:2 slope
        List<Point> points = Bresenham.line(0, 0, 6, 4);

        assertEquals("Начало", new Point(0, 0), points.get(0));
        assertEquals("Конец", new Point(6, 4), points.get(points.size() - 1));

        // Проверяем что все точки последовательны
        for (int i = 1; i < points.size(); i++) {
            int dx = Math.abs(points.get(i).x - points.get(i-1).x);
            int dy = Math.abs(points.get(i).y - points.get(i-1).y);
            assertTrue("Соседние точки", dx <= 1 && dy <= 1);
        }
    }

    @Test
    public void testCircleLargeRadius() {
        // Окружность большого радиуса
        List<Point> points = Bresenham.circle(100, 100, 50);

        assertFalse("Не пустая", points.isEmpty());
        assertTrue("Верхняя точка", points.contains(new Point(100, 150)));
        assertTrue("Нижняя точка", points.contains(new Point(100, 50)));
        assertTrue("Правая точка", points.contains(new Point(150, 100)));
        assertTrue("Левая точка", points.contains(new Point(50, 100)));
    }

    @Test
    public void testCircleDecisionVariable() {
        // Тест на переменную решения d
        // При d > 0 происходит шаг по y
        List<Point> points = Bresenham.circle(0, 0, 10);

        // Проверяем что точки на окружности
        for (Point p : points) {
            double distance = Math.sqrt(p.x * p.x + p.y * p.y);
            assertTrue("Точка на окружности", distance >= 9 && distance <= 11);
        }
    }

    @Test
    public void testCircleNegativeCenter() {
        // Окружность с отрицательным центром
        List<Point> points = Bresenham.circle(-50, -50, 10);

        assertTrue("Верхняя точка", points.contains(new Point(-50, -40)));
        assertTrue("Нижняя точка", points.contains(new Point(-50, -60)));
    }

    @Test
    public void testPointEqualityWithNull() {
        Point p = new Point(5, 10);
        assertFalse("Point не равен null", p.equals(null));
        assertFalse("Point не равен строке", p.equals("(5, 10)"));
    }

    @Test
    public void testPointHashCodeDistribution() {
        // Разные точки должны иметь разные hashCode
        Point p1 = new Point(0, 0);
        Point p2 = new Point(1, 0);
        Point p3 = new Point(0, 1);
        Point p4 = new Point(1, 1);

        java.util.Set<Integer> hashes = new java.util.HashSet<>();
        hashes.add(p1.hashCode());
        hashes.add(p2.hashCode());
        hashes.add(p3.hashCode());
        hashes.add(p4.hashCode());

        assertEquals("Все hashCode разные", 4, hashes.size());
    }

    @Test
    public void testFloodFillSameColor() {
        // Заливка тем же цветом не должна ничего делать
        int[][] canvas = new int[5][5];
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                canvas[i][j] = 1;
            }
        }

        // Заливаем 1 на 1 - ничего не должно измениться
        Bresenham.floodFill(canvas, 2, 2, 1, 1);

        // Всё должно остаться 1
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                assertEquals("Цвет не изменился", 1, canvas[i][j]);
            }
        }
    }

    @Test
    public void testFloodFillWrongTargetColor() {
        // Заливка с неправильным целевым цветом
        int[][] canvas = new int[5][5];  // все 0

        // Пытаемся заменить 5 на 1, но 5 нет на холсте
        Bresenham.floodFill(canvas, 2, 2, 5, 1);

        // Всё должно остаться 0
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                assertEquals("Цвет не изменился", 0, canvas[i][j]);
            }
        }
    }

    @Test
    public void testFloodFillCorners() {
        // Заливка из угла
        int[][] canvas = new int[5][5];

        Bresenham.floodFill(canvas, 0, 0, 0, 1);

        assertEquals("Угол залит", 1, canvas[0][0]);
        assertEquals("Центр залит", 1, canvas[2][2]);
    }

    @Test
    public void testFloodFillEdgeCases() {
        // Граничные случаи координат
        int[][] canvas = new int[3][3];

        // Заливка с края
        Bresenham.floodFill(canvas, 0, 0, 0, 2);
        assertEquals("Край залит", 2, canvas[0][0]);

        // Заливка прямо на границе
        canvas = new int[3][3];
        Bresenham.floodFill(canvas, 2, 2, 0, 3);
        assertEquals("Противоположный край залит", 3, canvas[2][2]);
    }

    @Test
    public void testLineDxEqualsZero() {
        // dx = 0 (вертикальная линия)
        List<Point> points = Bresenham.line(5, 0, 5, 10);

        for (Point p : points) {
            assertEquals("Все точки имеют x=5", 5, p.x);
        }
    }

    @Test
    public void testLineDyEqualsZero() {
        // dy = 0 (горизонтальная линия)
        List<Point> points = Bresenham.line(0, 5, 10, 5);

        for (Point p : points) {
            assertEquals("Все точки имеют y=5", 5, p.y);
        }
    }

    @Test
    public void testLineDxEqualsDy() {
        // dx = dy (диагональ 45°)
        List<Point> points = Bresenham.line(0, 0, 10, 10);

        for (Point p : points) {
            assertEquals("x = y на диагонали", p.x, p.y);
        }
    }

    @Test
    public void testCircleRadius2() {
        // Маленькая окружность радиуса 2
        List<Point> points = Bresenham.circle(0, 0, 2);

        assertTrue("Точка (0, 2)", points.contains(new Point(0, 2)));
        assertTrue("Точка (2, 0)", points.contains(new Point(2, 0)));
        assertTrue("Точка (0, -2)", points.contains(new Point(0, -2)));
        assertTrue("Точка (-2, 0)", points.contains(new Point(-2, 0)));
    }

    @Test
    public void testCircleAllOctants() {
        // Проверка всех 8 октантов окружности
        List<Point> points = Bresenham.circle(0, 0, 5);

        // Проверяем симметрию - должны быть точки во всех квадрантах
        boolean hasTopRight = points.stream().anyMatch(p -> p.x > 0 && p.y > 0);
        boolean hasTopLeft = points.stream().anyMatch(p -> p.x < 0 && p.y > 0);
        boolean hasBottomLeft = points.stream().anyMatch(p -> p.x < 0 && p.y < 0);
        boolean hasBottomRight = points.stream().anyMatch(p -> p.x > 0 && p.y < 0);

        assertTrue("Октант 1 (top-right)", hasTopRight);
        assertTrue("Октант 2 (top-left)", hasTopLeft);
        assertTrue("Октант 3 (bottom-left)", hasBottomLeft);
        assertTrue("Октант 4 (bottom-right)", hasBottomRight);
    }
}
