/*
 *      Этот код был создан Беном Хамфри (Ben Humphrey) 2001
 *      Если вы найдете этот код полезным, дайте мне знать.
 *      Посетите сайт NeHe: http://nehe.gamedev.net
 */

#include <windows.h>            // Заголовочный файл для Windows

#include <stdio.h>          // Заголовочный файл стандартного ввода/вывода (НОВОЕ)
#include <gl\gl.h>          // Заголовочный файл библиотеки OpenGL32
#include <gl\glu.h>         // Заголовочный файл библиотеки GLu32
#include <gl\glaux.h>           // Заголовочный файл библиотеки GLaux

#pragma comment(lib, "opengl32.lib")                // Ссылка на OpenGL32.lib
#pragma comment(lib, "glu32.lib")               // Ссылка на Glu32.lib

#define     MAP_SIZE    1024            // Размер карты вершин (НОВОЕ)
#define     STEP_SIZE   16          // Ширина и высота каждого квадрата (НОВОЕ)
#define     HEIGHT_RATIO    1.5f        // Коэффициент масштабирования по оси Y в соответствии с осями X и Z (НОВОЕ)

HDC     hDC = NULL;             // Приватный контекст устройства GDI
HGLRC       hRC = NULL;             // Постоянный контекст рендеринга
HWND        hWnd = NULL;            // Указатель на наше окно
HINSTANCE   hInstance;              // Указывает на дескриптор текущего приложения

bool        keys[256];              // Массив для процедуры обработки клавиатуры
bool        active = TRUE;              // Флаг активности окна, по умолчанию=TRUE
bool        fullscreen = TRUE;          // Флаг полноэкранного режима, по умолчанию=TRUE
bool        bRender = TRUE;         // Флаг режима отображения полигонов, по умолчанию=TRUE (НОВОЕ)

BYTE g_HeightMap[MAP_SIZE* MAP_SIZE];       // Содержит карту вершин (НОВОЕ)

float scaleValue = 0.15f;                   // Величина масштабирования поверхности (НОВОЕ)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // Объявление для WndProc

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
    if (Height == 0) {  // Предотвращение деления на ноль, если окно слишком мало
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    // Сброс текущей области вывода и перспективных преобразований

    glMatrixMode(GL_PROJECTION);// Выбор матрицы проекций
    glLoadIdentity();           // Сброс матрицы проекции

    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 500.0f);
    // Вычисление соотношения геометрических размеров для окна
    glMatrixMode(GL_MODELVIEW); // Выбор матрицы просмотра модели
}


// Чтение и сохранение .RAW файла в pHeightMap
void LoadRawFile(LPSTR strName, int nSize, BYTE* pHeightMap) {
    FILE* pFile = NULL;

    // открытие файла в режиме бинарного чтения
    pFile = fopen( strName, "rb" );

    // Файл найден?
    if ( pFile == NULL ) {
        // Выводим сообщение об ошибке и выходим из процедуры
        MessageBox(NULL, "Can't Find The Height Map!", "Error", MB_OK);
        return;
    }

    // Загружаем .RAW файл в массив pHeightMap
    // Каждый раз читаем по одному байту, размер = ширина * высота
    fread( pHeightMap, 1, nSize, pFile );

    // Проверяем на наличие ошибки
    int result = ferror( pFile );

    // Если произошла ошибка
    if (result) {
        MessageBox(NULL, "Failed To Get Data!", "Error", MB_OK);
    }

    // Закрываем файл
    fclose(pFile);
}

