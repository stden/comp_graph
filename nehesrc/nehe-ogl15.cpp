/*
 *      ��� ��������� ������� ������ ������ (Jeff Molofee) � 2000 �.
 *      �������������� Shawn T. ��� ��������� ���������� (%3.2f, num).
 *      �������� ������� ��������� ������ (Fredric Echols) �� ���������
 *      � ����������� �������� ����, �� ���������� ��� ��������!
 *      ���� �� ����� ��� ��������� ��������, ����������, ����� ��� �����.
 *      �������� ��� ���� nehe.gamedev.net
 */

#include <windows.h>        // ������������ ���� ��� Windows
#include <stdio.h>          // ������������ ���� ������������ �����/������
#include <gl\gl.h>          // ������������ ���� ���������� OpenGL32
#include <gl\glu.h>         // ������������ ���� ���������� GLu32
#include <gl\glaux.h>       // ������������ ���� ���������� GLaux
#include <math.h>     // ������������ ���� ��� �������������� ���������� ( ����� )


HDC         hDC = NULL;     // ������� �������� GDI ����������
HGLRC       hRC = NULL;     // ���������� �������� �����������
HWND        hWnd = NULL;    // �������� ����� ������ ����
HINSTANCE   hInstance;      // �������� ��������� ����������

bool    keys[256];          // ������ ����������� ��� ������������ ����������
bool    active = TRUE;      // ���� "��������" ����. ��������������� �������� (TRUE) �� ���������.
bool    fullscreen = TRUE;  // ���� "�� ������ �����". ��������������� � ������������� ����� �� ���������.

GLuint  texture[1]; // ���� �������� ( ����� )
GLuint  base;       // ���� ������ ����������� ��� �����

GLfloat   rot;      // ������������ ��� �������� ������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);   // ���������� ��� WndProc


GLvoid BuildFont(GLvoid) {     // ���������� ������
    GLYPHMETRICSFLOAT  gmf[256]; // ����� ������ ��� �������� ������
    HFONT  font;                 // ID ������ � Windows

    base = glGenLists(256);      // ������ 256 ��������
    font = CreateFont(  -12,     // ������ �����
                        0,        // ������ �����
                        0,        // ���� ���������
                        0,        // ���� �������
                        FW_BOLD,  // ������ ������
                        FALSE,    // ������
                        FALSE,    // �������������
                        FALSE,    // ��������������
                        SYMBOL_CHARSET,      // ������������� ������ �������� ( �������������� )
                        OUT_TT_PRECIS,       // �������� ������
                        CLIP_DEFAULT_PRECIS, // �������� ���������
                        ANTIALIASED_QUALITY, // �������� ������
                        FF_DONTCARE | DEFAULT_PITCH, // ��������� � ���
                        "Wingdings");       // ��� ������ ( �������������� )

    SelectObject(hDC, font);  // ������� �����, ��������� ����

    wglUseFontOutlines(  hDC, // ������� ������� �������� ���������� (DC)
                         0,                  // ��������� ������
                         255,                // ���������� ����������� ������� �����������
                         base,               // ��������� �������� ������ �����������
                         0.1f,        // ���������� �� ��������� �������
                         0.2f,        // ������� ������ �� ��� Z
                         WGL_FONT_POLYGONS, // ������������ ��������, � �� �����
                         gmf);        // ����� ������ ��� ������ ������ �����������
}

GLvoid KillFont(GLvoid) {          // �������� ������
    glDeleteLists(base, 256);        // �������� ���� 96 ������� �����������
}


GLvoid glPrint(const char* fmt, ...) {      // �������� ������� "������" GL
    char    text[256];      // ����� ��� ����� ������
    va_list    ap;          // ��������� �� ������ ����������

    if (fmt == NULL) {   // ���� ��� ������
        return;    // ������ �� ������
    }

    va_start(ap, fmt);           // ������ ������ ����������
    vsprintf(text, fmt, ap); // � ��������������� �������� � �������� ����
    va_end(ap);                  // ��������� ���������� � ������
    glPushAttrib(GL_LIST_BIT);      // ����������� ���� ������ �����������
    glListBase(base);          // ������ ���� �������
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // ����� �������� �����������
    glPopAttrib(); // ������� ����� ������ �����������
}

