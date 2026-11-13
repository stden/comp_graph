/*
 *      Этот код был написан Jeff'ом Molofee 2000
 *      И измене Giuseppe D'Agata'ом (waveform@tiscalinet.it)
 *      Если вы найдете его полезным, то дайте мне знать.
 *      Посетите мою страничку на nehe.gamedev.net
 */

#include <windows.h>        // Заголовок для Windows
#include <math.h>           // Заголовок для математической библиотеки Windows 
#include <stdio.h>          // Заголовок для стандартной библиотеки ввода/вывода
#include <gl\gl.h>          // Заголовок для библиотеки OpenGL32 
#include <gl\glu.h>         // Заголовок для библиотеки GLu32 
#include <gl\glaux.h>       // Заголовок для библиотеки Glaux 

HDC         hDC = NULL;     // Приватный контекст устройства GDI
HGLRC       hRC = NULL;     // Постоянный контекст рендеринга
HWND        hWnd = NULL;    // Сохраняет дескриптор окна
HINSTANCE   hInstance;      // Сохраняет экземпляр приложения

bool    keys[256];          // Массив для работы с клавиатурой
bool    active = TRUE;      // Флаг активациии окна, по умолчанию = TRUE
bool    fullscreen = TRUE;  // Флаг полноэкранного вывода

GLuint  base;               // Основной список отображения для шрифта
GLuint  texture[2];         // Место для текстуры нашего шрифта
GLuint  loop;               // Общая переменная для циклов

GLfloat cnt1;               // Первый счетчик для движения и раскрашивания текста
GLfloat cnt2;               // Второй счетчик для движения и раскрашивания текста

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);   // Объявление WndProc

AUX_RGBImageRec* LoadBMP(char* Filename) {              // Загрузка изображения
    FILE* File = NULL;                              // Дескриптор файла

    if (!Filename) {                                // Удоставеримся, что имя файла передано
        return NULL;                            // Если нет, возвратим NULL
    }

    File = fopen(Filename, "r");                    // Проверка, существует ли файл

    if (File) {                                     // Сувществует?
        fclose(File);                           // Закрываем файл
        return auxDIBImageLoad(Filename);       // Загружаем изображение и возвращаем указатель
    }

    return NULL;                                    // Если загрузка не удалась, возвращаем NULL
}

int LoadGLTextures() {                                  // Загрузка и преобразование текстур
    int Status = FALSE;                             // Индикатор статуса
    AUX_RGBImageRec* TextureImage[2];               // Место храниения для текстур
    memset(TextureImage, 0, sizeof(void*) * 2);     // Устанавливаем указатель в NULL

    if ((TextureImage[0] = LoadBMP("Data/Font.bmp")) &&
            (TextureImage[1] = LoadBMP("Data/Bumps.bmp"))) {
        Status = TRUE;                          // Устанавливаем статус в TRUE
        glGenTextures(2, &texture[0]);          // Создание 2-х текстур

        for (loop = 0; loop < 2; loop++) {
            glBindTexture(GL_TEXTURE_2D, texture[loop]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);
        }
    }

    for (loop = 0; loop < 2; loop++) {
        if (TextureImage[loop]) {                       // Если текстура существует
            if (TextureImage[loop]->data) {         // Если изображение текстуры существует
                free(TextureImage[loop]->data); // Освобождаем памяти от изображения текстуры
            }

            free(TextureImage[loop]);               // Освобождаем памяти от структуры изображения
        }
    }

    return Status;                                  // Возвращаем статус
}

