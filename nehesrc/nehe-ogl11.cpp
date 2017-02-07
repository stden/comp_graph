#include <windows.h>                        // ������������ ���� ��� Windows
#include <stdio.h>                        // ������������ ���� ��� ������������ �����/������
#include <math.h>                        // ������������ ���� ��� ���������� �������������� �������
#include <gl\gl.h>                        // ������������ ���� ��� ���������� OpenGL32
#include <gl\glu.h>                        // ������������ ���� ��� ��� ���������� GLu32
#include <gl\glaux.h>                        // ������������ ���� ��� ���������� GLaux

HDC        hDC = NULL;              // ������ �������� ���������� GDI
HGLRC      hRC = NULL;              // ���������� �������� ����������
HWND       hWnd = NULL;              // �������� ����� ������ ����
HINSTANCE  hInstance;                // �������� ��������� ������ ����������

bool        keys[256];                        // ������ ��� ��������� ��������� ����������
bool        active = TRUE;              // ���� ���������� ����, �� ��������� TRUE
bool        fullscreen = TRUE;      // ���� �������������� ������, �� ��������� ������ �����

float points[45][45][3];    // ������ ����� ����� ����� "�����"
int wiggle_count = 0;                // ������� ��� �������� �������� ���������� �����

GLfloat        xrot;                                // �������� �� X (�����)
GLfloat        yrot;                                // �������� �� Y (�����)
GLfloat        zrot;                                // �������� �� Z (�����)
GLfloat hold;                                // �������� �������� ����� � ��������� ������� (�����)

GLuint        texture[1];                        // ����� ��� ����� �������� (�����)

LRESULT        CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        // ���������� WndProc

AUX_RGBImageRec* LoadBMP(char* Filename) {                              // �������� ����������� �� �����
    FILE* File = NULL;                                                                      // ����� �����

    if (!Filename) {                                                                              // ���� �� ��� �����?
        return NULL;                                                                        // ���� ���, ���������� NULL
    }

    File = fopen(Filename, "r");                                                     // ���������, ���������� �� ����

    if (File) {                                                                                      // ����������?
        fclose(File);                                                                        // ������� �����
        return auxDIBImageLoad(Filename);                                // ��������� ����������� � ���������� ���������
    }

    return NULL;                                                                                // ���� �� �������, ���������� NULL
}

int LoadGLTextures() {                                                                      // ��������� ����������� � ������������� � ��������
    int Status = FALSE;                                                                      // ��������� ���������

    AUX_RGBImageRec* TextureImage[1];                                        // ����� ����� ��� ��������

    memset(TextureImage, 0, sizeof(void*) * 1);                // ��������� ��������� NULL

    // ��������� �����������, ��������� �� ������, �, ���� ��� �� �������, �����
    if (TextureImage[0] = LoadBMP("Data/Tim.bmp")) {
        Status = TRUE;                                                                      // ���������� ������ � TRUE

        glGenTextures(1, &texture[0]);                                        // ������� ��������

        // �������� ��������� �������� � �������������� ������ �� �����������
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (TextureImage[0]) {                                                                      //  ���� �������� ����������
        if (TextureImage[0]->data) {                                                      // ���� ����������� �������� ����������
            free(TextureImage[0]->data);                                        // ���������� ������, ���������� ��� ��������
        }

        free(TextureImage[0]);                                                                // ���������� ��������� �����������
    }

    return Status;                                                                                // ���������� ������
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height) {              // �������� ������ � ���������������� ���� GL
    if (height == 0) {                                                                            // �������������� ������� �� ����
        height = 1;                                                                           // ���� ���� ������� ����
    }

    glViewport(0, 0, width, height);                                             // ����� ������� ������� ������ � ������������� ��������������

    glMatrixMode(GL_PROJECTION);                                                // Select The Projection Matrix
    glLoadIdentity();                                                                        // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                                                        // ����� ������� ��������� ������
    glLoadIdentity();                                                                        // ����� ������� ��������� ������
}

int InitGL(GLvoid) {                                                                              // ��� ��������� OpenGL - �����
    if (!LoadGLTextures()) {                                                              // ������� �������� �������� (�����)
        return FALSE;                                                                        // ���� �� �����������, ���������� FALSE
    }

    glEnable(GL_TEXTURE_2D);                                                        // ��������� ������������� ������� (�����)
    glShadeModel(GL_SMOOTH);                                                        // ��������� ������� ���������
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                                // ������ ���
    glClearDepth(1.0f);                                                                        // ��������� ������ �������
    glEnable(GL_DEPTH_TEST);                                                        // ��������� ������������ �������
    glDepthFunc(GL_LEQUAL);                                                                // ��� ������������ �������
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);        // ������� ���������� �����������
    glPolygonMode( GL_BACK, GL_FILL );                                        // ������ ����������� ��������
    glPolygonMode( GL_FRONT, GL_LINE );                                        // �������� ����������� �� �����

    for(int x = 0; x < 45; x++) {
        for(int y = 0; y < 45; y++) {
            points[x][y][0] = float((x / 5.0f) - 4.5f);
            points[x][y][1] = float((y / 5.0f) - 4.5f);
            points[x][y][2] = float(sin((((x / 5.0f) * 40.0f) / 360.0f) * 3.141592654 * 2.0f));
        }
    }

    return TRUE;                                                                                // ��� OK
}

