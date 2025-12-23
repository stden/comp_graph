SET PATH=%PATH%;C:\CodeBlocks\MinGW\bin
for /D %%i in (01,02,03,05,06,07,09,11,12,13,15,16,17,19,20,34) do ( 
  c++ nehe-ogl%%i.cpp -o "nehe-ogl%%i.exe" -lglaux -lopengl32 -lglu32 -mwindows -lgmon
)
for /D %%i in (01) do ( 
  c++ nehe-ogl%%i.cpp -o "nehe-ogl%%i.exe" -lglaux -lopengl32 -lglu32 -mwindows -lgmon
)