GLvoid BuildFont(GLvoid) {                              // Создаем список отображения нашего шрифта
    float   cx;                                         // Содержит X координату символа
    float   cy;                                         // Содержит Y координату символа

    base = glGenLists(256);                             // Создаем списки
    glBindTexture(GL_TEXTURE_2D, texture[0]);           // Выбираем текстуру шрифта

    for (loop = 0; loop < 256; loop++) {                // Цикл по всем 256 спискам
        cx = float(loop % 16) / 16.0f;                  // X координата текущего символа
        cy = float(loop / 16) / 16.0f;                  // Y координата текущего символа

        glNewList(base + loop, GL_COMPILE);             // Начинаем делать список
        glBegin(GL_QUADS);                              // Используем четырехугольник, для каждого символа
        glTexCoord2f(cx, 1 - cy - 0.0625f);             // Точка в текстуре (Левая нижняя)
        glVertex2i(0, 0);                               // Координаты вершины (Левая нижняя)
        glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);   // Точка на текстуре (Правая нижняя)
        glVertex2i(16, 0);                      // Координаты вершины (Правая нижняя)
        glTexCoord2f(cx + 0.0625f, 1 - cy);     // Точка текстуры (Верхняя правая)
        glVertex2i(16, 16);                     // Координаты вершины (Верхняя правая)

        glTexCoord2f(cx, 1 - cy);               // Точка текстуры (Верхняя левая)
        glVertex2i(0, 16);                      // Координаты вершины (Верхняя левая)
        glEnd();                                    // Конец построения четырехугольника (Символа)
        glTranslated(10, 0, 0);                     // Двигаемя вправо от символа
        glEndList();                                    // Заканчиваем создавать список отображения
    }                                                   // Цикл для создания всех 256 символов
}

GLvoid KillFont(GLvoid) {                               // Удаляем шрифт из памяти
    glDeleteLists(base, 256);                           // Удаляем все 256 списков отображения
}

