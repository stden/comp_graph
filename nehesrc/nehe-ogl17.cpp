/*
 *      ���� ��� ��� ������� Jeff'�� Molofee 2000
 *      � ������ Giuseppe D'Agata'�� (waveform@tiscalinet.it)
 *      ���� �� ������� ��� ��������, �� ����� ��� �����.
 *      �������� ��� ��������� �� nehe.gamedev.net
 */

#include <windows.h>        // ��������� ��� Windows
#include <math.h>           // ��������� ��� �������������� ���������� Windows 
#include <stdio.h>          // ��������� ��� ����������� ���������� �����/������
#include <gl\gl.h>          // ��������� ��� ���������� OpenGL32 
#include <gl\glu.h>         // ��������� ��� ���������� GLu32 
#include <gl\glaux.h>       // ��������� ��� ���������� Glaux 

HDC         hDC = NULL;     // ��������� �������� ���������� GDI
HGLRC       hRC = NULL;     // ���������� �������� ����������
HWND        hWnd = NULL;    // ��������� ���������� ����
HINSTANCE   hInstance;      // ��������� ��������� ����������

bool    keys[256];          // ������ ��� ������ � �����������
bool    active = TRUE;      // ���� ���������� ����, �� ��������� = TRUE
bool    fullscreen = TRUE;  // ���� �������������� ������

GLuint  base;               // �������� ������ ����������� ��� ������
GLuint  texture[2];         // ����� ��� �������� ������ ������
GLuint  loop;               // ����� ���������� ��� ������

GLfloat cnt1;               // ������ ������� ��� �������� � ������������� ������
GLfloat cnt2;               // ������ ������� ��� �������� � ������������� ������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);   // ���������� WndProc

AUX_RGBImageRec* LoadBMP(char* Filename) {              // �������� �����������
    FILE* File = NULL;                              // ���������� �����

    if (!Filename) {                                // �������������, ��� ��� ����� ��������
        return NULL;                            // ���� ���, ��������� NULL
    }

    File = fopen(Filename, "r");                    // ��������, ���������� �� ����

    if (File) {                                     // �����������?
        fclose(File);                           // ��������� ����
        return auxDIBImageLoad(Filename);       // ��������� ����������� � ���������� ���������
    }

    return NULL;                                    // ���� �������� �� �������, ���������� NULL
}

int LoadGLTextures() {                                  // �������� � �������������� �������
    int Status = FALSE;                             // ��������� �������
    AUX_RGBImageRec* TextureImage[2];               // ����� ��������� ��� �������
    memset(TextureImage, 0, sizeof(void*) * 2);     // ������������� ��������� � NULL

    if ((TextureImage[0] = LoadBMP("Data/Font.bmp")) &&
            (TextureImage[1] = LoadBMP("Data/Bumps.bmp"))) {
        Status = TRUE;                          // ������������� ������ � TRUE
        glGenTextures(2, &texture[0]);          // �������� 2-� �������

        for (loop = 0; loop < 2; loop++) {
            glBindTexture(GL_TEXTURE_2D, texture[loop]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);
        }
    }

    for (loop = 0; loop < 2; loop++) {
        if (TextureImage[loop]) {                       // ���� �������� ����������
            if (TextureImage[loop]->data) {         // ���� ����������� �������� ����������
                free(TextureImage[loop]->data); // ����������� ������ �� ����������� ��������
            }

            free(TextureImage[loop]);               // ����������� ������ �� ��������� �����������
        }
    }

    return Status;                                  // ���������� ������
}

