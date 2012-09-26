#include <windows.h>  // Заголовочный файл для Windows
#include <stdio.h>    // Заголовочный файл для стандартного ввода/вывода (ДОБАВИЛИ)
#include <gl\gl.h>    // Заголовочный файл для библиотеки OpenGL32
#include <gl\glu.h>   // Заголовочный файл для для библиотеки GLu32
#include <gl\glaux.h> // Заголовочный файл для библиотеки GLaux


HDC    hDC = NULL;    // Служебный контекст GDI устройства
HGLRC  hRC = NULL;    // Постоянный контекст для визуализации
HWND   hWnd = NULL;   // Содержит дискриптор для окна
HINSTANCE hInstance;  // Содержит данные для нашей программы


bool keys[256];       // Массив, использующийся для сохранения состояния клавиатуры
bool active = TRUE;   // Флаг состояния активности приложения (по умолчанию: TRUE)
bool fullscreen = TRUE; // Флаг полноэкранного режима (по умолчанию: полноэкранное)


BOOL light;      // Свет ВКЛ / ВЫКЛ
BOOL lp;         // L нажата?
BOOL fp;         // F нажата?


GLfloat xrot;         // X вращение
GLfloat yrot;         // Y вращение
GLfloat xspeed;       // X скорость вращения
GLfloat yspeed;       // Y скорость вращения

GLfloat z = -5.0f;    // Сдвиг вглубь экрана

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Значения фонового света ( НОВОЕ )
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света ( НОВОЕ )
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };    // Позиция света ( НОВОЕ )

GLuint filter;         // Какой фильтр использовать
GLuint texture[3];     // Место для хранения 3 текстур

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);    // Декларация WndProc


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


