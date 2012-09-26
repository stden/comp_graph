unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, OpenGL, StdCtrls, Grids, Spin, ComCtrls;

type
  TfrmMain = class(TForm)
    Panel1: TPanel;
    Timer1: TTimer;
    Label1: TLabel;
    cbAnimation: TCheckBox;
    ObjColorShape: TShape;
    ColorDialog1: TColorDialog;
    Label4: TLabel;
    edtEyeX: TEdit;
    Label5: TLabel;
    Label7: TLabel;
    edtEyeY: TEdit;
    Label8: TLabel;
    Label9: TLabel;
    edtEyeZ: TEdit;
    Label10: TLabel;
    Label11: TLabel;
    edtCenterX: TEdit;
    Label12: TLabel;
    Label13: TLabel;
    edtCenterZ: TEdit;
    Label14: TLabel;
    edtCenterY: TEdit;
    Label15: TLabel;
    Label16: TLabel;
    Label17: TLabel;
    Label18: TLabel;
    edtUpX: TEdit;
    edtUpZ: TEdit;
    edtUpY: TEdit;
    ScrollBox1: TScrollBox;
    Label20: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure cbAnimationClick(Sender: TObject);
    procedure ObjColorShapeMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure seFovChange(Sender: TObject);
    procedure edtEyeXChange(Sender: TObject);
    procedure edtEyeYChange(Sender: TObject);
    procedure edtEyeZChange(Sender: TObject);
    procedure edtCenterXChange(Sender: TObject);
    procedure edtCenterYChange(Sender: TObject);
    procedure edtCenterZChange(Sender: TObject);
    procedure edtUpXChange(Sender: TObject);
    procedure edtUpYChange(Sender: TObject);
    procedure edtUpZChange(Sender: TObject);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure BackColorShapeMouseDown(Sender: TObject;
      Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure tbDetailsChange(Sender: TObject);
  private
  public
    eyex, eyey, eyez,
    centerx, centery, centerz,
    upx, upy, upz,
    Omega:Double;
    ObjectColor, BackColor:TColor;
    GridSize,
    OldMsX,OldMsY:Integer;
    procedure Draw;
    procedure InitGl;
    procedure SelectColor(var c:TColor);
    procedure SetupParam(s:String; var p:Double);
    procedure ParamToFields;
    procedure RotateCamera(alpha, beta, gamma:Double);
    procedure MoveCameraForward(v:Double);
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}
var  pfd:PIXELFORMATDESCRIPTOR=
	(
          nSize:SizeOf(PIXELFORMATDESCRIPTOR);
          nVersion:1;
	  dwFlags:PFD_DRAW_TO_WINDOW or PFD_SUPPORT_OPENGL or PFD_DOUBLEBUFFER;
	  iPixelType:PFD_TYPE_RGBA;
	  cColorBits:24;
	  cRedBits:0;
          cRedShift:0;
          cGreenBits:0;
          cGreenShift:0;
          cBlueBits:0;
          cBlueShift:0;
	  cAlphaBits:0;
          cAlphaShift:0;
	  cAccumBits:0;
          cAccumRedBits:0;
          cAccumGreenBits:0;
          cAccumBlueBits:0;
          cAccumAlphaBits:0;
	  cDepthBits:32;
	  cStencilBits:0;
	  cAuxBuffers:0;
	  iLayerType:PFD_MAIN_PLANE;
	  bReserved:0;
	  dwLayerMask:0;
          dwVisibleMask:0;
          dwDamageMask:0
	);
        hglrc:THandle;

procedure setDCPixelFormat(hdc:HDC);
var pixelFormat:INteger;
begin
  pixelFormat := ChoosePixelFormat(hdc, @pfd);
  SetPixelFormat(hdc, pixelFormat, @pfd);
  DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR),
                      pfd);
  if(pfd.dwFlags and PFD_NEED_PALETTE>0) then
 	  MessageBox(NULL, 'Palette mode not supported!', 'Error', MB_OK);
