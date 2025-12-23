unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, OpenGL, StdCtrls, Grids, Spin, ComCtrls, Buttons,
  Menus;

type
  TF = class(TForm)
    Panel1: TPanel;
    Label4: TLabel;
    seLineWidth: TSpinEdit;
    StatusBar1: TStatusBar;
    MainMenu1: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    cbShowT: TCheckBox;
    cbShowL9: TCheckBox;
    Label1: TLabel;
    L9_Depth: TSpinEdit;
    cbTRandomFractal: TCheckBox;
    L9_Panel: TPanel;
    T_Panel: TPanel;
    T_Depth: TSpinEdit;
    Label2: TLabel;
    maxRandomDepth: TSpinEdit;
    ReDraw: TBitBtn;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure PointsGrid_SetEditText(Sender: TObject; ACol, ARow: Integer;
      const Value: String);
    procedure cbStyleChange(Sender: TObject);
    procedure sePointSizeChange(Sender: TObject);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure cbNumsClick(Sender: TObject);
    procedure N4Click(Sender: TObject);
    procedure cbShowL9Click(Sender: TObject);
    procedure cbShowTClick(Sender: TObject);
    procedure L9_DepthChange(Sender: TObject);
    procedure ReDrawClick(Sender: TObject);
  public
    procedure Draw;
    { X,Y нижней точки T-шки, S-высота T-шки }
    procedure T( X,Y,S : Double; Dir,Rec:Integer );
    { X1,Y1,X2,Y2 }
    procedure L9( X1,Y1,X2,Y2 : Double; Rec:Integer );
  end;

var
  F: TF;

 { Описание точек }
  points : Array [1..1000] of record x,y,r,g,b:Double; end;
  points_num : Integer;

implementation

uses AboutUnit;

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

procedure TF.FormCreate(Sender: TObject);
var dc:HDC;
    wnd:HWND;
begin
  randomize;
  {}
  FormatSettings.DateSeparator:='.';
  Canvas.Brush.Style := bsClear;
  dc:=F.GetDeviceContext(wnd);
  { установка "правильного" формата битовой плоскости окна }
  setDCPixelFormat(dc);
  { создание контекста OpenGL }
  hglrc := wglCreateContext(dc);
  ReleaseDC(wnd, dc);
end;

procedure TF.FormDestroy(Sender: TObject);
begin
  wglMakeCurrent(0, 0);
  wglDeleteContext(hglrc);
end;


procedure TF.FormResize(Sender: TObject);
var dc:HDC;
    wnd:HWND;
begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);
  { установка области отображения для OpenGL }
  glViewport(GLsizei(Panel1.Width+1),StatusBar1.Height,
             GLsizei(ClientWidth-Panel1.Width-2),
             GLsizei(ClientHeight-StatusBar1.Height-2));
  ReleaseDC(wnd, dc);
  wglMakeCurrent(0, 0);
  Draw;
end;

procedure TF.FormPaint(Sender: TObject);
begin
  Draw;
end;

procedure TF.PointsGrid_SetEditText(Sender: TObject; ACol,
  ARow: Integer; const Value: String);
begin
  try
    with Points[Arow] do
      if ACol=1 then x:=StrToFloat(Value)
             else y:=StrToFloat(Value);
    Invalidate;
  except
  end;
end;

procedure TF.cbStyleChange(Sender: TObject);
begin
  Draw;
end;

procedure TF.sePointSizeChange(Sender: TObject);
begin
  Draw;
end;

procedure TF.Draw;
var dc:HDC;
    wnd:HWND;
begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);

  { Установка параметров примитивов }
  glLineWidth(seLineWidth.Value);

  if cbShowT.Checked then
    T(0,-0.9,0.8,+1,0);

  if cbShowL9.Checked then
    L9(-0.9,-0.8,0.9,0.8,0);

  glFlush();

  SwapBuffers(dc);

  ReleaseDC(wnd, dc);

  wglMakeCurrent(0, 0);
end;

procedure TF.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
 with StatusBar1.Panels[0] do
    text:='X='+FloatToStrF((x-Panel1.Width)/(ClientWidth-Panel1.Width)*2-1,ffFixed,1,2)+', Y='+
          FloatToStrF(y/(ClientHeight-StatusBar1.Height)*2-1,ffFixed,1,2);
end;

procedure TF.cbNumsClick(Sender: TObject);
begin
 Draw;
end;

procedure TF.N4Click(Sender: TObject);
begin
  FAbout.Show;
end;

procedure TF.T(X, Y, S: Double; Dir, Rec: Integer);
var D1,D2 : Integer;
begin
  if Rec >= T_Depth.Value then Exit;
  {}
  glBegin(GL_LINES);
  glColor(Random(256),Random(256),Random(256));
  {}
  glVertex2d(X,Y);
  glVertex2d(X,Y+Dir*S);
  {}
  glVertex2d(X-S*0.6,Y+Dir*S);
  glVertex2d(X+S*0.6,Y+Dir*S);
  {}
  glEnd;
  {}
  if cbTRandomFractal.Checked then begin
    D1 := random(2)*2-1;
    D2 := random(2)*2-1;
    if Rec>=maxRandomDepth.Value then begin
      D1 := Dir; D2 := Dir;
    end;
  end else begin
    D1 := Dir;  D2 := Dir;
  end;
  {}
  T(X-S*0.6,Y+Dir*S,S*0.5,D1,Rec+1);
  T(X+S*0.6,Y+Dir*S,S*0.5,D2,Rec+1);
end;

procedure TF.L9(X1, Y1, X2, Y2: Double; Rec: Integer);
var VX,VY,PX,PY : Double;
begin
  if Rec >= L9_Depth.Value then Exit;
  {}
  glBegin(GL_LINES);
  glColor(Random(256),Random(256),Random(256));
  {}
  glVertex2d(X1,Y1);
  glVertex2d(X2,Y2);
  {Посчитаем вектор в 3 раза короче}
  VX := (X2-X1)/3;
  VY := (Y2-Y1)/3;
  PX := -VY;
  PY := VX;
  { Отрезки по линии }
  L9(X1,Y1,X1+VX,Y1+VY,Rec+1);
  L9(X1+VX,Y1+VY,X1+2*VX,Y1+2*VY,Rec+1);
  L9(X1+VX*2,Y1+VY*2,X2,Y2,Rec+1);
  { Перпендикулярные отрезки }
  L9( X1+VX, Y1+VY, X1+VX+PX, Y1+VY+PY, Rec+1 );
  L9( X1+2*VX, Y1+2*VY, X1+2*VX+PX, Y1+2*VY+PY, Rec+1 );
  L9( X1+VX+PX, Y1+VY+PY, X1+2*VX+PX, Y1+2*VY+PY, Rec+1 );
  {}
  L9( X1+VX, Y1+VY, X1+VX-PX, Y1+VY-PY, Rec+1 );
  L9( X1+2*VX, Y1+2*VY, X1+2*VX-PX, Y1+2*VY-PY, Rec+1 );
  L9( X1+VX-PX, Y1+VY-PY, X1+2*VX-PX, Y1+2*VY-PY, Rec+1 );
  {}
  glEnd;
end;

procedure TF.cbShowL9Click(Sender: TObject);
begin
  Draw;
end;

procedure TF.cbShowTClick(Sender: TObject);
begin
  Draw;
end;

procedure TF.L9_DepthChange(Sender: TObject);
begin
  Draw;
end;

procedure TF.ReDrawClick(Sender: TObject);
begin
  Draw;
end;

end.
