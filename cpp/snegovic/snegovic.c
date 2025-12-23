

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
    static int time = 0;
    glPushMatrix();


    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();
    glRotated(time / 2, 0.0, 1.0, 0.0);

    glLineWidth(5.0f);
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);
    glVertex3d(-0.3, 0.0, 0.0);
    glVertex3d(1.5, 0.0, 0.0);
    glEnd();


    glPushMatrix();
    glRotated(2 * time, 1, 0, 0);
    glTranslated(-0.3, 0, 0);
    glColor3f(0, 0, 1);

    glPushMatrix();
    glRotated(90, 0, 1, 0);
    glLineWidth(1);
    auxWireTorus(0.2, 0.7);
    glPopMatrix();


    glLineWidth(7);
    glColor3f(0, 1, 0);

    glBegin(GL_LINES);
    glVertex3d(0, 0, 0);
    glVertex3d(0, 1, 0);
    glVertex3d(0, 0, 0);
    glVertex3d(0, -0.5, 1);
    glVertex3d(0, 0, 0);
    glVertex3d(0, -0.5, -1);
    glEnd();
    glPopMatrix();



    glPopMatrix();



    time++;


    auxSwapBuffers();

}



void main() {
    float pos[4] = {3, 3, 3, 1};
    float dir[3] = { -1, -1, -1};

    auxInitPosition( 50, 10, 400, 400);
    auxInitDisplayMode( AUX_RGB | AUX_DEPTH | AUX_DOUBLE );
    auxInitWindow( "Giroscop" );
    auxIdleFunc(display);
    auxReshapeFunc(resize);


    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    // glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

    auxMainLoop(display);
}
