package demos.nehe.lesson05;

/*
 * Lesson05.java
 *
 * Created on July 15, 2003, 11:30 AM
 */

import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.glu.GLU;

import static javax.media.opengl.GL.GL_COLOR_BUFFER_BIT;
import static javax.media.opengl.GL.GL_DEPTH_BUFFER_BIT;
import static javax.media.opengl.GL.GL_DEPTH_TEST;
import static javax.media.opengl.GL.GL_LEQUAL;
import static javax.media.opengl.GL.GL_NICEST;
import static javax.media.opengl.GL.GL_TRIANGLES;
import static javax.media.opengl.GL2.*;
import static javax.media.opengl.GL2ES1.GL_PERSPECTIVE_CORRECTION_HINT;
import static javax.media.opengl.fixedfunc.GLLightingFunc.GL_SMOOTH;

/**
 * Port of the NeHe OpenGL Tutorial (Lesson 5)
 * to Java using the Jogl interface to OpenGL.  Jogl can be obtained
 * at http://jogl.dev.java.net/
 *
 * @author Kevin Duling (jattier@hotmail.com)
 */
class Renderer implements GLEventListener {
    private float rquad = 0.0f;
    private float rtri = 0.0f;

    private GLU glu = new GLU();

    /**
     * Called by the drawable to initiate OpenGL rendering by the client.
     * After all GLEventListeners have been notified of a display event, the
     * drawable will swap its buffers if necessary.
     *
     * @param glDrawable The GLAutoDrawable object.
     */
    public void display(GLAutoDrawable glDrawable) {
        final GL2 gl = glDrawable.getGL().getGL2();
        gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl.glLoadIdentity();
        gl.glTranslatef(-1.5f, 0.0f, -6.0f);
        gl.glRotatef(rtri, 0.0f, 1.0f, 0.0f);
        gl.glBegin(GL_TRIANGLES);            // Drawing Using Triangles
        gl.glColor3f(1.0f, 0.0f, 0.0f);            // Red
        gl.glVertex3f(0.0f, 1.0f, 0.0f);            // Top Of Triangle (Front)
        gl.glColor3f(0.0f, 1.0f, 0.0f);            // Green
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);            // Left Of Triangle (Front)
        gl.glColor3f(0.0f, 0.0f, 1.0f);            // Blue
        gl.glVertex3f(1.0f, -1.0f, 1.0f);            // Right Of Triangle (Front)
        gl.glColor3f(1.0f, 0.0f, 0.0f);            // Red
        gl.glVertex3f(0.0f, 1.0f, 0.0f);            // Top Of Triangle (Right)
        gl.glColor3f(0.0f, 0.0f, 1.0f);            // Blue
        gl.glVertex3f(1.0f, -1.0f, 1.0f);            // Left Of Triangle (Right)
        gl.glColor3f(0.0f, 1.0f, 0.0f);            // Green
        gl.glVertex3f(1.0f, -1.0f, -1.0f);            // Right Of Triangle (Right)
        gl.glColor3f(1.0f, 0.0f, 0.0f);            // Red
        gl.glVertex3f(0.0f, 1.0f, 0.0f);            // Top Of Triangle (Back)
        gl.glColor3f(0.0f, 1.0f, 0.0f);            // Green
        gl.glVertex3f(1.0f, -1.0f, -1.0f);            // Left Of Triangle (Back)
        gl.glColor3f(0.0f, 0.0f, 1.0f);            // Blue
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);            // Right Of Triangle (Back)
        gl.glColor3f(1.0f, 0.0f, 0.0f);            // Red
        gl.glVertex3f(0.0f, 1.0f, 0.0f);            // Top Of Triangle (Left)
        gl.glColor3f(0.0f, 0.0f, 1.0f);            // Blue
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);            // Left Of Triangle (Left)
        gl.glColor3f(0.0f, 1.0f, 0.0f);            // Green
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);            // Right Of Triangle (Left)
        gl.glEnd();                // Finished Drawing The Triangle
        gl.glLoadIdentity();
        gl.glTranslatef(1.5f, 0.0f, -6.0f);
        gl.glRotatef(rquad, 1.0f, 1.0f, 1.0f);
        gl.glBegin(GL_QUADS);            // Draw A Quad
        gl.glColor3f(0.0f, 1.0f, 0.0f);            // Set The Color To Green
        gl.glVertex3f(1.0f, 1.0f, -1.0f);            // Top Right Of The Quad (Top)
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);            // Top Left Of The Quad (Top)
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);            // Bottom Left Of The Quad (Top)
        gl.glVertex3f(1.0f, 1.0f, 1.0f);            // Bottom Right Of The Quad (Top)

        gl.glColor3f(1.0f, 0.5f, 0.0f);            // Set The Color To Orange
        gl.glVertex3f(1.0f, -1.0f, 1.0f);            // Top Right Of The Quad (Bottom)
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);            // Top Left Of The Quad (Bottom)
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);            // Bottom Left Of The Quad (Bottom)
        gl.glVertex3f(1.0f, -1.0f, -1.0f);            // Bottom Right Of The Quad (Bottom)

        gl.glColor3f(1.0f, 0.0f, 0.0f);            // Set The Color To Red
        gl.glVertex3f(1.0f, 1.0f, 1.0f);            // Top Right Of The Quad (Front)
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);            // Top Left Of The Quad (Front)
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);            // Bottom Left Of The Quad (Front)
        gl.glVertex3f(1.0f, -1.0f, 1.0f);            // Bottom Right Of The Quad (Front)

        gl.glColor3f(1.0f, 1.0f, 0.0f);            // Set The Color To Yellow
        gl.glVertex3f(1.0f, -1.0f, -1.0f);            // Bottom Left Of The Quad (Back)
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);            // Bottom Right Of The Quad (Back)
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);            // Top Right Of The Quad (Back)
        gl.glVertex3f(1.0f, 1.0f, -1.0f);            // Top Left Of The Quad (Back)

        gl.glColor3f(0.0f, 0.0f, 1.0f);            // Set The Color To Blue
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);            // Top Right Of The Quad (Left)
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);            // Top Left Of The Quad (Left)
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);            // Bottom Left Of The Quad (Left)
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);            // Bottom Right Of The Quad (Left)

        gl.glColor3f(1.0f, 0.0f, 1.0f);            // Set The Color To Violet
        gl.glVertex3f(1.0f, 1.0f, -1.0f);            // Top Right Of The Quad (Right)
        gl.glVertex3f(1.0f, 1.0f, 1.0f);            // Top Left Of The Quad (Right)
        gl.glVertex3f(1.0f, -1.0f, 1.0f);            // Bottom Left Of The Quad (Right)
        gl.glVertex3f(1.0f, -1.0f, -1.0f);            // Bottom Right Of The Quad (Right)
        gl.glEnd();                // Done Drawing The Quad
        gl.glFlush();
        rtri += 0.2f;
        rquad += 0.15f;
    }


    /**
     * Called by the drawable immediately after the OpenGL context is
     * initialized for the first time. Can be used to perform one-time OpenGL
     * initialization such as setup of lights and display lists.
     *
     * @param glDrawable The GLAutoDrawable object.
     */
    public void init(GLAutoDrawable glDrawable) {
        GL2 gl = glDrawable.getGL().getGL2();
        gl.glShadeModel(GL_SMOOTH);              // Enable Smooth Shading
        gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
        gl.glClearDepth(1.0f);                      // Depth Buffer Setup
        gl.glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        gl.glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
        gl.glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations
    }

    @Override
    public void dispose(GLAutoDrawable glAutoDrawable) {

    }


    /**
     * Called by the drawable during the first repaint after the component has
     * been resized. The client can update the viewport and view volume of the
     * window appropriately, for example by a call to
     * GL.glViewport(int, int, int, int); note that for convenience the component
     * has already called GL.glViewport(int, int, int, int)(x, y, width, height)
     * when this method is called, so the client may not have to do anything in
     * this method.
     *
     * @param glDrawable The GLAutoDrawable object.
     * @param x          The X Coordinate of the viewport rectangle.
     * @param y          The Y coordinate of the viewport rectanble.
     * @param width      The new width of the window.
     * @param height     The new height of the window.
     */
    public void reshape(GLAutoDrawable glDrawable, int x, int y, int width, int height) {
        final GL2 gl = glDrawable.getGL().getGL2();

        if (height <= 0) // avoid a divide by zero error!
            height = 1;
        final float h = (float) width / (float) height;
        gl.glViewport(0, 0, width, height);
        gl.glMatrixMode(GL_PROJECTION);
        gl.glLoadIdentity();
        glu.gluPerspective(45.0f, h, 1.0, 20.0);
        gl.glMatrixMode(GL_MODELVIEW);
        gl.glLoadIdentity();
    }
}
