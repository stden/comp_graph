/*
 *      Эта программа создана Джефом Молофи (Jeff Molofee) в 2000 г.
 *      и модифицировано Giuseppe D'Agata (waveform@tiscalinet.it)
 *      Если вы нашли эту программу полезной, пожалуйста, дайте мне знать.
 *      Посетите мой сайт nehe.gamedev.net
 */

#include <windows.h>        // Заголовочный файл для Windows
#include <stdio.h>          // Заголовочный файл стандартного ввода/вывода
#include <gl\gl.h>          // Заголовочный файл библиотеки OpenGL32
#include <gl\glu.h>         // Заголовочный файл библиотеки GLu32
#include <gl\glaux.h>       // Заголовочный файл библиотеки GLaux
#include <math.h>     // Заголовочный файл для математической библиотеки ( НОВОЕ )

#define MAX_PARTICLES 1000 // Число частиц для создания ( НОВОЕ )

HDC         hDC = NULL;     // Частный контекст GDI устройства
HGLRC       hRC = NULL;     // Постоянный контекст отображения
HWND        hWnd = NULL;    // Содержит хэндл нашего окна
HINSTANCE   hInstance;      // Содержит экземпляр приложения

bool    keys[256];          // Массив применяемый для подпрограммы клавиатуры
bool    active = TRUE;      // Флаг "Активное" окна. Устанавливается истинным (TRUE) по умолчанию.
bool    fullscreen = TRUE;  // Флаг "На полный экран". Устанавливается в полноэкранный режим по умолчанию.

bool  masking = TRUE; // Маскирование Вкл/Выкл
bool  mp;             // M нажата?
bool  sp;             // Пробел нажат?
bool  scene;          // Какая сцена выводиться

GLuint  texture[5];   // Память для пяти наших текстур
GLuint  loop;         // Общая переменная цикла

GLfloat  roll;        // Катание текстуры

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);   // Объявление для WndProc


AUX_RGBImageRec* LoadBMP(char* Filename) {   // Загрузка картинки
    FILE* File = NULL;        // Индекс файла

    if (!Filename) {          // Проверка имени файла
        return NULL;             // Если нет вернем NULL
    }

    File = fopen(Filename, "r"); // Проверим существует ли файл

    if (File) {               // Файл существует?
        fclose(File);            // Закрыть файл
        return auxDIBImageLoad(Filename); // Загрузка картинки и вернем на нее указатель
    }

    return NULL;              // Если загрузка не удалась вернем NULL
}


int LoadGLTextures() {              // Загрузка картинки и конвертирование в текстуру
    int Status = FALSE;               // Индикатор состояния
    AUX_RGBImageRec* TextureImage[5]; // Создать место для текстуры
    memset(TextureImage, 0, sizeof(void*) * 5); // Установить указатель в NULL

    if ((TextureImage[0] = LoadBMP("Data/logo.bmp")) && // Текстура эмблемы
            (TextureImage[1] = LoadBMP("Data/mask1.bmp")) && // Первая маска
            (TextureImage[2] = LoadBMP("Data/image1.bmp")) && // Первое изображение
            (TextureImage[3] = LoadBMP("Data/mask2.bmp")) && // Вторая маска
            (TextureImage[4] = LoadBMP("Data/image2.bmp"))) { // Второе изображение
        Status = TRUE;                  // Задать статус в TRUE
        glGenTextures(5, &texture[0]);  // Создать пять текстур

        for (loop = 0; loop < 5; loop++) { // Цикл по всем пяти текстурам
            glBindTexture(GL_TEXTURE_2D, texture[loop]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY,
                         0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);
        }
    }

    for (loop = 0; loop < 5; loop++) { // Цикл по всем пяти текстурам
        if (TextureImage[loop]) {       // Если текстура существуют
            if (TextureImage[loop]->data) { // Если изображение текстуры существует
                free(TextureImage[loop]->data); // Освободить память изображения
            }

            free(TextureImage[loop]);     // Освободить структуру изображения
        }
    }

    return Status;                    // Возвращаем статус
}



