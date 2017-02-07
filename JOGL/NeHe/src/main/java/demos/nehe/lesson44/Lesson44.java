package demos.nehe.lesson44;

import demos.common.GLDisplay;
                  /*--.          .-"-.
                 /   o_O        / O o \
                 \_  (__\       \_ v _/
                 //   \\        //   \\
                ((     ))      ((     ))
 ��������������--""---""--����--""---""--��������������������������
 �                 |||            |||                             �
 �                  |              |                              �
 �                                                                �
 � Programmer:Abdul Bezrati                                       �
 � Program   :Nehe's 44th lesson port to JOGL                     �
 � Comments  :None                                                �
 �    _______                                                     �
 �  /` _____ `\;,    abezrati@hotmail.com                         �
 � (__(^===^)__)';,                                 ___           �
 �   /  :::  \   ,;                               /^   ^\         �
 �  |   :::   | ,;'                              ( �   � )        �
 ���'._______.'`��������������������������� --�oOo--(_)--oOo�--��*/
public class Lesson44 {
    public static void main(String[] args) {
        GLDisplay neheGLDisplay = GLDisplay.createGLDisplay("Lesson 44: Lens flare");
        Renderer renderer = new Renderer();
        InputHandler inputHandler = new InputHandler(renderer, neheGLDisplay);
        neheGLDisplay.addGLEventListener(renderer);
        neheGLDisplay.addKeyListener(inputHandler);
        neheGLDisplay.start();
    }
}
