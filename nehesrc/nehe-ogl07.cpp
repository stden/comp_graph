#include <windows.h>  // ������������ ���� ��� Windows
#include <stdio.h>    // ������������ ���� ��� ������������ �����/������ (��������)
#include <gl\gl.h>    // ������������ ���� ��� ���������� OpenGL32
#include <gl\glu.h>   // ������������ ���� ��� ��� ���������� GLu32
#include <gl\glaux.h> // ������������ ���� ��� ���������� GLaux


HDC    hDC = NULL;    // ��������� �������� GDI ����������
HGLRC  hRC = NULL;    // ���������� �������� ��� ������������
HWND   hWnd = NULL;   // �������� ���������� ��� ����
HINSTANCE hInstance;  // �������� ������ ��� ����� ���������


bool keys[256];       // ������, �������������� ��� ���������� ��������� ����������
bool active = TRUE;   // ���� ��������� ���������� ���������� (�� ���������: TRUE)
bool fullscreen = TRUE; // ���� �������������� ������ (�� ���������: �������������)


BOOL light;      // ���� ��� / ����
BOOL lp;         // L ������?
BOOL fp;         // F ������?


GLfloat xrot;         // X ��������
GLfloat yrot;         // Y ��������
GLfloat xspeed;       // X �������� ��������
GLfloat yspeed;       // Y �������� ��������

GLfloat z = -5.0f;    // ����� ������ ������

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // �������� �������� ����� ( ����� )
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // �������� ���������� ����� ( ����� )
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };    // ������� ����� ( ����� )

GLuint filter;         // ����� ������ ������������
GLuint texture[3];     // ����� ��� �������� 3 �������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);    // ���������� WndProc


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
    if (TextureImage[0] = LoadBMP("Data/Crate.bmp")) {
        Status = TRUE;     // ��������� Status � TRUE
        glGenTextures(3, &texture[0]);     // �������� ���� �������

        // �������� �������� � �������� �� �������� ��������
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // ( ����� )
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ( ����� )
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
                     0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // �������� �������� � �������� �����������
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
                     0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        // �������� �������� � ���-����������
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); // ( ����� )

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY,
                          GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data); // ( ����� )
    }


    if (TextureImage[0]) {         // ���� �������� ����������
        if (TextureImage[0]->data) {  // ���� ����������� �������� ����������
            free(TextureImage[0]->data); // ������������ ������ ����������� ��������
        }

        free(TextureImage[0]);        // ������������ ������ ��� ���������
    }

    return Status;        // ���������� ������
}


GLvoid InitGL(GLsizei Width, GLsizei Height) {  // ��� ��������� ��� OpenGL �������� �����
    if (!LoadGLTextures()) {      // ������� �� ��������� �������� ��������
        return;                // ���� �������� �� ��������� ���������� FALSE
    }

    glEnable(GL_TEXTURE_2D);      // ��������� ��������� ��������
    glShadeModel(GL_SMOOTH);      // ���������� ����������� ������������
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // ������ ���
    glClearDepth(1.0f);           // ��������� ������ �������
    glEnable(GL_DEPTH_TEST);      // ��������� ���� �������
    glDepthFunc(GL_LEQUAL);       // ��� ����� �������
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ���������� ���������� �����������


    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);    // ��������� �������� �����
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);    // ��������� ���������� �����
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   // ������� �����

    glEnable(GL_LIGHT1); // ���������� ��������� ����� ����� ����

    glMatrixMode(GL_PROJECTION);// ����� ������� ��������
    glLoadIdentity();      // ����� ������� ��������
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
    // ��������� ����������� �������������� �������� ��� ����
    glMatrixMode(GL_MODELVIEW);// ����� ������� ��������� ������
    return;         // ������������� ������ OK
}


