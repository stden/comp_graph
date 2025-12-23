#include <windows.h> //Заголовочные файлы для Windows
#include <stdio.h>    // Заголовочный файл для стандартного ввода/вывода (ДОБАВИЛИ)
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

bool light;      // Свет ВКЛ / ВЫКЛ

bool lp;         // L нажата?

bool fp;         // F нажата?
bool   gp;                              // G Нажата? ( Новое )

GLfloat xrot;         // X вращение

GLfloat yrot;         // Y вращение

GLfloat xspeed;       // X скорость вращения

GLfloat yspeed;       // Y скорость вращения



GLfloat z = -5.0f;    // Сдвиг вглубь экрана

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Значения фонового света ( НОВОЕ )
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света ( НОВОЕ )
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };    // Позиция света ( НОВОЕ )
GLuint filter;                          // Используемый фильтр для текстур
GLuint  texture[3];         // Storage For 3 Textures
GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR }; // Хранит три типа тумана

GLuint fogfilter = 0;                   // Тип используемого тумана

GLfloat fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f}; // Цвет тумана

LRESULT  CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );// Прототип функции WndProc

AUX_RGBImageRec* LoadBMP(char* Filename)     // Загрузка картинки

{

    FILE* File = NULL;        // Индекс файла



    if (!Filename)            // Проверка имени файла

    {

        return NULL;             // Если нет вернем NULL

    }



    File = fopen(Filename, "r"); // Проверим существует ли файл



    if (File)                 // Файл существует?

    {

        fclose(File);            // Закрыть файл

        return auxDIBImageLoad(Filename); // Загрузка картинки и вернем на нее указатель

    }

    return NULL;              // Если загрузка не удалась вернем NULL

}

int LoadGLTextures()                      // Загрузка картинки и конвертирование в текстуру

{

    int Status = FALSE;                      // Индикатор состояния



    AUX_RGBImageRec* TextureImage[1];        // Создать место для текстуры



    memset(TextureImage, 0, sizeof(void*) * 1); // Установить указатель в NULL
    // Загрузка картинки, проверка на ошибки, если картинка не найдена - выход

    if (TextureImage[0] = LoadBMP("Data/Crate.bmp"))

    {

        Status = TRUE;     // Установим Status в TRUE
        glGenTextures(3, &texture[0]);     // Создание трех текстур
        // Создание текстуры с фильтром по соседним пикселям

        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // ( НОВОЕ )

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ( НОВОЕ )

        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,

                     0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // Создание текстуры с линейной фильтрацией

        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,

                     0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // Создание Текстуры с Мип-Наложением

        glBindTexture(GL_TEXTURE_2D, texture[2]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // ( НОВОЕ )

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,

                          GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data); // ( НОВОЕ )

    }

    if (TextureImage[0])           // Если текстура существует

    {

        if (TextureImage[0]->data)    // Если изображение текстуры существует

        {

            free(TextureImage[0]->data); // Освобождение памяти изображения текстуры

        }



        free(TextureImage[0]);        // Освобождение памяти под структуру

    }

    return Status;        // Возвращаем статус

}


GLvoid ReSizeGLScene( GLsizei width, GLsizei height ) { // Изменить размер и инициализировать окно GL
    if( height == 0 ) { // Предотвращение деления на ноль
        height = 1;
    }

    glViewport( 0, 0, width, height ); // Сброс текущей области вывода

    glMatrixMode(GL_PROJECTION);     // Выбор матрицы проекций

    glLoadIdentity();    // Сброс матрицы проекции
    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix
    glLoadIdentity();                                   // Reset The Modelview Matrix
}

int InitGL(GLvoid)             // Все настройки для OpenGL делаются здесь

