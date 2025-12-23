import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Locale;
import java.util.Scanner;

import static java.lang.Math.abs;

public class B_line {

    public static void main(String[] args) throws FileNotFoundException {
        String fileName = "line";
        Scanner in = new Scanner(new File(fileName + ".in"));
        // Чтобы точка "." воспринималась как разделитель ставим локаль
        in.useLocale(Locale.ENGLISH);
        // Координаты точек из входного файла
        double x1, y1, x2, y2;
        x1 = in.nextDouble();
        y1 = in.nextDouble();
        x2 = in.nextDouble();
        y2 = in.nextDouble();
        in.close();

        // Вычисляем a b и c
        double a = y1 - y2;
        double b = x2 - x1;
        double c = -a * x1 - b * y1;
        double c2 = -a * x2 - b * y2;

        assertEquals(c, c2);
        assertEquals(0, a * x1 + b * y1 + c);
        assertEquals(0, a * x2 + b * y2 + c);

        PrintWriter out = new
                PrintWriter(new File(fileName + ".out"));
        out.printf("%.3f %.3f %.3f", a, b, c);
        out.close();
    }

    private static void assertEquals(double expected, double actual) {
        if (abs(expected - actual) > 1e-10) {
            throw new AssertionError("" +
                    expected + " != " + actual);
        }
    }
}
