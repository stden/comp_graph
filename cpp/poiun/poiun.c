
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
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4d(1, 1, 0, 1);
    auxSolidSphere( 1 );
    glColor4d(0, 1, 0, 0.6);
    auxSolidCylinder(2, 3);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    auxSwapBuffers();


}



void main() {
    float pos[4] = {3, 3, 3, 1};
    float dir[3] = { -1, -1, -1};

    auxInitPosition( 50, 10, 400, 400);
    auxInitDisplayMode( AUX_RGB | AUX_DEPTH | AUX_DOUBLE );
    auxInitWindow( "Glaux Template" );
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


    glShadeModel(GL_SMOOTH);

    auxMainLoop(display);
}