int InitGL(GLvoid) {          // Все начальные настройки OpenGL здесь
    if (!LoadGLTextures()) {    // Переход на процедуру загрузки текстуры
        return FALSE;             // Если текстура не загружена возвращаем FALSE
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Черный фон
    glClearDepth(1.0);          // Установка буфера глубины
    glEnable(GL_DEPTH_TEST);    // Разрешение теста глубины
    glShadeModel(GL_SMOOTH);    // Разрешить плавное закрашивание
    glEnable(GL_TEXTURE_2D);    // Разрешение наложения текстуры
    return TRUE;                // Инициализация завершена OK
}


GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {   // Изменить размеры и проинициализировать окно GL
    if (height == 0) {                                  // Предотвращаем деление на 0
        height = 1;                                     // Приравниваем высоту единице
    }

    glViewport(0, 0, width, height);                    // Обновляем текущий порт отображения

    glMatrixMode(GL_PROJECTION);                        // Выбираем матрицу проекции
    glLoadIdentity();                                   // Устанавливаем матрицу проекции

    // Вычисляем соотношение сторон окна
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f); // ( МОДИФИЦИРОВАНО )

    glMatrixMode(GL_MODELVIEW);                         // Выбираем матрицу модели отображения
    glLoadIdentity();                                   // Устанавливаем матрицу модели отображения
}

