
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>



void CALLBACK resize(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(-5, 5, -5, 5, 2, 12);
    gluLookAt( 0, 0, 5, 0, 0, 0, 0, 1, 0 );
    glMatrixMode( GL_MODELVIEW );
}



void CALLBACK display(void) {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();       // сохраняем текущие координаты

    glTranslated(1.4, 0, 0); // сдвигаемся по оси Х на единицу
    glColor3d(0, 1, 0);
    auxSolidSphere(0.5); // рисуем сферу в (1,0,0)
    // в абсолютных координатах

    glTranslated(1, 0, 0); // еще раз сдвигаемся
    glColor3d(0, 0, 1);
    auxSolidSphere(0.3);

    glPopMatrix();  // возвращаемся к старой системе координат

    glColor3d(1, 0, 0);
    auxSolidSphere(0.75); // рисуем сферу в точке (0,0,0)
    // в абсолютных координатах

    auxSwapBuffers();
}



void main() {
    float pos[4] = {3, 3, 3, 1};
    float dir[3] = { -1, -1, -1};

    auxInitPosition( 50, 10, 400, 400);
    auxInitDisplayMode( AUX_RGB | AUX_DEPTH | AUX_DOUBLE );
    auxInitWindow( "Sphere2" );
    auxIdleFunc(display);
    auxReshapeFunc(resize);


    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

    auxMainLoop(display);
}