int DrawGLScene(GLvoid) {                                                                      // ����� �� ������ ���, ��� ����
    int x, y;
    float float_x, float_y, float_xb, float_yb;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // �������� ����� � ����� �������
    glLoadIdentity();                                                                        // �������� ���

    glTranslatef(0.0f, 0.0f, -12.0f);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    for( x = 0; x < 44; x++ ) {
        for( y = 0; y < 44; y++ ) {
            float_x = float(x) / 44.0f;
            float_y = float(y) / 44.0f;
            float_xb = float(x + 1) / 44.0f;
            float_yb = float(y + 1) / 44.0f;

            glTexCoord2f( float_x, float_y);
            glVertex3f( points[x][y][0], points[x][y][1], points[x][y][2] );

            glTexCoord2f( float_x, float_yb );
            glVertex3f( points[x][y + 1][0], points[x][y + 1][1], points[x][y + 1][2] );

            glTexCoord2f( float_xb, float_yb );
            glVertex3f( points[x + 1][y + 1][0], points[x + 1][y + 1][1], points[x + 1][y + 1][2] );

            glTexCoord2f( float_xb, float_y );
            glVertex3f( points[x + 1][y][0], points[x + 1][y][1], points[x + 1][y][2] );
        }
    }

    glEnd();

    if( wiggle_count == 2 ) {
        for( y = 0; y < 45; y++ ) {
            hold = points[0][y][2];

            for( x = 0; x < 44; x++) {
                points[x][y][2] = points[x + 1][y][2];
            }

            points[44][y][2] = hold;
        }

        wiggle_count = 0;
    }

    wiggle_count++;

    xrot += 0.3f;
    yrot += 0.2f;
    zrot += 0.4f;

    return TRUE;                                                                                // ���� ������
}

