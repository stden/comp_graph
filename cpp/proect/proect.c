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

    glColor3d(1, 1, 1);
    auxSolidSphere(2);


    auxSwapBuffers();

}




void main() {
    float pos[4] = {3, 3, 3, 1};
    float color[4] = {1, 1, 1, 1};
    float sp[4] = {1, 1, 1, 1};
    float mat_specular[] = {1, 1, 1, 1};

    auxInitPosition( 50, 10, 400, 400);
    auxInitDisplayMode( AUX_RGB | AUX_DEPTH | AUX_DOUBLE );
    auxInitWindow( "Glaux Template" );
    auxIdleFunc(display);
    auxReshapeFunc(resize);


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);



    glLightfv(GL_LIGHT3, GL_SPECULAR, sp);
    glLightfv(GL_LIGHT5, GL_SPECULAR, sp);
    glLightfv(GL_LIGHT6, GL_SPECULAR, sp);


    color[1] = color[2] = 0;
    glLightfv(GL_LIGHT3, GL_DIFFUSE, color);

    color[0] = 0;
    color[1] = 1;
    glLightfv(GL_LIGHT5, GL_DIFFUSE, color);

    color[1] = 0;
    color[2] = 1;
    glLightfv(GL_LIGHT6, GL_DIFFUSE, color);


    glLightfv(GL_LIGHT3, GL_POSITION, pos);
    pos[0] = -3;
    glLightfv(GL_LIGHT5, GL_POSITION, pos);
    pos[0] = 0;
    pos[1] = -3;
    glLightfv(GL_LIGHT6, GL_POSITION, pos);




    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0);


    auxMainLoop(display);
}
