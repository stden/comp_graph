#include <windows.h> //Заголовочные файлы для Windows
#include <stdio.h>
#include <gl\gl.h> //Заголовочные файлы для библиотеки OpenGL32
#include <gl\glu.h> //Заголовочные файлы для библиотеки Glu32
#include <gl\glaux.h> //заголовочные файлы для библиотеки Glaux

HDC  hDC = NULL; // Приватный контекст устройства GDI
HGLRC  hRC = NULL; // Постоянный контекст рендеринга

HWND  hWnd = NULL; // Здесь будет хранится дескриптор окна
HINSTANCE  hInstance; // Здесь будет хранится дескриптор приложения

bool  keys[256]; // Массив, используемый для операций с клавиатурой
bool  active = TRUE; // Флаг активности окна, установленный в true по умолчанию
bool  fullscreen = TRUE; // Флаг режима окна, установленный в полноэкранный по умолчанию
bool    twinkle;                        // Twinkling Stars (Вращающиеся звезды)
bool    tp;                             // 'T' клавиша нажата?

const   num = 50;                       // Количество рисуемых звезд

typedef struct {                        // Создаём структуру для звезд
    int r, g, b;                    // Цвет звезды
    GLfloat dist;                   // Расстояние от центра
    GLfloat angle;                  // Текущий угол звезды
}
stars;                                  // Имя структуры - Stars
stars star[num];                        // Делаем массив 'star' длинной 'num',
// где элементом является структура 'stars'

GLfloat zoom = -15.0f;                  // Расстояние от наблюдателя до звезд
GLfloat tilt = 90.0f;                   // Начальный угол
GLfloat spin;                           // Для вращения звезд

GLuint  loop;                           // Используется для циклов
GLuint  texture[1];                     // Массив для одной текстуры



LRESULT  CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );// Прототип функции WndProc
AUX_RGBImageRec* LoadBMP(char* Filename) { // Функция для загрузки bmp файлов
    FILE* File = NULL;              // Переменная для файла

    if (!Filename) {                // Нужно убедиться в правильности переданого имени
        return NULL;            // Если неправильное имя, то возвращаем NULL
    }

    File = fopen(Filename, "r");    // Открываем и проверяем на наличие файла

    if (File) {                     // Файл существует?
        fclose(File);           // Если да, то закрываем файл
        // И загружаем его с помощью библиотеки AUX, возращая ссылку на изображение
        return auxDIBImageLoad(Filename);
    }

    // Если загрузить не удалось или файл не найден, то возращаем NULL
    return NULL;
}

int LoadGLTextures() {  // Функция загрузки изображения и конвертирования в текстуру
    int Status = FALSE;             // Индикатор статуса

    AUX_RGBImageRec* TextureImage[1];// Создаём место для хранения текстуры

    memset(TextureImage, 0, sizeof(void*) * 1); // устанавливаем ссылку на NULL

    // Загружаем изображение, Проверяем на ошибки, Если файл не найден то выходим
    if (TextureImage[0] = LoadBMP("Data/Star.bmp")) {
        Status = TRUE;          // Ставим статус в TRUE

        glGenTextures(1, &texture[0]);  // Генерируем один индификатор текстуры

        // Создаём текстуру с линейной фильтрацией (Linear Filtered)
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
                     TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }

    if (TextureImage[0]) {          // Если текстура существует
        if (TextureImage[0]->data) { // Если изображение существует
            // Освобождаем место выделенное под изображение
            free(TextureImage[0]->data);
        }

        free(TextureImage[0]);  // Освобождаем структуры изображения
    }

    return Status;                  // Возвращаем статус
}


GLvoid ReSizeGLScene( GLsizei width, GLsizei height ) { // Изменить размер и инициализировать окно GL
    if( height == 0 ) { // Предотвращение деления на ноль
        height = 1;
    }

    glViewport( 0, 0, width, height ); // Сброс текущей области вывода

    glMatrixMode(GL_PROJECTION);     // Выбор матрицы проекций

    glLoadIdentity();    // Сброс матрицы проекции



    // Вычисление соотношения геометрических размеров для окна

    gluPerspective( 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f );



    glMatrixMode( GL_MODELVIEW );            // Выбор матрицы вида модели

    glLoadIdentity();              // Сброс матрицы вида модели

}

