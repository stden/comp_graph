#include <windows.h>        // Заголовочный файл для Windows
#include "..\gl.h"      // Заголовочный файл для OpenGL32 библиотеки
#include "..\glu.h"     // Заголовочный файл для GLu32 библиотеки
#include "..\glaux.h"   // Заголовочный файл для GLaux библиотеки

static HGLRC hRC;       // Постоянный контекст рендеринга
static HDC hDC;         // Приватный контекст устройства GDI

BOOL    keys[256];      // Массив для процедуры обработки клавиатуры

GLvoid InitGL(GLsizei Width, GLsizei Height) {  // Вызвать после создания окна GL
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
    // Очистка экрана в черный цвет
    glClearDepth(1.0);      // Разрешить очистку буфера глубины
    glDepthFunc(GL_LESS);   // Тип теста глубины
    glEnable(GL_DEPTH_TEST);// разрешить тест глубины
    glShadeModel(GL_SMOOTH);// разрешить плавное цветовое сглаживание
    glMatrixMode(GL_PROJECTION);// Выбор матрицы проекции
    glLoadIdentity();       // Сброс матрицы проекции
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // Вычислить соотношение геометрических размеров для окна
    glMatrixMode(GL_MODELVIEW);// Выбор матрицы просмотра модели
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

GLvoid DrawGLScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // очистка Экрана и буфера глубины
    glLoadIdentity();
    // Сброс просмотра
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
                MessageBox(0, L"Can't Find A Suitable PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                // Это сообщение говорит, что программа должна завершится
                break;  // Предтовращение повтора кода
            }

            if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {
                MessageBox(0, L"Can't Set The PixelFormat.", L"Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            hRC = wglCreateContext(hDC);

            if(!hRC) {
                MessageBox(0, L"Can't Create A GL Rendering Context.", L"Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                break;
            }

            if(!wglMakeCurrent(hDC, hRC)) {
                MessageBox(0, L"Can't activate GLRC.", L"Error", MB_OK | MB_ICONERROR);
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

int __stdcall WINAPI WinMain(
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
    wc.lpszClassName    = L"OpenGL WinClass";

    if(!RegisterClass(&wc)) {
        MessageBox(0, L"Failed To Register The Window Class.", L"Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    hWnd = CreateWindow(
               L"OpenGL WinClass",
               L"Jeff Molofee's GL Code Tutorial ... NeHe '99",    // Заголовок вверху окна

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
        MessageBox(0, L"Window Creation Error.", L"Error", MB_OK | MB_ICONERROR);
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
    }
}

