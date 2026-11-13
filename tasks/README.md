Языки программирования и среды разработки
========================================= 

Для выполнения лабораторных работ и решения задач.

Условия задач: http://ts.lokos.net/statements/15_cg.pdf

Суть задач
----------
* Понять, что геометрические объекты задаются как набор уравнений/неравенств.
* Понять, что пересечение объектов - это решение системы уравнений.
* Научиться вычислять для простых геометрических объектов.
* Записывать вычисления в виде программы
* Научиться тестировать и исправлять ошибки.

С/C++
-----
* http://www.mingw.org - MinGW (компилятор GNU C/C++ для Windows)
* http://www.codeblocks.org - простейшая среда разработки на C/C++ для Windows.
* http://qt-project.org/downloads - Qt Creator (развитая графическая среда разработки).
* http://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx - Visual Studio Express Edition.

``` cpp
// Пример решения первой задачи
#include <stdio.h>
#include <math.h>

int main(){
  // Открываем входной файл
  // Когда мы вводим с консоли, на самом деле читаем из файла 
  freopen("vector.in","r",stdin); // "r" (read) - открываем для чтения
  // Открываем выходной файл
  // Когда выводим на консоль => записывается в файл vector.out
  freopen("vector.out","w",stdout); // "w" (write) - открываем для записи

  // Объявляем 3 переменные вещественного типа (двойной точности)
  double x, y, z;
  // Вводим координаты
  scanf("%lf %lf %lf", &x, &y, &z);

  // Вычисляем и выводим ответ
  printf("%.2lf\n", sqrt(x*x + y*y + z*z));

  return 0;
}
```

Java
----
* http://www.oracle.com/technetwork/java/javase/downloads/index.html - скачать Java
* https://www.jetbrains.com/idea/download/ - Idea (среда разработки)


``` java
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Locale;
import java.util.Scanner;

import static java.lang.Math.sqrt;

/** A. Длина вектора */
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
```


Python
------
* https://www.python.org/downloads/ - интерпретатор Python.
* https://www.jetbrains.com/pycharm/ - среда разработки PyCharm.

``` python
#!/usr/bin/env python

from math import sqrt

with open('vector.in') as fi:
	x, y, z = (float(i) for i in fi.readline().split())

with open('vector.out', 'w') as fo:
	fo.write('{0:.2f}\n'.format(sqrt(x**2 + y**2 + z**2)))
```

Delphi
------
``` delphi 
{$APPTYPE CONSOLE}

uses SysUtils;

var
  x,y,z,d : double;
begin
  AssignFile(input,'vector.in');
  AssignFile(output,'vector.out');
  reset(input);
  rewrite(output);
  { Читаем входные данные }
  Read(x,y,z);
  { Вычисляем ответ }
  d := sqrt(x*x+y*y+z*z);
  { Вывод ответа }
  Write(d:0:2);
end.
```

Установка и настройка Code::Blocks
----------------------------------
* Скачиваем версию соответветствующую разрядности вашей операционной системы (32 / 64)
 * В 64-битном Windows есть папка: C:\Program Files (x86) 
 * Страница для скачивания: http://www.codeblocks.org/downloads/binaries
 * Версия с компилятором GCC (MinGW): codeblocks-13.12mingw-setup.exe
* Устанавливаем Code::Blocks
 * 
* 

``` cpp
   // Печать с точностью до 3-х знаков
   printf("%.3f %.3f %.3f\n", a, b, c);
```