AUX_RGBImageRec* LoadBMP(char* Filename) {   // �������� ��������
    FILE* File = NULL;        // ������ �����

    if (!Filename) {          // �������� ����� �����
        return NULL;             // ���� ��� ������ NULL
    }

    File = fopen(Filename, "r"); // �������� ���������� �� ����

    if (File) {               // ���� ����������?
        fclose(File);            // ������� ����
        return auxDIBImageLoad(Filename); // �������� �������� � ������ �� ��� ���������
    }

    return NULL;              // ���� �������� �� ������� ������ NULL
}

int LoadGLTextures() {                    // �������� �������� � ��������������� � ��������
    int Status = FALSE;                      // ��������� ���������

    AUX_RGBImageRec* TextureImage[1];        // ������� ����� ��� ��������

    memset(TextureImage, 0, sizeof(void*) * 1); // ���������� ��������� � NULL

    // �������� ��������, �������� �� ������, ���� �������� �� ������� - �����
    if (TextureImage[0] = LoadBMP("Data/Lights.bmp")) {
        Status = TRUE;     // ��������� Status � TRUE
        glGenTextures(1, &texture[0]);     // �������� ���� �������
        // �������� �������� � ���-��� ����������
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
                          GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        // ������������� ������� ������������� �� ��������
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        // ������������� ������� ������������� �� ��������
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glEnable(GL_TEXTURE_GEN_S);      // �������������� ���������
        glEnable(GL_TEXTURE_GEN_T);      // �������������� ���������
    }

    if (TextureImage[0]) {          // ���� �������� ����������
        if (TextureImage[0]->data) {  // ���� ����������� �������� ����������
            free(TextureImage[0]->data); // ������������ ������ ����������� ��������
        }

        free(TextureImage[0]);         // ������������ ������ ��� ���������
    }

    return Status;        // ���������� ������
}


int InitGL(GLvoid) {      // ��� ��������� ��������� OpenGL �����
    if (!LoadGLTextures()) { // ������� �� ��������� �������� ��������
        return FALSE;         // ���� �������� �� ��������� ���������� FALSE
    }

    BuildFont();            // ��������� �����

    glShadeModel(GL_SMOOTH);    // ��������� ������� ���������
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // ������ ���
    glClearDepth(1.0f);         // ��������� ������ �������
    glEnable(GL_DEPTH_TEST);    // ���������� ����� �������
    glDepthFunc(GL_LEQUAL);     // ��� ����� �������
    glEnable(GL_LIGHT0);        // ������� ������� ���������
    // (������������� � �������� ��������� ��������� Light0)
    glEnable(GL_LIGHTING);      // �������� ���������
    // ������������� ������� ���������� �����������
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D); // ���������� ��������� ��������
    glBindTexture(GL_TEXTURE_2D, texture[0]); // ����� ��������
    return TRUE; // ������������� �������� �������
}


GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {   // �������� ������� � ������������������� ���� GL
    if (height == 0) {                                  // ������������� ������� �� 0
        height = 1;                                     // ������������ ������ �������
    }

    glViewport(0, 0, width, height);                    // ��������� ������� ���� �����������

    glMatrixMode(GL_PROJECTION);                        // �������� ������� ��������
    glLoadIdentity();                                   // ������������� ������� ��������

    // ��������� ����������� ������ ����
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                         // �������� ������� ������ �����������
    glLoadIdentity();                                   // ������������� ������� ������ �����������
}

