#include <windows.h>      // ������������ ���� ��� Windows
#include <stdio.h>      // ������������ ���� ��� ������������ �����/������
#include <gl\gl.h>      // ������������ ���� ��� ���������� OpenGL32
#include <gl\glu.h>     // ������������ ���� ��� ��� ���������� GLu32
#include <gl\glaux.h>     // ������������ ���� ��� ���������� GLaux

static HGLRC hRC;   // ���������� �������� ����������
static HDC hDC;     // ��������� �������� ���������� GDI

bool  keys[256];         // ������, �������������� ��� ���������� ��������� ����������
bool  active = TRUE;     // ���� ��������� ���������� ���������� (�� ���������: TRUE)
bool  fullscreen = TRUE; // ���� �������������� ������ (�� ���������: �������������)

BOOL  twinkle;      // Twinkling Stars (����������� ������)
BOOL  tp;       // 'T' ������� ������?

const int num = 50;   // ���������� �������� �����

typedef struct {      // ������ ��������� ��� �����
    int r, g, b;      // ���� ������
    GLfloat dist;     // ���������� �� ������
    GLfloat angle;    // ������� ���� ������
}
stars;          // ��� ��������� - Stars
stars star[num];      // ������ ������ 'star' ������� 'num',
// ��� ��������� �������� ��������� 'stars'

GLfloat zoom = -15.0f;    // ���������� �� ����������� �� �����
GLfloat tilt = 90.0f;   // ��������� ����
GLfloat spin;       // ��� �������� �����

GLuint  loop;       // ������������ ��� ������
GLuint  texture[1];     // ������ ��� ����� ��������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // ���������� ��� WndProc

AUX_RGBImageRec* LoadBMP(char* Filename) { // ������� ��� �������� bmp ������
    FILE* File = NULL;  // ���������� ��� �����

    if (!Filename) {    // ����� ��������� � ������������ ���������� �����
        return NULL;    // ���� ������������ ���, �� ���������� NULL
    }

    File = fopen(Filename, "r"); // ��������� � ��������� �� ������� �����

    if (File) {   // ���� ����������?
        fclose(File);   // ���� ��, �� ��������� ����
        // � ��������� ��� � ������� ���������� AUX, �������� ������ �� �����������
        return auxDIBImageLoad(Filename);
    }

    // ���� ��������� �� ������� ��� ���� �� ������, �� ��������� NULL
    return NULL;
}

int LoadGLTextures() { // ������� �������� ����������� � ��������������� � ��������
    int Status = FALSE; // ��������� �������

    AUX_RGBImageRec* TextureImage[1];// ������ ����� ��� �������� ��������

    memset(TextureImage, 0, sizeof(void*) * 1); // ������������� ������ �� NULL

    // ��������� �����������, ��������� �� ������, ���� ���� �� ������ �� �������
    if (TextureImage[0] = LoadBMP("Data/Star.bmp")) {
        Status = TRUE;  // ������ ������ � TRUE

        glGenTextures(1, &texture[0]);  // ���������� ���� ����������� ��������

        // ������ �������� � �������� ����������� (Linear Filtered)
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX,
                     TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }

    if (TextureImage[0]) {  // ���� �������� ����������
        if (TextureImage[0]->data) { // ���� ����������� ����������
            // ����������� ����� ���������� ��� �����������
            free(TextureImage[0]->data);
        }

        free(TextureImage[0]);  // ����������� ��������� �����������
    }

    return Status;      // ���������� ������
}

