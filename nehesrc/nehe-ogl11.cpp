#include <windows.h>                        // заголовочный файл для Windows
#include <stdio.h>                        // заголовочный файл для стандартного ввода/вывода
#include <math.h>                        // заголовочный файл для библиотеки математических функций
#include <gl\gl.h>                        // заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>                        // заголовочный файл для для библиотеки GLu32
#include <gl\glaux.h>                        // заголовочный файл для библиотеки GLaux

HDC        hDC = NULL;              // личный контекст устройства GDI
HGLRC      hRC = NULL;              // постоянный контекст рендеринга
HWND       hWnd = NULL;              // содержит хэндл нашего окна
HINSTANCE  hInstance;                // содержит экземпляр нашего приложения

bool        keys[256];                        // массив для процедуры обработки клавиатуры
bool        active = TRUE;              // флаг активности окна, по умолчанию TRUE
bool        fullscreen = TRUE;      // флаг полноэкранного режима, по умолчанию полный экран

float points[45][45][3];    // массив точек сетки нашей "волны"
int wiggle_count = 0;                // счетчик для контроля быстроты развевания флага

GLfloat        xrot;                                // вращение по X (НОВОЕ)
GLfloat        yrot;                                // вращение по Y (НОВОЕ)
GLfloat        zrot;                                // вращение по Z (НОВОЕ)
GLfloat hold;                                // временно содержит число с плавающей запятой (НОВОЕ)

GLuint        texture[1];                        // место для одной текстуры (НОВОЕ)

LRESULT        CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // объявление WndProc

AUX_RGBImageRec* LoadBMP(char* Filename) {                              // загрузка изображения из файла
    FILE* File = NULL;                                                                      // хэндл файла

    if (!Filename) {                                                                              // дано ли имя файла?
        return NULL;                                                                        // если нет, возвратить NULL
    }

    File = fopen(Filename, "r");                                                     // проверить, существует ли файл

    if (File) {                                                                                      // существует?
        fclose(File);                                                                        // закрыть хэндл
        return auxDIBImageLoad(Filename);                                // загрузить изображение и возвратить указатель
    }

    return NULL;                                                                                // если не удалось, возвратить NULL
}

