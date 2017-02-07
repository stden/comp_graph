package demos.nehe.lesson44;

import com.jogamp.common.nio.Buffers;

import javax.media.opengl.GL2;
import java.nio.ByteBuffer;

import static javax.media.opengl.GL2.*;

/**
 * I don't mind if you use this class in your own code. All I ask is
 * that you give me and Giuseppe D'Agata credit for it if you do.
 * And plug NeHe while your at it! :P  Thanks go to Giuseppe D'Agata
 * for the code that this class is based off of. Thanks Enjoy.
 *
 * @author Vic Hollis
 * @author Abdul Bezrati
 */
class Font {
    private double m_WindowHeight;
    private double m_WindowWidth;
    private int m_FontTexture;
    private int m_ListBase;
    private ByteBuffer stringBuffer = Buffers.newDirectByteBuffer(512);

    public Font() {
        m_FontTexture = 0;    // Initalize the texture to 0
        m_ListBase = 0;    // Initalize the List base to 0
    }

    public void setFontTexture(int tex) {
        if (tex != 0) {          // If the texture is valid
            m_FontTexture = tex; // Set the font texture
        }
    }

    public void buildFont(GL2 gl, float scale) {
        m_ListBase = gl.glGenLists(256);                                // Creating 256 Display Lists
        if (m_FontTexture != 0) {
            gl.glBindTexture(GL_TEXTURE_2D, m_FontTexture);            // Select Our Font Texture
            for (int loop = 0; loop < 256; loop++) {                             // Loop Through All 256 Lists
                float cx = (float) (loop % 16) / 16.0f;                              // X Position Of Current Character
                float cy = (float) (loop / 16) / 16.0f;                              // Y Position Of Current Character

                gl.glNewList(m_ListBase + loop, GL_COMPILE);              // Start Building A List
                gl.glBegin(GL_QUADS);                                // Use A Quad For Each Character
                gl.glTexCoord2f(cx, 1 - cy - 0.0625f);                  // Texture Coord (Bottom Left)
                gl.glVertex2f(0, 0);                                     // Vertex Coord (Bottom Left)
                gl.glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);        // Texture Coord (Bottom Right)
                gl.glVertex2f(16 * scale, 0);                            // Vertex Coord (Bottom Right)
                gl.glTexCoord2f(cx + 0.0625f, 1 - cy);                  // Texture Coord (Top Right)
                gl.glVertex2f(16 * scale, 16 * scale);                  // Vertex Coord (Top Right)
                gl.glTexCoord2f(cx, 1 - cy);                            // Texture Coord (Top Left)
                gl.glVertex2f(0, 16 * scale);                           // Vertex Coord (Top Left)
                gl.glEnd();                                               // Done Building Our Quad (Character)
                gl.glTranslated(10 * scale, 0, 0);                            // Move To The Right Of The Character
                gl.glEndList();                                           // Done Building The Display List
            }                                                           // Loop Until All 256 Are Built
        }
    }

    public void glPrintf(GL2 gl, int x, int y, int set, String format) {
        if (format == null)                                              // If There's No Text
            return;

        byte text[] = format.getBytes();

        if (set > 1)                                                       // Did User Choose An Invalid Character Set?
            set = 1;

        gl.glEnable(GL_TEXTURE_2D);                                  // Enable 2d Textures
        gl.glEnable(GL_BLEND);                                       // Enable Blending
        gl.glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
        gl.glBindTexture(GL_TEXTURE_2D, m_FontTexture);              // Select Our Font Texture
        gl.glDisable(GL_DEPTH_TEST);                                 // Disables Depth Testing
        gl.glMatrixMode(GL_PROJECTION);                              // Select The Projection Matrix
        gl.glPushMatrix();                                              // Store The Projection Matrix
        gl.glLoadIdentity();                                            // Reset The Projection Matrix
        gl.glOrtho(0, m_WindowWidth, 0, m_WindowHeight, -1, 1);              // Set Up An Ortho Screen
        gl.glMatrixMode(GL_MODELVIEW);                               // Select The Modelview Matrix
        gl.glPushMatrix();                                              // Store The Modelview Matrix
        gl.glLoadIdentity();                                            // Reset The Modelview Matrix
        gl.glTranslated(x, y, 0);                                         // Position The Text (0,0 - Bottom Left)
        gl.glListBase(m_ListBase - 32 + (128 * set));                         // Choose The Font Set (0 or 1)

        if (stringBuffer.capacity() < format.length()) {
            stringBuffer = Buffers.newDirectByteBuffer(format.length());
        }

        stringBuffer.clear();
        stringBuffer.put(format.getBytes());
        stringBuffer.flip();

        gl.glCallLists(text.length, GL_BYTE, stringBuffer);                    // Write The Text To The Screen
        gl.glMatrixMode(GL_PROJECTION);                              // Select The Projection Matrix
        gl.glPopMatrix();                                               // Restore The Old Projection Matrix
        gl.glMatrixMode(GL_MODELVIEW);                               // Select The Modelview Matrix
        gl.glPopMatrix();                                               // Restore The Old Projection Matrix
        gl.glEnable(GL_DEPTH_TEST);
        gl.glDisable(GL_BLEND);
        gl.glDisable(GL_TEXTURE_2D);
    }

    public void setWindowSize(int width, int height) {
        m_WindowWidth = width;                                         // Set the window size width
        m_WindowHeight = height;                                        // Set the window size height
    }
}
