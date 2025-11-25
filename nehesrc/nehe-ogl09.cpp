#include <windows.h>      // Заголовочный файл для Windows
#include <stdio.h>      // Заголовочный файл для стандартного ввода/вывода
#include <gl\gl.h>      // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>     // Заголовочный файл для для библиотеки GLu32
#include <gl\glaux.h>     // Заголовочный файл для библиотеки GLaux

static HGLRC hRC;   // Постоянный контекст рендеринга
static HDC hDC;     // Приватный контекст устройства GDI

bool  keys[256];         // Массив, использующийся для сохранения состояния клавиатуры
bool  active = TRUE;     // Флаг состояния активности приложения (по умолчанию: TRUE)
bool  fullscreen = TRUE; // Флаг полноэкранного режима (по умолчанию: полноэкранное)

BOOL  twinkle;      // Twinkling Stars (Вращающиеся звезды)
BOOL  tp;       // 'T' клавиша нажата?

const int num = 50;   // Количество рисуемых звезд

typedef struct {      // Создаём структуру для звезд
    int r, g, b;      // Цвет звезды
    GLfloat dist;     // Расстояние от центра
    GLfloat angle;    // Текущий угол звезды
}
stars;          // Имя структуры - Stars
stars star[num];      // Делаем массив 'star' длинной 'num',
// где элементом является структура 'stars'

GLfloat zoom = -15.0f;    // Расстояние от наблюдателя до звезд
GLfloat tilt = 90.0f;   // Начальный угол
GLfloat spin;       // Для вращения звезд

GLuint  loop;       // Используется для циклов
GLuint  texture[1];     // Массив для одной текстуры

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Объявления для WndProc

AUX_RGBImageRec* LoadBMP(char* Filename) { // Функция для загрузки bmp файлов
    FILE* File = NULL;  // Переменная для файла

    if (!Filename) {    // Нужно убедиться в правильности переданого имени
        return NULL;    // Если неправильное имя, то возвращаем NULL
    }

    File = fopen(Filename, "r"); // Открываем и проверяем на наличие файла

    if (File) {   // Файл существует?
        fclose(File);   // Если да, то закрываем файл
        // И загружаем его с помощью библиотеки AUX, возращая ссылку на изображение
        return auxDIBImageLoad(Filename);
    }

    // Если загрузить не удалось или файл не найден, то возращаем NULL
    return NULL;
}

int LoadGLTextures() { // Функция загрузки изображения и конвертирования в текстуру
    int Status = FALSE; // Индикатор статуса

    AUX_RGBImageRec* TextureImage[1];// Создаём место для хранения текстуры

    memset(TextureImage, 0, sizeof(void*) * 1); // устанавливаем ссылку на NULL

    // Загружаем изображение, Проверяем на ошибки, Если файл не найден то выходим
    if (TextureImage[0] = LoadBMP("Data/Star.bmp")) {
        Status = TRUE;  // Ставим статус в TRUE

        glGenTextures(1, &texture[0]);  // Генерируем один индификатор текстуры

        // Создаём текстуру с линейной фильтрацией (Linear Filtered)
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
                     TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }

    if (TextureImage[0]) {  // Если текстура существует
        if (TextureImage[0]->data) { // Если изображение существует
            // Освобождаем место выделенное под изображение
            free(TextureImage[0]->data);
        }

        free(TextureImage[0]);  // Освобождаем структуры изображения
    }

    return Status;      // Возвращаем статус
}

GLvoid InitGL(GLsizei Width, GLsizei Height) { // Вызвать после создания окна GL
    LoadGLTextures();     // Загрузка текстур
    glEnable(GL_TEXTURE_2D);  // Разрешение наложение текстуры

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Очистка экрана в черный цвет
    glClearDepth(1.0);    // Разрешить очистку буфера глубины
    glDepthFunc(GL_LESS); // Тип теста глубины
    glEnable(GL_DEPTH_TEST);// разрешить тест глубины
    glShadeModel(GL_SMOOTH);// разрешить плавное цветовое сглаживание
    glMatrixMode(GL_PROJECTION);// Выбор матрицы проекции
    glLoadIdentity();   // Сброс матрицы проекции
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // Вычислить соотношение геометрических размеров для окна
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Изящная коррекция перспективы
    glDepthFunc(GL_LEQUAL); // Тип выполняемой проверки глубины
    // Устанавливаем функцию смешивания
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);     // Включаем смешивание
    glMatrixMode(GL_MODELVIEW);// Выбор матрицы просмотра модели

    for (loop = 0; loop < num; loop++) { // Делаем цикл и бежим по всем звездам
        star[loop].angle = 0.0f; // Устанавливаем всё углы в 0
        // Вычисляем растояние до центра
        star[loop].dist = (float(loop) / num) * 5.0f;
        // Присваиваем star[loop] случайное значение (красный).
        star[loop].r = rand() % 256;
        // Присваиваем star[loop] случайное значение (зеленый)
        star[loop].g = rand() % 256;
        // Присваиваем star[loop] случайное значение (голубой)
        star[loop].b = rand() % 256;
    }
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
    if (Height == 0) { // Предотвращение деления на ноль, если окно слишком мало
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    // Сброс текущей области вывода и перспективных преобразований

    glMatrixMode(GL_PROJECTION);// Выбор матрицы проекций
    glLoadIdentity();     // Сброс матрицы проекции

    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // Вычисление соотношения геометрических размеров для окна
    glMatrixMode(GL_MODELVIEW); // Выбор матрицы просмотра модели
}

