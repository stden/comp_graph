unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, OpenGL, StdCtrls, Grids, Spin, ComCtrls;

type
  TfrmMain = class(TForm)
    Panel1: TPanel;
    StatusBar1: TStatusBar;
    ScrollBox1: TScrollBox;
    Panel3: TPanel;
    Label2: TLabel;
    cbAlpha: TCheckBox;
    cbAlphaFunc: TComboBox;
    edtRef: TEdit;
    Label3: TLabel;
    Label4: TLabel;
    cbSFactor: TComboBox;
    cbDFactor: TComboBox;
    cbBlend: TCheckBox;
    cbScissor: TCheckBox;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    edtSx: TEdit;
    edtSy: TEdit;
    edtSW: TEdit;
    edtSh: TEdit;
    Label9: TLabel;
    Panel4: TPanel;
    PointsGrid: TStringGrid;
    Label1: TLabel;
    ColorDialog1: TColorDialog;
    Label10: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    PointsShow: TComboBox;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure PointsGridSetEditText(Sender: TObject; ACol, ARow: Integer;
      const Value: String);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure cbNumsClick(Sender: TObject);
    procedure PointsGridDrawCell(Sender: TObject; ACol, ARow: Integer;
      Rect: TRect; State: TGridDrawState);
    procedure cbAlphaClick(Sender: TObject);
    procedure cbAlphaFuncChange(Sender: TObject);
    procedure cbBlendClick(Sender: TObject);
    procedure cbDFactorChange(Sender: TObject);
    procedure cbSFactorChange(Sender: TObject);
    procedure edtRefChange(Sender: TObject);
    procedure cbScissorClick(Sender: TObject);
    procedure edtSxChange(Sender: TObject);
    procedure edtSyChange(Sender: TObject);
    procedure edtSWChange(Sender: TObject);
    procedure edtShChange(Sender: TObject);
    procedure PointsGridSelectCell(Sender: TObject; ACol, ARow: Integer;
      var CanSelect: Boolean);
    procedure PointsGridDblClick(Sender: TObject);
    procedure PointsGridExit(Sender: TObject);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    points:Array[1..6] of record x,y,r,g,b,alpha:Double; end;
  public
    eyex, eyey, eyez,
    centerx, centery, centerz,
    upx, upy, upz,
    Omega:Double;
    OldMsX,OldMsY:Integer;
    procedure InitPoints;
    procedure ConvertFromTable;
    procedure ConvertToTable;
    procedure SetupPointsGrid;
    procedure SelectColor(i:Integer);
    procedure Draw;
    procedure RotateCamera(alpha, beta, gamma:Double);
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

procedure TfrmMain.FormCreate(Sender: TObject);
var dc:HDC;
    wnd:HWND;
 aspect:Double;
begin
  DecimalSeparator:='.';
  Canvas.Brush.Style := bsClear;
  dc:=frmMain.GetDeviceContext(wnd);
  setDCPixelFormat(dc);
  hglrc := wglCreateContext(dc);
  wglMakeCurrent(DC, hglRC);

   { Значение очистки буфера глубены }
    glClearDepth( 2 );

   { разрешение использование тестирования глубины }
    glEnable(GL_DEPTH_TEST);

    { установка матрицы проецирования }
    glShadeModel(GL_SMOOTH);        // разрешить плавное цветовое сглаживание
    glMatrixMode( GL_PROJECTION );
    aspect := (width -panel1.Width)/ height;
    gluPerspective(45, aspect, 2.0, 2.0 );

    glMatrixMode( GL_MODELVIEW );

   { включение освещения }
    glEnable(GL_LIGHTING);

    { установка одного источника света }
    glLightfv(GL_LIGHT0,GL_AMBIENT,@ambientLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,@diffuseLight);
    glLightfv(GL_LIGHT0,GL_POSITION,@lightPos);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

  glClearColor(0.0, 0.0, 0.0, 0.0);   // Очистка экрана в черный цвет

  ReleaseDC(wnd, dc);
  InitPoints;
  ConvertToTable;
  SetupPointsGrid;
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
 wglMakeCurrent(0, 0);
 wglDeleteContext(hglrc);