{

    if (!LoadGLTextures())        // Переход на процедуру загрузки текстуры

    {

        return FALSE;                // Если текстура не загружена возвращаем FALSE

    }



    glEnable(GL_TEXTURE_2D);      // Разрешить наложение текстуры

    glShadeModel(GL_SMOOTH);      // Разрешение сглаженного закрашивания

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);   // Будем очищать экран, заполняя его цветом тумана. ( Изменено )

    glClearDepth(1.0f);           // Установка буфера глубины

    glEnable(GL_DEPTH_TEST);      // Разрешить тест глубины

    glDepthFunc(GL_LEQUAL);       // Тип теста глубины

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Улучшенные вычисления перспективы


    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // Установка Диффузного Света
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   // Позиция света
    glEnable(GL_LIGHT1); // Разрешение источника света номер один
    glEnable(GL_FOG);                       // Включает туман (GL_FOG)

    glFogi(GL_FOG_MODE, fogMode[fogfilter]);// Выбираем тип тумана

    glFogfv(GL_FOG_COLOR, fogColor);        // Устанавливаем цвет тумана

    glFogf(GL_FOG_DENSITY, 0.35f);          // Насколько густым будет туман

    glHint(GL_FOG_HINT, GL_DONT_CARE);      // Вспомогательная установка тумана

    glFogf(GL_FOG_START, 1.0f);             // Глубина, с которой начинается туман

    glFogf(GL_FOG_END, 5.0f);               // Глубина, где туман заканчивается.


    return TRUE;         // Инициализация прошла OK

}
int DrawGLScene(GLvoid)        // Здесь мы делаем все рисование

{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // Очистка Экрана и Буфера Глубины

    glLoadIdentity();       // Сброс Просмотра
    glTranslatef(0.0f, 0.0f, z);    // Перенос В/Вне экрана по z

    glRotatef(xrot, 1.0f, 0.0f, 0.0f); // Вращение по оси X на xrot

    glRotatef(yrot, 0.0f, 1.0f, 0.0f); // Вращение по оси Y по yrot
    glBindTexture(GL_TEXTURE_2D, texture[filter]);    // Выбор текстуры основываясь на filter



    glBegin(GL_QUADS);       // Начало рисования четырехугольников

    // Передняя грань

    glNormal3f( 0.0f, 0.0f, 1.0f);     // Нормаль указывает на наблюдателя

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); // Точка 1 (Перед)

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); // Точка 2 (Перед)

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); // Точка 3 (Перед)

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); // Точка 4 (Перед)

    // Задняя грань

    glNormal3f( 0.0f, 0.0f, -1.0f);    // Нормаль указывает от наблюдателя

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Зад)

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); // Точка 2 (Зад)

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); // Точка 3 (Зад)

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); // Точка 4 (Зад)

    // Верхняя грань

    glNormal3f( 0.0f, 1.0f, 0.0f);     // Нормаль указывает вверх

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); // Точка 1 (Верх)

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); // Точка 2 (Верх)

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); // Точка 3 (Верх)

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); // Точка 4 (Верх)

    // Нижняя грань

    glNormal3f( 0.0f, -1.0f, 0.0f);    // Нормаль указывает вниз

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Низ)

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); // Точка 2 (Низ)

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); // Точка 3 (Низ)

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); // Точка 4 (Низ)

    // Правая грань

    glNormal3f( 1.0f, 0.0f, 0.0f);     // Нормаль указывает вправо

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); // Точка 1 (Право)

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); // Точка 2 (Право)

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); // Точка 3 (Право)

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); // Точка 4 (Право)

    // Левая грань

    glNormal3f(-1.0f, 0.0f, 0.0f);     // Нормаль указывает влево

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // Точка 1 (Лево)

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); // Точка 2 (Лево)

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); // Точка 3 (Лево)

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); // Точка 4 (Лево)

    glEnd();        // Кончили рисовать четырехугольник
    xrot += xspeed;      // Добавить в xspeed значение xrot

    yrot += yspeed;      // Добавить в yspeed значение yrot

    return TRUE;         // Выйти

}


GLvoid KillGLWindow(GLvoid)               // Правильное уничтожение окна