int DrawGLScene(GLvoid) {        // Здесь мы все рисуем
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      // Очистка экрана и буфера глубины
    glLoadIdentity();              // Сброс матрицы вида
    glTranslatef(0.0f, 0.0f, -2.0f); // Перемещение вглубь экрана на 2 единицы
    glBindTexture(GL_TEXTURE_2D, texture[0]); // Выбор текстуры эмблемы
    glBegin(GL_QUADS);       // Начало рисования текстурного четырехугольника
    glTexCoord2f(0.0f, -roll + 0.0f);
    glVertex3f(-1.1f, -1.1f,  0.0f);  // Лево Низ
    glTexCoord2f(3.0f, -roll + 0.0f);
    glVertex3f( 1.1f, -1.1f,  0.0f);  // Право Низ
    glTexCoord2f(3.0f, -roll + 3.0f);
    glVertex3f( 1.1f,  1.1f,  0.0f);  // Право Верх
    glTexCoord2f(0.0f, -roll + 3.0f);
    glVertex3f(-1.1f,  1.1f,  0.0f);  // Лево Верх
    glEnd();                // Завершения рисования четырехугольника
    glEnable(GL_BLEND);       // Разрешение смешивания
    glDisable(GL_DEPTH_TEST); // Запрет теста глубины

    if (masking) {            // Маскировка разрешена?
        glBlendFunc(GL_DST_COLOR, GL_ZERO); // Смешивание цвета экрана с нулем (Черное)
    }

    if (scene) {              // Рисовать вторую сцену?
        glTranslatef(0.0f, 0.0f, -1.0f);    // Перемещение вглубь экрана на одну единицу
        glRotatef(roll * 360, 0.0f, 0.0f, 1.0f); // Вращение по оси Z на 360 градусов

        if (masking) {            // Маскирование включено?
            glBindTexture(GL_TEXTURE_2D, texture[3]); // Выбор второй маски текстуры
            glBegin(GL_QUADS);  // Начало рисования текстурного четырехугольника
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.1f, -1.1f,  0.0f);  // Низ Лево
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( 1.1f, -1.1f,  0.0f);  // Низ Право
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( 1.1f,  1.1f,  0.0f);  // Верх Право
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.1f,  1.1f,  0.0f);  // верх Лево
            glEnd();            // Конец рисования четырехугольника
        }

        glBlendFunc(GL_ONE, GL_ONE);  // Копирование цветного изображения 2 на экран
        glBindTexture(GL_TEXTURE_2D, texture[4]); // Выбор второго изображения текстуры
        glBegin(GL_QUADS);            // Начало рисования текстурного четырехугольника
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.1f, -1.1f,  0.0f);  // Низ Лево
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 1.1f, -1.1f,  0.0f);  // Низ Право
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 1.1f,  1.1f,  0.0f);  // Верх Право
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.1f,  1.1f,  0.0f);  // Верх Лево
        glEnd();                      // Завершение рисования четырехугольника
    } else {              // Иначе
        if (masking) {            // Вкл. маскировка?
            glBindTexture(GL_TEXTURE_2D, texture[1]); // Выбор первой маски текстуры
            glBegin(GL_QUADS);  // Начало рисования текстурного четырехугольника
            glTexCoord2f(roll + 0.0f, 0.0f);
            glVertex3f(-1.1f, -1.1f,  0.0f);  // Низ Лево
            glTexCoord2f(roll + 4.0f, 0.0f);
            glVertex3f( 1.1f, -1.1f,  0.0f);  // Низ Право
            glTexCoord2f(roll + 4.0f, 4.0f);
            glVertex3f( 1.1f,  1.1f,  0.0f);  // Верх Право
            glTexCoord2f(roll + 0.0f, 4.0f);
            glVertex3f(-1.1f,  1.1f,  0.0f);  // Верх Лево
            glEnd();            // Конец рисования четырехугольника
        }

        glBlendFunc(GL_ONE, GL_ONE); // Копирование цветного изображения 1 на экран
        glBindTexture(GL_TEXTURE_2D, texture[2]); // Выбор первого изображения текстуры
        glBegin(GL_QUADS);    // Начало рисования текстурного четырехугольника
        glTexCoord2f(roll + 0.0f, 0.0f);
        glVertex3f(-1.1f, -1.1f,  0.0f);  // Низ Лево
        glTexCoord2f(roll + 4.0f, 0.0f);
        glVertex3f( 1.1f, -1.1f,  0.0f);  // Низ Право
        glTexCoord2f(roll + 4.0f, 4.0f);
        glVertex3f( 1.1f,  1.1f,  0.0f);  // Верх Право
        glTexCoord2f(roll + 0.0f, 4.0f);
        glVertex3f(-1.1f,  1.1f,  0.0f);  // Верх Лево
        glEnd();              // Конец рисования четырехугольника
    }

    glEnable(GL_DEPTH_TEST); // Разрешение теста глубины
    glDisable(GL_BLEND);     // Запрещение смешивания
    roll += 0.002f;          // Увеличим прокрутку нашей текстуры

    if (roll > 1.0f) {       // Roll больше чем
        roll -= 1.0f;          // Вычтем 1 из Roll
    }

    return TRUE;             // Все OK
}


GLvoid KillGLWindow(GLvoid) {                           // Должным образом уничтожаем окно
    if (fullscreen) {                                   // Мы в полно экранном режииме?
        ChangeDisplaySettings(NULL, 0);                 // Если да, то перключаемся на рабочий стол
        ShowCursor(TRUE);                               // Показываем курсор мыши
    }

    if (hRC) {                                          // Контекст отображения уже есть?
        if (!wglMakeCurrent(NULL, NULL)) {              // Мы можем освободить RC и DC контексты?
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC)) {                   // Мы можем удалить RC?
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        hRC = NULL;                                     // Устанавливаем RC в NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC)) {                 // Мы можем высвободить контекст DC
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                     // Устанавливаем DC в NULL
    }

    if (hWnd && !DestroyWindow(hWnd)) {                 // Мы можем уничтожить окно?
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                    // Устанавливаем hWnd в NULL
    }

    if (!UnregisterClass("OpenGL", hInstance)) {        // Можем разрегистрировать класс
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                               // Устанавливаем hInstance в NULL
    }
}