end;


procedure TfrmMain.FormResize(Sender: TObject);
var dc:HDC;
    wnd:HWND;
begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);
  glViewport(GLsizei(Panel1.Width+1),StatusBar1.Height,
             GLsizei(ClientWidth-Panel1.Width-2),
             GLsizei(ClientHeight-StatusBar1.Height-2));
  ReleaseDC(wnd, dc);
  wglMakeCurrent(0, 0);
  Draw;
end;

procedure TfrmMain.FormPaint(Sender: TObject);
begin
 Draw;
end;

procedure TfrmMain.ConvertFromTable;
var i:Integer;
begin
 with PointsGrid do
  for i:=1 to 12 do with points[i] do begin
   x:=StrToFloat(cells[1,i]);
   y:=StrToFloat(cells[2,i]);
   alpha:=StrToFloat(cells[4,i]);
  end;
end;

procedure TfrmMain.ConvertToTable;
var i:Integer;
begin
 with PointsGrid do
  for i:=1 to 6 do with points[i] do begin
   cells[1,i]:=FloatToStr(x);
   cells[2,i]:=FloatToStr(y);
   cells[4,i]:=FloatToStr(alpha);
  end;
end;

procedure TfrmMain.InitPoints;
var i:Integer;
begin
  with points[1] do begin x:=-0.8; y:=0.7; end;
  with points[2] do begin x:=0.6; y:=0.9; end;
  with points[3] do begin x:=0; y:=-0.3; end;
  with points[4] do begin x:=-0.9; y:=-0.6; end;
  with points[5] do begin x:=0.7; y:=-0.9; end;
  with points[6] do begin x:=0; y:=0.3; end;
  for i:=1 to 6 do with points[i] do begin
   r:=random(100)/100; g:=random(100)/100;  b:=random(100)/100;
   alpha:=random(100)/100;
  end;
end;

procedure TfrmMain.SetupPointsGrid;
var i:Integer;
begin
 with PointsGrid do begin
  RowCount:=7; ColCount:=5;
  Height:=RowCount*(DefaultRowHeight+1)+4;
  ColWidths[0]:=Trunc(0.15*ClientWidth);
  ColWidths[1]:=Trunc(0.225*ClientWidth)-1;
  ColWidths[2]:=Trunc(0.225*ClientWidth)-1;
  ColWidths[3]:=Trunc(0.1725*ClientWidth)-1;
  ColWidths[4]:=Trunc(0.225*ClientWidth)-1;
  FixedRows:=1; FixedCols:=1;
  Cells[0,0]:='№';
  Cells[1,0]:='X';
  Cells[2,0]:='Y';
  Cells[3,0]:='C';
  Cells[4,0]:='Alpha';
  for i:=1 to 6 do Cells[0,i]:=IntToStr(i);
 end;
end;

procedure TfrmMain.PointsGridSetEditText(Sender: TObject; ACol,
  ARow: Integer; const Value: String);
begin
 try
  with Points[Arow] do
   case ACol of
    1: x:=StrToFloat(Value);
    2: y:=StrToFloat(Value);
    4: alpha:=StrToFloat(Value);
   end;
  Invalidate;
 except
 end;
end;

var SFactors:Array [0..8] of GLEnum = (
GL_ZERO,
GL_ONE,
GL_DST_COLOR,
GL_ONE_MINUS_DST_COLOR,
GL_SRC_ALPHA,
GL_ONE_MINUS_SRC_ALPHA,
GL_DST_ALPHA,
GL_ONE_MINUS_DST_ALPHA,
GL_SRC_ALPHA_SATURATE);

var DFactors:Array [0..7] of GLEnum = (
GL_ZERO,
GL_ONE,
GL_SRC_COLOR,
GL_ONE_MINUS_SRC_COLOR,
GL_SRC_ALPHA,
GL_ONE_MINUS_SRC_ALPHA,
GL_DST_ALPHA,
GL_ONE_MINUS_DST_ALPHA);

     AlphaFuncs:Array [0..7]of GLEnum = (
GL_NEVER,
GL_LESS,
GL_EQUAL,
GL_LEQUAL,
GL_GREATER,
GL_NOTEQUAL,
GL_GEQUAL,
GL_ALWAYS);

