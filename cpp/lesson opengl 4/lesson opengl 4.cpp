#include <windows.h> //������������ ����� ��� Windows
#include <gl\gl.h> //������������ ����� ��� ���������� OpenGL32
#include <gl\glu.h> //������������ ����� ��� ���������� Glu32
#include <gl\glaux.h> //������������ ����� ��� ���������� Glaux

HDC  hDC = NULL; // ��������� �������� ���������� GDI
HGLRC  hRC = NULL; // ���������� �������� ����������

HWND  hWnd = NULL; // ����� ����� �������� ���������� ����
HINSTANCE  hInstance; // ����� ����� �������� ���������� ����������

bool  keys[256]; // ������, ������������ ��� �������� � �����������

GLfloat rtri;           // ���� ��� �����������
GLfloat rquad;          // ���� ��� ����������������
bool  active = TRUE; // ���� ���������� ����, ������������� � true �� ���������
bool  fullscreen = TRUE; // ���� ������ ����, ������������� � ������������� �� ���������
LRESULT  CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );// �������� ������� WndProc

GLvoid ReSizeGLScene( GLsizei width, GLsizei height ) { // �������� ������ � ���������������� ���� GL
    if( height == 0 ) { // �������������� ������� �� ����
        height = 1;
    }

    glViewport( 0, 0, width, height ); // ����� ������� ������� ������

    glMatrixMode(GL_PROJECTION);     // ����� ������� ��������

    glLoadIdentity();    // ����� ������� ��������



    // ���������� ����������� �������������� �������� ��� ����

    gluPerspective( 45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f );



    glMatrixMode( GL_MODELVIEW );            // ����� ������� ���� ������

    glLoadIdentity();              // ����� ������� ���� ������

}

int InitGL( GLvoid )  // ��� ��������� ������� OpenGL ���������� �����

{

    glShadeModel( GL_SMOOTH ); // ��������� ������� �������� �����������

    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // ������� ������ � ������ ����

    glClearDepth( 1.0f ); // ��������� ������� ������ �������

    glEnable( GL_DEPTH_TEST ); // ��������� ���� �������

    glDepthFunc( GL_LEQUAL ); // ��� ����� �������

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );// ��������� � ���������� �����������
    return TRUE;                // ������������� ������ �������

}

int DrawGLScene( GLvoid ) // ����� ����� ����������� ��� ����������

{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );      // �������� ����� � ����� �������

    glLoadIdentity();              // �������� ������� �������

    glTranslatef(-1.5f, 0.0f, -6.0f);
    glRotatef(rtri, 0.0f, 1.0f, 0.0f);      // �������� ������������ �� ��� Y

    glBegin(GL_TRIANGLES);                  // ������ ��������� ������������
    glColor3f(1.0f, 0.0f, 0.0f);    // ������� ����� - �������
    glVertex3f( 0.0f, 1.0f, 0.0f);  // ������ �����
    glColor3f(0.0f, 1.0f, 0.0f);    // ����� ����� - ��������
    glVertex3f(-1.0f, -1.0f, 0.0f); // ������
    glColor3f(0.0f, 0.0f, 1.0f);    // ������ - �����
    glVertex3f( 1.0f, -1.0f, 0.0f); // ������
    glEnd();                                // ����� ���������

    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -6.0f);        // ����� ������ �� 1.5
    glRotatef(rquad, 1.0f, 0.0f, 0.0f);     // �������� �� ��� X
    glColor3f(0.5f, 0.5f, 1.0f);            // ����� ����
    glBegin(GL_QUADS);                      // ������
    glVertex3f(-1.0f, 1.0f, 0.0f);  // ���� ����
    glVertex3f( 1.0f, 1.0f, 0.0f);  // ���� �����
    glVertex3f( 1.0f, -1.0f, 0.0f); // ��� �����
    glVertex3f(-1.0f, -1.0f, 0.0f); // ��� ����
    glEnd();                                // �������

    rtri += 0.2f;           // ���������� ���������� �������� ��� ������������
    rquad -= 0.15f;         // ���������� ���������� �������� ��� ��������



    return TRUE;                // ���������� ������ �������

}

GLvoid KillGLWindow( GLvoid )              // ���������� ���������� ����

{

    if( fullscreen ) {            // �� � ������������� ������?

        ChangeDisplaySettings( NULL, 0 );        // ���� ��, �� ������������� ������� � ������� �����

        ShowCursor( TRUE );            // �������� ������ �����

    }


    if( hRC )                // ���������� �� �������� ����������?

    {
        if( !wglMakeCurrent( NULL, NULL ) )        // �������� �� ���������� RC � DC?

        {
            MessageBox( NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        }

        if( !wglDeleteContext( hRC ) )        // �������� �� ������� RC?

        {

            MessageBox( NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        }

        hRC = NULL;              // ���������� RC � NULL

    }


    if( hDC && !ReleaseDC( hWnd, hDC ) )          // �������� �� ���������� DC?

    {

        MessageBox( NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        hDC = NULL;              // ���������� DC � NULL

    }

    if(hWnd && !DestroyWindow(hWnd))            // �������� �� ���������� ����?

    {

        MessageBox( NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        hWnd = NULL;                // ���������� hWnd � NULL

    }

    if( !UnregisterClass( "OpenGL", hInstance ) )        // �������� �� ����������������� �����

    {

        MessageBox( NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

        hInstance = NULL;                // ���������� hInstance � NULL

    }

}


BOOL CreateGLWindow( char* title, int width, int height, int bits, bool fullscreenflag )

{

    GLuint    PixelFormat;              // ������ ��������� ����� ������


    WNDCLASS  wc;

    DWORD    dwExStyle;              // ����������� ����� ����

    DWORD    dwStyle;              // ������� ����� ����

    RECT WindowRect;                // Grabs Rectangle Upper Left / Lower Right Values

    WindowRect.left = (long)0;            // ���������� ����� ������������ � 0

    WindowRect.right = (long)width;            // ���������� ������ ������������ � Width

    WindowRect.top = (long)0;              // ���������� ������� ������������ � 0

    WindowRect.bottom = (long)height;            // ���������� ������ ������������ � Height


    fullscreen = fullscreenflag;            // ������������� �������� ���������� ���������� fullscreen


    hInstance    = GetModuleHandle(NULL);        // ������� ���������� ������ ����������

    wc.style    = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;      // ���������� ��� ����������� � ������ ������� DC

    wc.lpfnWndProc    = (WNDPROC) WndProc;          // ��������� ��������� ���������

    wc.cbClsExtra    = 0;              // ��� �������������� ���������� ��� ����

    wc.cbWndExtra    = 0;              // ��� �������������� ���������� ��� ����

    wc.hInstance    = hInstance;            // ������������� ����������

    wc.hIcon    = LoadIcon(NULL, IDI_WINLOGO);        // ��������� ������ �� ���������

    wc.hCursor    = LoadCursor(NULL, IDC_ARROW);        // ��������� ��������� �����

    wc.hbrBackground  = NULL;              // ��� �� ��������� ��� GL

    wc.lpszMenuName    = NULL;              // ���� � ���� �� �����

    wc.lpszClassName  = "OpenGL";            // ������������� ��� ������


    if( !RegisterClass( &wc ) )              // �������� ���������������� ����� ����

    {

        MessageBox( NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ����� � ����������� �������� �������� false

    }


    if( fullscreen )                // ������������� �����?

    {


        DEVMODE dmScreenSettings;            // ����� ����������

        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );    // ������� ��� �������� ���������

        dmScreenSettings.dmSize = sizeof( dmScreenSettings );    // ������ ��������� Devmode

        dmScreenSettings.dmPelsWidth  =   width;        // ������ ������

        dmScreenSettings.dmPelsHeight  =   height;        // ������ ������

        dmScreenSettings.dmBitsPerPel  =   bits;        // ������� �����

        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // ����� �������



        // �������� ���������� ��������� ����� � �������� ���������.  ����������: CDS_FULLSCREEN ������� ������ ����������.

        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )

        {

            // ���� ������������ � ������������� ����� ����������, ����� ���������� ��� ��������: ������� ����� ��� �����.

            if( MessageBox( NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",

                            "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES )

            {

                fullscreen = FALSE;          // ����� �������� ������ (fullscreen = false)

            }

            else

            {

                // ������������� ����, ���������� ������������ � �������� ����.

                MessageBox( NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP );

                return FALSE;            // ����� � ����������� �������� false

            }

        }

    }

    if(fullscreen)                  // �� �������� � ������������� ������?

    {
        dwExStyle  =   WS_EX_APPWINDOW;          // ����������� ����� ����

        dwStyle    =   WS_POPUP;            // ������� ����� ����

        ShowCursor( FALSE );              // ������ ��������� �����

    }

    else

    {

        dwExStyle  =   WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      // ����������� ����� ����

        dwStyle    =   WS_OVERLAPPEDWINDOW;        // ������� ����� ����

    }


    AdjustWindowRectEx( &WindowRect, dwStyle, FALSE, dwExStyle );      // ��������� ���� ���������� �������


    if( !( hWnd = CreateWindowEx(  dwExStyle,         // ����������� ����� ��� ����

                                   "OpenGL",          // ��� ������

                                   title,            // ��������� ����
                                   dwStyle |   // ���������� ����� ��� ����

                                   WS_CLIPSIBLINGS |        // ��������� ����� ��� ����

                                   WS_CLIPCHILDREN ,        // ��������� ����� ��� ����


                                   0, 0,            // ������� ����

                                   WindowRect.right - WindowRect.left,  // ���������� ���������� ������

                                   WindowRect.bottom - WindowRect.top,  // ���������� ���������� ������

                                   NULL,            // ��� �������������

                                   NULL,            // ��� ����

                                   hInstance,          // ���������� ����������

                                   NULL ) ) )          // �� ������� ������ �� WM_CREATE (???)


    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ������� false

    }

    static  PIXELFORMATDESCRIPTOR pfd =           // pfd �������� Windows ����� ����� ����� �� ����� ������� �������

    {

        sizeof(PIXELFORMATDESCRIPTOR),            // ������ ����������� ������� ������� ��������

        1,                  // ����� ������

        PFD_DRAW_TO_WINDOW |              // ������ ��� ����

        PFD_SUPPORT_OPENGL |              // ������ ��� OpenGL

        PFD_DOUBLEBUFFER,              // ������ ��� �������� ������

        PFD_TYPE_RGBA,                // ��������� RGBA ������

        bits,                  // ���������� ��� ������� �����

        0, 0, 0, 0, 0, 0,              // ������������� �������� �����

        0,                  // ��� ������ ������������

        0,                  // ��������� ��� ������������

        0,                  // ��� ������ ����������

        0, 0, 0, 0,                // ���� ���������� ������������

        32,                  // 32 ������ Z-����� (����� �������)

        0,                  // ��� ������ ���������

        0,                  // ��� ��������������� �������

        PFD_MAIN_PLANE,                // ������� ���� ���������

        0,                  // ���������������

        0, 0, 0                  // ����� ���� ������������

    };


    if( !( hDC = GetDC( hWnd ) ) )              // ����� �� �� �������� �������� ����������?

    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ������� false

    }

    if( !( PixelFormat = ChoosePixelFormat( hDC, &pfd ) ) )        // ������ �� ���������� ������ �������?

    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ������� false

    }


    if( !SetPixelFormat( hDC, PixelFormat, &pfd ) )          // �������� �� ���������� ������ �������?

    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ������� false

    }

    if( !( hRC = wglCreateContext( hDC ) ) )          // �������� �� ���������� �������� ����������?

    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);

        return FALSE;                // ������� false

    }


    if( !wglMakeCurrent( hDC, hRC ) )            // ����������� ������������ �������� ����������

    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ������� false

    }


    ShowWindow( hWnd, SW_SHOW );              // �������� ����

    SetForegroundWindow( hWnd );              // ������ ������� ���������

    SetFocus( hWnd );                // ���������� ����� ���������� �� ���� ����

    ReSizeGLScene( width, height );              // �������� ����������� ��� ������ OpenGL ������.


    if( !InitGL() )                  // ������������� ������ ��� ���������� ����

    {

        KillGLWindow();                // ������������ �����

        MessageBox( NULL, ("Initialization Failed."), ("ERROR"), MB_OK | MB_ICONEXCLAMATION );

        return FALSE;                // ������� false

    }


    return TRUE;                  // �� � �������!

}

