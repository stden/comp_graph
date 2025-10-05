#include <windows.h>        // ������������ ���� ��� Windows
#include <gl\gl.h>          // ������������ ���� ��� OpenGL32 ����������
#include <gl\glu.h>         // ������������ ���� ��� GLu32 ����������
#include <gl\glaux.h>       // ������������ ���� ��� GLaux ����������

static HGLRC hRC;       // ���������� �������� ����������
static HDC hDC;         // ��������� �������� ���������� GDI

BOOL    keys[256];      // ������ ��� ��������� ��������� ����������

GLvoid InitGL(GLsizei Width, GLsizei Height) {  // ������� ����� �������� ���� GL
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // ������� ������ � ������ ����
    glClearDepth(1.0);      // ��������� ������� ������ �������
    glDepthFunc(GL_LESS);   // ��� ����� �������
    glEnable(GL_DEPTH_TEST);// ��������� ���� �������
    glShadeModel(GL_SMOOTH);// ��������� ������� �������� �����������
    glMatrixMode(GL_PROJECTION);// ����� ������� ��������
    glLoadIdentity();       // ����� ������� ��������
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // ��������� ����������� �������������� �������� ��� ����
    glMatrixMode(GL_MODELVIEW);// ����� ������� ��������� ������
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
    if (Height == 0) {  // �������������� ������� �� ����, ���� ���� ������� ����
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    // ����� ������� ������� ������ � ������������� ��������������

    glMatrixMode(GL_PROJECTION);// ����� ������� ��������
    glLoadIdentity();           // ����� ������� ��������

    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // ���������� ����������� �������������� �������� ��� ����
    glMatrixMode(GL_MODELVIEW); // ����� ������� ��������� ������
}

GLvoid DrawGLScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ������� ������ � ������ �������
    glLoadIdentity();
    // ����� ���������
    glTranslatef(-1.5f, 0.0f, -6.0f); // ��������� ����� �� 1.5 ������� �
    // � ����� �� 6.0
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // ������� ����
    glVertex3f( 0.0f, 1.0f, 0.0f);  // �����
    glColor3f(0.0f, 1.0f, 0.0f); // �������� ����
    glVertex3f(-1.0f, -1.0f, 0.0f); // ����� �����
    glColor3f(0.0f, 0.0f, 1.0f); // ����� ����
    glVertex3f( 1.0f, -1.0f, 0.0f); // ������ �����
    glEnd();

    glTranslatef(3.0f, 0.0f, 0.0f);     // ������� ������ �� 3 �������
    glColor3f(0.5f, 0.5f, 1.0f); // ��������� ����� ���� ������ ���� ���
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, 0.0f);  // ����� ������
    glVertex3f( 1.0f, 1.0f, 0.0f);  // ������ ������
    glVertex3f( 1.0f, -1.0f, 0.0f); // ������ �����
    glVertex3f(-1.0f, -1.0f, 0.0f); // ����� �����
    glEnd();

}

LRESULT CALLBACK WndProc(
    HWND    hWnd,
    UINT    message,
    WPARAM  wParam,
    LPARAM  lParam) {
    RECT    Screen;     // ������������ ������� ��� �������� ����
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���� ���������
        1,              // ����� ������ (?)
        PFD_DRAW_TO_WINDOW |// ������ ��� ����
        PFD_SUPPORT_OPENGL |// ������ ��� OpenGL
        PFD_DOUBLEBUFFER,// ������ ��� �������� ������
        PFD_TYPE_RGBA,  // ��������� RGBA ������
        16,             // ����� 16 ��� ������� �����
        0, 0, 0, 0, 0, 0,// ������������� �������� ����� (?)
        0,              // ��� ������ ������������
        0,              // ��������� ��� ������������ (?)
        0,              // ��� ������ �����������
        0, 0, 0, 0,     // ���� ����������� ������������ (?)
        16,             // 16 ������ Z-����� (����� �������)
        0,              // ��� ������ ����������
        0,              // ��� ��������������� ������� (?)
        PFD_MAIN_PLANE, // ������� ���� ���������
        0,              // ������ (?)
        0, 0, 0         // ����� ���� ������������ (?)
    };

    switch (message) {  // ��� ���������
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
    MSG     msg;    // ��������� ��������� Windows
    WNDCLASS    wc; // ��������� ������ Windows ��� ��������� ���� ����
    HWND        hWnd;// ���������� ����������� ����

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
               "Jeff Molofee's GL Code Tutorial ... NeHe '99", // ��������� ������ ����

               WS_POPUP |
               WS_CLIPCHILDREN |
               WS_CLIPSIBLINGS,

               0, 0,           // ������� ���� �� ������
               640, 480,       // ������ � ������ ����

               NULL,
               NULL,
               hInstance,
               NULL);

    if(!hWnd) {
        MessageBox(0, "Window Creation Error.", "Error", MB_OK | MB_ICONERROR);
        return FALSE;
    }

    DEVMODE dmScreenSettings;           // ����� ������

    memset(&dmScreenSettings, 0, sizeof(DEVMODE));  // ������� ��� �������� ���������
    dmScreenSettings.dmSize = sizeof(DEVMODE);      // ������ ��������� Devmode
    dmScreenSettings.dmPelsWidth    = 640;          // ������ ������
    dmScreenSettings.dmPelsHeight   = 480;          // ������ ������
    dmScreenSettings.dmFields   = DM_PELSWIDTH | DM_PELSHEIGHT; // ����� �������
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

        DrawGLScene();              // ���������� �����
        SwapBuffers(hDC);           // ����������� ����� ������

        if (keys[VK_ESCAPE]) {
            SendMessage(hWnd, WM_CLOSE, 0, 0);    // ���� ESC - �����
        }
    }
}