GLvoid glPrint(GLint x, GLint y, char* string, int set) { // Где печатать
    if (set > 1) {
        set = 1;
    }

    glBindTexture(GL_TEXTURE_2D, texture[0]);           // Выбираем нашу текстуру шрифта
    glDisable(GL_DEPTH_TEST);                           // Отмена проверки глубины
    glMatrixMode(GL_PROJECTION);                        // Выбираем матрицу проектирования
    glPushMatrix();                                     // Сохраняем матрицу проектирования
    glLoadIdentity();                                   // Сбрасываем матрицу проекции
    glOrtho(0, 640, 0, 480, -1, 1);                     // Устанавливаем Ortho экран
    glMatrixMode(GL_MODELVIEW);                         // Выбираем матрицу модели просмотра
    glPushMatrix();                                     // Сохраняем матрицу модели просмотра
    glLoadIdentity();                                   // Сбрасываем матрицу модели просмотра
    glTranslated(x, y, 0);                              // Позиция текста (0,0 - Нижняя левая)
    glListBase(base - 32 + (128 * set));                // Выбираем набор символов (0 или 1)
    glCallLists(strlen(string), GL_BYTE, string);       // Рисуем текст на экране
    glMatrixMode(GL_PROJECTION);                        // Выбираем матрицу проектирования
    glPopMatrix();                                      // Восстанавливаем старую матрицу проектирования
    glMatrixMode(GL_MODELVIEW);                         // Выбираем матрицу просмотра модели
    glPopMatrix();                                      // Восстанавливаем старую матрицу проектирования
    glEnable(GL_DEPTH_TEST);                            // Разрешаем тест глубины
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {   // Инициализация и изменение размеров окна GL
    if (height == 0) {                                  // Предупреждаем деление на 0
        height = 1;                                     // Делаем высоту равную 1
    }

    glViewport(0, 0, width, height);                    // Сбрасываем порт вывода
    glMatrixMode(GL_PROJECTION);                        // Выбираем матрицу проектирования
    glLoadIdentity();                                   // Сбрасываем матрицу проектирования
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // Вычисляем коэффициент отношения
    glMatrixMode(GL_MODELVIEW);                         // Выбираем матрицу просмотра модели
    glLoadIdentity();                                   // Сбрасываем матрицу просмотра модели
}

int InitGL(GLvoid) {                                    // Все установки для OpenGL здесь
    if (!LoadGLTextures()) {                            // Переходим к загрузке текстуры
        return FALSE;                                   // Если текстура не загрузилась - возвращаем FALSE
    }

    BuildFont();                                        // Создаем шрифт
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               // Очищаем фон черным цветом
    glClearDepth(1.0);                                  // Разрешаем очистку буфера глубины
    glDepthFunc(GL_LEQUAL);                             // Тип теста глубины
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                  // Тип смешивания
    glShadeModel(GL_SMOOTH);                            // Разрешаем плавную заливку
    glEnable(GL_TEXTURE_2D);                            // Разрешаем 2-х мерное текстурирование
    return TRUE;                                        // Инициализация прошла успешно
}

int DrawGLScene(GLvoid) {                               // Здесь мы рисуем все объекты
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экрана и буфера глубины
    glLoadIdentity();                                   // Сброс матрици просмотра модели
    glBindTexture(GL_TEXTURE_2D, texture[1]);           // Выбираем вторую текстуру
    glTranslatef(0.0f, 0.0f, -5.0f);                    // Двигаемся на 5 едениц
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);                 // Поворачивемся на 45 градусов (по часовой стрелке)
    glRotatef(cnt1 * 30.0f, 1.0f, 1.0f, 0.0f);          // Вращение по X & Y на cnt1 (слева на право)
    glDisable(GL_BLEND);                                // Отменяем смешивание перед рисованием 3D
    glColor3f(1.0f, 1.0f, 1.0f);                        // Ярко белый
    glBegin(GL_QUADS);                                  // Рисуем первый текстурированный прямоугольник
    glTexCoord2d(0.0f, 0.0f);                       // Первая точка в текстуре
    glVertex2f(-1.0f, 1.0f);                        // Первая вершина
    glTexCoord2d(1.0f, 0.0f);                       // Вторая точка в текстуре
    glVertex2f( 1.0f, 1.0f);                        // Вторая вершина
    glTexCoord2d(1.0f, 1.0f);                       // Третья точка в текстуре
    glVertex2f( 1.0f, -1.0f);                       // Третья вершина
    glTexCoord2d(0.0f, 1.0f);                       // Четвертая точка в текстуре
    glVertex2f(-1.0f, -1.0f);                       // Четвертая вершина
    glEnd();                                            // Заканчиваем рисование четырехугольника
    glRotatef(90.0f, 1.0f, 1.0f, 0.0f);                 // Поворачиваемся по X и Y на 90 градусов (слева на право)
    glBegin(GL_QUADS);                                  // Рисуем второй текстурированный четырехугольник
    glTexCoord2d(0.0f, 0.0f);                       // Первая точка текстуры
    glVertex2f(-1.0f, 1.0f);                        // Первая вершина
    glTexCoord2d(1.0f, 0.0f);                       // Вторая точка текстуры
    glVertex2f( 1.0f, 1.0f);                        // Вторая вершина
    glTexCoord2d(1.0f, 1.0f);                       // Третья точка текстуры
    glVertex2f( 1.0f, -1.0f);                       // Третья вершина
    glTexCoord2d(0.0f, 1.0f);                       // Четвертая точка текстуры
    glVertex2f(-1.0f, -1.0f);                       // Четвертая вершина
    glEnd();                                            // Заканчиваем рисовать четырехугольник
    glEnable(GL_BLEND);                                 // Разрешаем смешивание

    glLoadIdentity();                                   // Сбрасываем просмотр
    // Изменение цвета основывается на положении текста
    glColor3f(1.0f * float(cos(cnt1)), 1.0f * float(sin(cnt2)), 1.0f - 0.5f * float(cos(cnt1 + cnt2)));
    glPrint(int((280 + 250 * cos(cnt1))), int(235 + 200 * sin(cnt2)), "NeHe", 0); // Печатаем GL текст на экране

    glColor3f(1.0f * float(sin(cnt2)), 1.0f - 0.5f * float(cos(cnt1 + cnt2)), 1.0f * float(cos(cnt1)));
    glPrint(int((280 + 230 * cos(cnt2))), int(235 + 200 * sin(cnt1)), "OpenGL", 1); // Печатаем GL текст на экране

    glColor3f(0.0f, 0.0f, 1.0f);
    glPrint(int(240 + 200 * cos((cnt2 + cnt1) / 5)), 2, "Giuseppe D'Agata", 0);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPrint(int(242 + 200 * cos((cnt2 + cnt1) / 5)), 2, "Giuseppe D'Agata", 0);

    cnt1 += 0.01f;                                      // Увеличим первый счетчик
    cnt2 += 0.0081f;                                    // Увеличим второй счетчик
    return TRUE;                                        // Все прошло успешно
}