GLvoid InitGL(GLsizei Width, GLsizei Height) { // ������� ����� �������� ���� GL
    LoadGLTextures();     // �������� �������
    glEnable(GL_TEXTURE_2D);  // ���������� ��������� ��������

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // ������� ������ � ������ ����
    glClearDepth(1.0);    // ��������� ������� ������ �������
    glDepthFunc(GL_LESS); // ��� ����� �������
    glEnable(GL_DEPTH_TEST);// ��������� ���� �������
    glShadeModel(GL_SMOOTH);// ��������� ������� �������� �����������
    glMatrixMode(GL_PROJECTION);// ����� ������� ��������
    glLoadIdentity();   // ����� ������� ��������
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // ��������� ����������� �������������� �������� ��� ����
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // ������� ��������� �����������
    glDepthFunc(GL_LEQUAL); // ��� ����������� �������� �������
    // ������������� ������� ����������
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);     // �������� ����������
    glMatrixMode(GL_MODELVIEW);// ����� ������� ��������� ������

    for (loop = 0; loop < num; loop++) { // ������ ���� � ����� �� ���� �������
        star[loop].angle = 0.0f; // ������������� �� ���� � 0
        // ��������� ��������� �� ������
        star[loop].dist = (float(loop) / num) * 5.0f;
        // ����������� star[loop] ��������� �������� (�������).
        star[loop].r = rand() % 256;
        // ����������� star[loop] ��������� �������� (�������)
        star[loop].g = rand() % 256;
        // ����������� star[loop] ��������� �������� (�������)
        star[loop].b = rand() % 256;
    }
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
    if (Height == 0) { // �������������� ������� �� ����, ���� ���� ������� ����
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    // ����� ������� ������� ������ � ������������� ��������������

    glMatrixMode(GL_PROJECTION);// ����� ������� ��������
    glLoadIdentity();     // ����� ������� ��������

    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // ���������� ����������� �������������� �������� ��� ����
    glMatrixMode(GL_MODELVIEW); // ����� ������� ��������� ������
}