int InitGL(GLvoid) {                    // Инициализация OpenGL
    glShadeModel(GL_SMOOTH);            // Включить сглаживание
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   // Очистка экрана черным цветом
    glClearDepth(1.0f);             // Установка буфера глубины
    glEnable(GL_DEPTH_TEST);            // Включить буфер глубины
    glDepthFunc(GL_LEQUAL);         // Тип теста глубины
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Улучшенные вычисления перспективы

    // Читаем данные из файла и сохраняем их в массиве g_HeightMap array.
    // Также передаем размер файла (1024).

    LoadRawFile("Data/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);  // ( NEW )

    return TRUE;                        // Инициализация прошла успешно
}


int Height(BYTE* pHeightMap, int X, int Y) {        // Возвращает высоту из карты вершин (?)
    int x = X % MAP_SIZE;                   // Проверка переменной х
    int y = Y % MAP_SIZE;                   // Проверка переменной y

    if(!pHeightMap) {
        return 0;    // Убедимся, что данные корректны
    }

    return pHeightMap[x + (y * MAP_SIZE)];          // Возвращаем значение высоты
}

void SetVertexColor(BYTE* pHeightMap, int x, int y) {   // Эта функция устанавливает значение цвета для конкретного номера,
    // зависящего от номера высоты
    if(!pHeightMap) {
        return;    // Данные корректны?
    }

    float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);

    // Присвоить оттенок синего цвета для конкретной точки
    glColor3f(0.0f, 0.0f, fColor );
}

void RenderHeightMap(BYTE pHeightMap[]) {           // Рендеринг карты высоты с помощью квадратов
    int X = 0, Y = 0;                   // Создаем пару переменных для перемещения по массиву
    int x, y, z;                        // И еще три для удобства чтения

    if(!pHeightMap) {
        return;    // Данные корректны?
    }

    if(bRender) {                   // Что хотим рендерить?
        glBegin( GL_QUADS );    // Полигоны
    } else {
        glBegin( GL_LINES );    // Линии
    }

    for ( X = 0; X < MAP_SIZE; X += STEP_SIZE )
        for ( Y = 0; Y < MAP_SIZE; Y += STEP_SIZE ) {
            // Получаем (X, Y, Z) координаты нижней левой вершины
            x = X;
            y = Height(pHeightMap, X, Y );
            z = Y;

            // Устанавливаем цвет конкретной точки
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // Рендерим ее

            // Получаем (X, Y, Z) координаты верхней левой вершины
            x = X;
            y = Height(pHeightMap, X, Y + STEP_SIZE );
            z = Y + STEP_SIZE ;

            // Устанавливаем цвет конкретной точки
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // Рендерим ее

            // Получаем (X, Y, Z) координаты верхней правой вершины
            x = X + STEP_SIZE;
            y = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE );
            z = Y + STEP_SIZE ;

            // Устанавливаем цвет конкретной точки
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // Рендерим ее

            // Получаем (X, Y, Z) координаты нижней правой вершины
            x = X + STEP_SIZE;
            y = Height(pHeightMap, X + STEP_SIZE, Y );
            z = Y;

            // Устанавливаем цвет конкретной точки
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // Рендерим ее
        }

    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // Сбрасываем цвет
}

int DrawGLScene(GLvoid) {                   // Здесь содержится код рисования
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экрана и буфера глубины
    glLoadIdentity();                   // Сброс просмотра

    //    Положение  Вид        Вектор вертикали
    gluLookAt(212, 60, 194,  186, 55, 171,  0, 1, 0);   // Определяет вид и положение камеры

    glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);

    RenderHeightMap(g_HeightMap);               // Рендеринг карты вершин

    return TRUE;                        // Идем дальше
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