GLvoid BuildFont(GLvoid) {                              // ������� ������ ����������� ������ ������
    float   cx;                                         // �������� X ���������� �������
    float   cy;                                         // �������� Y ���������� �������

    base = glGenLists(256);                             // ������� ������
    glBindTexture(GL_TEXTURE_2D, texture[0]);           // �������� �������� ������

    for (loop = 0; loop < 256; loop++) {                // ���� �� ���� 256 �������
        cx = float(loop % 16) / 16.0f;                  // X ���������� �������� �������
        cy = float(loop / 16) / 16.0f;                  // Y ���������� �������� �������

        glNewList(base + loop, GL_COMPILE);             // �������� ������ ������
        glBegin(GL_QUADS);                              // ���������� ���������������, ��� ������� �������
        glTexCoord2f(cx, 1 - cy - 0.0625f);             // ����� � �������� (����� ������)
        glVertex2i(0, 0);                               // ���������� ������� (����� ������)
        glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);   // ����� �� �������� (������ ������)
        glVertex2i(16, 0);                      // ���������� ������� (������ ������)
        glTexCoord2f(cx + 0.0625f, 1 - cy);     // ����� �������� (������� ������)
        glVertex2i(16, 16);                     // ���������� ������� (������� ������)

        glTexCoord2f(cx, 1 - cy);               // ����� �������� (������� �����)
        glVertex2i(0, 16);                      // ���������� ������� (������� �����)
        glEnd();                                    // ����� ���������� ���������������� (�������)
        glTranslated(10, 0, 0);                     // �������� ������ �� �������
        glEndList();                                    // ����������� ��������� ������ �����������
    }                                                   // ���� ��� �������� ���� 256 ��������
}

GLvoid KillFont(GLvoid) {                               // ������� ����� �� ������
    glDeleteLists(base, 256);                           // ������� ��� 256 ������� �����������
}

GLvoid glPrint(GLint x, GLint y, char* string, int set) { // ��� ��������
    if (set > 1) {
        set = 1;
    }

    glBindTexture(GL_TEXTURE_2D, texture[0]);           // �������� ���� �������� ������
    glDisable(GL_DEPTH_TEST);                           // ������ �������� �������
    glMatrixMode(GL_PROJECTION);                        // �������� ������� ��������������
    glPushMatrix();                                     // ��������� ������� ��������������
    glLoadIdentity();                                   // ���������� ������� ��������
    glOrtho(0, 640, 0, 480, -1, 1);                     // ������������� Ortho �����
    glMatrixMode(GL_MODELVIEW);                         // �������� ������� ������ ���������
    glPushMatrix();                                     // ��������� ������� ������ ���������
    glLoadIdentity();                                   // ���������� ������� ������ ���������
    glTranslated(x, y, 0);                              // ������� ������ (0,0 - ������ �����)
    glListBase(base - 32 + (128 * set));                // �������� ����� �������� (0 ��� 1)
    glCallLists(strlen(string), GL_BYTE, string);       // ������ ����� �� ������
    glMatrixMode(GL_PROJECTION);                        // �������� ������� ��������������
    glPopMatrix();                                      // ��������������� ������ ������� ��������������
    glMatrixMode(GL_MODELVIEW);                         // �������� ������� ��������� ������
    glPopMatrix();                                      // ��������������� ������ ������� ��������������
    glEnable(GL_DEPTH_TEST);                            // ��������� ���� �������
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {   // ������������� � ��������� �������� ���� GL
    if (height == 0) {                                  // ������������� ������� �� 0
        height = 1;                                     // ������ ������ ������ 1
    }

    glViewport(0, 0, width, height);                    // ���������� ���� ������
    glMatrixMode(GL_PROJECTION);                        // �������� ������� ��������������
    glLoadIdentity();                                   // ���������� ������� ��������������
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f); // ��������� ����������� ���������
    glMatrixMode(GL_MODELVIEW);                         // �������� ������� ��������� ������
    glLoadIdentity();                                   // ���������� ������� ��������� ������
}