int DrawGLScene(GLvoid) {   // ����� �� �� ������
    // ������� ����� ����� � �������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // �������� ���� ��������
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    for (loop = 0; loop < num; loop++) { // ���� �� ���� �������
        // �������� ������� ������� (Model Matrix) ����� ������ �������
        glLoadIdentity();
        // ��������� �� ��� z �� 'zoom'
        glTranslatef(0.0f, 0.0f, zoom);
        // ������� ������ ��� x �� ���� 'tilt'
        glRotatef(tilt, 1.0f, 0.0f, 0.0f);
        // ������������ �� ���� ������ ������ ��� y
        glRotatef(star[loop].angle, 0.0f, 1.0f, 0.0f);
        // ��������� ������ �� ��� x
        glTranslatef(star[loop].dist, 0.0f, 0.0f);
        glRotatef(-star[loop].angle, 0.0f, 1.0f, 0.0f);
        // �������� ������� ������� ������
        glRotatef(-tilt, 1.0f, 0.0f, 0.0f); // �������� ������� ������

        if (twinkle) {      // ���� Twinkling �������
            // ������ ���� ���������� �����
            glColor4ub(star[(num - loop) - 1].r, star[(num - loop) - 1].g,
                       star[(num - loop) - 1].b, 255);
            glBegin(GL_QUADS);// �������� �������� ��������������� �������
            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(-1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f( 1.0f, -1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3f( 1.0f, 1.0f, 0.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(-1.0f, 1.0f, 0.0f);
            glEnd();        // ��������� ��������
        }

        glRotatef(spin, 0.0f, 0.0f, 1.0f); // ������������ ������ �� ��� z
        // ���� ���������� �����
        glColor4ub(star[loop].r, star[loop].g, star[loop].b, 255);
        glBegin(GL_QUADS);    // �������� �������� ���������� �������
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 1.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glEnd();          // ��������� ��������
        spin += 0.01f;    // ������������ ��� �������� ������
        star[loop].angle += float(loop) / num; // ������ ���� ������
        star[loop].dist -= 0.01f; // ������ ��������� �� ������

        if (star[loop].dist < 0.0f) { // ������ � ������ ������?
            star[loop].dist += 5.0f; // ���������� �� 5 ������ �� ������
            // ����� �������� ������� ���������� �����
            star[loop].r = rand() % 256;
            // ����� �������� �������� ���������� �����
            star[loop].g = rand() % 256;
            // ����� �������� ����� ���������� �����
            star[loop].b = rand() % 256;
        }
    }

    return TRUE;            // �� ��
}


LRESULT CALLBACK WndProc(
    HWND  hWnd,
    UINT  message,
    WPARAM  wParam,
    LPARAM  lParam) {
    RECT  Screen;   // ������������ ������� ��� �������� ����
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���� ���������
        1,        // ����� ������ (?)
        PFD_DRAW_TO_WINDOW |// ������ ��� ����
        PFD_SUPPORT_OPENGL |// ������ ��� OpenGL
        PFD_DOUBLEBUFFER,// ������ ��� �������� ������
        PFD_TYPE_RGBA,  // ��������� RGBA ������
        16,       // ����� 16 ��� ������� �����
        0, 0, 0, 0, 0, 0,// ������������� �������� ����� (?)
        0,        // ��� ������ ������������
        0,        // ��������� ��� ������������ (?)
        0,        // ��� ������ �����������
        0, 0, 0, 0,   // ���� ����������� ������������ (?)
        16,       // 16 ������ Z-����� (����� �������)
        0,        // ��� ������ ����������
        0,        // ��� ��������������� ������� (?)
        PFD_MAIN_PLANE, // ������� ���� ���������
        0,        // ������ (?)
        0, 0, 0     // ����� ���� ������������ (?)
    };

    switch (message) { // ��� ���������
        case WM_CREATE:
            hDC = GetDC(hWnd);  // �������� �������� ���������� ��� ����
            PixelFormat = ChoosePixelFormat(hDC, &pfd);

            // ����� ��������� ���������� ��� ������ ������� ��������
            if (!PixelFormat) {
                MessageBox(0, "Can't Find A Suitable PixelFormat.", "Error", MB_OK | MB_ICONERROR);
                PostQuitMessage(0);
                // ��� ��������� �������, ��� ��������� ������ ����������
                break;  // �������������� ������� ����
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
    MSG   msg;  // ��������� ��������� Windows
    WNDCLASS  wc; // ��������� ������ Windows ��� ��������� ���� ����
    HWND    hWnd;// ���������� ����������� ����

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
               "Jeff Molofee's GL Code Tutorial ... NeHe '99", // ��������� ������ ����

               WS_POPUP |
               WS_CLIPCHILDREN |
               WS_CLIPSIBLINGS,

               0, 0,     // ������� ���� �� ������
               640, 480,   // ������ � ������ ����

               NULL,
               NULL,
               hInstance,
               NULL);

    if(!hWnd) {
        MessageBox(0, "Window Creation Error.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    DEVMODE dmScreenSettings;     // ����� ������

    memset(&dmScreenSettings, 0, sizeof(DEVMODE));  // ������� ��� �������� ���������
    dmScreenSettings.dmSize = sizeof(DEVMODE);    // ������ ��������� Devmode
    dmScreenSettings.dmPelsWidth  = 640;      // ������ ������
    dmScreenSettings.dmPelsHeight = 480;      // ������ ������
    dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT; // ����� �������
    ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
    // ������������ � ������ �����

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

    while (1) {
        // ��������� ���� ���������
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
            if (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                return TRUE;
            }
        }

        DrawGLScene();        // ���������� �����
        SwapBuffers(hDC);     // ����������� ����� ������

        if (keys['T'] && !tp) { // ���� 'T' ������ � tp ����� FALSE
            tp = TRUE;  // �� ������ tp ������ TRUE
            twinkle = !twinkle; // ������ �������� twinkle �� ��������
        }

        if (!keys['T']) {   // ������� 'T' ���� ���������
            tp = FALSE; // ������ tp ������ FALSE
        }

        if (keys[VK_UP]) {  // ������� ����� ���� ������?
            tilt -= 0.5f; // ������� ����� �����
        }

        if (keys[VK_DOWN]) {  // ������� ���� ������?
            tilt += 0.5f; // ������� ����� ����
        }

        if (keys[VK_PRIOR]) { // Page Up �����?
            zoom -= 0.2f; // ���������
        }

        if (keys[VK_NEXT]) {  // Page Down ������?
            zoom += 0.2f; // �����������
        }

        if (keys[VK_ESCAPE]) {
            SendMessage(hWnd, WM_CLOSE, 0, 0);    // ���� ESC - �����
        }
    }
}

