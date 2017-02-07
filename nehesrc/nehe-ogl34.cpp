/*
 *      ���� ��� ��� ������ ����� ������ (Ben Humphrey) 2001
 *      ���� �� ������� ���� ��� ��������, ����� ��� �����.
 *      �������� ���� NeHe: http://nehe.gamedev.net
 */

#include <windows.h>            // ������������ ���� ��� Windows

#include <stdio.h>          // ������������ ���� ������������ �����/������ (�����)
#include <gl\gl.h>          // ������������ ���� ���������� OpenGL32
#include <gl\glu.h>         // ������������ ���� ���������� GLu32
#include <gl\glaux.h>           // ������������ ���� ���������� GLaux

#pragma comment(lib, "opengl32.lib")                // ������ �� OpenGL32.lib
#pragma comment(lib, "glu32.lib")               // ������ �� Glu32.lib

#define     MAP_SIZE    1024            // ������ ����� ������ (�����)
#define     STEP_SIZE   16          // ������ � ������ ������� �������� (�����)
#define     HEIGHT_RATIO    1.5f        // ����������� ��������������� �� ��� Y � ������������ � ����� X � Z (�����)

HDC     hDC = NULL;             // ��������� �������� ���������� GDI
HGLRC       hRC = NULL;             // ���������� �������� ����������
HWND        hWnd = NULL;            // ��������� �� ���� ����
HINSTANCE   hInstance;              // ��������� �� ���������� �������� ����������

bool        keys[256];              // ������ ��� ��������� ��������� ����������
bool        active = TRUE;              // ���� ���������� ����, �� ���������=TRUE
bool        fullscreen = TRUE;          // ���� �������������� ������, �� ���������=TRUE
bool        bRender = TRUE;         // ���� ������ ����������� ���������, �� ���������=TRUE (�����)

BYTE g_HeightMap[MAP_SIZE* MAP_SIZE];       // �������� ����� ������ (�����)

float scaleValue = 0.15f;                   // �������� ��������������� ����������� (�����)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // ���������� ��� WndProc

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height) {
    if (Height == 0) {  // �������������� ������� �� ����, ���� ���� ������� ����
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    // ����� ������� ������� ������ � ������������� ��������������

    glMatrixMode(GL_PROJECTION);// ����� ������� ��������
    glLoadIdentity();           // ����� ������� ��������

    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 500.0f);
    // ���������� ����������� �������������� �������� ��� ����
    glMatrixMode(GL_MODELVIEW); // ����� ������� ��������� ������
}


// ������ � ���������� .RAW ����� � pHeightMap
void LoadRawFile(LPSTR strName, int nSize, BYTE* pHeightMap) {
    FILE* pFile = NULL;

    // �������� ����� � ������ ��������� ������
    pFile = fopen( strName, "rb" );

    // ���� ������?
    if ( pFile == NULL ) {
        // ������� ��������� �� ������ � ������� �� ���������
        MessageBox(NULL, "Can't Find The Height Map!", "Error", MB_OK);
        return;
    }

    // ��������� .RAW ���� � ������ pHeightMap
    // ������ ��� ������ �� ������ �����, ������ = ������ * ������
    fread( pHeightMap, 1, nSize, pFile );

    // ��������� �� ������� ������
    int result = ferror( pFile );

    // ���� ��������� ������
    if (result) {
        MessageBox(NULL, "Failed To Get Data!", "Error", MB_OK);
    }

    // ��������� ����
    fclose(pFile);
}

int InitGL(GLvoid) {                    // ������������� OpenGL
    glShadeModel(GL_SMOOTH);            // �������� �����������
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);   // ������� ������ ������ ������
    glClearDepth(1.0f);             // ��������� ������ �������
    glEnable(GL_DEPTH_TEST);            // �������� ����� �������
    glDepthFunc(GL_LEQUAL);         // ��� ����� �������
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // ���������� ���������� �����������

    // ������ ������ �� ����� � ��������� �� � ������� g_HeightMap array.
    // ����� �������� ������ ����� (1024).

    LoadRawFile("Data/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);  // ( NEW )

    return TRUE;                        // ������������� ������ �������
}


