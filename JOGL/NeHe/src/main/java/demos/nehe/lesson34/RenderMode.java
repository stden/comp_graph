package demos.nehe.lesson34;

import static javax.media.opengl.GL2GL3.GL_LINES;
import static javax.media.opengl.GL2GL3.GL_QUADS;

class RenderMode {
    public static final RenderMode QUADS = new RenderMode(GL_QUADS);
    public static final RenderMode LINES = new RenderMode(GL_LINES);
    private int value;

    private RenderMode(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}