/*  Эта часть программы создает наше окно OpenGL.  Параметры:                           *
 *  title           - Заголовок, отображаемый наверху окна                              *
 *  width           - Ширина GL окна или полноэкранного режима                          *
 *  height          - Высота GL окна или полноэкранного режима                          *
 *  bits            - Количество бит на цвет (8/16/24/32)                               *
 *  fullscreenflag  - Использование полноэкранного (TRUE) или оконного(FALSE) режимов   */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {
    GLuint      PixelFormat;            // Содержит результаты подбора глубины цвета
    WNDCLASS    wc;                     // Структура классов Windows
    DWORD       dwExStyle;              // Расширенный стиль окна
    DWORD       dwStyle;                // Стиль окна
    RECT        WindowRect;             // Получает значения верхнего левого и нижнего правого углов прямоугольника
    WindowRect.left = (long)0;          // Устанавливает значение лево (Left) в 0
    WindowRect.right = (long)width;     // Устанавливает значение право (Right) в требуемую ширину (Width)
    WindowRect.top = (long)0;           // Устанавливает значение верх (Top) в 0
    WindowRect.bottom = (long)height;   // Устанавливает значение низ (Bottom) в требуемую высоту (Height)

    fullscreen = fullscreenflag;        // Устанавливаем глобальный флвг Fullscreen

    hInstance           = GetModuleHandle(NULL);                // Захватываем Instance для нашего окна
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // Перерисовываем по размеру, и получаем DC для окна.
    wc.lpfnWndProc      = (WNDPROC) WndProc;                    // WndProc Handles Messages
    wc.cbClsExtra       = 0;                                    // Нет дополнительных данных окна
    wc.cbWndExtra       = 0;                                    // Нет дополнительных данных окна
    wc.hInstance        = hInstance;                            // Установим Instance
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);          // Згрузим иконку по умолчанию
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);          // Згрузим стрелку курсора
    wc.hbrBackground    = NULL;                                 // Фон для GL не требуется
    wc.lpszMenuName     = NULL;                                 // Нам не нужны меню
    wc.lpszClassName    = "OpenGL";                             // Установим имя класса

    if (!RegisterClass(&wc)) {                                  // Попытаемся зарегистрировать класс окна
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                           // Возращаем FALSE
    }

    if (fullscreen) {                                           // Попытаться включить полноеэкранный режим?
        DEVMODE dmScreenSettings;                               // Режим устройства
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); // Убедимся, что памать очищена
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // Размер структуры devmode
        dmScreenSettings.dmPelsWidth    = width;                // Выбрана ширина экрана
        dmScreenSettings.dmPelsHeight   = height;               // Выбрана высота экрана
        dmScreenSettings.dmBitsPerPel   = bits;                 // Выбрано количество бит на пиксель
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Попытаемся установить выбранный режим и получить резутьтаты.  К седению: CDS_FULLSCREEN избавляется от кнопки стариGets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // Если режиим не включился, предложим две возможности. Выйти или использовать оконный режим.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE;     // Выбран оконный режим.  Fullscreen = FALSE
            } else {
                // Показать сообщение, что приложение закончило работу.
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return FALSE;                                   // Возвращаем FALSE
            }
        }
    }

    if (fullscreen) {                                           // Так мы в полноэкранном режиме?
        dwExStyle = WS_EX_APPWINDOW;                            // Расширенный стиль окна
        dwStyle = WS_POPUP;                                     // Стиль окна
        ShowCursor(FALSE);                                      // Скрыть курсор мыши
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;         // Расширенный стиль окна
        dwStyle = WS_OVERLAPPEDWINDOW;                          // Стиль окна
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Подстроить окно, чтобы оно соответствовало требуемому размеру

    // Создать окно
    if (!(hWnd = CreateWindowEx(  dwExStyle,                        // Расширенный стиль для окна
                                  "OpenGL",                           // Наименование класса
                                  title,                              // Заголовок окна
                                  dwStyle |                           // Определенный стиль окна
                                  WS_CLIPSIBLINGS |                   // Требуемый стиль окна
                                  WS_CLIPCHILDREN,                    // Требуемый стиль окна
                                  0, 0,                               // Местоположение окна
                                  WindowRect.right - WindowRect.left, // Вычисление ширины окна
                                  WindowRect.bottom - WindowRect.top, // Вычисление высоты окна
                                  NULL,                               // Нет родительского окна
                                  NULL,                               // Нет меню
                                  hInstance,                          // Instance
                                  NULL))) {                           // Не посылать сообщение по WM_CREATE
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Вернуть FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd = {           // pfd сообщает Windows какие параметры мы хотим
        sizeof(PIXELFORMATDESCRIPTOR),              // Размер дескриптора формата пикселей
        1,                                          // Номер версии
        PFD_DRAW_TO_WINDOW |                        // Формат должен поддерживать окно
        PFD_SUPPORT_OPENGL |                        // Формат должен поддерживать OpenGL
        PFD_DOUBLEBUFFER,                           // Должна поддерживаться двойная буферизация
        PFD_TYPE_RGBA,                              // Запрос RGBA формата
        bits,                                       // Выбор глубины цвета
        0, 0, 0, 0, 0, 0,                           // Биты цвета игнорируются
        0,                                          // Нет альфа буферизации
        0,                                          // Бит сдвига игнорируется
        0,                                          // Нет буфера накопления
        0, 0, 0, 0,                                 // Биты накопления игнорируются
        16,                                         // 16битный Z-бувер (Буфер глубины)
        0,                                          // Нет буфера трафарета (stencil buffer)
        0,                                          // Нет вспомогательного буфера
        PFD_MAIN_PLANE,                             // Главная плоскость рисования
        0,                                          // Зарезервировано
        0, 0, 0                                     // Слой масок игнорируется
    };

    if (!(hDC = GetDC(hWnd))) {                     // Мы получили контекст устройства?
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Вернуть FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) { // Windows нашла соответствующий формат пикселя?
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Вернуть FALSE
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {   // Мы можем установить формат пикселя?
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Вернуть FALSE
    }

    if (!(hRC = wglCreateContext(hDC))) {           // Мы можем получить контекст изображения?
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Вернуть FALSE
    }

    if(!wglMakeCurrent(hDC, hRC)) {                 // Пытаемся активировать контекст изображения
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Вернуть FALSE
    }

    ShowWindow(hWnd, SW_SHOW);                      // Показать окно
    SetForegroundWindow(hWnd);                      // Слегка увеличим приоритет
    SetFocus(hWnd);                                 // Устанавливаем фокус клавииатуры на окно
    ReSizeGLScene(width, height);                   // Устанавливаем наше GL окно с перспективой

    if (!InitGL()) {                                // Инициализируем наше GL окно
        KillGLWindow();                             // Инициализация дисплея
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // Возращает FALSE
    }

    return TRUE;                                    // Успешное завершение инициализациии
}

LRESULT CALLBACK WndProc(   HWND    hWnd,           // Хэндл для этого окна
                            UINT    uMsg,           // Сообщение для этого окна
                            WPARAM  wParam,         // Дополнительная информация сообщения
                            LPARAM  lParam) {       // Дополнительная информация сообщения
    switch (uMsg) {                                 // Проверим сообщения окна
        case WM_ACTIVATE: {                         // Наблюдаем за сообщением об активизации окна
            if (!HIWORD(wParam)) {                  // Проверим состояние минимизациии
                active = TRUE;                      // Программа активна
            } else {
                active = FALSE;                     // Программа больше не активна
            }

            return 0;                               // Вернууться к циклу сообщения
        }

        case WM_SYSCOMMAND: {                       // Перехватваем системную команду
            switch (wParam) {                       // Проверка ывбора системы
                case SC_SCREENSAVE:                 // Пытаеттся включиться скринсэйвер?
                case SC_MONITORPOWER:               // Монитор пытается переключитться в режим сохранения энергии?
                    return 0;                           // Не давать этому случиться
            }

            break;                                  // Выход
        }

        case WM_CLOSE: {                            // Мы получили сообщение о закрытии программы?
            PostQuitMessage(0);                     // Послать сообщение о выходе
            return 0;                               // Вернуться
        }

        case WM_KEYDOWN: {                          // Клавиша была нажата?
            keys[wParam] = TRUE;                    // Если это так пометим это TRUE
            return 0;                               // Вернуться
        }

        case WM_KEYUP: {                            // Клавиша была отпущена?
            keys[wParam] = FALSE;                   // Если это так пометим это FALSE
            return 0;                               // Вернуться
        }

        case WM_SIZE: {                             // Изменились окна OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord=ширина, HiWord=высота
            return 0;                               // Вернуться
        }
    }

    // Пересылаем все прочие сообщения в DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,     // Экземпляр
    HINSTANCE hPrevInstance, // Предыдущий экземпляр
    LPSTR     lpCmdLine,     // Параметры командной строки
    int       nCmdShow) {    // Показать состояние окна
    MSG  msg;        // Структура сообщения окна
    BOOL done = FALSE; // Булевская переменная выхода из цикла

    // Запросим пользователя какой режим отображения он предпочитает
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?",
                   "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;            // Оконный режим
    }

    // Создадим наше окно OpenGL
    if (!CreateGLWindow("NeHe's Masking Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                  // Выходим если окно не было создано
    }

    while (!done) { // Цикл, который продолжается пока done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Есть ожидаемое сообщение?
            if (msg.message == WM_QUIT) { // Мы получили сообщение о выходе?
                done = TRUE; // Если так done=TRUE
            } else { // Если нет, продолжаем работать с сообщениями окна
                TranslateMessage(&msg); // Переводим сообщение
                DispatchMessage(&msg);  // Отсылаем сообщение
            }
        } else { // Если сообщений нет
            // Рисуем сцену. Ожидаем нажатия кнопки ESC и сообщения о выходе от DrawGLScene()
            // Активно?  Было получено сообщение о выходе?
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) {
                done = TRUE; // ESC или DrawGLScene просигналили "выход"
            } else { // Не время выходить, обновляем экран
                SwapBuffers(hDC); // Переключаем буферы (Двойная буферизация)

                if (keys[' '] && !sp) { // Пробел нажат?
                    sp = TRUE;          // Сообщим программе, что пробел нажат
                    scene = !scene;     // Переключение сцен
                }

                if (!keys[' ']) {  // Пробел отжат?
                    sp = FALSE;      // Сообщим программе, что пробел отжат
                }

                if (keys['M'] && !mp) { // M нажата?
                    mp = TRUE;          // Сообщим программе, что M нажата
                    masking = !masking; // Переключение режима маскирования Выкл/Вкл
                }

                if (!keys['M']) { // M отжата?
                    mp = FALSE;   // Сообщим программе, что M отжата
                }

                if (keys[VK_F1]) {           // Была нажата кнопка F1?
                    keys[VK_F1] = FALSE;       // Если так - установим значение FALSE
                    KillGLWindow();            // Закроем текущее окно OpenGL
                    fullscreen = !fullscreen;  // Переключим режим "Полный экран"/"Оконный"

                    // Заново создадим наше окно OpenGL
                    if (!CreateGLWindow("NeHe's Masking Tutorial", 640, 480, 16, fullscreen)) {
                        return 0;                // Выйти, если окно не было создано

                    }
                }
            }
        }
    }

    // Сброс
    KillGLWindow();                    // Закроем окно
    return (msg.wParam);               // Выйдем из программы
}
