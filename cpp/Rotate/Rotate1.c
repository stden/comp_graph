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

    /* remove next tree lines
     * and enter your code here
     */
    /* glTranslated(0.01,0,0);
     glColor3d(1,0,0);
     auxSolidSphere( 1 );*/
    glColor3d(1, 0, 0);
    auxSolidCone(1, 2);  // рисуем конус в центре координат
    glPushMatrix();     // сохраняем текущие координаты
    glTranslated(2, 0, 0); // сдвигаемся в точку (1,0,0)
    glRotated(75, 1, 0, 0); // поворачиваем систему координат на 75 градусов
    glColor3d(0, 1, 0);
    auxSolidCone(1, 2);   // рисуем конус
    glPopMatrix();     //

    auxSwapBuffers();

}



void main() {
    float pos[4] = {3, 3, 3, 1};
    float dir[3] = { -1, -1, -1};

    GLfloat mat_specular[] = {1, 1, 1, 1};

    auxInitPosition( 50, 10, 400, 400);
    auxInitDisplayMode( AUX_RGB | AUX_DEPTH | AUX_DOUBLE );
    auxInitWindow( "Glaux Template" );
    auxIdleFunc(display);
    auxReshapeFunc(resize);


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);


    /*
    * Enter your cod here
    */
    auxMainLoop(display);
}