int InitGL(GLvoid) {                                    // ��� ��������� ��� OpenGL �����
    if (!LoadGLTextures()) {                            // ��������� � �������� ��������
        return FALSE;                                   // ���� �������� �� ����������� - ���������� FALSE
    }

    BuildFont();                                        // ������� �����
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               // ������� ��� ������ ������
    glClearDepth(1.0);                                  // ��������� ������� ������ �������
    glDepthFunc(GL_LEQUAL);                             // ��� ����� �������
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                  // ��� ����������
    glShadeModel(GL_SMOOTH);                            // ��������� ������� �������
    glEnable(GL_TEXTURE_2D);                            // ��������� 2-� ������ ���������������
    return TRUE;                                        // ������������� ������ �������
}

int DrawGLScene(GLvoid) {                               // ����� �� ������ ��� �������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������ � ������ �������
    glLoadIdentity();                                   // ����� ������� ��������� ������
    glBindTexture(GL_TEXTURE_2D, texture[1]);           // �������� ������ ��������
    glTranslatef(0.0f, 0.0f, -5.0f);                    // ��������� �� 5 ������
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);                 // ������������� �� 45 �������� (�� ������� �������)
    glRotatef(cnt1 * 30.0f, 1.0f, 1.0f, 0.0f);          // �������� �� X & Y �� cnt1 (����� �� �����)
    glDisable(GL_BLEND);                                // �������� ���������� ����� ���������� 3D
    glColor3f(1.0f, 1.0f, 1.0f);                        // ���� �����
    glBegin(GL_QUADS);                                  // ������ ������ ���������������� �������������
    glTexCoord2d(0.0f, 0.0f);                       // ������ ����� � ��������
    glVertex2f(-1.0f, 1.0f);                        // ������ �������
    glTexCoord2d(1.0f, 0.0f);                       // ������ ����� � ��������
    glVertex2f( 1.0f, 1.0f);                        // ������ �������
    glTexCoord2d(1.0f, 1.0f);                       // ������ ����� � ��������
    glVertex2f( 1.0f, -1.0f);                       // ������ �������
    glTexCoord2d(0.0f, 1.0f);                       // ��������� ����� � ��������
    glVertex2f(-1.0f, -1.0f);                       // ��������� �������
    glEnd();                                            // ����������� ��������� ����������������
    glRotatef(90.0f, 1.0f, 1.0f, 0.0f);                 // �������������� �� X � Y �� 90 �������� (����� �� �����)
    glBegin(GL_QUADS);                                  // ������ ������ ���������������� ���������������
    glTexCoord2d(0.0f, 0.0f);                       // ������ ����� ��������
    glVertex2f(-1.0f, 1.0f);                        // ������ �������
    glTexCoord2d(1.0f, 0.0f);                       // ������ ����� ��������
    glVertex2f( 1.0f, 1.0f);                        // ������ �������
    glTexCoord2d(1.0f, 1.0f);                       // ������ ����� ��������
    glVertex2f( 1.0f, -1.0f);                       // ������ �������
    glTexCoord2d(0.0f, 1.0f);                       // ��������� ����� ��������
    glVertex2f(-1.0f, -1.0f);                       // ��������� �������
    glEnd();                                            // ����������� �������� ���������������
    glEnable(GL_BLEND);                                 // ��������� ����������

    glLoadIdentity();                                   // ���������� ��������
    // ��������� ����� ������������ �� ��������� ������
    glColor3f(1.0f * float(cos(cnt1)), 1.0f * float(sin(cnt2)), 1.0f - 0.5f * float(cos(cnt1 + cnt2)));
    glPrint(int((280 + 250 * cos(cnt1))), int(235 + 200 * sin(cnt2)), "NeHe", 0); // �������� GL ����� �� ������

    glColor3f(1.0f * float(sin(cnt2)), 1.0f - 0.5f * float(cos(cnt1 + cnt2)), 1.0f * float(cos(cnt1)));
    glPrint(int((280 + 230 * cos(cnt2))), int(235 + 200 * sin(cnt1)), "OpenGL", 1); // �������� GL ����� �� ������

    glColor3f(0.0f, 0.0f, 1.0f);
    glPrint(int(240 + 200 * cos((cnt2 + cnt1) / 5)), 2, "Giuseppe D'Agata", 0);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPrint(int(242 + 200 * cos((cnt2 + cnt1) / 5)), 2, "Giuseppe D'Agata", 0);

    cnt1 += 0.01f;                                      // �������� ������ �������
    cnt2 += 0.0081f;                                    // �������� ������ �������
    return TRUE;                                        // ��� ������ �������
}