end;


Type TVec = Array[0..3] of GLFloat;
     TVec3d = Array[0..3] of GLFloat;

var ambientLight:TVec = ( 0.3, 0.1, 0.1, 1.0 );
    diffuseLight:TVec = ( 0.8, 0.6, 0.9, 1.0 );
    lightPos:TVec = ( -25.0, -10.0, 10.0, 1.0 );

procedure TfrmMain.InitGl;
var aspect:Double;
begin
    { Значение очистки буфера глубены }
    glClearDepth( 1.0 );

   { разрешение использование тестирования глубины }
    glEnable(GL_DEPTH_TEST);

    { установка матрицы проецирования }
    glMatrixMode( GL_PROJECTION );
    aspect := (width -panel1.Width)/ height;
    gluPerspective(45, aspect, 1.0, 10.0 );

    glMatrixMode( GL_MODELVIEW );

   { включение освещения }
    glEnable(GL_LIGHTING);

    { установка одного источника света }
    glLightfv(GL_LIGHT0,GL_AMBIENT,@ambientLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,@diffuseLight);
    glLightfv(GL_LIGHT0,GL_POSITION,@lightPos);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
end;

procedure TfrmMain.FormCreate(Sender: TObject);
var dc:HDC;
    wnd:HWND;
begin
 DecimalSeparator:='.';

 omega:=0;
 GridSize:=50;
 // tbDetails.Position:=GridSize;
 ObjectColor:=ObjColorShape.Brush.Color;
 { начально положение глаза наблюдателя }
 eyex:=-3.0; eyey:=-3.0; eyez:=3.0;
 { точка наблюдения }
 centerx:=0; centery:=0; centerz:=0;
 { направление оси Y на проекционной плоскости }
 upx:=0.0; upy:=0.0; upz:=1.0;

 dc:=frmMain.GetDeviceContext(wnd);

 setDCPixelFormat(dc);

 hglrc := wglCreateContext(dc);

 wglMakeCurrent(DC, hglRC);

 InitGl;

 ReleaseDC(wnd, dc);

 ParamToFields;
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
 wglMakeCurrent(0, 0);
 wglDeleteContext(hglrc);
end;


procedure TfrmMain.FormResize(Sender: TObject);
var dc:HDC;
    wnd:HWND;
    aspect:GLfloat;
begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);
  { установка новой области отображения OpenGl }
  glViewport(GLsizei(Panel1.Width+1),0,
             GLsizei(ClientWidth-Panel1.Width-2),
             GLsizei(ClientHeight-2));

  { вычисление нового соотвношения ширены к высоте видемой области }
  aspect :=  (ClientWidth-Panel1.Width-2) / (Clientheight-2);

  { соответствующее изменение матрицы проецирования }
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45, aspect, 1.0, 10.0 );

  glMatrixMode( GL_MODELVIEW );
  wglMakeCurrent(0, 0);
  ReleaseDC(wnd, dc);
  { вызов процедуры рисования сцены }
  Draw;
end;

procedure TfrmMain.FormPaint(Sender: TObject);
begin
 Draw;
end;

var z:Array[0..127,0..127] of record
         val,nx,ny,nz:Double;
      end;

procedure TfrmMain.Draw;
var dc:HDC;
    wnd:HWND;
    x,y,step,len,r:Double;
    i,j:Integer;
begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);

  glClearColor((BackColor and 255)/255,
               ((BackColor shr 8) and 255)/255,
               ((BackColor shr 16) and 255)/255,
                1.0 );

  glClearColor(0,0,0,0);

  glEnable(GL_DEPTH_TEST);    // Включаем тест глубины

  glClear( GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );

  glPushMatrix();
  { установка камеры }
  gluLookAt(eyex, eyey, eyez,
            centerx, centery, centerz,
            upx, upy, upz);

 { построенние матриц высот и составляющих вектора нормали в
   матрица квадратная размера GridCount X GridCount}
 step:=1/GridSize; y:=0;
 for i:=0 to GridSize-1 do begin
  x:=0;
  for j:=0 to GridSize-1 do with z[i,j] do begin
   r:=sqrt(sqr(x-0.5)+sqr(y-0.4));
   val:=0.9*cos(2.0*(r-omega)); { высота в данной точке }
   r:=-5.0*(sin(6.0*(r-omega))+sin(1.0*(r-omega)))/r;
   nx:=-r*(x-0.2);
   ny:=-r*(y-0.4);
   len:=sqrt(sqr(nx)+sqr(ny)+4.0);
   nx:=nx/len; { состовляющие новрмали в данной точке }
   ny:=ny/len;
   nz:=1.0/len;
   x:=x+step;
  end;
  y:=y+step;
 end;

 glColor3ub(ObjectColor,
            ObjectColor shr 8,
            ObjectColor shr 16);

 { аппроксимация поверхности треугольниками }
 glBegin(gl_Triangles);
 y:=-1; step:=step*2;
  for i:=0 to GridSize-2 do begin
    x:=-1;
    for j:=0 to GridSize-2 do begin
      with z[i,j] do begin glNormal3d(nx,ny,nz); glVertex3d(x,y,val); end;
      with z[i,j+1] do begin glNormal3d(nx,ny,nz); glVertex3d(x+step,y,val); end;
      with z[i+1,j] do begin glNormal3d(nx,ny,nz); glVertex3d(x,y+step,val); end;

      with z[i+1,j] do begin glNormal3d(nx,ny,nz); glVertex3d(x,y+step,val); end;
      with z[i,j+1] do begin glNormal3d(nx,ny,nz); glVertex3d(x+step,y,val); end;
      with z[i+1,j+1] do begin glNormal3d(nx,ny,nz); glVertex3d(x+step,y+step,val); end;

      x:=x+step;
    end;
    y:=y+step;
  end;
  glEnd;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  { Рисуем пирамиду }
  glBegin(gl_Triangles);
  {---}
  glColor4f(1.0,0.0,0.0,0.3);                      // Красный
  glVertex3f( 0.0, 1.0, 0.0);                  // Верх треугольника (Передняя)
  glColor3f(0.0,1.0,0.0);                      // Зеленный
  glVertex3f(-1.0,-1.0, 1.0);                  // Левая точка
  glColor3f(0.0,0.0,1.0);                      // Синий
  glVertex3f( 1.0,-1.0, 1.0);                  // Правая точка
  {---}
  glColor4f(1.0,0.0,0.0,0.4);                      // Красная
  glVertex3f( 0.0, 1.0, 0.0);                  // Верх треугольника (Правая)
  glColor3f(0.0,0.0,1.0);                      // Синия
  glVertex3f( 1.0,-1.0, 1.0);                  // Лево треугольника (Правая)
  glColor3f(0.0,1.0,0.0);                      // Зеленная
  glVertex3f( 1.0,-1.0, -1.0);                 // Право треугольника (Правая)
  {---}
  glColor4f(1.0,0.0,0.0,0.2);                      // Красный
  glVertex3f( 0.0, 1.0, 0.0);                  // Низ треугольника (Сзади)
  glColor3f(0.0,1.0,0.0);                      // Зеленный
  glVertex3f( 1.0,-1.0, -1.0);                 // Лево треугольника (Сзади)
  glColor3f(0.0,0.0,1.0);                      // Синий
  glVertex3f(-1.0,-1.0, -1.0);                 // Право треугольника (Сзади
  {---}
  glColor4f(1.0,0.0,0.0,0.1);                      // Красный
  glVertex3f( 0.0, 1.0, 0.0);                  // Верх треугольника (Лево)
  glColor3f(0.0,0.0,1.0);                      // Синий
  glVertex3f(-1.0,-1.0,-1.0);                  // Лево треугольника (Лево)
  glColor3f(0.0,1.0,0.0);                      // Зеленный
  glVertex3f(-1.0,-1.0, 1.0);                  // Право треугольника (Лево)
  {---}
  glEnd;

  { Рисуем кубик }
  glBegin(GL_QUADS);                      // Рисуем куб
  {---}
  glColor4f(0.0,1.0,0.0,0.25);              // Синий
  glVertex3f( 1.0, 1.0,-1.0);          // Право верх квадрата (Верх)
  glVertex3f(-1.0, 1.0,-1.0);          // Лево верх
  glVertex3f(-1.0, 1.0, 1.0);          // Лево низ
  glVertex3f( 1.0, 1.0, 1.0);          // Право низ
  {---}
  glColor4f(1.0,0.0,0.0,0.25);              // Красный
  glVertex3f( 1.0, 1.0, 1.0);          // Верх право квадрата (Перед)
  glVertex3f(-1.0, 1.0, 1.0);          // Верх лево
  glVertex3f(-1.0,-1.0, 1.0);          // Низ лево
  glVertex3f( 1.0,-1.0, 1.0);          // Низ право
  {---}
  glColor4f(1.0,1.0,0.0,0.25);              // Желтый
  glVertex3f( 1.0,-1.0,-1.0);          // Верх право квадрата (Зад)
  glVertex3f(-1.0,-1.0,-1.0);          // Верх лево
  glVertex3f(-1.0, 1.0,-1.0);          // Низ лево
  glVertex3f( 1.0, 1.0,-1.0);          // Низ право
  {---}
  glColor4f(0.0,0.0,1.0,0.25);              // Синий
  glVertex3f(-1.0, 1.0, 1.0);          // Верх право квадрата (Лево)
  glVertex3f(-1.0, 1.0,-1.0);          // Верх лево
  glVertex3f(-1.0,-1.0,-1.0);          // Низ лево
  glVertex3f(-1.0,-1.0, 1.0);          // Низ право
  {---}
  glColor4f(1.0,0.0,1.0,0.25);              // Фиолетовый
  glVertex3f( 1.0, 1.0,-1.0);          // Верх право квадрата (Право)
  glVertex3f( 1.0, 1.0, 1.0);          // Верх лево
  glVertex3f( 1.0,-1.0, 1.0);          // Низ лево
  glVertex3f( 1.0,-1.0,-1.0);          // Низ право
  glEnd();                                // Закончили квадраты

  glDisable(GL_BLEND);

  glPopMatrix();

  glFlush();

  SwapBuffers(dc);
  ReleaseDC(wnd, dc);

  wglMakeCurrent(0, 0);