LRESULT CALLBACK WndProc(  HWND  hWnd,            // ���������� ������� ����

                           UINT  uMsg,            // ��������� ��� ����� ����

                           WPARAM  wParam,            // �������������� ����������

                           LPARAM  lParam)            // �������������� ����������

{


    switch (uMsg)                // �������� ��������� ��� ����

    {

        case WM_ACTIVATE:            // �������� ��������� ���������� ����

        {

            if( !HIWORD( wParam ) )          // ��������� ��������� �����������

            {

                active = TRUE;          // ��������� �������

            }

            else

            {

                active = FALSE;          // ��������� ������ �� �������

            }



            return 0;            // ������������ � ���� ��������� ���������

        }


        case WM_SYSCOMMAND:            // ������������� ��������� �������

        {

            switch ( wParam )            // ������������� ��������� �����

            {

                case SC_SCREENSAVE:        // �������� �� ���������� �����������?

                case SC_MONITORPOWER:        // �������� �� ������� ������� � ����� ���������� �������?

                    return 0;          // ������������� ���

            }

            break;              // �����

        }


        case WM_CLOSE:              // �� �������� ��������� � ��������?

        {

            PostQuitMessage( 0 );          // ��������� ��������� � ������

            return 0;            // ��������� �����

        }

        case WM_KEYDOWN:            // ���� �� ������ ������?

        {

            keys[wParam] = TRUE;          // ���� ���, �� ����������� ���� ������ true

            return 0;            // ������������

        }


        case WM_KEYUP:              // ���� �� �������� �������?

        {

            keys[wParam] = FALSE;          //  ���� ���, �� ����������� ���� ������ false

            return 0;            // ������������

        }


        case WM_SIZE:              // �������� ������� OpenGL ����

        {

            ReSizeGLScene( LOWORD(lParam), HIWORD(lParam) );  // ������� �����=Width, ������� �����=Height

            return 0;            // ������������

        }

    }


    // ���������� ��� �������������� ��������� DefWindowProc

    return DefWindowProc( hWnd, uMsg, wParam, lParam );

}