int Height(BYTE* pHeightMap, int X, int Y) {        // ���������� ������ �� ����� ������ (?)
    int x = X % MAP_SIZE;                   // �������� ���������� �
    int y = Y % MAP_SIZE;                   // �������� ���������� y

    if(!pHeightMap) {
        return 0;    // ��������, ��� ������ ���������
    }

    return pHeightMap[x + (y * MAP_SIZE)];          // ���������� �������� ������
}

void SetVertexColor(BYTE* pHeightMap, int x, int y) {   // ��� ������� ������������� �������� ����� ��� ����������� ������,
    // ���������� �� ������ ������
    if(!pHeightMap) {
        return;    // ������ ���������?
    }

    float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);

    // ��������� ������� ������ ����� ��� ���������� �����
    glColor3f(0.0f, 0.0f, fColor );
}

void RenderHeightMap(BYTE pHeightMap[]) {           // ��������� ����� ������ � ������� ���������
    int X = 0, Y = 0;                   // ������� ���� ���������� ��� ����������� �� �������
    int x, y, z;                        // � ��� ��� ��� �������� ������

    if(!pHeightMap) {
        return;    // ������ ���������?
    }

    if(bRender) {                   // ��� ����� ���������?
        glBegin( GL_QUADS );    // ��������
    } else {
        glBegin( GL_LINES );    // �����
    }

    for ( X = 0; X < MAP_SIZE; X += STEP_SIZE )
        for ( Y = 0; Y < MAP_SIZE; Y += STEP_SIZE ) {
            // �������� (X, Y, Z) ���������� ������ ����� �������
            x = X;
            y = Height(pHeightMap, X, Y );
            z = Y;

            // ������������� ���� ���������� �����
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // �������� ��

            // �������� (X, Y, Z) ���������� ������� ����� �������
            x = X;
            y = Height(pHeightMap, X, Y + STEP_SIZE );
            z = Y + STEP_SIZE ;

            // ������������� ���� ���������� �����
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // �������� ��

            // �������� (X, Y, Z) ���������� ������� ������ �������
            x = X + STEP_SIZE;
            y = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE );
            z = Y + STEP_SIZE ;

            // ������������� ���� ���������� �����
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // �������� ��

            // �������� (X, Y, Z) ���������� ������ ������ �������
            x = X + STEP_SIZE;
            y = Height(pHeightMap, X + STEP_SIZE, Y );
            z = Y;

            // ������������� ���� ���������� �����
            SetVertexColor(pHeightMap, x, z);

            glVertex3i(x, y, z);            // �������� ��
        }

    glEnd();

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);          // ���������� ����
}

int DrawGLScene(GLvoid) {                   // ����� ���������� ��� ���������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������ � ������ �������
    glLoadIdentity();                   // ����� ���������

    //    ���������  ���        ������ ���������
    gluLookAt(212, 60, 194,  186, 55, 171,  0, 1, 0);   // ���������� ��� � ��������� ������

    glScalef(scaleValue, scaleValue * HEIGHT_RATIO, scaleValue);

    RenderHeightMap(g_HeightMap);               // ��������� ����� ������

    return TRUE;                        // ���� ������
}

GLvoid KillGLWindow(GLvoid) {                           // ������� ������� ���������� ����
    if (fullscreen) {                                   // �� � ����� �������� �������?
        ChangeDisplaySettings(NULL, 0);                 // ���� ��, �� ������������ �� ������� ����
        ShowCursor(TRUE);                               // ���������� ������ ����
    }

    if (hRC) {                                          // �������� ����������� ��� ����?
        if (!wglMakeCurrent(NULL, NULL)) {              // �� ����� ���������� RC � DC ���������?
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC)) {                   // �� ����� ������� RC?
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        hRC = NULL;                                     // ������������� RC � NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC)) {                 // �� ����� ����������� �������� DC
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                     // ������������� DC � NULL
    }

    if (hWnd && !DestroyWindow(hWnd)) {                 // �� ����� ���������� ����?
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                    // ������������� hWnd � NULL
    }

    if (!UnregisterClass("OpenGL", hInstance)) {        // ����� ����������������� �����
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                               // ������������� hInstance � NULL
    }
}