Var ambient : PGLFLoat;

procedure TfrmMain.Draw;
var dc:HDC;
    wnd:HWND;
    i,x,y,h,w:integer;
    s:String[4];
    ref:Single;

begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);

  if cbAlpha.Checked then begin   { Если тестирование альфа-канала включено }
    glEnable(GL_ALPHA_TEST);       { Разрешить тестирование в OpenGL}
    ref:=StrToFloat(edtRef.Text);  { Получить значение REF из соответствующего поля ввода}
    glAlphaFunc(AlphaFuncs[cbAlphaFunc.ItemIndex], ref); { Устоновить нужную функцию сравнения }
  end else
    glDisable(GL_ALPHA_TEST);  { иначе запрещаем тестирование альфа-канал в OpenGL }

  // ----------------------------------------------------------------------------
  if cbBlend.Checked then begin { Если смешивание цветов включено }
    glEnable(GL_BLEND);          { Разрешаем смешивание в OpenGL }
    glBlendFunc(SFactors[cbSFactor.ItemIndex],  { Устанавливаем параметры }
                DFactors[cbDFactor.ItemIndex]);
  end else
    glDisable(GL_BLEND); { Запрещаем смешивание }

  glDisable(GL_SCISSOR_TEST); { Запрет "вырезки" в целях очистки всего окна }
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );

  if cbScissor.Checked then begin { если "вырезка" включена }
    glEnable(GL_SCISSOR_TEST);     { разрешить вырезку в OpenGL }
    glScissor(StrToInt(edtSx.Text)+Panel1.Width,   { прочитать соответствующие параметры из полей ввода }
              StrToInt(edtSy.Text)+StatusBar1.Height,
              StrToInt(edtSw.Text),StrToInt(edtSh.Text));
  end;

  { рисуем два треугольника на основи точеку хранящихся в массиве points }
  glBegin(GL_TRIANGLES);
//  glColor4f(  1,0.9,0.2,0.5); glVertex3d( 1,  0, 0.5);
//  glColor4f(0.4,0.4,0.8,0.5); glVertex3d(-1, -1, 1);
//  glColor4f(0.5,0.9,0.3,0.5); glVertex3d(-1,  1, 0);
//  glColor4f(0.8,0.6,0.2,0.5); glVertex3d(-1,  0, 0.5);
//  glColor4f(0.2,0.5,0.9,0.5); glVertex3d( 1,  1, 1);
//  glColor4f(0.9,0.7,  1,0.5); glVertex3d( 1, -1, 0);

  for i:=1 to 6 do with points[i] do begin
    glColor4f(r,g,b,alpha);
    glVertex3d(x, y, 1/i);
  end;
  glEnd();

  glFlush();

  SwapBuffers(dc);
 { вывод нумеров точек, но уже с помощью GDI }
  if PointsShow.ItemIndex<>0 then
    for i:=1 to 6 do with points[i] do begin
      Case PointsShow.ItemIndex of
        1: s:=IntToStr(i);
        2: s:=PointsGrid.Cells[4,i];
      end;
      TextOut(dc,
        Panel1.Width+Trunc((x+1)*(ClientWidth-Panel1.Width)/2)-Length(s)*4,
        Trunc((1-y)*(ClientHeight-StatusBar1.Height-2)/2)+5,
        @s[1], Length(s));
    end;

  if cbScissor.Checked then begin
    glLineWidth(0.5);
    x := StrToInt(edtSx.Text)+Panel1.Width;
    y := frmMain.ClientHeight-StrToInt(edtSy.Text)-StatusBar1.Height;
    w := StrToInt(edtSw.Text);
    h := StrToInt(edtSh.Text);
    MoveToEx(dc,x,y,nil);
    LineTo(dc,x+w,y);
    LineTo(dc,x+w,y-h);
    LineTo(dc,x,y-h);
    LineTo(dc,x,y);
    glEnd();
    glFlush();
  end;

  ReleaseDC(wnd, dc);
  wglMakeCurrent(0, 0);
