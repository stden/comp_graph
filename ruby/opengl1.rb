require 'opengl'
require 'sdl'
# init
SDL.init(SDL::INIT_VIDEO)
SDL.setGLAttr(SDL::GL_DOUBLEBUFFER,1)
SDL.setVideoMode(512,512,32,SDL::OPENGL)
#...
Gl.glVertex3f(1.0,0.0,0.0)
#...
SDL.GLSwapBuffers()