/*  ��� ����� ��������� ������� ���� ���� OpenGL.  ���������:                           *
 *  title           - ���������, ������������ ������� ����                              *
 *  width           - ������ GL ���� ��� �������������� ������                          *
 *  height          - ������ GL ���� ��� �������������� ������                          *
 *  bits            - ���������� ��� �� ���� (8/16/24/32)                               *
 *  fullscreenflag  - ������������� �������������� (TRUE) ��� ��������(FALSE) �������   */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {
    GLuint      PixelFormat;            // �������� ���������� ������� ������� �����
    WNDCLASS    wc;                     // ��������� ������� Windows
    DWORD       dwExStyle;              // ����������� ����� ����
    DWORD       dwStyle;                // ����� ����
    RECT        WindowRect;             // �������� �������� �������� ������ � ������� ������� ����� ��������������
    WindowRect.left = (long)0;          // ������������� �������� ���� (Left) � 0
    WindowRect.right = (long)width;     // ������������� �������� ����� (Right) � ��������� ������ (Width)
    WindowRect.top = (long)0;           // ������������� �������� ���� (Top) � 0
    WindowRect.bottom = (long)height;   // ������������� �������� ��� (Bottom) � ��������� ������ (Height)

    fullscreen = fullscreenflag;        // ������������� ���������� ���� Fullscreen

    hInstance           = GetModuleHandle(NULL);                // ����������� Instance ��� ������ ����
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // �������������� �� �������, � �������� DC ��� ����.
    wc.lpfnWndProc      = (WNDPROC) WndProc;                    // WndProc Handles Messages
    wc.cbClsExtra       = 0;                                    // ��� �������������� ������ ����
    wc.cbWndExtra       = 0;                                    // ��� �������������� ������ ����
    wc.hInstance        = hInstance;                            // ��������� Instance
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);          // ������� ������ �� ���������
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);          // ������� ������� �������
    wc.hbrBackground    = NULL;                                 // ��� ��� GL �� ���������
    wc.lpszMenuName     = NULL;                                 // ��� �� ����� ����
    wc.lpszClassName    = "OpenGL";                             // ��������� ��� ������

    if (!RegisterClass(&wc)) {                                  // ���������� ���������������� ����� ����
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                           // ��������� FALSE
    }

    if (fullscreen) {                                           // ���������� �������� �������������� �����?
        DEVMODE dmScreenSettings;                               // ����� ����������
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); // ��������, ��� ������ �������
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // ������ ��������� devmode
        dmScreenSettings.dmPelsWidth    = width;                // ������� ������ ������
        dmScreenSettings.dmPelsHeight   = height;               // ������� ������ ������
        dmScreenSettings.dmBitsPerPel   = bits;                 // ������� ���������� ��� �� �������
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // ���������� ���������� ��������� ����� � �������� ����������.  � �������: CDS_FULLSCREEN ����������� �� ������ �����Gets Rid Of Start Bar.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // ���� ������ �� ���������, ��������� ��� �����������. ����� ��� ������������ ������� �����.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE;     // ������ ������� �����.  Fullscreen = FALSE
            } else {
                // �������� ���������, ��� ���������� ��������� ������.
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return FALSE;                                   // ���������� FALSE
            }
        }
    }

    if (fullscreen) {                                           // ��� �� � ������������� ������?
        dwExStyle = WS_EX_APPWINDOW;                            // ����������� ����� ����
        dwStyle = WS_POPUP;                                     // ����� ����
        ShowCursor(FALSE);                                      // ������ ������ ����
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;         // ����������� ����� ����
        dwStyle = WS_OVERLAPPEDWINDOW;                          // ����� ����
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // ���������� ����, ����� ��� ��������������� ���������� �������

    // ������� ����
    if (!(hWnd = CreateWindowEx(  dwExStyle,                        // ����������� ����� ��� ����
                                  "OpenGL",                           // ������������ ������
                                  title,                              // ��������� ����
                                  dwStyle |                           // ������������ ����� ����
                                  WS_CLIPSIBLINGS |                   // ��������� ����� ����
                                  WS_CLIPCHILDREN,                    // ��������� ����� ����
                                  0, 0,                               // �������������� ����
                                  WindowRect.right - WindowRect.left, // ���������� ������ ����
                                  WindowRect.bottom - WindowRect.top, // ���������� ������ ����
                                  NULL,                               // ��� ������������� ����
                                  NULL,                               // ��� ����
                                  hInstance,                          // Instance
                                  NULL))) {                           // �� �������� ��������� �� WM_CREATE
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ������� FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd = {           // pfd �������� Windows ����� ��������� �� �����
        sizeof(PIXELFORMATDESCRIPTOR),              // ������ ����������� ������� ��������
        1,                                          // ����� ������
        PFD_DRAW_TO_WINDOW |                        // ������ ������ ������������ ����
        PFD_SUPPORT_OPENGL |                        // ������ ������ ������������ OpenGL
        PFD_DOUBLEBUFFER,                           // ������ �������������� ������� �����������
        PFD_TYPE_RGBA,                              // ������ RGBA �������
        bits,                                       // ����� ������� �����
        0, 0, 0, 0, 0, 0,                           // ���� ����� ������������
        0,                                          // ��� ����� �����������
        0,                                          // ��� ������ ������������
        0,                                          // ��� ������ ����������
        0, 0, 0, 0,                                 // ���� ���������� ������������
        16,                                         // 16������ Z-����� (����� �������)
        0,                                          // ��� ������ ��������� (stencil buffer)
        0,                                          // ��� ���������������� ������
        PFD_MAIN_PLANE,                             // ������� ��������� ���������
        0,                                          // ���������������
        0, 0, 0                                     // ���� ����� ������������
    };

    if (!(hDC = GetDC(hWnd))) {                     // �� �������� �������� ����������?
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ������� FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) { // Windows ����� ��������������� ������ �������?
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ������� FALSE
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {   // �� ����� ���������� ������ �������?
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ������� FALSE
    }

    if (!(hRC = wglCreateContext(hDC))) {           // �� ����� �������� �������� �����������?
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ������� FALSE
    }

    if(!wglMakeCurrent(hDC, hRC)) {                 // �������� ������������ �������� �����������
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ������� FALSE
    }

    ShowWindow(hWnd, SW_SHOW);                      // �������� ����
    SetForegroundWindow(hWnd);                      // ������ �������� ���������
    SetFocus(hWnd);                                 // ������������� ����� ����������� �� ����
    ReSizeGLScene(width, height);                   // ������������� ���� GL ���� � ������������

    if (!InitGL()) {                                // �������������� ���� GL ����
        KillGLWindow();                             // ������������� �������
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ��������� FALSE
    }

    return TRUE;                                    // �������� ���������� ��������������
}


LRESULT CALLBACK WndProc(   HWND    hWnd,           // ��������� �� ����
                            UINT    uMsg,           // ��������� ��� ����� ����
                            WPARAM  wParam,         // ��������� ���������
                            LPARAM  lParam) {       // ��������� ���������
    switch (uMsg) {                     // �������� ��������� ����
        case WM_ACTIVATE: {             // ��������� �� ���������� �� ����������� ����
            if (!HIWORD(wParam)) {          // �������� ��������� ������������
                active = TRUE;          // ��������� �������
            } else {
                active = FALSE;         // ��������� ������ �� �������
            }

            return 0;               // ��������� � ����� ���������
        }

        case WM_SYSCOMMAND: {           // ������������ ��������� �������
            switch (wParam) {           // �������� ������ �������
                case SC_SCREENSAVE:     // �������� ���������� �����������?
                case SC_MONITORPOWER:       // ������� �������� �������������� � ����� ���������� �������?
                    return 0;           // �� ������ ����� ���������
            }

            break;                  // �����
        }

        case WM_CLOSE: {                // �� �������� ��������� � �������� ���������?
            PostQuitMessage(0);         // ������� ��������� � ������
            return 0;               // Jump Back
        }

        case WM_LBUTTONDOWN: {              // Did We Receive A Left Mouse Click?
            bRender = !bRender;         // Change Rendering State Between Fill/Wire Frame
            return 0;               // ���������
        }

        case WM_KEYDOWN: {              // ������� ���� ������?
            keys[wParam] = TRUE;            // ���� ��� � ������� ��� TRUE
            return 0;               // ���������
        }

        case WM_KEYUP: {                // ������� ���� ��������?
            keys[wParam] = FALSE;           // ���� ��� � ������� ��� FALSE
            return 0;               // ���������
        }

        case WM_SIZE: {                 // ���������� ���� OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=������, HiWord=������
            return 0;               // ���������
        }
    }

    // ���������� ��� ������ ��������� � DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int WINAPI WinMain( HINSTANCE   hInstance,      // ���������
                    HINSTANCE   hPrevInstance,      // ���������� ���������
                    LPSTR       lpCmdLine,      // ��������� ��������� ������
                    int     nCmdShow) {     // �������� ��������� ����
    MSG     msg;                    // ��������� ��������� ����
    BOOL    done = FALSE;               // ��������� ���������� ������ �� �����

    // �������� ������������ ����� ����� ����������� �� ������������
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;             // ������� �����
    }

    // �������� ���� ���� OpenGL
    if (!CreateGLWindow("NeHe & Ben Humphrey's Height Map Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                   // ������� ���� ���� �� ���� �������
    }

    while(!done) {                      // ����, ������� ������������ ���� done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // ���� ��������� ���������?
            if (msg.message == WM_QUIT) {   // �� �������� ��������� � ������?
                done = TRUE;        // ���� ��� done=TRUE
            } else {                // ���� ���, ���������� �������� � ����������� ����
                TranslateMessage(&msg);     // ��������� ���������
                DispatchMessage(&msg);      // �������� ���������
            }
        } else {                    // ���� ��������� ���
            // ������ �����. ������� ������� ������ ESC � ��������� � ������ �� DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) { // �������?  ���� �������� ��������� � ������?
                done = TRUE;        // ESC ��� DrawGLScene ������������ "�����"
            } else if (active) {        // �� ����� ��������, ��������� �����
                SwapBuffers(hDC);       // ����������� ������ (������� �����������)
            }

            if (keys[VK_F1]) {          // ���� ������ ������ F1?
                keys[VK_F1] = FALSE;    // ���� ��� - ��������� �������� FALSE
                KillGLWindow();         // ������� ������� ���� OpenGL
                fullscreen = !fullscreen;   // ���������� ����� "������ �����"/"�������"

                // ������ �������� ���� ���� OpenGL
                if (!CreateGLWindow("NeHe & Ben Humphrey's Height Map Tutorial", 640, 480, 16, fullscreen)) {
                    return 0;       // �����, ���� ���� �� ���� �������
                }
            }

            if (keys[VK_UP]) {          // ������ ������� �����?
                scaleValue += 0.001f;    // ��������� ���������� ���������������
            }

            if (keys[VK_DOWN]) {        // ������ ������� ����?
                scaleValue -= 0.001f;    // ��������� ���������� ���������������
            }
        }
    }

    // Shutdown
    KillGLWindow();                     // ������� ����
    return (msg.wParam);                    // ������ �� ���������
}