end;

procedure TfrmMain.Timer1Timer(Sender: TObject);
begin
  omega:=omega+0.02;
  if omega>2*pi then omega:=omega-2*pi;
  Draw;
end;

procedure TfrmMain.cbAnimationClick(Sender: TObject);
begin
 Timer1.Enabled := cbAnimation.Checked;
end;

procedure TfrmMain.SelectColor(Var c:TColor);
begin
 with ColorDialog1 do
  if Execute then c:=Color;
end;

procedure TfrmMain.ObjColorShapeMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
 SelectColor(ObjectColor);
 ObjColorShape.Brush.Color:=ObjectColor;
 if not cbAnimation.Checked then Draw;
end;

procedure TfrmMain.seFovChange(Sender: TObject);
begin
 OnResize(Sender);
end;

procedure TfrmMain.SetupParam(s:String; var p:Double);
begin
 try
  p:=StrToFloat(s);
  Draw;
 except
 end;
end;

procedure TfrmMain.edtEyeXChange(Sender: TObject);
begin
 SetupParam(edtEyeX.Text, EyeX);
end;

procedure TfrmMain.edtEyeYChange(Sender: TObject);
begin
 SetupParam(edtEyeY.Text, EyeY);
end;

procedure TfrmMain.edtEyeZChange(Sender: TObject);
begin
 SetupParam(edtEyeZ.Text, EyeZ);
