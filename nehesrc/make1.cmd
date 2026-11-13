SET PATH=%PATH%;C:\CodeBlocks\MinGW\bin
for /D %%i in (01) do ( 
  c++ nehe-ogl%%i.cpp -o "nehe-ogl%%i.exe" -lglaux -lopengl32 -lglu32 -mwindows -lgmon
)