int LoadGLTextures() {                                                                      // загрузить изображения и преобразовать в текстуры
    int Status = FALSE;                                                                      // индикатор состояния

    AUX_RGBImageRec* TextureImage[1];                                        // найти место для текстуры

    memset(TextureImage, 0, sizeof(void*) * 1);                // присвоить указателю NULL

    // загрузить изображение, проверить на ошибки, и, если оно не найдено, выйти
    if (TextureImage[0] = LoadBMP("Data/Tim.bmp")) {
        Status = TRUE;                                                                      // установить статус в TRUE

        glGenTextures(1, &texture[0]);                                        // создать текстуру

        // типичная генерация текстуры с использованием данных из изображения
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (TextureImage[0]) {                                                                      //  если текстура существует
        if (TextureImage[0]->data) {                                                      // если изображение текстуры существует
            free(TextureImage[0]->data);                                        // освободить память, выделенную под текстуру
        }

        free(TextureImage[0]);                                                                // освободить структуру изображения
    }

    return Status;                                                                                // возвратить статус
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {              // изменить размер и инициализировать окно GL
    if (height == 0) {                                                                            // предотвращение деления на ноль
        height = 1;                                                                           // если окно слишком мало
    }

    glViewport(0, 0, width, height);                                             // сброс текущей области вывода и перспективных преобразований

    glMatrixMode(GL_PROJECTION);                                                // Select The Projection Matrix
    glLoadIdentity();                                                                        // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                                                        // выбор матрицы просмотра модели
    glLoadIdentity();                                                                        // сброс матрицы просмотра модели
}

int InitGL(GLvoid) {                                                                              // вся настройка OpenGL - здесь
    if (!LoadGLTextures()) {                                                              // вызвать загрузку текстуры (НОВОЕ)
        return FALSE;                                                                        // если не загрузилась, возвратить FALSE
    }

    glEnable(GL_TEXTURE_2D);                                                        // разрешить распределение текстур (НОВОЕ)
    glShadeModel(GL_SMOOTH);                                                        // разрешить гладкое затенение
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                // черный фон
    glClearDepth(1.0f);                                                                        // настройка буфера глубины
    glEnable(GL_DEPTH_TEST);                                                        // разрешить тестирование глубины
    glDepthFunc(GL_LEQUAL);                                                                // тип тестирования глубины
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);        // хорошее вычисление перспективы
    glPolygonMode( GL_BACK, GL_FILL );                                        // задняя поверхность сплошная
    glPolygonMode( GL_FRONT, GL_LINE );                                        // передняя поверхность из линий

    for(int x = 0; x < 45; x++) {
        for(int y = 0; y < 45; y++) {
            points[x][y][0] = float((x / 5.0f) - 4.5f);
            points[x][y][1] = float((y / 5.0f) - 4.5f);
            points[x][y][2] = float(sin((((x / 5.0f) * 40.0f) / 360.0f) * 3.141592654 * 2.0f));
        }
    }

    return TRUE;                                                                                // все OK
}

int DrawGLScene(GLvoid) {                                                                      // здесь мы рисуем все, что надо
    int x, y;
    float float_x, float_y, float_xb, float_yb;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // очистить экран и буфер глубины
    glLoadIdentity();                                                                        // сбросить вид

    glTranslatef(0.0f, 0.0f, -12.0f);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    for( x = 0; x < 44; x++ ) {
        for( y = 0; y < 44; y++ ) {
            float_x = float(x) / 44.0f;
            float_y = float(y) / 44.0f;
            float_xb = float(x + 1) / 44.0f;
            float_yb = float(y + 1) / 44.0f;

            glTexCoord2f( float_x, float_y);
            glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

            glTexCoord2f( float_x, float_yb );
            glVertex3f( points[x][y + 1][0], points[x][y + 1][1], points[x][y + 1][2] );

            glTexCoord2f( float_xb, float_yb );
            glVertex3f( points[x + 1][y + 1][0], points[x + 1][y + 1][1], points[x + 1][y + 1][2] );

            glTexCoord2f( float_xb, float_y );
            glVertex3f( points[x + 1][y][0], points[x + 1][y][1], points[x + 1][y][2] );
        }
    }

    glEnd();

    if( wiggle_count == 2 ) {
        for( y = 0; y < 45; y++ ) {
            hold = points[0][y][2];

            for( x = 0; x < 44; x++) {
                points[x][y][2] = points[x + 1][y][2];
            }

            points[44][y][2] = hold;
        }

        wiggle_count = 0;
    }

    wiggle_count++;

    xrot += 0.3f;
    yrot += 0.2f;
    zrot += 0.4f;

    return TRUE;                                                                                // идем дальше
}

GLvoid KillGLWindow(GLvoid) {                                                              // правильно убить окно :)
    if (fullscreen) {                                                                              // мы в полноэкранном режиме?
        ChangeDisplaySettings(NULL, 0);                                       // если да, то переключаемся на Рабочий стол
        ShowCursor(TRUE);                                                                // показать мышиный курсор
    }

    if (hRC) {                                                                                      // есть ли у нас контекст отображения?
        if (!wglMakeCurrent(NULL, NULL)) {                                     // можем ли мы освободить DC и RC?
            MessageBox(NULL, "Не отпускаются DC и RC.", "ОШИБКА ЗАВЕРШЕНИЯ РАБОТЫ", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC)) {                                              // можем ли мы удалить RC?
            MessageBox(NULL, "Не отпускается контекст отображения.", "ОШИБКА ЗАВЕРШЕНИЯ РАБОТЫ", MB_OK | MB_ICONINFORMATION);
        }

        hRC = NULL;                                                                              // RC присвоить NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC)) {                                     // можем ли мы освободить DC
        MessageBox(NULL, "Не отпускается контекст устройства.", "ОШИБКА ЗАВЕРШЕНИЯ РАБОТЫ", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                                                              // DC присвоить NULL
    }

    if (hWnd && !DestroyWindow(hWnd)) {                                      // можем ли мы разрушить окно?
        MessageBox(NULL, "Окно не хочет разрушаться :(", "ОШИБКА ЗАВЕРШЕНИЯ РАБОТЫ", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                                                              // hWnd присвоить NULL
    }

    if (!UnregisterClass("OpenGL", hInstance)) {                     // можем ли мы убрать класс окна
        MessageBox(NULL, "Не убирается класс окна.", "ОШИБКА ЗАВЕРШЕНИЯ РАБОТЫ", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                                                                      // hInstance присвоить NULL
    }
}

/*        Эта часть программы создает наше окно OpenGL.  Параметры:                                                               *
 *        title                 - Заголовок окна                                                            *
 *        width                 - Ширина GL окна или полноэкранного режима                                                        *
 *        height                - Высота GL окна или полноэкранного режима                                                        *
 *        bits                  - Количество бит на цвет (8/16/24/32)                                                             *
 *        fullscreenflag        - Использование полноэкранного (TRUE) или оконного (FALSE) режимов
 */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {
    GLuint                PixelFormat;                        // содержит результат поиска совпадений
    WNDCLASS        wc;                                                // структура класса окна
    DWORD                dwExStyle;                                // расширенный стиль окна
    DWORD                dwStyle;                                // стиль окна
    RECT                WindowRect;                    // получает значения верхнего левого и нижнего правого углов прямоугольника
    WindowRect.left = (long)0;                      // устанавливает Лево в 0
    WindowRect.right = (long)width;              // устанавливает Право в запрошенную ширину
    WindowRect.top = (long)0;                              // устанавливает Верх в 0
    WindowRect.bottom = (long)height;              // устанавливает Низ в запрошенную высоту

    fullscreen = fullscreenflag;                      // поставить глобальный полноэкранный флаг

    hInstance                        = GetModuleHandle(NULL);                                // получает экземпляр окна
    wc.style                        = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;        // перерисовка при изменении размеров, собственный DC для окна
    wc.lpfnWndProc                = (WNDPROC) WndProc;                                        // WndProc обрабатывает сообщения
    wc.cbClsExtra                = 0;
    wc.cbWndExtra                = 0;
    wc.hInstance                = hInstance;                                                        // установить экземпляр
    wc.hIcon                        = LoadIcon(NULL, IDI_WINLOGO);                        // загрузить пиктограмму по умолчанию
    wc.hCursor                        = LoadCursor(NULL, IDC_ARROW);                        // загрузить стандартный указатель
    wc.hbrBackground        = NULL;                                                                        // для GL не нужно фона
    wc.lpszMenuName                = NULL;                                                                        // нам не нужно меню
    wc.lpszClassName        = "OpenGL";                                                                // установить имя класса

    if (!RegisterClass(&wc)) {                                                                      // попытка регистрации класса окна
        MessageBox(NULL, "Не удается зарегистрировать класс окна.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                                        // возвратить FALSE
    }

    if (fullscreen) {                                                                                              // попробовать на полный экран?
        DEVMODE dmScreenSettings;                                                                // режим устройства
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));      // убедиться, что память освобождена
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);              // размер структуры режима устройства
        dmScreenSettings.dmPelsWidth        = width;                                // ширина экрана
        dmScreenSettings.dmPelsHeight        = height;                                // высота экрана
        dmScreenSettings.dmBitsPerPel        = bits;                                        // кол-во бит на пиксел
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // попробовать поставить выбранный режим и получить результат.  ПОМНИТЕ: CDS_FULLSCREEN скрывает панел задач.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // если режим не удается установить, предложить два варианта. Выход или в окне.
            if (MessageBox(NULL, "Запрошенный полноэкранный режим не поддерживается вашей\nвидео-картой. Будем работать в окне?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE;              // значит, будем в окне.
            } else {
                // выдать сообщение о закрытии программы.
                MessageBox(NULL, "Программа завершает работу.", "ОШИБКА", MB_OK | MB_ICONSTOP);
                return FALSE;                                                                        // возвратить FALSE
            }
        }
    }

    if (fullscreen) {                                                                                              // мы все еще на полном экране?
        dwExStyle = WS_EX_APPWINDOW;                                                              // расширенный стиль окна
        dwStyle = WS_POPUP;                                                                              // стиль окна
        ShowCursor(FALSE);                                                                                // спрятать курсор мыши
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                      // расширенный стиль окна
        dwStyle = WS_OVERLAPPEDWINDOW;                                                      // стиль окна
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);                // подогнать окно к требуемому размеру

    // создать окно
    if (!(hWnd = CreateWindowEx(        dwExStyle,                                                      // расширенный стиль для окна
                                        "OpenGL",                                                        // имя класса
                                        title,                                                                // заголовок окна
                                        dwStyle |                                                        // определенный стиль окна
                                        WS_CLIPSIBLINGS |                                        // нужный стиль окна
                                        WS_CLIPCHILDREN,                                        // нужный стиль окна
                                        0, 0,                                                                // расположение окна
                                        WindowRect.right - WindowRect.left,      // посчитать ширину окна
                                        WindowRect.bottom - WindowRect.top,      // посчитать высоту окна
                                        NULL,                                                                // без родительского окна
                                        NULL,                                                                // без меню
                                        hInstance,                                                        // экземпляр
                                        NULL))) {                                                              // не сообщать WM_CREATE ничего
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Ошибка создания окна.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    static        PIXELFORMATDESCRIPTOR pfd = {                             // pfd рассказывает виндам, как мы хотим, чтобы это все выглядело
        sizeof(PIXELFORMATDESCRIPTOR),                                // размер этого Описания Формата Пикселя
        1,                                                                                        // номер версии
        PFD_DRAW_TO_WINDOW |                                                // формат должен поддерживать окно
        PFD_SUPPORT_OPENGL |                                                // формат должен поддерживать OpenGL
        PFD_DOUBLEBUFFER,                                                        // должен поддерживать двойную буферизацию
        PFD_TYPE_RGBA,                                                                // попросить формат RGBA
        bits,                                                                                // выбрать нашу глубину цвета
        0, 0, 0, 0, 0, 0,                                                        // биты цвета игнорируются
        0,                                                                                        // обойдемся без альфа-буфера
        0,                                                                                        // биты сдвига проигнорируем
        0,                                                                                        // без буфера накопления
        0, 0, 0, 0,                                                                        // биты накопления проигнорированы
        16,                                                                                        // 16-разрядный Z-буфер (буфер глубины)
        0,                                                                                        // без буфера трафарета
        0,                                                                                        // без вспомогательного буфера
        PFD_MAIN_PLANE,                                                                // главный рисовальный слой
        0,                                                                                        // забронировано :)
        0, 0, 0                                                                                // маска слоя проигнорирована
    };

    if (!(hDC = GetDC(hWnd))) {                                                    // получили ли мы контекст устройства?
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Не могу создать контекст устройства GL.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {   // нашла ли винда подходящий формат пиксела?
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Не могу найти подходящий формат пиксела.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {            // можем ли мы установить The Pixel Format?
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Не могу установить формат пиксела.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    if (!(hRC = wglCreateContext(hDC))) {                            // можем ли мы получить контекст отображения?
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Не могу создать контекст отображения GL.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    if(!wglMakeCurrent(hDC, hRC)) {                                     // попробовать задействовать конекст отображения
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Не могу активизировать контекст отбражения GL.", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    ShowWindow(hWnd, SW_SHOW);                                               // показать окно
    SetForegroundWindow(hWnd);                                                // приоритет немного выше
    SetFocus(hWnd);                                                                        // окно в фокусе клавиатуры
    ReSizeGLScene(width, height);                                        // установить нашу GL-перспективу

    if (!InitGL()) {                                                                      // инициализировать наше новое GL-окно
        KillGLWindow();                                                                // сброс дисплея
        MessageBox(NULL, "Инициализация провалена :(", "ОШИБКА", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // возвратить FALSE
    }

    return TRUE;                                                                        // удалось!
}

LRESULT CALLBACK WndProc(        HWND        hWnd,                        // хэндл для этого окна
                                 UINT        uMsg,                        // сообщение для этого окна
                                 WPARAM        wParam,                        // дополнительная информация
                                 LPARAM        lParam) {                      // дополнительная информация
    switch (uMsg) {                                                                      // проверим сообщения окна
        case WM_ACTIVATE: {                                                      // наблюдаем за сообщением об активизации окна
            if (!HIWORD(wParam)) {                                      // проверим состояние минимизациии
                active = TRUE;                                              // программа активна
            } else {
                active = FALSE;                                              // программа неактивна
            }

            return 0;                                                                // вернуться к циклу сообщений
        }

        case WM_SYSCOMMAND: {                                                      // перехватываем системную команду
            switch (wParam) {                                                      // проверка выбора системы
                case SC_SCREENSAVE:                                        // пытается включить скринсэйвер?
                case SC_MONITORPOWER:                                // монитор пытается переключиться в режим сохранения энергии?
                    return 0;                                                        // ФИГ ИМ!
            }

            break;                                                                        // выход из итерации
        }

        case WM_CLOSE: {                                                              // мы получили сообщение о закрытии программы?
            PostQuitMessage(0);                                                // послать сообщение о выходе
            return 0;                                                                // вернуться
        }

        case WM_KEYDOWN: {                                                      // была нажата клавиша?
            keys[wParam] = TRUE;                                        // пометим соответствующую ячейку массива TRUE
            return 0;                                                                // вернуться
        }

        case WM_KEYUP: {                                                              // клавиша была отпущена?
            keys[wParam] = FALSE;                                        // пометим соответствующую ячейку массива FALSE
            return 0;                                                                // вернуться
        }

        case WM_SIZE: {                                                              // изменение размера окна OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord = ширина, HiWord = высота
            return 0;                                                                // вернуться
        }
    }

    // отсылаем все прочие сообщения в DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(        HINSTANCE        hInstance,                        // экземпляр
                           HINSTANCE        hPrevInstance,                // предыдущий экземпляр
                           LPSTR                lpCmdLine,                        // параметры командной строки
                           int                        nCmdShow) {                      // состояние показа окна
    MSG                msg;                                                                        // структура сообщения винды
    BOOL        done = FALSE;                                                              // переменная булевского типа для выхода из цикла

    // спросить юзера, какой режим экрана он предпочитает
    if (MessageBox(NULL, "Хотите запустить в полноэкранном режиме?", "На полный экран?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;                                                      // оконный режим
    }

    // создать наше окно OpenGL
    if (!CreateGLWindow("Урок bosco и NeHe \"Развевающаяся текстура\"", 640, 480, 16, fullscreen)) {
        return 0;                                                                        // выйти, если окно так и не было создано
    }

    while(!done) {                                                                      // цикл, выполняемый, пока done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  // есть ли для нас сообщения?
            if (msg.message == WM_QUIT) {                            // мы получили сообщение о выходе?
                done = TRUE;                                                      // если так, то done=TRUE
            } else {                                                                    // если нет, париться с другими сообщениями
                TranslateMessage(&msg);                                // перевести сообщение
                DispatchMessage(&msg);                                // отправить сообщение
            }
        } else {                                                                            // если нет сообщений
            // нарисовать сцену. Следить за ESC и сообщениями о выходе от DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) {      // активно? получена ли команда "выход"?
                done = TRUE;                                                      // ESC либо DrawGLScene сигнализирует о выходе
            } else {                                                                    // не время выходить, обновляем изображение
                SwapBuffers(hDC);                                        // взаимозаменить буферы (двойная буферизация)
            }

            if (keys[VK_F1]) {                                              // нажата ли F1?
                keys[VK_F1] = FALSE;                                      // если так, установить ячейку в FALSE
                KillGLWindow();                                                // убить текущее окно :)
                fullscreen = !fullscreen;                              // переключить полный экран / оконный режим

                // пересоздать окно OpenGL
                if (!CreateGLWindow("bosco & NeHe's Waving Texture Tutorial", 640, 480, 16, fullscreen)) {
                    return 0;                                                // выйти, если окно так и не было создано
                }
            }
        }
    }

    // завершение работы
    KillGLWindow();                                                                        // убить окно :)
    return (msg.wParam);                                                        // выйти из программы
}
