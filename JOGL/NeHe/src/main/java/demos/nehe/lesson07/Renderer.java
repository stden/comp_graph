package demos.nehe.lesson07;

import demos.common.TextureReader;

import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.glu.GLU;
import java.io.IOException;

import static javax.media.opengl.GL2.*;

/**
 * Port of the NeHe OpenGL Tutorial (Lesson 7)
 * to Java using the Jogl interface to OpenGL.  Jogl can be obtained
 * at http://jogl.dev.java.net/
 *
 * @author Kevin Duling (jattier@hotmail.com)
 */
class Renderer implements GLEventListener {
    private boolean lightingEnabled;                // Lighting ON/OFF

    private int filter;                                // Which texture to use
    private int[] textures = new int[3];            // Storage For 3 Textures

    private float xrot;                // X Rotation
    private float yrot;                // Y Rotation

    private float xspeed = 0.5f;                // X Rotation Speed
    private boolean increaseX;
    private boolean decreaseX;

    private float yspeed = 0.3f;                // Y Rotation Speed
    private boolean increaseY;
    private boolean decreaseY;

    private float z = -5.0f;            // Depth Into The Screen
    private boolean zoomIn;
    private boolean zoomOut;

    private float[] lightAmbient = {0.5f, 0.5f, 0.5f, 1.0f};
    private float[] lightDiffuse = {1.0f, 1.0f, 1.0f, 1.0f};
    private float[] lightPosition = {0.0f, 0.0f, 2.0f, 1.0f};

    private GLU glu = new GLU();

    public void toggleLighting() {
        lightingEnabled = !lightingEnabled;
    }

    public void increaseXspeed(boolean increaseX) {
        this.increaseX = increaseX;
    }

    public void decreaseXspeed(boolean decreaseX) {
        this.decreaseX = decreaseX;
    }

    public void increaseYspeed(boolean increaseY) {
        this.increaseY = increaseY;
    }

    public void decreaseYspeed(boolean decreaseY) {
        this.decreaseY = decreaseY;
    }

    public void zoomIn(boolean zoomIn) {
        this.zoomIn = zoomIn;
    }

    public void zoomOut(boolean zoomOut) {
        this.zoomOut = zoomOut;
    }

    public void switchFilter() {
        filter = (filter + 1) % textures.length;
    }

    private void update() {
        if (increaseX)
            xspeed += 0.01f;
        if (decreaseX)
            xspeed -= 0.01f;
        if (increaseY)
            yspeed += 0.01f;
        if (decreaseY)
            yspeed -= 0.01f;
        if (zoomIn)
            z += 0.02f;
        if (zoomOut)
            z -= 0.02f;
    }

    /**
     * Called by the drawable to initiate OpenGL rendering by the client.
     * After all GLEventListeners have been notified of a display event, the
     * drawable will swap its buffers if necessary.
     *
     * @param glDrawable The GLAutoDrawable object.
     */
    public void display(GLAutoDrawable glDrawable) {
        update();

        final GL2 gl = glDrawable.getGL().getGL2();
        gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl.glLoadIdentity();                                    // Reset The View
        gl.glTranslatef(0.0f, 0.0f, this.z);

        gl.glRotatef(xrot, 1.0f, 0.0f, 0.0f);
        gl.glRotatef(yrot, 0.0f, 1.0f, 0.0f);

        gl.glBindTexture(GL_TEXTURE_2D, textures[filter]);
        if (lightingEnabled)
            gl.glEnable(GL_LIGHTING);
        else
            gl.glDisable(GL_LIGHTING);

        gl.glBegin(GL_QUADS);
        // Front Face
        gl.glNormal3f(0.0f, 0.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        // Back Face
        gl.glNormal3f(0.0f, 0.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        // Top Face
        gl.glNormal3f(0.0f, 1.0f, 0.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        // Bottom Face
        gl.glNormal3f(0.0f, -1.0f, 0.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        // Right face
        gl.glNormal3f(1.0f, 0.0f, 0.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        // Left Face
        gl.glNormal3f(-1.0f, 0.0f, 0.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glEnd();

        xrot += xspeed;
        yrot += yspeed;
    }

    /**
     * Called when the display mode has been changed.  <B>!! CURRENTLY UNIMPLEMENTED IN JOGL !!</B>
     *
     * @param glDrawable    The GLAutoDrawable object.
     * @param modeChanged   Indicates if the video mode has changed.
     * @param deviceChanged Indicates if the video device has changed.
     */
    public void displayChanged(GLAutoDrawable glDrawable, boolean modeChanged, boolean deviceChanged) {
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
        gl.glEnable(GL_TEXTURE_2D);

        TextureReader.Texture texture = null;
        try {
            texture = TextureReader.readTexture("demos/data/images/crate.png");
        } catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException(e);
        }

        gl.glGenTextures(3, this.textures, 0);

        // Create Nearest Filtered Texture
        gl.glBindTexture(GL_TEXTURE_2D, textures[0]);
        gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        makeRGBTexture(gl, glu, texture, GL_TEXTURE_2D, false);

        // Create Linear Filtered Texture
        gl.glBindTexture(GL_TEXTURE_2D, textures[1]);
        gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        makeRGBTexture(gl, glu, texture, GL_TEXTURE_2D, false);

        // Create MipMapped Texture
        gl.glBindTexture(GL_TEXTURE_2D, textures[2]);
        gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        makeRGBTexture(gl, glu, texture, GL_TEXTURE_2D, true);

        // Set up lighting
        gl.glLightfv(GL_LIGHT1, GL_AMBIENT, this.lightAmbient, 0);
        gl.glLightfv(GL_LIGHT1, GL_DIFFUSE, this.lightDiffuse, 0);
        gl.glLightfv(GL_LIGHT1, GL_POSITION, this.lightPosition, 0);
        gl.glEnable(GL_LIGHT1);
        gl.glEnable(GL_LIGHTING);
        this.lightingEnabled = true;
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

    private void makeRGBTexture(GL2 gl, GLU glu, TextureReader.Texture img, int target, boolean mipmapped) {
        if (mipmapped) {
            glu.gluBuild2DMipmaps(target, GL_RGB8, img.getWidth(), img.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, img.getPixels());
        } else {
            gl.glTexImage2D(target, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.getPixels());
        }
    }
}