GLvoid KillGLWindow(GLvoid) {                           // ���������� ����� ����
    if (fullscreen) {                                   // ����� �������������?
        ChangeDisplaySettings(NULL, 0);                 // ���� ��, ������������� � ����� ��������
        ShowCursor(TRUE);                               // ���������� ������ ����
    }

    if (hRC) {                                          // � ��� ���� �������� ����������?
        if (!wglMakeCurrent(NULL, NULL)) {              // ����� ������� ��������� DC � RC?
            MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC)) {                   // ����� ������� RC?
            MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        }

        hRC = NULL;                                     // ��������� RC � NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC)) {                 // ����� ������� DC
        MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                     // ������������� DC � NULL
    }

    if (hWnd && !DestroyWindow(hWnd)) {                 // ����� ���������� ����?
        MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                    // ������������� hWnd � NULL
    }

    if (!UnregisterClass("OpenGL", hInstance)) {        // ����� ����������������� �����?
        MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                               // ������������� hInstance � NULL
    }

    KillFont();                                         // ������� �����
}

/*  ��� ��� ������� ���� ���� Open GL. ���������:
 *  title           - ��������� �������� ������ ����                        *
 *  width           - ������ GL ���� ��� ������� ������                     *
 *  height          - ������ GL ���� ��� ������� ������                     *
 *  bits            - ���������� ����� ����� (8/16/24/32)                   *
 *  fullscreenflag  - ������ ����� (TRUE) ��� ������� �����         (FALSE) */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {
    GLuint      PixelFormat;            // ��������� ���������� ����� ������ ����������
    WNDCLASS    wc;                     // ��������� ������ ����
    DWORD       dwExStyle;              // ����������� ����� ����
    DWORD       dwStyle;                // ����� ����
    RECT        WindowRect;             // ������������� ������� �����/������ ������ ����
    WindowRect.left = (long)0;          // ����� ������� = 0
    WindowRect.right = (long)width;     // ������ - ����������� �����
    WindowRect.top = (long)0;           // ������� �������� = 0
    WindowRect.bottom = (long)height;   // ������ - ����������� ������

    fullscreen = fullscreenflag;        // ���������� ���������� �������������� ������

    hInstance           = GetModuleHandle(NULL);                // ����������� ���������
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;   // ����������� ��� ��������� ������� � DC ��� ����
    wc.lpfnWndProc      = (WNDPROC) WndProc;                    // WndProc ���������� ���������
    wc.cbClsExtra       = 0;                                    // ��� �������������� ������ ����
    wc.cbWndExtra       = 0;                                    // ��� �������������� ������ ����
    wc.hInstance        = hInstance;                            // ������������� ���������
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);          // ��������� ����������� ������
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);          // ��������� ������� ������� ����
    wc.hbrBackground    = NULL;                                 // ��� ���� ��� GL
    wc.lpszMenuName     = NULL;                                 // ��� ����
    wc.lpszClassName    = "OpenGL";                             // ��� ������

    if (!RegisterClass(&wc)) {                                  // ������� �������������� �����
        MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                           // ���������� FALSE
    }

    if (fullscreen) {                                           // ������������� �����?
        DEVMODE dmScreenSettings;                               // ����� ����������
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); // �������������, ��� ������ �������
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);     // ������ ��������� Devmode
        dmScreenSettings.dmPelsWidth    = width;                // ������ ������
        dmScreenSettings.dmPelsHeight   = height;               // ������ ������
        dmScreenSettings.dmBitsPerPel   = bits;                 // ��� �� ������
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // �������� ���������� ������� ����� � �������� ���������.  ���������: CDS_FULLSCREEN ����������� �� Start ����.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // ���� ����� �� ������� ����������, �� ��� ��������. ����� ��� ������� �����.
            if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE;     // ������� �����.  Fullscreen = FALSE
            } else {
                // ���� � ���������� � ���, ��� ��������� �����������
                MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
                return FALSE;                                   // ���������� FALSE
            }
        }
    }

    if (fullscreen) {                                           // �� ��� ��� � ������������� ������?
        dwExStyle = WS_EX_APPWINDOW;                            // ����������� ����� ����
        dwStyle = WS_POPUP;                                     // ����� ����
        ShowCursor(FALSE);                                      // �������� ����
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;         // ����������� ����� ����
        dwStyle = WS_OVERLAPPEDWINDOW;                          // ����� ����
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // �������� ���� � ���������� �������

    // ������� ����
    if (!(hWnd = CreateWindowEx(  dwExStyle,                        // ����������� ����� ��� ����
                                  "OpenGL",                           // �������� ������
                                  title,                              // ��������� ����
                                  dwStyle |                           // ������������ ����� ����
                                  WS_CLIPSIBLINGS |                   // ������� �����
                                  WS_CLIPCHILDREN,                    // ������� �����
                                  0, 0,                               // ������� ����
                                  WindowRect.right - WindowRect.left, // ����������� ������
                                  WindowRect.bottom - WindowRect.top, // ����������� ������
                                  NULL,                               // ��� ������������� ����
                                  NULL,                               // ��� ����
                                  hInstance,                          // ���������
                                  NULL))) {                           // ������ �� �������� � WM_CREATE
        KillGLWindow();                             // ���������� �����
        MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    static  PIXELFORMATDESCRIPTOR pfd = {           // pfd ������� Windows ����� ������
        sizeof(PIXELFORMATDESCRIPTOR),              // ������ ����������� ������� ��������
        1,                                          // ����� ������
        PFD_DRAW_TO_WINDOW |                        // ������ ������ ������������ ����
        PFD_SUPPORT_OPENGL |                        // ������ ������ ������������ OpenGL
        PFD_DOUBLEBUFFER,                           // ������ ������ ������������ ������� �����������
        PFD_TYPE_RGBA,                              // ����� �������� RGBA
        bits,                                       // ������� �����
        0, 0, 0, 0, 0, 0,                           // ���������� �������� ����
        0,                                          // ��� ����� ������
        0,                                          // ����� ����� �� �����
        0,                                          // ��� ���������������� ������
        0, 0, 0, 0,                                 // ��������������� ����� ���
        16,                                         // 16������ Z-������ (����� �������)
        0,                                          // ��� ������ ���������
        0,                                          // ��� ���������������� ������
        PFD_MAIN_PLANE,                             // �������� ���� ���������
        0,                                          // ���������������
        0, 0, 0                                     // ��� ����� ����
    };

    if (!(hDC = GetDC(hWnd))) {                     // � ��� ���� �������� ����������?
        KillGLWindow();                             // ����� ������
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) { // ����� Windows ����������� ������?
        KillGLWindow();                             // ����� �������
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {   // ����� ���������� ������ �������?
        KillGLWindow();                             // ����� �������
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    if (!(hRC = wglCreateContext(hDC))) {           // ����� �������� �������� ����������?
        KillGLWindow();                             // ����� �������
        MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    if(!wglMakeCurrent(hDC, hRC)) {                 // �������� �������������� �������� ����������
        KillGLWindow();                             // ����� �������
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    ShowWindow(hWnd, SW_SHOW);                      // ������� ����
    SetForegroundWindow(hWnd);                      // ������� ������� ���������
    SetFocus(hWnd);                                 // ������������� ����� �� ����
    ReSizeGLScene(width, height);                   // ������������� ����������� GL ������

    if (!InitGL()) {                                // �������������� ������ ��� ��������� GL ����
        KillGLWindow();                             // ����� �������
        MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                               // ���������� FALSE
    }

    return TRUE;                                    // �������� ����������
}

LRESULT CALLBACK WndProc(   HWND    hWnd,
                            UINT    uMsg,
                            WPARAM  wParam,
                            LPARAM  lParam) {
    switch (uMsg) {                                 // �������� ��������� Windows
        case WM_ACTIVATE: {                         // ��������� ��������� ����
            if (!HIWORD(wParam)) {                  // ��������� ������ �����������
                active = TRUE;                      // ��������� �������
            } else {
                active = FALSE;                     // ��������� ������ �� �������
            }

            return 0;                               // ������������ � ���� ���������
        }

        case WM_SYSCOMMAND: {                       // �������� ��������� ������
            switch (wParam) {                       // �������� ������� �������
                case SC_SCREENSAVE:                 // ���������� �����������?
                case SC_MONITORPOWER:               // ������� �������� ������� � ����������������� �����?
                    return 0;                           // ������������� ���
            }

            break;                                  // �����
        }

        case WM_CLOSE: {                            // �� �������� ��������� � ��������?
            PostQuitMessage(0);                     // �������� ��������� � ������
            return 0;                               // �������
        }

        case WM_KEYDOWN: {                          // ������ �������?
            keys[wParam] = TRUE;                    // ���� ��, �������� �� ��� TRUE
            return 0;                               // �������
        }

        case WM_KEYUP: {                            // ������� ��������?
            keys[wParam] = FALSE;                   // ���� ��, �� �������� �� ��� FALSE
            return 0;                               // �������
        }

        case WM_SIZE: {                             // ��������� �������� ���� OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // ������������=������, ������������=������
            return 0;                               // �������
        }
    }

    // ��� ������ ��������� �������� � DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE   hInstance,          // ���������
                    HINSTANCE   hPrevInstance,      // ���������� ���������
                    LPSTR       lpCmdLine,          // ��������� ��������� ������
                    int         nCmdShow) {         // ����� ������ ����
    MSG     msg;                                    // ��������� ���������
    BOOL    done = FALSE;                           // ���������� ��� ������ �� �����

    // ���������� � ������������, ����� ����� �� ������������
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;                         // ����� ����
    }

    // ������� ���� OpenGL
    if (!CreateGLWindow("NeHe & Giuseppe D'Agata's 2D Font Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                                   // ���� �� ��������� - �������
    }

    while(!done) {                                  // ���� ���� done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // ������ ���������?
            if (msg.message == WM_QUIT) {           // ��� ��������� � ������?
                done = TRUE;                        // ���� ��, �� done=TRUE
            } else {                                // ���� ���, �� ������������
                TranslateMessage(&msg);             // ��������� ���������
                DispatchMessage(&msg);              // �������� ���������
            }
        } else {                                    // ��� ���������
            // ������ �����.  ���� ������� ESC ��� ��������� � ������ �� DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) { // �������?  ���� ��������� � ������?
                done = TRUE;                        // ESC ��� DrawGLScene �������� � ������
            } else {                                // �� ����� ��������, ��������� �����
                SwapBuffers(hDC);                   // ������ ������ (������� �����������)
            }

            if (keys[VK_F1]) {                      // ������ F1?
                keys[VK_F1] = FALSE;                // ����, �� �� ������������� FALSE
                KillGLWindow();                     // ������� ������� ����
                fullscreen = !fullscreen;           // ��������� � ������������� �����

                // ����������� ���� GL
                if (!CreateGLWindow("NeHe & Giuseppe D'Agata's 2D Font Tutorial", 640, 480, 16, fullscreen)) {
                    return 0;                       // �������, ���� ������� �� �������
                }
            }
        }
    }

    // ��������
    KillGLWindow();                                 // ������� ����
    return (msg.wParam);                            // ������� �� ���������
}