{

    if (fullscreen)                       // Полноэкранный режим?

    {

        ChangeDisplaySettings(NULL, 0);   // Переход в режим разрешения рабочего стола

        ShowCursor(TRUE);// Показать указатель мыши

    }



    if (hRC)                              // Существует контекст рендеринга?

    {

        if (!wglMakeCurrent(NULL, NULL))  // Можно ли освободить DC и RC контексты?

        {

            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR",

                       MB_OK | MB_ICONINFORMATION);

        }

        if (!wglDeleteContext(hRC))         // Можно ли уничтожить RC?

        {

            MessageBox(NULL, "Release Rendering Context Failed.",

                       "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        }

        hRC = NULL;                             // Установим RC в NULL

    }



    if (hDC && !ReleaseDC(hWnd, hDC))     // Можно ли уничтожить DC?

    {

        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR",

                   MB_OK | MB_ICONINFORMATION);

        hDC = NULL;                           // Установим DC в NULL

    }

    if (hWnd && !DestroyWindow(hWnd))     // Можно ли уничтожить окно?

    {

        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK |

                   MB_ICONINFORMATION);

        hWnd = NULL;                          // Уствновим hWnd в NULL

    }

    if (!UnregisterClass("OpenGL", hInstance)) // Можно ли уничтожить класс?

    {

        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK |

                   MB_ICONINFORMATION);

        hInstance = NULL;                     // Устанавливаем hInstance в NULL

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

            if( (active && !DrawGLScene()) || keys[VK_ESCAPE])          // Активна ли программа?

            {
                done = TRUE;
            } else {                              // Not Time To Quit, Update Screen
                SwapBuffers(hDC);     // Переключение буферов (Двойная буферизация)

                if (keys['L'] && !lp) // Клавиша 'L' нажата и не удерживается?

                {
                    lp = TRUE;    // lp присвоили TRUE

                    light = !light; // Переключение света TRUE/FALSE

                    if (!light)               // Если не свет

                    {

                        glDisable(GL_LIGHTING);  // Запрет освещения

                    }

                    else                      // В противном случае

                    {

                        glEnable(GL_LIGHTING);   // Разрешить освещение

                    }

                }

                if (!keys['L']) // Клавиша 'L' Отжата?

                {

                    lp = FALSE;    // Если так, то lp равно FALSE

                }

                if (keys['F'] && !fp) // Клавиша 'F' нажата?

                {

                    fp = TRUE;           // fp равно TRUE

                    filter += 1;         // значение filter увеличивается на один

                    if (filter > 2)      // Значение больше чем 2?

                    {

                        filter = 0;         // Если так, то установим filter в 0

                    }

                }

                if (!keys['F'])       // Клавиша 'F' отжата?

                {

                    fp = FALSE;          // Если так, то fp равно FALSE

                }

                if (keys[VK_PRIOR])   // Клавиша 'Page Up' нажата?

                {

                    z -= 0.02f;          // Если так, то сдвинем вглубь экрана

                }

                if (keys[VK_NEXT])    // Клавиша 'Page Down' нажата?

                {

                    z += 0.02f;          // Если так, то придвинем к наблюдателю

                }

                if (keys[VK_UP])     // Клавиша стрелка вверх нажата?

                {

                    xspeed -= 0.01f;    // Если так, то уменьшим xspeed

                }

                if (keys[VK_DOWN])   // Клавиша стрелка вниз нажата?

                {

                    xspeed += 0.01f;    // Если так, то увеличим xspeed

                }

                if (keys[VK_RIGHT])  // Клавиша стрелка вправо нажата?

                {

                    yspeed += 0.01f;    // Если так, то увеличим yspeed

                }

                if (keys[VK_LEFT])   // Клавиша стрелка влево нажата?

                {

                    yspeed -= 0.01f;    // Если так, то уменьшим yspeed

                }

                if (keys['G'] && !gp)                   // Нажата ли клавиша "G"?

                {

                    gp = TRUE;                       // gp устанавливаем в TRUE

                    fogfilter += 1;                  // Увеличиние fogfilter на 1

                    if (fogfilter > 2)               // fogfilter больше 2 ... ?

                    {

                        fogfilter = 0;            // Если так, установить fogfilter в ноль

                    }

                    glFogi (GL_FOG_MODE, fogMode[fogfilter]); // Режим тумана

                }

                if (!keys['G'])                         // Клавиша "G" отпущена?

                {

                    gp = FALSE;                      // Если да, gp установить в FALSE

                }

                if (keys[VK_F1])          // Клавиша 'F1' нажата?

                {

                    keys[VK_F1] = FALSE;     // Если так, то сделаем Key FALSE

                    KillGLWindow();          // Уничтожим наше текущее окно

                    fullscreen = !fullscreen; // Переключение между режимами Полноэкранный/Оконный

                    // Повторное создание нашего окна OpenGL

                    if (!CreateGLWindow("Урок NeHe Текстуры, Свет & Обработка Клавиатуры", 640, 480, 16, fullscreen))

                    {

                        return 0;               // Выход, если окно не создано

                    }

                }

            }

        }

    }



    // Сброс

    KillGLWindow();              // Уничтожение окна

    return (msg.wParam);         // Выход из программы

}