GLvoid KillGLWindow(GLvoid) {                           // Необходимо убить окно
    if (fullscreen) {                                   // Режим полноэкранный?
        ChangeDisplaySettings(NULL, 0);                 // Если да, переключаемся в режим десктопа
        ShowCursor(TRUE);                               // Показываем курсор мыши
    }

    if (hRC) {                                          // У нас есть контекст рендеринга?
        if (!wglMakeCurrent(NULL, NULL)) {              // Можем закрыть контексты DC и RC?
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC)) {                   // Можем удалить RC?
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        hRC = NULL;                                     // Установим RC в NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC)) {                 // Можем закрыть DC
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                     // Устанавливаем DC в NULL
    }

    if (hWnd && !DestroyWindow(hWnd)) {                 // Можем уничтожить окно?
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                    // Устанавливаем hWnd в NULL
    }

    if (!UnregisterClass("OpenGL", hInstance)) {        // Можем разрегистрировать класс?
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                               // Устанавливаем hInstance в NULL
    }

    KillFont();                                         // Убиваем шрифт
}

/*  Это код создает наше окно Open GL. Параметры:
 *  title           - Заголовок появится сверху окна                        *
 *  width           - Ширина GL окна или полного экрана                     *
 *  height          - Высота GL окна или полного экрана                     *
 *  bits            - Количество битов цвета (8/16/24/32)                   *
 *  fullscreenflag  - Полный экран (TRUE) или оконный режим         (FALSE) */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {
    GLuint      PixelFormat;            // Сохраняет результаты после поиска совпадения
    WNDCLASS    wc;                     // Структура класса окна
    DWORD       dwExStyle;              // Расширенный стиль окна
    DWORD       dwStyle;                // Стиль окна
    RECT        WindowRect;             // Прямоугольник Верхний левый/Нижний правый углы
    WindowRect.left = (long)0;          // Левая позиция = 0
    WindowRect.right = (long)width;     // Правая - запрошенная длина
    WindowRect.top = (long)0;           // Верхнее значение = 0
    WindowRect.bottom = (long)height;   // Нижнее - запрошенная высота

    fullscreen = fullscreenflag;        // Глобальная переменная полноэкранного режима

    hInstance           = GetModuleHandle(NULL);                // Захватываем экземпляр
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // Перерисовка при изменении размера и DC для окна
    wc.lpfnWndProc      = (WNDPROC) WndProc;                    // WndProc обработчик сообщений
    wc.cbClsExtra       = 0;                                    // Без дополнительных данных окна
    wc.cbWndExtra       = 0;                                    // Без дополнительных данных окна
    wc.hInstance        = hInstance;                            // Устанавливаем экземпляр
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);          // Загружаем стандартную иконку
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);          // Загружаем стрелку курсора мыши
    wc.hbrBackground    = NULL;                                 // Без фона для GL
    wc.lpszMenuName     = NULL;                                 // Без меню
    wc.lpszClassName    = "OpenGL";                             // Имя класса

    if (!RegisterClass(&wc)) {                                  // Пробуем регистрировать класс
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                           // Возвращаем FALSE
    }

    if (fullscreen) {                                           // Полноэкранный режим?
        DEVMODE dmScreenSettings;                               // Режим устройства
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); // Удостоверимся, что память очищена
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // Размер структуры Devmode
        dmScreenSettings.dmPelsWidth    = width;                // Шерина экрана
        dmScreenSettings.dmPelsHeight   = height;               // Высота экрана
        dmScreenSettings.dmBitsPerPel   = bits;                 // Бит на пиксел
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Пытаемся установить текущий режим и получаем результат.  Замечание: CDS_FULLSCREEN избавляемся от Start бара.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // Если режим не удалось установить, то два варианта. Выход или оконный режим.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE;     // Оконный режим.  Fullscreen = FALSE
            } else {
                // Окно с сообщением о том, что программа закрывается
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return FALSE;                                   // Возвращаем FALSE
            }
        }
    }

    if (fullscreen) {                                           // Мы все еще в полноэкранном режиме?
        dwExStyle = WS_EX_APPWINDOW;                            // Расширенный стиль окна
        dwStyle = WS_POPUP;                                     // Стиль окна
        ShowCursor(FALSE);                                      // Скрываем мышь
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;         // Расширенный стиль окна
        dwStyle = WS_OVERLAPPEDWINDOW;                          // Стиль окна
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Приводим окно к требуемому размеру

    // Создаем окно
    if (!(hWnd = CreateWindowEx(  dwExStyle,                        // Расширенный стиль для окна
                                  "OpenGL",                           // Название класса
                                  title,                              // Заголовок окна
                                  dwStyle |                           // Определенный стиль окна
                                  WS_CLIPSIBLINGS |                   // Требуем стиль
                                  WS_CLIPCHILDREN,                    // Требуем стиль
                                  0, 0,                               // Позиция окна
                                  WindowRect.right - WindowRect.left, // Расчитанная ширина
                                  WindowRect.bottom - WindowRect.top, // Расчитанная высота
                                  NULL,                               // Нет родительского окна
                                  NULL,                               // Нет меню
                                  hInstance,                          // Экземпляр
                                  NULL))) {                           // Ничего не передаем в WM_CREATE
        KillGLWindow();                             // Сбрасываем экран
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd = {           // pfd говорит Windows какой формат
        sizeof(PIXELFORMATDESCRIPTOR),              // Размер дескриптора формата пикселей
        1,                                          // Номер версии
        PFD_DRAW_TO_WINDOW |                        // Формат должен поддерживать окна
        PFD_SUPPORT_OPENGL |                        // Формат должен поддерживать OpenGL
        PFD_DOUBLEBUFFER,                           // Формат должен поддерживать двойную буферизацию
        PFD_TYPE_RGBA,                              // Запрс наформат RGBA
        bits,                                       // Глубина цвета
        0, 0, 0, 0, 0, 0,                           // Игнорируем цветовые биты
        0,                                          // Без альфа буфера
        0,                                          // Сдвиг битов не нужен
        0,                                          // Без аккумуляционного буфера
        0, 0, 0, 0,                                 // Аккумуляционных битов нет
        16,                                         // 16битный Z-буффер (буфер глубины)
        0,                                          // Без буфера трафарета
        0,                                          // Без вспомогательного буфера
        PFD_MAIN_PLANE,                             // Основной слой рисования
        0,                                          // Зарезервировано
        0, 0, 0                                     // Без маски слоя
    };

    if (!(hDC = GetDC(hWnd))) {                     // У нас есть контекст устройства?
        KillGLWindow();                             // Сброс экрана
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) { // Нашел Windows совпадающий формат?
        KillGLWindow();                             // Сброс дисплея
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {   // Можем установить формат пикслей?
        KillGLWindow();                             // Сброс дисплея
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    if (!(hRC = wglCreateContext(hDC))) {           // Можем получить контекст рендеринга?
        KillGLWindow();                             // Сброс дисплея
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    if(!wglMakeCurrent(hDC, hRC)) {                 // Пытаемся активизировать контекст устройства
        KillGLWindow();                             // Сброс дисплея
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    ShowWindow(hWnd, SW_SHOW);                      // Выводим окно
    SetForegroundWindow(hWnd);                      // Немного высокий приоритет
    SetFocus(hWnd);                                 // Устанавливаем фокус на окно
    ReSizeGLScene(width, height);                   // Устанавливаем перспективу GL экрана

    if (!InitGL()) {                                // Инициализируем только что созданное GL окно
        KillGLWindow();                             // Сброс дисплея
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возвращаем FALSE
    }

    return TRUE;                                    // Успешное завершение
}

LRESULT CALLBACK WndProc(   HWND    hWnd,
                            UINT    uMsg,
                            WPARAM  wParam,
                            LPARAM  lParam) {
    switch (uMsg) {                                 // Проверка сообщений Windows
        case WM_ACTIVATE: {                         // Сообщение активации окна
            if (!HIWORD(wParam)) {                  // Проверяем статус минимизации
                active = TRUE;                      // Программа активна
            } else {
                active = FALSE;                     // Программа больше не активна
            }

            return 0;                               // Возвращаемся в цикл сообщений
        }

        case WM_SYSCOMMAND: {                       // Перехват системных команд
            switch (wParam) {                       // Проверка вызовов системы
                case SC_SCREENSAVE:                 // Начинается скринсейвер?
                case SC_MONITORPOWER:               // Монитор пытается перейти в энергосберегающий режим?
                    return 0;                           // Предотвращаем это
            }

            break;                                  // Выход
        }

        case WM_CLOSE: {                            // Мы получили сообщение о закрытии?
            PostQuitMessage(0);                     // Посылаем сообщение о выходе
            return 0;                               // Возврат
        }

        case WM_KEYDOWN: {                          // Нажата клавиша?
            keys[wParam] = TRUE;                    // Если да, отмечаем ее как TRUE
            return 0;                               // Возврат
        }

        case WM_KEYUP: {                            // Клавиша отпущена?
            keys[wParam] = FALSE;                   // Если да, то отмечаем ее как FALSE
            return 0;                               // Возврат
        }

        case WM_SIZE: {                             // Изменение размеров окна OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // МладшееСлово=Ширина, СтаршееСлово=Высота
            return 0;                               // Возврат
        }
    }

    // Все другие сообщения посылаем в DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE   hInstance,          // Экземпляр
                    HINSTANCE   hPrevInstance,      // Предыдущий экземпляр
                    LPSTR       lpCmdLine,          // Параметры командной строки
                    int         nCmdShow) {         // Стиль вывода окна
    MSG     msg;                                    // Структура сообщения
    BOOL    done = FALSE;                           // Переменная для выхода из цикла

    // Спрашиваем у пользователя, какой режим он предпочитает
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;                         // Режим окна
    }

    // Создаем окно OpenGL
    if (!CreateGLWindow("NeHe & Giuseppe D'Agata's 2D Font Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                                   // Окно не создалось - выходим
    }

    while(!done) {                                  // Цикл пока done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Пришло сообщение?
            if (msg.message == WM_QUIT) {           // Это сообщение о выходе?
                done = TRUE;                        // Если да, то done=TRUE
            } else {                                // Если нет, то обрабатываем
                TranslateMessage(&msg);             // Переводим сообщение
                DispatchMessage(&msg);              // Отсылаем сообщение
            }
        } else {                                    // Нет сообщений
            // Рисуем сцену.  Ждем клавишу ESC или сообщение о выходе из DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) { // Активно?  Было сообщение о выходе?
                done = TRUE;                        // ESC или DrawGLScene сообщает о выходе
            } else {                                // Не время выходить, обнавляем экран
                SwapBuffers(hDC);                   // Меняем экраны (Двойная буферизация)
            }

            if (keys[VK_F1]) {                      // Нажата F1?
                keys[VK_F1] = FALSE;                // Если, да то устанавливаем FALSE
                KillGLWindow();                     // Убиваем текущее окно
                fullscreen = !fullscreen;           // Переходим в полноэкранный режим

                // Пересоздаем окно GL
                if (!CreateGLWindow("NeHe & Giuseppe D'Agata's 2D Font Tutorial", 640, 480, 16, fullscreen)) {
                    return 0;                       // Выходим, если создать не удалось
                }
            }
        }
    }

    // Закрытие
    KillGLWindow();                                 // Убиваем окно
    return (msg.wParam);                            // Выходим из программы
}