int DrawGLScene(GLvoid) {      // ����� �� ������ ��� ���������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // ������� ������ � ������ �������
    glLoadIdentity();       // ����� ���������

    glTranslatef(0.0f, 0.0f, z);    // ������� �/��� ������ �� z
    glRotatef(xrot, 1.0f, 0.0f, 0.0f); // �������� �� ��� X �� xrot
    glRotatef(yrot, 0.0f, 1.0f, 0.0f); // �������� �� ��� Y �� yrot

    glBindTexture(GL_TEXTURE_2D, texture[filter]);    // ����� �������� ����������� �� filter

    glBegin(GL_QUADS);       // ������ ��������� �����������������

    // �������� �����
    glNormal3f( 0.0f, 0.0f, 1.0f);     // ������� ��������� �� �����������
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); // ����� 1 (�����)
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); // ����� 2 (�����)
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); // ����� 3 (�����)
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); // ����� 4 (�����)

    // ������ �����
    glNormal3f( 0.0f, 0.0f, -1.0f);    // ������� ��������� �� �����������
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // ����� 1 (���)
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); // ����� 2 (���)
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); // ����� 3 (���)
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); // ����� 4 (���)

    // ������� �����
    glNormal3f( 0.0f, 1.0f, 0.0f);     // ������� ��������� �����
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); // ����� 1 (����)
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); // ����� 2 (����)
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); // ����� 3 (����)
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); // ����� 4 (����)

    // ������ �����
    glNormal3f( 0.0f, -1.0f, 0.0f);    // ������� ��������� ����
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // ����� 1 (���)
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); // ����� 2 (���)
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); // ����� 3 (���)
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); // ����� 4 (���)

    // ������ �����
    glNormal3f( 1.0f, 0.0f, 0.0f);     // ������� ��������� ������
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); // ����� 1 (�����)
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); // ����� 2 (�����)
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); // ����� 3 (�����)
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); // ����� 4 (�����)

    // ����� �����
    glNormal3f(-1.0f, 0.0f, 0.0f);     // ������� ��������� �����
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // ����� 1 (����)
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); // ����� 2 (����)
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); // ����� 3 (����)
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); // ����� 4 (����)
    glEnd();        // ������� �������� ���������������

    xrot += xspeed;      // �������� � xspeed �������� xrot
    yrot += yspeed;      // �������� � yspeed �������� yrot
    return TRUE;         // �����
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

        if (keys['L'] && !lp) { // ������� 'L' ������ � �� ������������?
            lp = TRUE;    // lp ��������� TRUE
            light = !light; // ������������ ����� TRUE/FALSE

            if (!light) {             // ���� �� ����
                glDisable(GL_LIGHTING);  // ������ ���������
            } else {                  // � ��������� ������
                glEnable(GL_LIGHTING);   // ��������� ���������
            }
        }

        if (!keys['L']) { // ������� 'L' ������?
            lp = FALSE;    // ���� ���, �� lp ����� FALSE
        }

        if (keys['F'] && !fp) { // ������� 'F' ������?
            fp = TRUE;           // fp ����� TRUE
            filter += 1;         // �������� filter ������������� �� ����

            if (filter > 2) {    // �������� ������ ��� 2?
                filter = 0;         // ���� ���, �� ��������� filter � 0
            }
        }

        if (!keys['F']) {     // ������� 'F' ������?
            fp = FALSE;          // ���� ���, �� fp ����� FALSE
        }

        if (keys[VK_PRIOR]) { // ������� 'Page Up' ������?
            z -= 0.02f;          // ���� ���, �� ������� ������ ������
        }

        if (keys[VK_NEXT]) {  // ������� 'Page Down' ������?
            z += 0.02f;          // ���� ���, �� ��������� � �����������
        }

        if (keys[VK_UP]) {   // ������� ������� ����� ������?
            xspeed -= 0.01f;    // ���� ���, �� �������� xspeed
        }

        if (keys[VK_DOWN]) { // ������� ������� ���� ������?
            xspeed += 0.01f;    // ���� ���, �� �������� xspeed
        }

        if (keys[VK_RIGHT]) { // ������� ������� ������ ������?
            yspeed += 0.01f;    // ���� ���, �� �������� yspeed
        }

        if (keys[VK_LEFT]) { // ������� ������� ����� ������?
            yspeed -= 0.01f;    // ���� ���, �� �������� yspeed
        }

        if (keys['S']) {     // ���� ��������
            yspeed = 0.0f;
            xspeed = 0.0f;
            xrot = 0.0f;
            yrot = 0.0f;
        }

        if (keys['M']) {     // ���� ��������
            filter = 2;
        }
    }
}