end;

procedure TfrmMain.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  with StatusBar1.Panels[0] do
    text:='X='+FloatToStrF((x-Panel1.Width)/(ClientWidth-Panel1.Width)*2-1,ffFixed,1,2)+', '+
          'Y='+FloatToStrF(y/(ClientHeight-StatusBar1.Height)*2-1,ffFixed,1,2);
  if ssLeft in Shift then begin
    RotateCamera((x-OldMsx)/200,-(y-OldMsY)/200,0);
    Draw;
  end;
(*  if ssRight in Shift then begin
    MoveCameraForward((y-OldMsY)/200);
    Rotate(UpX,UpZ,(x-OldMsX)/200);
    Draw;
  end; *)
  OldMsX:=X; OldMsY:=Y;
end;

procedure TfrmMain.cbNumsClick(Sender: TObject);
begin
  Draw;
  Invalidate;
end;

procedure TfrmMain.PointsGridDrawCell(Sender: TObject; ACol, ARow: Integer;
  Rect: TRect; State: TGridDrawState);
begin
  if (ACol=3) and (ARow>0) then
    with PointsGrid.Canvas, Points[Arow] do begin
      Brush.Color:=RGB(Round(255*r), Round(255*g), Round(255*b));
      FillRect(rect);
    end;
end;


procedure TfrmMain.cbAlphaClick(Sender: TObject);
begin
  Draw;
  Invalidate;
end;

procedure TfrmMain.cbAlphaFuncChange(Sender: TObject);
begin
  Draw;
  Invalidate;
end;

procedure TfrmMain.cbBlendClick(Sender: TObject);
begin
  Draw;
  Invalidate;
end;

procedure TfrmMain.cbDFactorChange(Sender: TObject);
begin
  Draw;
  Invalidate;
end;

procedure TfrmMain.cbSFactorChange(Sender: TObject);
begin
  Draw;
  Invalidate;
end;

procedure TfrmMain.edtRefChange(Sender: TObject);
begin
 try
  StrToFloat(edtRef.Text);
  Draw;
 except
 end;
 Invalidate;
end;

procedure TfrmMain.cbScissorClick(Sender: TObject);
begin
 Draw;
 Invalidate;
end;

procedure TfrmMain.edtSxChange(Sender: TObject);
begin
 try
  StrToInt(edtSx.Text);
  Draw;
 except
 end;
 Invalidate;
end;

procedure TfrmMain.edtSyChange(Sender: TObject);
begin
 try
  StrToInt(edtSy.Text);
  Draw;
 except
 end;
 Invalidate;
end;

procedure TfrmMain.edtSWChange(Sender: TObject);
begin
 try
  StrToInt(edtSw.Text);
  Draw;
 except
 end;
 Invalidate;
end;

procedure TfrmMain.edtShChange(Sender: TObject);
begin
 try
  StrToInt(edtSh.Text);
  Draw;
 except
 end;
 Invalidate;
end;

procedure TfrmMain.SelectColor(i:Integer);
begin
 if ColorDialog1.Execute then with points[i], ColorDialog1 do begin
  r:=(Color and 255)/255;
  g:=((color shr 8) and 255)/255;
  b:=((color shr 16) and 255)/255;
 end;
 Invalidate;
end;


procedure TfrmMain.PointsGridSelectCell(Sender: TObject; ACol,
  ARow: Integer; var CanSelect: Boolean);
begin
 CanSelect:=ACol<>3;
end;

procedure TfrmMain.PointsGridDblClick(Sender: TObject);
var c:TGridCoord;
    p:TPoint;
begin
 p:=PointsGrid.ScreenToClient(Mouse.CursorPos);
 c:=PointsGrid.MouseCoord(p.x,p.y);
 if (c.X =3) and (c.y>0) then begin
  SelectColor(c.Y);
  Draw;
  PointsGrid.Repaint;
 end;
 Invalidate;
end;

procedure TfrmMain.PointsGridExit(Sender: TObject);
begin
  Invalidate;
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

procedure TfrmMain.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  OldMsX:=x; OldMsY:=y;
end;

end.