int InitGL(GLvoid) {                    // Всё установки OpenGL будут здесь
    if (!LoadGLTextures()) {        // Загружаем текстуру
        return FALSE;           // Если не загрузилась, то возвращаем FALSE
    }

    glEnable(GL_TEXTURE_2D);        // Включаем текстурирование
    // Включаем плавную ракраску (интерполирование по вершинам)
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   // Фоном будет черный цвет
    glClearDepth(1.0f);                     // Установки буфера глубины (Depth Buffer)
    // Максимальное качество перспективной коррекции
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // Устанавливаем функцию смешивания
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);                     // Включаем смешивание

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

    return TRUE;                    // Инициализация прошла нормально.

}


int DrawGLScene(GLvoid) {               // Здесь мы всё рисуем
    // Очищаем буфер цвета и глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Выбираем нашу текстуру
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    for (loop = 0; loop < num; loop++) {         // Цикл по всем звездам
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
        glRotatef(-tilt, 1.0f, 0.0f, 0.0f);     // Отменяет поворот экрана


        if (twinkle) {                          // Если Twinkling включен
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
            glEnd();                                // Закончили рисовать
        }


        glRotatef(spin, 0.0f, 0.0f, 1.0f); // Поворачиваем звезду по оси z
        // Цвет использует байты
        glColor4ub(star[loop].r, star[loop].g, star[loop].b, 255);
        glBegin(GL_QUADS);              // Начинаем рисовать текстурный квадрат
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();                                        // Закончили рисовать


        spin += 0.01f;                  // Используется для вращения звезды
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

    return TRUE;                                            // Всё ок
}

GLvoid KillGLWindow( GLvoid )              // Корректное разрушение окна

{

    if( fullscreen ) {            // Мы в полноэкранном режиме?

        ChangeDisplaySettings( NULL, 0 );        // Если да, то переключаемся обратно в оконный режим

        ShowCursor( TRUE );            // Показать курсор мышки

    }


    if( hRC )                // Существует ли Контекст Рендеринга?

    {
        if( !wglMakeCurrent( NULL, NULL ) )        // Возможно ли освободить RC и DC?

        {
            MessageBox( NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        }

        if( !wglDeleteContext( hRC ) )        // Возможно ли удалить RC?

        {

            MessageBox( NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        }

        hRC = NULL;              // Установить RC в NULL

    }


    if( hDC && !ReleaseDC( hWnd, hDC ) )          // Возможно ли уничтожить DC?

    {

        MessageBox( NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        hDC = NULL;              // Установить DC в NULL

    }

    if(hWnd && !DestroyWindow(hWnd))            // Возможно ли уничтожить окно?

    {

        MessageBox( NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        hWnd = NULL;                // Установить hWnd в NULL

    }

    if( !UnregisterClass( "OpenGL", hInstance ) )        // Возможно ли разрегистрировать класс

    {

        MessageBox( NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hInstance = NULL;                // Установить hInstance в NULL

    }

}


BOOL CreateGLWindow( char* title, int width, int height, int bits, bool fullscreenflag )

{

    GLuint    PixelFormat;              // Хранит результат после поиска


    WNDCLASS  wc;

    DWORD    dwExStyle;              // Расширенный стиль окна

    DWORD    dwStyle;              // Обычный стиль окна

    RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values

    WindowRect.left = (long)0;            // Установить левую составляющую в 0

    WindowRect.right = (long)width;            // Установить правую составляющую в Width

    WindowRect.top = (long)0;              // Установить верхнюю составляющую в 0

    WindowRect.bottom = (long)height;            // Установить нижнюю составляющую в Height


    fullscreen = fullscreenflag;            // Устанавливаем значение глобальной переменной fullscreen


    hInstance    = GetModuleHandle(NULL);        // Считаем дескриптор нашего приложения

    wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // Перерисуем при перемещении и создаём скрытый DC

    wc.lpfnWndProc    = (WNDPROC) WndProc;          // Процедура обработки сообщений

    wc.cbClsExtra    = 0;              // Нет дополнительной информации для окна

    wc.cbWndExtra    = 0;              // Нет дополнительной информации для окна

    wc.hInstance    = hInstance;            // Устанавливаем дескриптор

    wc.hIcon    = LoadIcon(NULL, IDI_WINLOGO);        // Загружаем иконку по умолчанию

    wc.hCursor    = LoadCursor(NULL, IDC_ARROW);        // Загружаем указатель мышки

    wc.hbrBackground  = NULL;              // Фон не требуется для GL

    wc.lpszMenuName    = NULL;              // Меню в окне не будет

    wc.lpszClassName  = "OpenGL";            // Устанавливаем имя классу


    if( !RegisterClass( &wc ) )              // Пытаемся зарегистрировать класс окна

    {

        MessageBox( NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Выход и возвращение функцией значения false

    }


    if( fullscreen )                // Полноэкранный режим?

    {


        DEVMODE dmScreenSettings;            // Режим устройства

        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );    // Очистка для хранения установок

        dmScreenSettings.dmSize = sizeof( dmScreenSettings );    // Размер структуры Devmode

        dmScreenSettings.dmPelsWidth  =   width;        // Ширина экрана

        dmScreenSettings.dmPelsHeight  =   height;        // Высота экрана

        dmScreenSettings.dmBitsPerPel  =   bits;        // Глубина цвета

        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пикселя



        // Пытаемся установить выбранный режим и получить результат.  Примечание: CDS_FULLSCREEN убирает панель управления.

        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )

        {

            // Если переключение в полноэкранный режим невозможно, будет предложено два варианта: оконный режим или выход.

            if( MessageBox( NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",

                            "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES )

            {

                fullscreen = FALSE;          // Выбор оконного режима (fullscreen = false)

            }

            else

            {

                // Выскакивающее окно, сообщающее пользователю о закрытие окна.

                MessageBox( NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP );

                return FALSE;            // Выход и возвращение функцией false

            }

        }

    }

    if(fullscreen)                  // Мы остались в полноэкранном режиме?

    {
        dwExStyle  =   WS_EX_APPWINDOW;          // Расширенный стиль окна

        dwStyle    =   WS_POPUP;            // Обычный стиль окна

        ShowCursor( FALSE );              // Скрыть указатель мышки

    }

    else

    {

        dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // Расширенный стиль окна

        dwStyle    =   WS_OVERLAPPEDWINDOW;        // Обычный стиль окна

    }


    AdjustWindowRectEx( &WindowRect, dwStyle, FALSE, dwExStyle );      // Подбирает окну подходящие размеры


    if( !( hWnd = CreateWindowEx(  dwExStyle,         // Расширенный стиль для окна

                                   "OpenGL",          // Имя класса

                                   title,            // Заголовок окна
                                   dwStyle |   // Выбираемые стили для окна

                                   WS_CLIPSIBLINGS |        // Требуемый стиль для окна

                                   WS_CLIPCHILDREN ,        // Требуемый стиль для окна


                                   0, 0,            // Позиция окна

                                   WindowRect.right - WindowRect.left,  // Вычисление подходящей ширины

                                   WindowRect.bottom - WindowRect.top,  // Вычисление подходящей высоты

                                   NULL,            // Нет родительского

                                   NULL,            // Нет меню

                                   hInstance,          // Дескриптор приложения

                                   NULL ) ) )          // Не передаём ничего до WM_CREATE (???)


    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Вернуть false

    }

    static  PIXELFORMATDESCRIPTOR pfd =           // pfd сообщает Windows каким будет вывод на экран каждого пикселя

    {

        sizeof(PIXELFORMATDESCRIPTOR),            // Размер дескриптора данного формата пикселей

        1,                  // Номер версии

        PFD_DRAW_TO_WINDOW |              // Формат для Окна

        PFD_SUPPORT_OPENGL |              // Формат для OpenGL

        PFD_DOUBLEBUFFER,              // Формат для двойного буфера

        PFD_TYPE_RGBA,                // Требуется RGBA формат

        bits,                  // Выбирается бит глубины цвета

        0, 0, 0, 0, 0, 0,              // Игнорирование цветовых битов

        0,                  // Нет буфера прозрачности

        0,                  // Сдвиговый бит игнорируется

        0,                  // Нет буфера накопления

        0, 0, 0, 0,                // Биты накопления игнорируются

        32,                  // 32 битный Z-буфер (буфер глубины)

        0,                  // Нет буфера трафарета

        0,                  // Нет вспомогательных буферов

        PFD_MAIN_PLANE,                // Главный слой рисования

        0,                  // Зарезервировано

        0, 0, 0                  // Маски слоя игнорируются

    };


    if( !( hDC = GetDC( hWnd ) ) )              // Можем ли мы получить Контекст Устройства?

    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Вернуть false

    }

    if( !( PixelFormat = ChoosePixelFormat( hDC, &pfd ) ) )        // Найден ли подходящий формат пикселя?

    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Вернуть false

    }


    if( !SetPixelFormat( hDC, PixelFormat, &pfd ) )          // Возможно ли установить Формат Пикселя?

    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Вернуть false

    }

    if( !( hRC = wglCreateContext( hDC ) ) )          // Возможно ли установить Контекст Рендеринга?

    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

        return FALSE;                // Вернуть false

    }


    if( !wglMakeCurrent( hDC, hRC ) )            // Попробовать активировать Контекст Рендеринга

    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Вернуть false

    }


    ShowWindow( hWnd, SW_SHOW );              // Показать окно

    SetForegroundWindow( hWnd );              // Слегка повысим приоритет

    SetFocus( hWnd );                // Установить фокус клавиатуры на наше окно

    ReSizeGLScene( width, height );              // Настроим перспективу для нашего OpenGL экрана.


    if( !InitGL() )                  // Инициализация только что созданного окна

    {

        KillGLWindow();                // Восстановить экран

        MessageBox( NULL, ("Initialization Failed."), ("ERROR"), MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // Вернуть false

    }


    return TRUE;                  // Всё в порядке!

}

LRESULT CALLBACK WndProc(  HWND  hWnd,            // Дескриптор нужного окна

                           UINT  uMsg,            // Сообщение для этого окна

                           WPARAM  wParam,            // Дополнительная информация

                           LPARAM  lParam)            // Дополнительная информация

{


    switch (uMsg)                // Проверка сообщения для окна

    {

        case WM_ACTIVATE:            // Проверка сообщения активности окна

        {

            if( !HIWORD( wParam ) )          // Проверить состояние минимизации

            {

                active = TRUE;          // Программа активна

            }

            else

            {

                active = FALSE;          // Программа теперь не активна

            }



            return 0;            // Возвращаемся в цикл обработки сообщений

        }


        case WM_SYSCOMMAND:            // Перехватываем системную команду

        {

            switch ( wParam )            // Останавливаем системный вызов

            {

                case SC_SCREENSAVE:        // Пытается ли запустится скринсейвер?

                case SC_MONITORPOWER:        // Пытается ли монитор перейти в режим сбережения энергии?

                    return 0;          // Предотвращаем это

            }

            break;              // Выход

        }


        case WM_CLOSE:              // Мы получили сообщение о закрытие?

        {

            PostQuitMessage( 0 );          // Отправить сообщение о выходе

            return 0;            // Вернуться назад

        }

        case WM_KEYDOWN:            // Была ли нажата кнопка?

        {

            keys[wParam] = TRUE;          // Если так, мы присваиваем этой ячейке true

            return 0;            // Возвращаемся

        }


        case WM_KEYUP:              // Была ли отпущена клавиша?

        {

            keys[wParam] = FALSE;          //  Если так, мы присваиваем этой ячейке false

            return 0;            // Возвращаемся

        }


        case WM_SIZE:              // Изменены размеры OpenGL окна

        {

            ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );  // Младшее слово=Width, старшее слово=Height

            return 0;            // Возвращаемся

        }

    }


    // пересылаем все необработанные сообщения DefWindowProc

    return DefWindowProc( hWnd, uMsg, wParam, lParam );

}

int WINAPI WinMain(  HINSTANCE  hInstance,        // Дескриптор приложения

                     HINSTANCE  hPrevInstance,        // Дескриптор родительского приложения

                     LPSTR    lpCmdLine,        // Параметры командной строки

                     int    nCmdShow )        // Состояние отображения окна

{

    MSG  msg;              // Структура для хранения сообщения Windows

    BOOL  done = FALSE;            // Логическая переменная для выхода из цикла


    // Спрашивает пользователя, какой режим экрана он предпочитает

    if( MessageBox( NULL, "Хотите ли Вы запустить приложение в полноэкранном режиме?",  "Запустить в полноэкранном режиме?", MB_YESNO | MB_ICONQUESTION) == IDNO )

    {

        fullscreen = FALSE;          // Оконный режим

    }


    // Создать наше OpenGL окно

    if( !CreateGLWindow( "NeHe OpenGL окно", 800, 600, 32, fullscreen ) )

    {

        return 0;              // Выйти, если окно не может быть создано

    }


    while( !done )                // Цикл продолжается, пока done не равно true

    {

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )    // Есть ли в очереди какое-нибудь сообщение?

        {

            if( msg.message == WM_QUIT )        // Мы поучили сообщение о выходе?

            {

                done = TRUE;          // Если так, done=true

            }

            else              // Если нет, обрабатывает сообщения

            {


                TranslateMessage( &msg );        // Переводим сообщение

                DispatchMessage( &msg );        // Отсылаем сообщение

            }

        }

        else                // Если нет сообщений

        {


            // Прорисовываем сцену.

            if( (active && !DrawGLScene()) || keys[VK_ESCAPE])         // Активна ли программа?

            {

                done = TRUE;                        // ESC or DrawGLScene Signalled A Quit
            } else {                                // Not Time To Quit, Update Screen
                SwapBuffers(hDC);               // Смена буфера (Double Buffering)

                if (keys['T'] && !tp) {         // Если 'T' нажата и tp равно FALSE
                    tp = TRUE;              // то делаем tp равным TRUE
                    twinkle = !twinkle;     // Меняем значение twinkle на обратное
                }

                if (!keys['T']) {               // Клавиша 'T' была отключена
                    tp = FALSE;             // Делаем tp равное FALSE
                }

                if (keys[VK_UP]) {              // Стрелка вверх была нажата?
                    tilt -= 0.5f;           // Вращаем экран вверх
                }

                if (keys[VK_DOWN]) {            // Стрелка вниз нажата?
                    tilt += 0.5f;           // Вращаем экран вниз
                }

                if (keys[VK_PRIOR]) {           // Page Up нажат?
                    zoom -= 0.2f;           // Уменьшаем
                }

                if (keys[VK_NEXT]) {            // Page Down нажата?
                    zoom += 0.2f;           // Увеличиваем
                }

                if (keys[VK_F1]) {              // Если F1 нажата?
                    keys[VK_F1] = FALSE;    // Делаем клавишу равной FALSE
                    KillGLWindow();         // Закрываем текущее окно
                    fullscreen = !fullscreen;

                    // Переключаем режимы Fullscreen (полноэкранный) / Windowed (обычный)
                    // Пересоздаём OpenGL окно
                    if (!CreateGLWindow("NeHe's Textures,", 640, 480, 16, fullscreen)) {
                        return 0;       //Выходим если не получилось
                    }
                }
            }
        }
    }

    // Shutdown

    KillGLWindow();                // Разрушаем окно
    return ( msg.wParam );              // Выходим из программы
}








