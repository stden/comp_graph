import org.junit.Assert;
import org.junit.Test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

/**
 * Тесты для класса B_line - уравнение прямой по двум точкам
 *
 * Обучающая цель: понять, как строится уравнение прямой ax + by + c = 0
 *
 * Теория:
 * - Прямая проходящая через точки (x1, y1) и (x2, y2)
 * - Коэффициенты: a = y1 - y2, b = x2 - x1, c = -a*x1 - b*y1
 * - Точка лежит на прямой, если ax + by + c = 0
 */
public class B_LineTest extends Assert {

    /**
     * Тест 1: Горизонтальная прямая
     *
     * Две точки с одинаковой y-координатой образуют горизонтальную прямую
     * Пример: (1, 2) и (3, 2) -> y = 2 -> 0x + 1y - 2 = 0
     * a = y1 - y2 = 2 - 2 = 0
     * b = x2 - x1 = 3 - 1 = 2
     * c = -a*x1 - b*y1 = 0 - 2*2 = -4
     */
    @Test
    public void testHorizontalLine() throws IOException {
        // Входные данные: две точки на одной высоте
        doTest("1 2 3 2", "0.000 2.000 -4.000");
    }

    /**
     * Тест 2: Вертикальная прямая
     *
     * Две точки с одинаковой x-координатой образуют вертикальную прямую
     * Пример: (2, 1) и (2, 3) -> x = 2 -> 1x + 0y - 2 = 0
     * a = y1 - y2 = 1 - 3 = -2
     * b = x2 - x1 = 2 - 2 = 0
     * c = -a*x1 - b*y1 = -(-2)*2 - 0 = 4
     */
    @Test
    public void testVerticalLine() throws IOException {
        // Входные данные: две точки на одной вертикали
        doTest("2 1 2 3", "-2.000 0.000 4.000");
    }

    /**
     * Тест 3: Диагональная прямая под углом 45 градусов
     *
     * Прямая y = x проходит через (0,0) и (1,1)
     * Уравнение: a = 0-1 = -1, b = 1-0 = 1, c = -(-1)*0 - 1*0 = 0
     * Результат: -1x + 1y + 0 = 0 или -x + y = 0 или y = x
     */
    @Test
    public void testDiagonalLine() throws IOException {
        doTest("0 0 1 1", "-1.000 1.000 0.000");
    }

    /**
     * Тест 4: Прямая с произвольным наклоном
     *
     * Точки (1, 2) и (4, 6) образуют прямую
     * a = y1-y2 = 2-6 = -4
     * b = x2-x1 = 4-1 = 3
     * c = -a*x1 - b*y1 = -(-4)*1 - 3*2 = 4 - 6 = -2
     * Проверка: -4*1 + 3*2 + (-2) = -4 + 6 - 2 = 0 ✓
     */
    @Test
    public void testArbitraryLine() throws IOException {
        doTest("1 2 4 6", "-4.000 3.000 -2.000");
    }

    /**
     * Тест 5: Прямая с дробными координатами
     *
     * Демонстрирует работу с вещественными числами
     * Точки (1.5, 2.5) и (3.5, 4.5)
     * a = 2.5 - 4.5 = -2
     * b = 3.5 - 1.5 = 2
     * c = -(-2)*1.5 - 2*2.5 = 3 - 5 = -2
     */
    @Test
    public void testDecimalCoordinates() throws IOException {
        doTest("1.5 2.5 3.5 4.5", "-2.000 2.000 -2.000");
    }

    /**
     * Тест 6: Прямая через начало координат
     *
     * Особый случай: одна из точек - начало координат (0, 0)
     * Упрощает вычисления: c = 0
     */
    @Test
    public void testLineThroughOrigin() throws IOException {
        // Прямая через (0,0) и (2,3)
        // a = 0-3 = -3
        // b = 2-0 = 2
        // c = -(-3)*0 - 2*0 = 0
        doTest("0 0 2 3", "-3.000 2.000 0.000");
    }

    /**
     * Вспомогательный метод для выполнения теста
     *
     * @param input входные данные "x1 y1 x2 y2"
     * @param answer ожидаемый результат "a b c"
     */
    private void doTest(String input, String answer) throws IOException {
        String fileName = "line";

        // Записываем входные данные в файл
        SaveToFile(input, fileName + ".in");

        // Запускаем программу
        B_line.main(null);

        // Читаем результат из выходного файла
        try (Scanner in = new Scanner(new File(fileName + ".out"))) {
            String output = in.nextLine();
            assertEquals("Уравнение прямой должно совпадать", answer, output);
        }
    }

    /**
     * Сохраняет текст в файл
     *
     * @param text содержимое файла
     * @param fileName имя файла
     */
    private void SaveToFile(String text, String fileName) throws FileNotFoundException {
        try (PrintWriter pw = new PrintWriter(fileName)) {
            pw.println(text);
        }
    }
}
