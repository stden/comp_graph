import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Locale;
import java.util.Scanner;

import static java.lang.Math.sqrt;

/**
 * Задача A. Длина вектора
 */
public class A_Vector {

    public static final String fileName = "vector";

    /**
     * @param args Аргументы командной строки
     * @throws FileNotFoundException Если нет входного файла
     */
    public static void main(String[] args)
            throws IOException {
        // Указываем формат чисел и дат
        // чтобы использовалась десятичная точка
        // а не запятая :)
        Locale.setDefault(Locale.ENGLISH);
        // Чтение входных данных
        Scanner in = new Scanner(new File(fileName + ".in"));
        // Считываем вектор из входного файла
        double x = in.nextDouble();
        double y = in.nextDouble();
        double z = in.nextDouble();
        // Закрываем входной файл
        in.close();

        // Вычисляем длину вектора
        double d = sqrt(x * x + y * y + z * z);

        // Открываем выходной файл для записи
        PrintWriter out = new PrintWriter(fileName + ".out");
        out.println(String.format("%.2f", d));
        out.close();
    }
}