LRESULT CALLBACK WndProc(   HWND    hWnd,           // Указатель на окно
                            UINT    uMsg,           // Сообщение для этого окна
                            WPARAM  wParam,         // Параметры сообщения
                            LPARAM  lParam) {       // Параметры сообщения
    switch (uMsg) {                     // Проверим сообщения окна
        case WM_ACTIVATE: {             // Наблюдаем за сообщением об активизации окна
            if (!HIWORD(wParam)) {          // Проверим состояние минимизациии
                active = TRUE;          // Программа активна
            } else {
                active = FALSE;         // Программа больше не активна
            }

            return 0;               // Вернуться к циклу сообщений
        }

        case WM_SYSCOMMAND: {           // Перехватваем системную команду
            switch (wParam) {           // Проверка выбора системы
                case SC_SCREENSAVE:     // Пытается включиться скринсэйвер?
                case SC_MONITORPOWER:       // Монитор пытается переключитться в режим сохранения энергии?
                    return 0;           // Не давать этому случиться
            }

            break;                  // Выход
        }

        case WM_CLOSE: {                // Мы получили сообщение о закрытии программы?
            PostQuitMessage(0);         // Послать сообщение о выходе
            return 0;               // Jump Back
        }

        case WM_LBUTTONDOWN: {              // Did We Receive A Left Mouse Click?
            bRender = !bRender;         // Change Rendering State Between Fill/Wire Frame
            return 0;               // Вернуться
        }

        case WM_KEYDOWN: {              // Клавиша была нажата?
            keys[wParam] = TRUE;            // Если так – пометим это TRUE
            return 0;               // Вернуться
        }

        case WM_KEYUP: {                // Клавиша была отпущена?
            keys[wParam] = FALSE;           // Если так – пометим это FALSE
            return 0;               // Вернуться
        }

        case WM_SIZE: {                 // Изменились окна OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=ширина, HiWord=высота
            return 0;               // Вернуться
        }
    }

    // Пересылаем все прочие сообщения в DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int WINAPI WinMain( HINSTANCE   hInstance,      // Экземпляр
                    HINSTANCE   hPrevInstance,      // Предыдущий экземпляр
                    LPSTR       lpCmdLine,      // Параметры командной строки
                    int     nCmdShow) {     // Показать состояние окна
    MSG     msg;                    // Структура сообщения окна
    BOOL    done = FALSE;               // Булевская переменная выхода из цикла

    // Запросим пользователя какой режим отображения он предпочитает
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;             // Оконный режим
    }

    // Создадим наше окно OpenGL
    if (!CreateGLWindow("NeHe & Ben Humphrey's Height Map Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                   // Выходим если окно не было создано
    }

    while(!done) {                      // Цикл, который продолжается пока done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Есть ожидаемое сообщение?
            if (msg.message == WM_QUIT) {   // Мы получили сообщение о выходе?
                done = TRUE;        // Если так done=TRUE
            } else {                // Если нет, продолжаем работать с сообщениями окна
                TranslateMessage(&msg);     // Переводим сообщение
                DispatchMessage(&msg);      // Отсылаем сообщение
            }
        } else {                    // Если сообщений нет
            // Рисуем сцену. Ожидаем нажатия кнопки ESC и сообщения о выходе от DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) { // Активно?  Было получено сообщение о выходе?
                done = TRUE;        // ESC или DrawGLScene просигналили "выход"
            } else if (active) {        // Не время выходить, обновляем экран
                SwapBuffers(hDC);       // Переключаем буферы (Двойная буферизация)
            }

            if (keys[VK_F1]) {          // Была нажата кнопка F1?
                keys[VK_F1] = FALSE;    // Если так - установим значение FALSE
                KillGLWindow();         // Закроем текущее окно OpenGL
                fullscreen = !fullscreen;   // Переключим режим "Полный экран"/"Оконный"

                // Заново создадим наше окно OpenGL
                if (!CreateGLWindow("NeHe & Ben Humphrey's Height Map Tutorial", 640, 480, 16, fullscreen)) {
                    return 0;       // Выйти, если окно не было создано
                }
            }

            if (keys[VK_UP]) {          // Нажата клавиша ВВЕРХ?
                scaleValue += 0.001f;    // Увеличить переменную масштабирования
            }

            if (keys[VK_DOWN]) {        // Нажата клавиша ВНИЗ?
                scaleValue -= 0.001f;    // Уменьшить переменную масштабирования
            }
        }
    }

    // Shutdown
    KillGLWindow();                     // Закроем окно
    return (msg.wParam);                    // Выйдем из программы
}