int DrawGLScene(GLvoid) { // ����� �� ����� �������� ���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ����� � ������ �������
    glLoadIdentity(); // ����� ���������
    // ������� ������
    glTranslatef(1.1f * float(cos(rot / 16.0f)), 0.8f * float(sin(rot / 20.0f)), -3.0f);

    glRotatef(rot, 1.0f, 0.0f, 0.0f);     // �������� �� ��� X
    glRotatef(rot * 1.2f, 0.0f, 1.0f, 0.0f); // �������� �� ��� Y
    glRotatef(rot * 1.4f, 0.0f, 0.0f, 1.0f); // �������� �� ��� Z

    glTranslatef(-0.35f, -0.35f, 0.1f);    // ����� �� ���� X, Y, Z

    glPrint("N"); // �������� ������ ������� ������
    rot += 0.1f;  // �������� ���������� ��������
    return TRUE;  // �������� ��� ���������
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

    KillFont();            // ���������� �����
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

LRESULT CALLBACK WndProc(   HWND    hWnd,           // ����� ��� ����� ����
                            UINT    uMsg,           // ��������� ��� ����� ����
                            WPARAM  wParam,         // �������������� ���������� ���������
                            LPARAM  lParam) {       // �������������� ���������� ���������
    switch (uMsg) {                                 // �������� ��������� ����
        case WM_ACTIVATE: {                         // ��������� �� ���������� �� ����������� ����
            if (!HIWORD(wParam)) {                  // �������� ��������� ������������
                active = TRUE;                      // ��������� �������
            } else {
                active = FALSE;                     // ��������� ������ �� �������
            }

            return 0;                               // ���������� � ����� ���������
        }

        case WM_SYSCOMMAND: {                       // ������������ ��������� �������
            switch (wParam) {                       // �������� ������ �������
                case SC_SCREENSAVE:                 // ��������� ���������� �����������?
                case SC_MONITORPOWER:               // ������� �������� �������������� � ����� ���������� �������?
                    return 0;                           // �� ������ ����� ���������
            }

            break;                                  // �����
        }

        case WM_CLOSE: {                            // �� �������� ��������� � �������� ���������?
            PostQuitMessage(0);                     // ������� ��������� � ������
            return 0;                               // ���������
        }

        case WM_KEYDOWN: {                          // ������� ���� ������?
            keys[wParam] = TRUE;                    // ���� ��� ��� ������� ��� TRUE
            return 0;                               // ���������
        }

        case WM_KEYUP: {                            // ������� ���� ��������?
            keys[wParam] = FALSE;                   // ���� ��� ��� ������� ��� FALSE
            return 0;                               // ���������
        }

        case WM_SIZE: {                             // ���������� ���� OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord=������, HiWord=������
            return 0;                               // ���������
        }
    }

    // ���������� ��� ������ ��������� � DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE   hInstance,          // Instance
                    HINSTANCE   hPrevInstance,      // ���������� Instance
                    LPSTR       lpCmdLine,          // ��������� ��������� ������
                    int         nCmdShow) {         // �������� ��������� ����
    MSG     msg;                                    // ��������� ��������� ����
    BOOL    done = FALSE;                           // ��������� ���������� ������ �� �����

    // �������� ������������ ����� ����� ����������� �� ������������
    if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", "Start FullScreen?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;                         // ������� �����
    }

    // �������� ���� ���� OpenGL
    if (!CreateGLWindow("NeHe's Display List Tutorial", 640, 480, 16, fullscreen)) {
        return 0;                                   // ������� ���� ���� �� ���� �������
    }

    while(!done) {                                  // ����, ������� ������������ ���� done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // ���� ��������� ���������?
            if (msg.message == WM_QUIT) {           // �� �������� ��������� � ������?
                done = TRUE;                        // ���� ��� done=TRUE
            } else {                                // ���� ���,���������� �������� � ����������� ����
                TranslateMessage(&msg);             // ��������� ���������
                DispatchMessage(&msg);              // �������� ���������
            }
        } else {                                    // ���� ��������� ���
            // ������ �����.  ������� ������� ������ ESC � ��������� � ������ �� DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) { // �������?  ���� �������� ��������� � ������?
                done = TRUE;                        // ESC ��� DrawGLScene ������������ "�����"
            } else {                                // �� ����� ��������, ��������� �����
                SwapBuffers(hDC);                   // ����������� ������ (Double Buffering)

                if (keys[VK_F1]) {                      // ���� ������ ������ F1?
                    keys[VK_F1] = FALSE;                // ���� ��� - ��������� �������� FALSE
                    KillGLWindow();                     // ������� ������� ���� OpenGL
                    fullscreen = !fullscreen;           // // ���������� ����� "������ �����"/"�������"

                    // ������ �������� ���� ���� OpenGL
                    if (!CreateGLWindow("NeHe's Display List Tutorial", 640, 480, 16, fullscreen)) {
                        return 0;                       // �����, ���� ���� �� ���� �������

                    }
                }
            }
        }
    }

    // Shutdown
    KillGLWindow();                                 // ������� ����
    return (msg.wParam);                            // ������ �� ���������
}