int LoadGLTextures() {                    // Загрузка картинки и конвертирование в текстуру
    int Status = FALSE;                      // Индикатор состояния

    AUX_RGBImageRec* TextureImage[1];        // Создать место для текстуры

    memset(TextureImage, 0, sizeof(void*) * 1); // Установить указатель в NULL

    // Загрузка картинки, проверка на ошибки, если картинка не найдена - выход
    if (TextureImage[0] = LoadBMP("Data/Crate.bmp")) {
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


    if (TextureImage[0]) {         // Если текстура существует
        if (TextureImage[0]->data) {  // Если изображение текстуры существует
            free(TextureImage[0]->data); // Освобождение памяти изображения текстуры
        }

        free(TextureImage[0]);        // Освобождение памяти под структуру
    }

    return Status;        // Возвращаем статус
}


GLvoid InitGL(GLsizei Width, GLsizei Height) {  // Все настройки для OpenGL делаются здесь
    if (!LoadGLTextures()) {      // Переход на процедуру загрузки текстуры
        return;                // Если текстура не загружена возвращаем FALSE
    }

    glEnable(GL_TEXTURE_2D);      // Разрешить наложение текстуры
    glShadeModel(GL_SMOOTH);      // Разрешение сглаженного закрашивания
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Черный фон
    glClearDepth(1.0f);           // Установка буфера глубины
    glEnable(GL_DEPTH_TEST);      // Разрешить тест глубины
    glDepthFunc(GL_LEQUAL);       // Тип теста глубины
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Улучшенные вычисления перспективы


    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // Установка Фонового Света
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // Установка Диффузного Света
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   // Позиция света

    glEnable(GL_LIGHT1); // Разрешение источника света номер один

    glMatrixMode(GL_PROJECTION);// Выбор матрицы проекции
    glLoadIdentity();      // Сброс матрицы проекции
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // Вычислить соотношение геометрических размеров для окна
    glMatrixMode(GL_MODELVIEW);// Выбор матрицы просмотра модели
    return;         // Инициализация прошла OK
}


int DrawGLScene(GLvoid) {      // Здесь мы делаем все рисование
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


GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
    if (Height == 0) {  // Предотвращение деления на ноль, если окно слишком мало
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    // Сброс текущей области вывода и перспективных преобразований

    glMatrixMode(GL_PROJECTION);// Выбор матрицы проекций
    glLoadIdentity();           // Сброс матрицы проекции

    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // Вычисление соотношения геометрических размеров для окна
    glMatrixMode(GL_MODELVIEW); // Выбор матрицы просмотра модели
}


LRESULT CALLBACK WndProc(
    HWND    hWnd,
    UINT    message,
    WPARAM  wParam,
    LPARAM  lParam) {
    RECT    Screen;     // используется позднее для размеров окна
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // Размер этой структуры
        1,              // Номер версии (?)
        PFD_DRAW_TO_WINDOW |// Формат для Окна
        PFD_SUPPORT_OPENGL |// Формат для OpenGL
        PFD_DOUBLEBUFFER,// Формат для двойного буфера
        PFD_TYPE_RGBA,  // Требуется RGBA формат
        16,             // Выбор 16 бит глубины цвета
        0, 0, 0, 0, 0, 0,// Игнорирование цветовых битов (?)
        0,              // нет буфера прозрачности
        0,              // Сдвиговый бит игнорируется (?)
        0,              // Нет буфера аккумуляции
        0, 0, 0, 0,     // Биты аккумуляции игнорируются (?)
        16,             // 16 битный Z-буфер (буфер глубины)
        0,              // Нет буфера траффарета
        0,              // Нет вспомогательных буферов (?)
        PFD_MAIN_PLANE, // Главный слой рисования
        0,              // Резерв (?)
        0, 0, 0         // Маски слоя игнорируются (?)
    };

    switch (message) {  // Тип сообщения
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
    MSG     msg;    // Структура сообщения Windows
    WNDCLASS    wc; // Структура класса Windows для установки типа окна
    HWND        hWnd;// Сохранение дискриптора окна

    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = NULL;
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "OpenGL WinClass";

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

               0, 0,           // Позиция окна на экране
               640, 480,       // Ширина и высота окна

               NULL,
               NULL,
               hInstance,
               NULL);

    if(!hWnd) {
        MessageBox(0, "Window Creation Error.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    DEVMODE dmScreenSettings;           // Режим работы

    memset(&dmScreenSettings, 0, sizeof(DEVMODE));  // Очистка для хранения установок
    dmScreenSettings.dmSize = sizeof(DEVMODE);      // Размер структуры Devmode
    dmScreenSettings.dmPelsWidth    = 640;          // Ширина экрана
    dmScreenSettings.dmPelsHeight   = 480;          // Высота экрана
    dmScreenSettings.dmFields   = DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пиксела
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

        DrawGLScene();              // Нарисовать сцену
        SwapBuffers(hDC);           // Переключить буфер экрана

        if (keys[VK_ESCAPE]) {
            SendMessage(hWnd, WM_CLOSE, 0, 0);    // Если ESC - выйти
        }

        if (keys['L'] && !lp) { // Клавиша 'L' нажата и не удерживается?
            lp = TRUE;    // lp присвоили TRUE
            light = !light; // Переключение света TRUE/FALSE

            if (!light) {             // Если не свет
                glDisable(GL_LIGHTING);  // Запрет освещения
            } else {                  // В противном случае
                glEnable(GL_LIGHTING);   // Разрешить освещение
            }
        }

        if (!keys['L']) { // Клавиша 'L' Отжата?
            lp = FALSE;    // Если так, то lp равно FALSE
        }

        if (keys['F'] && !fp) { // Клавиша 'F' нажата?
            fp = TRUE;           // fp равно TRUE
            filter += 1;         // значение filter увеличивается на один

            if (filter > 2) {    // Значение больше чем 2?
                filter = 0;         // Если так, то установим filter в 0
            }
        }

        if (!keys['F']) {     // Клавиша 'F' отжата?
            fp = FALSE;          // Если так, то fp равно FALSE
        }

        if (keys[VK_PRIOR]) { // Клавиша 'Page Up' нажата?
            z -= 0.02f;          // Если так, то сдвинем вглубь экрана
        }

        if (keys[VK_NEXT]) {  // Клавиша 'Page Down' нажата?
            z += 0.02f;          // Если так, то придвинем к наблюдателю
        }

        if (keys[VK_UP]) {   // Клавиша стрелка вверх нажата?
            xspeed -= 0.01f;    // Если так, то уменьшим xspeed
        }

        if (keys[VK_DOWN]) { // Клавиша стрелка вниз нажата?
            xspeed += 0.01f;    // Если так, то увеличим xspeed
        }

        if (keys[VK_RIGHT]) { // Клавиша стрелка вправо нажата?
            yspeed += 0.01f;    // Если так, то увеличим yspeed
        }

        if (keys[VK_LEFT]) { // Клавиша стрелка влево нажата?
            yspeed -= 0.01f;    // Если так, то уменьшим yspeed
        }

        if (keys['S']) {     // Стоп вращение
            yspeed = 0.0f;
            xspeed = 0.0f;
            xrot = 0.0f;
            yrot = 0.0f;
        }

        if (keys['M']) {     // Стоп вращение
            filter = 2;
        }
    }
}

