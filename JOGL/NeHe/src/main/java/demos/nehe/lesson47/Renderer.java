package demos.nehe.lesson47;

import com.jogamp.common.nio.Buffers;
import demos.common.ResourceRetriever;

import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.glu.GLU;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringWriter;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import static javax.media.opengl.GL.GL_COLOR_BUFFER_BIT;
import static javax.media.opengl.GL.GL_DEPTH_BUFFER_BIT;
import static javax.media.opengl.GL.GL_EXTENSIONS;
import static javax.media.opengl.GL2.GL_DEPTH_TEST;
import static javax.media.opengl.GL2.GL_FRONT_AND_BACK;
import static javax.media.opengl.GL2.GL_LEQUAL;
import static javax.media.opengl.GL2.*;
import static javax.media.opengl.GL2.GL_NICEST;
import static javax.media.opengl.GL2.GL_TRIANGLE_STRIP;
import static javax.media.opengl.GL2ES1.GL_PERSPECTIVE_CORRECTION_HINT;
import static javax.media.opengl.fixedfunc.GLLightingFunc.GL_SMOOTH;

class Renderer implements GLEventListener {
    // User Defined Variables
    private static final int SIZE = 64;
    private static final float TWO_PI = (float) (Math.PI * 2);

    private boolean vertexShaderSupported;
    private boolean vertexShaderEnabled;

    private int programObject;
    private int waveAttrib;

    private float[][][] mesh = new float[SIZE][SIZE][3];
    private float wave_movement = 0.0f;
    private GLU glu = new GLU();

    public Renderer() {
    }

    public void toggleShader() {
        if (vertexShaderSupported) {
            vertexShaderEnabled = !vertexShaderEnabled;
        }
    }

    public void init(GLAutoDrawable drawable) {
        //drawable.setGL(new DebugGL(drawable.getGL()));
        GL2 gl = drawable.getGL().getGL2();

        String extensions = gl.glGetString(GL_EXTENSIONS);
        vertexShaderSupported = extensions.contains("GL_ARB_vertex_shader");
        vertexShaderEnabled = true;

        if (vertexShaderSupported) {
            String shaderSource;

            try {
                BufferedReader shaderReader = new BufferedReader(
                        new InputStreamReader(
                                ResourceRetriever.getResourceAsStream(
                                        "demos/data/shaders/Wave.glsl"
                                )
                        )
                );
                StringWriter shaderWriter = new StringWriter();

                String line = shaderReader.readLine();
                while (line != null) {
                    shaderWriter.write(line);
                    shaderWriter.write("\n");
                    line = shaderReader.readLine();
                }

                shaderSource = shaderWriter.getBuffer().toString();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

            int shader = gl.glCreateShaderObjectARB(GL_VERTEX_SHADER);

            gl.glShaderSourceARB(shader, 1, new String[]{shaderSource}, (int[]) null, 0);

            gl.glCompileShaderARB(shader);
            checkLogInfo(gl, shader);

            programObject = gl.glCreateProgramObjectARB();

            gl.glAttachObjectARB(programObject, shader);
            gl.glLinkProgramARB(programObject);
            gl.glValidateProgramARB(programObject);
            checkLogInfo(gl, programObject);

            waveAttrib = gl.glGetAttribLocation(programObject, "wave");
        }

        // Create Our Mesh
        for (int x = 0; x < SIZE; x++) {
            for (int z = 0; z < SIZE; z++) {
                mesh[x][z][0] = (float) (SIZE / 2) - x;                        // We Want To Center Our Mesh Around The Origin
                mesh[x][z][1] = 0.0f;                                        // Set The Y Values For All Points To 0
                mesh[x][z][2] = (float) (SIZE / 2) - z;                        // We Want To Center Our Mesh Around The Origin
            }
        }

        // Setup GL States
        gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                        // Black Background
        gl.glClearDepth(1.0f);                                        // Depth Buffer Setup
        gl.glDepthFunc(GL_LEQUAL);                                    // The Type Of Depth Testing (Less Or Equal)
        gl.glEnable(GL_DEPTH_TEST);                                    // Enable Depth Testing
        gl.glShadeModel(GL_SMOOTH);                                    // Select Smooth Shading
        gl.glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);            // Set Perspective Calculations To Most Accurate
        gl.glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);                            // Draw Our Mesh In Wireframe Mode
    }

    @Override
    public void dispose(GLAutoDrawable glAutoDrawable) {
        System.out.println("Renderer.dispose");
    }

    public void display(GLAutoDrawable drawable) {
        GL2 gl = drawable.getGL().getGL2();

        gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear Screen And Depth Buffer
        gl.glLoadIdentity();                                            // Reset The Modelview Matrix

        // Position The Camera To Look At Our Mesh From A Distance
        glu.gluLookAt(0.0f, 25.0f, -45.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);

        if (vertexShaderEnabled) {
            gl.glUseProgramObjectARB(programObject);
        }

        // Start Drawing Our Mesh
        gl.glColor3f(0.5f, 1f, 0.5f);
        for (int x = 0; x < SIZE - 1; x++) {
            // Draw A Triangle Strip For Each Column Of Our Mesh
            gl.glBegin(GL_TRIANGLE_STRIP);
            for (int z = 0; z < SIZE - 1; z++) {
                // Set The Wave Parameter Of Our Shader To The Incremented Wave Value From Our Main Program
                if (vertexShaderEnabled) {
                    gl.glVertexAttrib1f(waveAttrib, wave_movement);
                }
                gl.glVertex3f(mesh[x][z][0], mesh[x][z][1], mesh[x][z][2]);        // Draw Vertex
                gl.glVertex3f(mesh[x + 1][z][0], mesh[x + 1][z][1], mesh[x + 1][z][2]);    // Draw Vertex
                wave_movement += 0.00001f;                                    // Increment Our Wave Movement
                if (wave_movement > TWO_PI) {                                // Prevent Crashing
                    wave_movement = 0.0f;
                }
            }
            gl.glEnd();
        }

        if (vertexShaderEnabled) {
            gl.glUseProgramObjectARB(0);
        }
    }

    public void reshape(GLAutoDrawable drawable,
                        int xstart,
                        int ystart,
                        int width,
                        int height) {
        GL2 gl = drawable.getGL().getGL2();

        height = (height == 0) ? 1 : height;

        gl.glViewport(0, 0, width, height);
        gl.glMatrixMode(GL_PROJECTION);
        gl.glLoadIdentity();

        glu.gluPerspective(45, (float) width / height, 1, 1000);
        gl.glMatrixMode(GL_MODELVIEW);
        gl.glLoadIdentity();
    }

    public void displayChanged(GLAutoDrawable drawable,
                               boolean modeChanged,
                               boolean deviceChanged) {
        System.out.println("drawable = [" + drawable + "], modeChanged = [" + modeChanged + "], deviceChanged = [" + deviceChanged + "]");
    }

    private void checkLogInfo(GL2 gl, int obj) {
        IntBuffer iVal = Buffers.newDirectIntBuffer(1);
        gl.glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, iVal);

        int length = iVal.get();

        if (length <= 1) {
            return;
        }

        ByteBuffer infoLog = Buffers.newDirectByteBuffer(length);

        iVal.flip();
        gl.glGetInfoLogARB(obj, length, iVal, infoLog);

        byte[] infoBytes = new byte[length];
        infoLog.get(infoBytes);
        System.out.println("GLSL Validation >> " + new String(infoBytes));
    }
}