int WINAPI WinMain(  HINSTANCE  hInstance,        // ���������� ����������

                     HINSTANCE  hPrevInstance,        // ���������� ������������� ����������

                     LPSTR    lpCmdLine,        // ��������� ��������� ������

                     int    nCmdShow )        // ��������� ����������� ����

{

    MSG  msg;              // ��������� ��� �������� ��������� Windows

    BOOL  done = FALSE;            // ���������� ���������� ��� ������ �� �����


    // ���������� ������������, ����� ����� ������ �� ������������

    if( MessageBox( NULL, "������ �� �� ��������� ���������� � ������������� ������?",  "��������� � ������������� ������?", MB_YESNO | MB_ICONQUESTION) == IDNO )

    {

        fullscreen = FALSE;          // ������� �����

    }


    // ������� ���� OpenGL ����

    if( !CreateGLWindow( "NeHe OpenGL ����", 800, 600, 32, fullscreen ) )

    {

        return 0;              // �����, ���� ���� �� ����� ���� �������

    }


    while( !done )                // ���� ������������, ���� done �� ����� true

    {

        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )    // ���� �� � ������� �����-������ ���������?

        {

            if( msg.message == WM_QUIT )        // �� ������� ��������� � ������?

            {

                done = TRUE;          // ���� ���, done=true

            }

            else              // ���� ���, ������������ ���������

            {


                TranslateMessage( &msg );        // ��������� ���������

                DispatchMessage( &msg );        // �������� ���������

            }

        }

        else                // ���� ��� ���������

        {


            // ������������� �����.

            if( active )          // ������� �� ���������?

            {

                if(keys[VK_ESCAPE])        // ���� �� ������ ������� ESC?

                {

                    done = TRUE;      // ESC ������� �� �������� ���������� ���������

                }

                else            // �� ����� ��� ������, ������� �����.

                {

                    DrawGLScene();        // ������ �����

                    SwapBuffers( hDC );    // ������ ����� (������� �����������)

                }

            }

            if( keys[VK_F1] )          // ���� �� ������ F1?

            {

                keys[VK_F1] = FALSE;        // ���� ���, ������ �������� ������ ������� �� false

                KillGLWindow();          // ��������� ������� ����

                fullscreen = !fullscreen;      // ����������� �����

                // ���������� ���� OpenGL ����

                if( !CreateGLWindow( "NeHe's OpenGL ���������", 800, 600, 32, fullscreen ) )

                {

                    return 0;        // �������, ���� ��� ����������

                }

            }

        }

    }


    // Shutdown

    KillGLWindow();                // ��������� ����

    return ( msg.wParam );              // ������� �� ���������

}