GLvoid KillGLWindow(GLvoid) {                                                              // ��������� ����� ���� :)
    if (fullscreen) {                                                                              // �� � ������������� ������?
        ChangeDisplaySettings(NULL, 0);                                       // ���� ��, �� ������������� �� ������� ����
        ShowCursor(TRUE);                                                                // �������� ������� ������
    }

    if (hRC) {                                                                                      // ���� �� � ��� �������� �����������?
        if (!wglMakeCurrent(NULL, NULL)) {                                     // ����� �� �� ���������� DC � RC?
            MessageBox(NULL, "�� ����������� DC � RC.", "������ ���������� ������", MB_OK | MB_ICONINFORMATION);
        }

        if (!wglDeleteContext(hRC)) {                                              // ����� �� �� ������� RC?
            MessageBox(NULL, "�� ����������� �������� �����������.", "������ ���������� ������", MB_OK | MB_ICONINFORMATION);
        }

        hRC = NULL;                                                                              // RC ��������� NULL
    }

    if (hDC && !ReleaseDC(hWnd, hDC)) {                                     // ����� �� �� ���������� DC
        MessageBox(NULL, "�� ����������� �������� ����������.", "������ ���������� ������", MB_OK | MB_ICONINFORMATION);
        hDC = NULL;                                                                              // DC ��������� NULL
    }

    if (hWnd && !DestroyWindow(hWnd)) {                                      // ����� �� �� ��������� ����?
        MessageBox(NULL, "���� �� ����� ����������� :(", "������ ���������� ������", MB_OK | MB_ICONINFORMATION);
        hWnd = NULL;                                                                              // hWnd ��������� NULL
    }

    if (!UnregisterClass("OpenGL", hInstance)) {                     // ����� �� �� ������ ����� ����
        MessageBox(NULL, "�� ��������� ����� ����.", "������ ���������� ������", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL;                                                                      // hInstance ��������� NULL
    }
}

/*        ��� ����� ��������� ������� ���� ���� OpenGL.  ���������:                                                               *
 *        title                 - ��������� ����                                                            *
 *        width                 - ������ GL ���� ��� �������������� ������                                                        *
 *        height                - ������ GL ���� ��� �������������� ������                                                        *
 *        bits                  - ���������� ��� �� ���� (8/16/24/32)                                                             *
 *        fullscreenflag        - ������������� �������������� (TRUE) ��� �������� (FALSE) �������
 */

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) {
    GLuint                PixelFormat;                        // �������� ��������� ������ ����������
    WNDCLASS        wc;                                                // ��������� ������ ����
    DWORD                dwExStyle;                                // ����������� ����� ����
    DWORD                dwStyle;                                // ����� ����
    RECT                WindowRect;                    // �������� �������� �������� ������ � ������� ������� ����� ��������������
    WindowRect.left = (long)0;                      // ������������� ���� � 0
    WindowRect.right = (long)width;              // ������������� ����� � ����������� ������
    WindowRect.top = (long)0;                              // ������������� ���� � 0
    WindowRect.bottom = (long)height;              // ������������� ��� � ����������� ������

    fullscreen = fullscreenflag;                      // ��������� ���������� ������������� ����

    hInstance                        = GetModuleHandle(NULL);                                // �������� ��������� ����
    wc.style                        = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;        // ����������� ��� ��������� ��������, ����������� DC ��� ����
    wc.lpfnWndProc                = (WNDPROC) WndProc;                                        // WndProc ������������ ���������
    wc.cbClsExtra                = 0;
    wc.cbWndExtra                = 0;
    wc.hInstance                = hInstance;                                                        // ���������� ���������
    wc.hIcon                        = LoadIcon(NULL, IDI_WINLOGO);                        // ��������� ����������� �� ���������
    wc.hCursor                        = LoadCursor(NULL, IDC_ARROW);                        // ��������� ����������� ���������
    wc.hbrBackground        = NULL;                                                                        // ��� GL �� ����� ����
    wc.lpszMenuName                = NULL;                                                                        // ��� �� ����� ����
    wc.lpszClassName        = "OpenGL";                                                                // ���������� ��� ������

    if (!RegisterClass(&wc)) {                                                                      // ������� ����������� ������ ����
        MessageBox(NULL, "�� ������� ���������������� ����� ����.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                                        // ���������� FALSE
    }

    if (fullscreen) {                                                                                              // ����������� �� ������ �����?
        DEVMODE dmScreenSettings;                                                                // ����� ����������
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));      // ���������, ��� ������ �����������
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);              // ������ ��������� ������ ����������
        dmScreenSettings.dmPelsWidth        = width;                                // ������ ������
        dmScreenSettings.dmPelsHeight        = height;                                // ������ ������
        dmScreenSettings.dmBitsPerPel        = bits;                                        // ���-�� ��� �� ������
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // ����������� ��������� ��������� ����� � �������� ���������.  �������: CDS_FULLSCREEN �������� ����� �����.
        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            // ���� ����� �� ������� ����������, ���������� ��� ��������. ����� ��� � ����.
            if (MessageBox(NULL, "����������� ������������� ����� �� �������������� �����\n�����-������. ����� �������� � ����?", "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) {
                fullscreen = FALSE;              // ������, ����� � ����.
            } else {
                // ������ ��������� � �������� ���������.
                MessageBox(NULL, "��������� ��������� ������.", "������", MB_OK | MB_ICONSTOP);
                return FALSE;                                                                        // ���������� FALSE
            }
        }
    }

    if (fullscreen) {                                                                                              // �� ��� ��� �� ������ ������?
        dwExStyle = WS_EX_APPWINDOW;                                                              // ����������� ����� ����
        dwStyle = WS_POPUP;                                                                              // ����� ����
        ShowCursor(FALSE);                                                                                // �������� ������ ����
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                      // ����������� ����� ����
        dwStyle = WS_OVERLAPPEDWINDOW;                                                      // ����� ����
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);                // ��������� ���� � ���������� �������

    // ������� ����
    if (!(hWnd = CreateWindowEx(        dwExStyle,                                                      // ����������� ����� ��� ����
                                        "OpenGL",                                                        // ��� ������
                                        title,                                                                // ��������� ����
                                        dwStyle |                                                        // ������������ ����� ����
                                        WS_CLIPSIBLINGS |                                        // ������ ����� ����
                                        WS_CLIPCHILDREN,                                        // ������ ����� ����
                                        0, 0,                                                                // ������������ ����
                                        WindowRect.right - WindowRect.left,      // ��������� ������ ����
                                        WindowRect.bottom - WindowRect.top,      // ��������� ������ ����
                                        NULL,                                                                // ��� ������������� ����
                                        NULL,                                                                // ��� ����
                                        hInstance,                                                        // ���������
                                        NULL))) {                                                              // �� �������� WM_CREATE ������
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "������ �������� ����.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    static        PIXELFORMATDESCRIPTOR pfd = {                             // pfd ������������ ������, ��� �� �����, ����� ��� ��� ���������
        sizeof(PIXELFORMATDESCRIPTOR),                                // ������ ����� �������� ������� �������
        1,                                                                                        // ����� ������
        PFD_DRAW_TO_WINDOW |                                                // ������ ������ ������������ ����
        PFD_SUPPORT_OPENGL |                                                // ������ ������ ������������ OpenGL
        PFD_DOUBLEBUFFER,                                                        // ������ ������������ ������� �����������
        PFD_TYPE_RGBA,                                                                // ��������� ������ RGBA
        bits,                                                                                // ������� ���� ������� �����
        0, 0, 0, 0, 0, 0,                                                        // ���� ����� ������������
        0,                                                                                        // ��������� ��� �����-������
        0,                                                                                        // ���� ������ �������������
        0,                                                                                        // ��� ������ ����������
        0, 0, 0, 0,                                                                        // ���� ���������� ���������������
        16,                                                                                        // 16-��������� Z-����� (����� �������)
        0,                                                                                        // ��� ������ ���������
        0,                                                                                        // ��� ���������������� ������
        PFD_MAIN_PLANE,                                                                // ������� ����������� ����
        0,                                                                                        // ������������� :)
        0, 0, 0                                                                                // ����� ���� ���������������
    };

    if (!(hDC = GetDC(hWnd))) {                                                    // �������� �� �� �������� ����������?
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "�� ���� ������� �������� ���������� GL.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {   // ����� �� ����� ���������� ������ �������?
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "�� ���� ����� ���������� ������ �������.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) {            // ����� �� �� ���������� The Pixel Format?
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "�� ���� ���������� ������ �������.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    if (!(hRC = wglCreateContext(hDC))) {                            // ����� �� �� �������� �������� �����������?
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "�� ���� ������� �������� ����������� GL.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    if(!wglMakeCurrent(hDC, hRC)) {                                     // ����������� ������������� ������� �����������
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "�� ���� �������������� �������� ���������� GL.", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    ShowWindow(hWnd, SW_SHOW);                                               // �������� ����
    SetForegroundWindow(hWnd);                                                // ��������� ������� ����
    SetFocus(hWnd);                                                                        // ���� � ������ ����������
    ReSizeGLScene(width, height);                                        // ���������� ���� GL-�����������

    if (!InitGL()) {                                                                      // ���������������� ���� ����� GL-����
        KillGLWindow();                                                                // ����� �������
        MessageBox(NULL, "������������� ��������� :(", "������", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;                                                                // ���������� FALSE
    }

    return TRUE;                                                                        // �������!
}

LRESULT CALLBACK WndProc(        HWND        hWnd,                        // ����� ��� ����� ����
                                 UINT        uMsg,                        // ��������� ��� ����� ����
                                 WPARAM        wParam,                        // �������������� ����������
                                 LPARAM        lParam) {                      // �������������� ����������
    switch (uMsg) {                                                                      // �������� ��������� ����
        case WM_ACTIVATE: {                                                      // ��������� �� ���������� �� ����������� ����
            if (!HIWORD(wParam)) {                                      // �������� ��������� ������������
                active = TRUE;                                              // ��������� �������
            } else {
                active = FALSE;                                              // ��������� ���������
            }

            return 0;                                                                // ��������� � ����� ���������
        }

        case WM_SYSCOMMAND: {                                                      // ������������� ��������� �������
            switch (wParam) {                                                      // �������� ������ �������
                case SC_SCREENSAVE:                                        // �������� �������� �����������?
                case SC_MONITORPOWER:                                // ������� �������� ������������� � ����� ���������� �������?
                    return 0;                                                        // ��� ��!
            }

            break;                                                                        // ����� �� ��������
        }

        case WM_CLOSE: {                                                              // �� �������� ��������� � �������� ���������?
            PostQuitMessage(0);                                                // ������� ��������� � ������
            return 0;                                                                // ���������
        }

        case WM_KEYDOWN: {                                                      // ���� ������ �������?
            keys[wParam] = TRUE;                                        // ������� ��������������� ������ ������� TRUE
            return 0;                                                                // ���������
        }

        case WM_KEYUP: {                                                              // ������� ���� ��������?
            keys[wParam] = FALSE;                                        // ������� ��������������� ������ ������� FALSE
            return 0;                                                                // ���������
        }

        case WM_SIZE: {                                                              // ��������� ������� ���� OpenGL
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam)); // LoWord = ������, HiWord = ������
            return 0;                                                                // ���������
        }
    }

    // �������� ��� ������ ��������� � DefWindowProc
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(        HINSTANCE        hInstance,                        // ���������
                           HINSTANCE        hPrevInstance,                // ���������� ���������
                           LPSTR                lpCmdLine,                        // ��������� ��������� ������
                           int                        nCmdShow) {                      // ��������� ������ ����
    MSG                msg;                                                                        // ��������� ��������� �����
    BOOL        done = FALSE;                                                              // ���������� ���������� ���� ��� ������ �� �����

    // �������� �����, ����� ����� ������ �� ������������
    if (MessageBox(NULL, "������ ��������� � ������������� ������?", "�� ������ �����?", MB_YESNO | MB_ICONQUESTION) == IDNO) {
        fullscreen = FALSE;                                                      // ������� �����
    }

    // ������� ���� ���� OpenGL
    if (!CreateGLWindow("���� bosco � NeHe \"������������� ��������\"", 640, 480, 16, fullscreen)) {
        return 0;                                                                        // �����, ���� ���� ��� � �� ���� �������
    }

    while(!done) {                                                                      // ����, �����������, ���� done=FALSE
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  // ���� �� ��� ��� ���������?
            if (msg.message == WM_QUIT) {                            // �� �������� ��������� � ������?
                done = TRUE;                                                      // ���� ���, �� done=TRUE
            } else {                                                                    // ���� ���, �������� � ������� �����������
                TranslateMessage(&msg);                                // ��������� ���������
                DispatchMessage(&msg);                                // ��������� ���������
            }
        } else {                                                                            // ���� ��� ���������
            // ���������� �����. ������� �� ESC � ����������� � ������ �� DrawGLScene()
            if ((active && !DrawGLScene()) || keys[VK_ESCAPE]) {      // �������? �������� �� ������� "�����"?
                done = TRUE;                                                      // ESC ���� DrawGLScene ������������� � ������
            } else {                                                                    // �� ����� ��������, ��������� �����������
                SwapBuffers(hDC);                                        // �������������� ������ (������� �����������)
            }

            if (keys[VK_F1]) {                                              // ������ �� F1?
                keys[VK_F1] = FALSE;                                      // ���� ���, ���������� ������ � FALSE
                KillGLWindow();                                                // ����� ������� ���� :)
                fullscreen = !fullscreen;                              // ����������� ������ ����� / ������� �����

                // ����������� ���� OpenGL
                if (!CreateGLWindow("bosco & NeHe's Waving Texture Tutorial", 640, 480, 16, fullscreen)) {
                    return 0;                                                // �����, ���� ���� ��� � �� ���� �������
                }
            }
        }
    }

    // ���������� ������
    KillGLWindow();                                                                        // ����� ���� :)
    return (msg.wParam);                                                        // ����� �� ���������
}
