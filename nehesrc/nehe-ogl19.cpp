/*
 *      Эта программа создана Джефом Молофи (Jeff Molofee) в 2000 г.
 *      Модифицировано Shawn T. для поддержки параметров (%3.2f, num).
 *      ОГРОМНОЕ спасибо Фредерику Эхолзу (Fredric Echols) за улучшение
 *      и оптимизацию базового кода, за увеличение его гибкости!
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

bool rainbow = true; // Режим радуги? ( НОВОЕ )
bool sp; // Пробел нажат? ( НОВОЕ )
bool rp; // Ввод нажат? ( НОВОЕ)

float slowdown = 2.0f; // Торможение частиц
float xspeed; // Основная скорость по X (с клавиатуры изменяется направление хвоста)
float yspeed; // Основная скорость по Y (с клавиатуры изменяется направление хвоста)
float zoom = -40.0f; // Масштаб пучка частиц

GLuint loop;       // Переменная цикла
GLuint col;        // Текущий выбранный цвет
GLuint delay;      // Задержка для эффекта радуги
GLuint texture[1]; // Память для нашей текстуры

typedef struct { // Структура частицы
    bool active; // Активность (Да/нет)
    float life;  // Жизнь
    float fade;  // Скорость угасания
    float r; // Красное значение
    float g; // Зеленное значение
    float b; // Синие значение
    float x; // X позиция
    float y; // Y позиция
    float z; // Z позиция
    float xi; // X направление
    float yi; // Y направление
    float zi; // Z направление
    float xg; // X гравитация
    float yg; // Y гравитация
    float zg; // Z гравитация
}
particles; // Структура Частиц

particles particle[MAX_PARTICLES]; // Массив частиц (Место для информации о частицах)

static GLfloat colors[12][3] = // Цветовая радуга

{
    {1.0f, 0.5f, 0.5f}, {1.0f, 0.75f, 0.5f}, {1.0f, 1.0f, 0.5f}, {0.75f, 1.0f, 0.5f},
    {0.5f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.75f}, {0.5f, 1.0f, 1.0f}, {0.5f, 0.75f, 1.0f},
    {0.5f, 0.5f, 1.0f}, {0.75f, 0.5f, 1.0f}, {1.0f, 0.5f, 1.0f}, {1.0f, 0.5f, 0.75f}
};



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

int LoadGLTextures() {                    // Загрузка картинки и конвертирование в текстуру
    int Status = FALSE;                      // Индикатор состояния

    AUX_RGBImageRec* TextureImage[1];        // Создать место для текстуры

    memset(TextureImage, 0, sizeof(void*) * 1); // Установить указатель в NULL

    // Загрузка картинки, проверка на ошибки, если картинка не найдена - выход
    if (TextureImage[0] = LoadBMP("Data/Particle.bmp")) {
        Status = TRUE;     // Установим Status в TRUE
        glGenTextures(1, &texture[0]);     // Создание трех текстур

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
                     0, GL_RGB,   GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }

    if (TextureImage[0]) {          // Если текстура существует
        if (TextureImage[0]->data) {  // Если изображение текстуры существует
            free(TextureImage[0]->data); // Освобождение памяти изображения текстуры
        }

        free(TextureImage[0]);         // Освобождение памяти под структуру
    }

    return Status;        // Возвращаем статус
}


int InitGL(GLvoid) {      // Все начальные настройки OpenGL здесь
    if (!LoadGLTextures()) { // Переход на процедуру загрузки текстуры
        return FALSE;         // Если текстура не загружена возвращаем FALSE
    }

    glShadeModel(GL_SMOOTH);    // Разрешить плавное затенение
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Черный фон
    glClearDepth(1.0f);         // Установка буфера глубины
    glDisable(GL_DEPTH_TEST);    // Разрешение теста глубины
    glEnable(GL_BLEND);         // Разрешаем смешивание
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Тип смешивания

    // Улучшенные вычисления перспективы
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Улучшенные точечное смешение
    glEnable(GL_TEXTURE_2D);                 // Разрешение наложения текстуры
    glBindTexture(GL_TEXTURE_2D, texture[0]); // Выбор нашей текстуры

    for (loop = 0; loop < MAX_PARTICLES; loop++) { // Инициализация всех частиц
        particle[loop].active = true; // Сделать все частицы активными
        particle[loop].life = 1.0f; // Сделать все частицы с полной жизнью
        //Случайная скорость угасания
        particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;
        particle[loop].r = colors[loop * (12 / MAX_PARTICLES)][0]; // Выбор красного цвета радуги
        particle[loop].g = colors[loop * (12 / MAX_PARTICLES)][1]; // Выбор зеленного цвета радуги
        particle[loop].b = colors[loop * (12 / MAX_PARTICLES)][2]; // Выбор синего цвета радуги
        particle[loop].xi = float((rand() % 50) - 26.0f) * 10.0f; // Случайная скорость по оси X
        particle[loop].yi = float((rand() % 50) - 25.0f) * 10.0f; // Случайная скорость по оси Y
        particle[loop].zi = float((rand() % 50) - 25.0f) * 10.0f; // Случайная скорость по оси Z
        particle[loop].xg = 0.0f; // Зададим горизонтальное притяжение в ноль
        particle[loop].yg = -0.8f; // Зададим вертикальное притяжение вниз
        particle[loop].zg = 0.0f; // зададим притяжение по оси Z в ноль
    }

    return TRUE; // Инициализация завершена OK
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

int DrawGLScene(GLvoid) { // Здесь мы будем рисовать все
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка экран и буфера глубины
    glLoadIdentity(); // Сброс просмотра

    for (loop = 0; loop < MAX_PARTICLES; loop++) { // Цикл по всем частицам
        if (particle[loop].active) { // Если частицы не активны
            float x = particle[loop].x; // Захватим позицию X нашей частицы
            float y = particle[loop].y; // Захватим позицию Н нашей частицы
            float z = particle[loop].z + zoom; // Позиция частицы по Z + Zoom
            // Вывод частицы, используя наши RGB значения, угасание частицы согласно её жизни
            glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);
            glBegin(GL_TRIANGLE_STRIP); // Построение четырехугольника из треугольной полоски
            glTexCoord2d(1, 1);
            glVertex3f(x + 0.5f, y + 0.5f, z); // Верхняя правая
            glTexCoord2d(0, 1);
            glVertex3f(x - 0.5f, y + 0.5f, z); // Верхняя левая
            glTexCoord2d(1, 0);
            glVertex3f(x + 0.5f, y - 0.5f, z); // Нижняя правая
            glTexCoord2d(0, 0);
            glVertex3f(x - 0.5f, y - 0.5f, z); // Нижняя левая
            glEnd(); // Завершение построения полоски треугольников
            // Передвижение по оси X на скорость по X
            particle[loop].x += particle[loop].xi / (slowdown * 1000);
            // Передвижение по оси Y на скорость по Y
            particle[loop].y += particle[loop].yi / (slowdown * 1000);
            // Передвижение по оси Z на скорость по Z
            particle[loop].z += particle[loop].zi / (slowdown * 1000);

            particle[loop].xi += particle[loop].xg; // Притяжение по X для этой записи
            particle[loop].yi += particle[loop].yg; // Притяжение по Y для этой записи
            particle[loop].zi += particle[loop].zg; // Притяжение по Z для этой записи
            particle[loop].life -= particle[loop].fade; // Уменьшить жизнь частицы на ‘угасание’

            if (particle[loop].life < 0.0f) { // Если частица погасла
                particle[loop].life = 1.0f; // Дать новую жизнь
                // Случайное значение угасания
                particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;
                particle[loop].x = 0.0f; // На центр оси X
                particle[loop].y = 0.0f; // На центр оси Y
                particle[loop].z = 0.0f; // На центр оси Z
                particle[loop].xi = xspeed + float((rand() % 60) - 32.0f); //Скорость и направление по оси X
                particle[loop].yi = yspeed + float((rand() % 60) - 30.0f); //Скорость и направление по оси Y
                particle[loop].zi = float((rand() % 60) - 30.0f); //Скорость и направление по оси Z
                particle[loop].r = colors[col][0]; // Выбор красного из таблицы цветов
                particle[loop].g = colors[col][1]; // Выбор зеленого из таблицы цветов
                particle[loop].b = colors[col][2]; // Выбор синего из таблицы цветов
            }

            // Если клавиша 8 на цифровой клавиатуре нажата и гравитация меньше чем 1.5
            // тогда увеличим притяжение вверх
            if (keys[VK_NUMPAD8] && (particle[loop].yg < 1.5f)) {
                particle[loop].yg += 0.01f;
            }

            // Если клавиша 2 на цифровой клавиатуре нажата и гравитация больше чем -1.5
            // тогда увеличим притяжение вниз
            if (keys[VK_NUMPAD2] && (particle[loop].yg > -1.5f)) {
                particle[loop].yg -= 0.01f;
            }

            // Если клавиша 6 на цифровой клавиатуре нажата и гравитация меньше чем 1.5
            // тогда увеличим притяжение вправо
            if (keys[VK_NUMPAD6] && (particle[loop].xg < 1.5f)) {
                particle[loop].xg += 0.01f;
            }

            // Если клавиша 4 на цифровой клавиатуре нажата и гравитация больше чем -1.5
            // тогда увеличим притяжение влево
            if (keys[VK_NUMPAD4] && (particle[loop].xg > -1.5f)) {
                particle[loop].xg -= 0.01f;
            }

            if (keys[VK_TAB]) { // Клавиша табуляции вызывает взрыв
                particle[loop].x = 0.0f; // Центр по оси X
                particle[loop].y = 0.0f; // Центр по оси Y
                particle[loop].z = 0.0f; // Центр по оси Z
                particle[loop].xi = float((rand() % 50) - 26.0f) * 10.0f; // Случайная скорость по оси X
                particle[loop].yi = float((rand() % 50) - 25.0f) * 10.0f; // Случайная скорость по оси Y
                particle[loop].zi = float((rand() % 50) - 25.0f) * 10.0f; // Случайная скорость по оси Z
            }
        }
    }

    return TRUE; // Все OK
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

int WINAPI WinMain( HINSTANCE   hInstance,          // Instance
                    HINSTANCE   hPrevInstance,      // Предыдущий Instance
                    LPSTR       lpCmdLine,          // Параметры командной строки
                    int         nCmdShow) {         // Показать состояние окна
    MSG     msg;                                    // Структура сообщения окна
    BOOL    done = FALSE;                           // Булевская переменная выхода из цикла

    // Запросим пользователя какой режим отображения он предпочитает
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;                         // Оконный режим
    }

    // Создадим наше окно OpenGL
    if (!CreateGLWindow("NeHe's Particle Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                                   // Выходим если окно не было создано
    }

    if (fullscreen) { // Полноэкранный режим ( ДОБАВЛЕНО )
        slowdown = 1.0f; // Скорость частиц (для 3dfx) ( ДОБАВЛЕНО )
    }

    while(!done) {                                  // Цикл, который продолжается пока done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Есть ожидаемое сообщение?
            if (msg.message == WM_QUIT) {           // Мы получили сообщение о выходе?
                done = TRUE;                        // Если так done=TRUE
            } else {                                // Если нет,продолжаем работать с сообщениями окна
                TranslateMessage(&msg);             // Переводим сообщение
                DispatchMessage(&msg);              // Отсылаем сообщение
            }
        } else {                                    // Если сообщений нет
            // Рисуем сцену.  Ожидаем нажатия кнопки ESC и сообщения о выходе от DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) { // Активно?  Было получено сообщение о выходе?
                done = TRUE;                        // ESC или DrawGLScene просигналили "выход"
            } else {                                // Не время выходить, обновляем экран
                SwapBuffers(hDC);                   // Переключаем буферы (Double Buffering)

                if (keys[VK_ADD] && (slowdown > 1.0f)) {
                    slowdown -= 0.01f;    //Скорость частицы увеличилась
                }

                if (keys[VK_SUBTRACT] && (slowdown < 4.0f)) {
                    slowdown += 0.01f;    // Торможение частиц
                }

                if (keys[VK_PRIOR]) {
                    zoom += 0.1f;    // Крупный план
                }

                if (keys[VK_NEXT]) {
                    zoom -= 0.1f;    // Мелкий план
                }

                if (keys[VK_RETURN] && !rp) { // нажата клавиша Enter
                    rp = true; // Установка флага, что клавиша нажата
                    rainbow = !rainbow; // Переключение режима радуги в Вкл/Выкл
                }

                if (!keys[VK_RETURN]) {
                    rp = false;    // Если клавиша Enter не нажата – сбросить флаг
                }

                if ((keys[' '] && !sp) || (rainbow && (delay > 25))) { // Пробел или режим радуги
                    if (keys[' ']) {
                        rainbow = false;    // Если пробел нажат запрет режима радуги
                    }

                    sp = true; // Установка флага нам скажет, что пробел нажат
                    delay = 0; // Сброс задержки циклической смены цветов радуги
                    col++;   // Изменить цвет частицы

                    if (col > 11) {
                        col = 0;    // Если цвет выше, то сбросить его
                    }
                }

                if (!keys[' ']) {
                    sp = false;    // Если клавиша пробел не нажата, то сбросим флаг
                }

                //Если нажата клавиша вверх и скорость по Y меньше чем 200, то увеличим скорость
                if (keys[VK_UP] && (yspeed < 200)) {
                    yspeed += 1.0f;
                }

                // Если стрелка вниз и скорость по Y больше чем –200, то увеличим скорость падения
                if (keys[VK_DOWN] && (yspeed > -200)) {
                    yspeed -= 1.0f;
                }

                // Если стрелка вправо и X скорость меньше чем 200, то увеличить скорость вправо
                if (keys[VK_RIGHT] && (xspeed < 200)) {
                    xspeed += 1.0f;
                }

                // Если стрелка влево и X скорость больше чем –200, то увеличить скорость влево
                if (keys[VK_LEFT] && (xspeed > -200)) {
                    xspeed -= 1.0f;
                }

                delay++; // Увеличить счетчик задержки циклической смены цветов в режиме радуги

                if (keys[VK_F1]) {                      // Была нажата кнопка F1?
                    keys[VK_F1] = FALSE;                // Если так - установим значение FALSE
                    KillGLWindow();                     // Закроем текущее окно OpenGL
                    fullscreen = !fullscreen;           // // Переключим режим "Полный экран"/"Оконный"

                    // Заново создадим наше окно OpenGL
                    if (!CreateGLWindow("NeHe's Particle Tutorial", 640, 480, 16, fullscreen)) {
                        return 0;                       // Выйти, если окно не было создано

                    }
                }
            }
        }
    }

    // Shutdown
    KillGLWindow();                                 // Закроем окно
    return (msg.wParam);                            // Выйдем из программы
}