end;

procedure TfrmMain.edtCenterXChange(Sender: TObject);
begin
 SetupParam(edtCenterX.Text, CenterX);
end;

procedure TfrmMain.edtCenterYChange(Sender: TObject);
begin
 SetupParam(edtCenterY.Text, CenterY);
end;

procedure TfrmMain.edtCenterZChange(Sender: TObject);
begin
  SetupParam(edtCenterZ.Text, CenterZ);
end;

procedure TfrmMain.edtUpXChange(Sender: TObject);
begin
  try
    SetupParam(edtUpX.Text, UpX);
  except
  end;
end;

procedure TfrmMain.edtUpYChange(Sender: TObject);
begin
  try
    SetupParam(edtUpY.Text, UpY);
  except
  end;
end;

procedure TfrmMain.edtUpZChange(Sender: TObject);
begin
  try
    SetupParam(edtUpZ.Text, UpZ);
  except
  end;
end;

procedure TfrmMain.ParamToFields;
begin
  edtEyeX.Text := FloatToStrF(EyeX, ffFixed, 10, 3);
  edtEyeY.Text := FloatToStrF(EyeY, ffFixed, 10, 3);
  edtEyeZ.Text := FloatToStrF(EyeZ, ffFixed, 10, 3);
  edtCenterX.Text := FloatToStrF(CenterX, ffFixed, 10, 3);
  edtCenterY.Text := FloatToStrF(CenterY, ffFixed, 10, 3);
  edtCenterZ.Text := FloatToStrF(CenterZ, ffFixed, 10, 3);
  edtUpX.Text := FloatToStrF(UpX, ffFixed, 10, 3);
  edtUpY.Text := FloatToStrF(UpY, ffFixed, 10, 3);
  edtUpZ.Text := FloatToStrF(UpZ, ffFixed, 10, 3);
end;

procedure Rotate(var x,y:Double; alpha:Double);
var t:Double;
begin
  t:=x*cos(alpha)-y*sin(alpha);
  y:=x*sin(alpha)+y*cos(alpha);
  x:=t;
end;

procedure TfrmMain.RotateCamera(alpha, beta, gamma:Double);
begin
  Rotate(EyeX,EyeY, alpha);
  Rotate(EyeX,EyeZ, beta);
end;

procedure TfrmMain.MoveCameraForward(v:Double);
var vx,vy,vz,l:Double;
begin
  vx:=CenterX-Eyex;
  vy:=CenterY-EyeY;
  vz:=CenterZ-EyeZ;
  l:=sqrt(sqr(vx)+sqr(vy)+sqr(vz));
  EyeX:=EyeX+vx*v/l;
  EyeY:=EyeY+vy*v/l;
  EyeZ:=EyeZ+vz*v/l;
end;

procedure TfrmMain.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if ssLeft in Shift then begin
    RotateCamera((x-OldMsx)/200,-(y-OldMsY)/200,0);
    Draw;
    ParamToFields;
  end;
  if ssRight in Shift then begin
    MoveCameraForward((y-OldMsY)/200);
    Rotate(UpX,UpZ,(x-OldMsX)/200);
    Draw;
    ParamToFields;
  end;
  OldMsX:=X; OldMsY:=Y;
end;

procedure TfrmMain.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  OldMsX:=x; OldMsY:=y;
end;


procedure TfrmMain.BackColorShapeMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  SelectColor(BackColor);
  if not cbAnimation.Checked then Draw;
end;

procedure TfrmMain.tbDetailsChange(Sender: TObject);
begin
//  GridSize:=tbDetails.Position;
  if not cbAnimation.Checked then Draw;
end;

end.