int DrawGLScene(GLvoid) {   // Здесь мы всё рисуем
    // Очищаем буфер цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Выбираем нашу текстуру
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    for (loop = 0; loop < num; loop++) { // Цикл по всем звездам
        // Обнуляем видовую матрицу (Model Matrix) перед каждой звездой
        glLoadIdentity();
        // Переносим по оси z на 'zoom'
        glTranslatef(0.0f, 0.0f, zoom);
        // Вращаем вокруг оси x на угол 'tilt'
        glRotatef(tilt, 1.0f, 0.0f, 0.0f);
        // Поворачиваем на угол звезды вокруг оси y
        glRotatef(star[loop].angle, 0.0f, 1.0f, 0.0f);
        // Двигаемся вперед по оси x
        glTranslatef(star[loop].dist, 0.0f, 0.0f);
        glRotatef(-star[loop].angle, 0.0f, 1.0f, 0.0f);
        // Отменяет текущий поворот звезды
        glRotatef(-tilt, 1.0f, 0.0f, 0.0f); // Отменяет поворот экрана

        if (twinkle) {      // Если Twinkling включен
            // Данный цвет использует байты
            glColor4ub(star[(num - loop) - 1].r, star[(num - loop) - 1].g,
                       star[(num - loop) - 1].b, 255);
            glBegin(GL_QUADS);// Начинаем рисовать текстурированый квадрат
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( 1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 0.0f);
            glEnd();        // Закончили рисовать
        }

        glRotatef(spin, 0.0f, 0.0f, 1.0f); // Поворачиваем звезду по оси z
        // Цвет использует байты
        glColor4ub(star[loop].r, star[loop].g, star[loop].b, 255);
        glBegin(GL_QUADS);    // Начинаем рисовать текстурный квадрат
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();          // Закончили рисовать
        spin += 0.01f;    // Используется для вращения звезды
        star[loop].angle += float(loop) / num; // Меняем угол звезды
        star[loop].dist -= 0.01f; // Меняем растояние до центра

        if (star[loop].dist < 0.0f) { // Звезда в центре экрана?
            star[loop].dist += 5.0f; // Перемещаем на 5 единиц от центра
            // Новое значение красной компоненты цвета
            star[loop].r = rand() % 256;
            // Новое значение зеленной компоненты цвета
            star[loop].g = rand() % 256;
            // Новое значение синей компоненты цвета
            star[loop].b = rand() % 256;
        }
    }

    return TRUE;            // Всё ок
}


