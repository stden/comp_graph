package demos.nehe.lesson03;

import demos.common.GLDisplay;

/**
 * ÷ветные треугольник и квадрат
 */
public class Lesson03 {
    public static void main(String[] args) {
        GLDisplay neheGLDisplay = GLDisplay.createGLDisplay("Lesson 03: Colors");
        neheGLDisplay.addGLEventListener(new Renderer());
        neheGLDisplay.start();
    }
}