LRESULT CALLBACK WndProc(
    HWND  hWnd,
    UINT  message,
    WPARAM  wParam,
    LPARAM  lParam) {
    RECT  Screen;   // используется позднее для размеров окна
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // Размер этой структуры
        1,        // Номер версии (?)
        PFD_DRAW_TO_WINDOW |// Формат для Окна
        PFD_SUPPORT_OPENGL |// Формат для OpenGL
        PFD_DOUBLEBUFFER,// Формат для двойного буфера
        PFD_TYPE_RGBA,  // Требуется RGBA формат
        16,       // Выбор 16 бит глубины цвета
        0, 0, 0, 0, 0, 0,// Игнорирование цветовых битов (?)
        0,        // нет буфера прозрачности
        0,        // Сдвиговый бит игнорируется (?)
        0,        // Нет буфера аккумуляции
        0, 0, 0, 0,   // Биты аккумуляции игнорируются (?)
        16,       // 16 битный Z-буфер (буфер глубины)
        0,        // Нет буфера траффарета
        0,        // Нет вспомогательных буферов (?)
        PFD_MAIN_PLANE, // Главный слой рисования
        0,        // Резерв (?)
        0, 0, 0     // Маски слоя игнорируются (?)
    };

    switch (message) { // Тип сообщения
        case WM_CREATE:
            hDC = GetDC(hWnd);  // Получить контекст устройства для окна
            PixelFormat = ChoosePixelFormat(hDC, &pfd);

            // Найти ближайшее совпадение для нашего формата пикселов
            if (!PixelFormat) {
                MessageBox(0, "Can't Find A Suitable PixelFormat.", "Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                // Это сообщение говорит, что программа должна завершится
                break;  // Предтовращение повтора кода
            }

            if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {
                MessageBox(0, "Can't Set The PixelFormat.", "Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            hRC = wglCreateContext(hDC);

            if(!hRC) {
                MessageBox(0, "Can't Create A GL Rendering Context.", "Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            if(!wglMakeCurrent(hDC, hRC)) {
                MessageBox(0, "Can't activate GLRC.", "Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            GetClientRect(hWnd, &Screen);
            InitGL(Screen.right, Screen.bottom);
            break;

        case WM_DESTROY:
        case WM_CLOSE:
            ChangeDisplaySettings(NULL, 0);

            wglMakeCurrent(hDC, NULL);
            wglDeleteContext(hRC);
            ReleaseDC(hWnd, hDC);

            PostQuitMessage(0);
            break;

        case WM_KEYDOWN:
            keys[wParam] = TRUE;
            break;

        case WM_KEYUP:
            keys[wParam] = FALSE;
            break;

        case WM_SIZE:
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
            break;

        default:
            return (DefWindowProc(hWnd, message, wParam, lParam));
    }

    return (0);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow) {
    MSG   msg;  // Структура сообщения Windows
    WNDCLASS  wc; // Структура класса Windows для установки типа окна
    HWND    hWnd;// Сохранение дискриптора окна

    wc.style      = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc    = (WNDPROC) WndProc;
    wc.cbClsExtra   = 0;
    wc.cbWndExtra   = 0;
    wc.hInstance    = hInstance;
    wc.hIcon      = NULL;
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground  = NULL;
    wc.lpszMenuName   = NULL;
    wc.lpszClassName  = "OpenGL WinClass";

    if(!RegisterClass(&wc)) {
        MessageBox(0, "Failed To Register The Window Class.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hWnd = CreateWindow(
               "OpenGL WinClass",
               "Jeff Molofee's GL Code Tutorial ... NeHe '99", // Заголовок вверху окна

               WS_POPUP |
               WS_CLIPCHILDREN |
               WS_CLIPSIBLINGS,

               0, 0,     // Позиция окна на экране
               640, 480,   // Ширина и высота окна

               NULL,
               NULL,
               hInstance,
               NULL);

    if(!hWnd) {
        MessageBox(0, "Window Creation Error.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    DEVMODE dmScreenSettings;     // Режим работы

    memset(&dmScreenSettings, 0, sizeof(DEVMODE));  // Очистка для хранения установок
    dmScreenSettings.dmSize = sizeof(DEVMODE);    // Размер структуры Devmode
    dmScreenSettings.dmPelsWidth  = 640;      // Ширина экрана
    dmScreenSettings.dmPelsHeight = 480;      // Высота экрана
    dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пиксела
    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
    // Переключение в полный экран

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

    while (1) {
        // Обработка всех сообщений
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
            if (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                return TRUE;
            }
        }

        DrawGLScene();        // Нарисовать сцену
        SwapBuffers(hDC);     // Переключить буфер экрана

        if (keys['T'] && !tp) { // Если 'T' нажата и tp равно FALSE
            tp = TRUE;  // то делаем tp равным TRUE
            twinkle = !twinkle; // Меняем значение twinkle на обратное
        }

        if (!keys['T']) {   // Клавиша 'T' была отключена
            tp = FALSE; // Делаем tp равное FALSE
        }

        if (keys[VK_UP]) {  // Стрелка вверх была нажата?
            tilt -= 0.5f; // Вращаем экран вверх
        }

        if (keys[VK_DOWN]) {  // Стрелка вниз нажата?
            tilt += 0.5f; // Вращаем экран вниз
        }

        if (keys[VK_PRIOR]) { // Page Up нажат?
            zoom -= 0.2f; // Уменьшаем
        }

        if (keys[VK_NEXT]) {  // Page Down нажата?
            zoom += 0.2f; // Увеличиваем
        }

        if (keys[VK_ESCAPE]) {
            SendMessage(hWnd, WM_CLOSE, 0, 0);    // Если ESC - выйти
        }
    }
}

