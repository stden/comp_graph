unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, Grids, Spin, ComCtrls, Buttons,
  Menus, { -->} OpenGL { <-- подключение библиотеки OpenGL };

type
  TF = class(TForm)
    Panel1: TPanel;
    PointsGrid_: TStringGrid;
    Label1: TLabel;
    cbStyle: TComboBox;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    sePointSize: TSpinEdit;
    seLineWidth: TSpinEdit;
    StatusBar1: TStatusBar;
    cbNums: TCheckBox;
    BitBtn1: TBitBtn;
    MainMenu1: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
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
    procedure BitBtn1Click(Sender: TObject);
    procedure N2Click(Sender: TObject);
    procedure N3Click(Sender: TObject);
    procedure N4Click(Sender: TObject);
  public
    procedure ConvertFromTable;
    procedure ConvertToTable;
    procedure SetupPointsGrid;
    procedure Draw;
  end;

var
  F: TF;

 { Описание точек }
  points : Array [1..1000] of record x,y,r,g,b:Double; end;
  points_num : Integer;

implementation

uses CreatePolygon, CreateRandom, AboutUnit;

{$R *.dfm}

{ Настройки вывода для OpenGL }
var  pfd : PIXELFORMATDESCRIPTOR =
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

  { Число, соответствующее окну Windows }
  { В этом окне рисуется кадр OpenGL }
  hglrc : THandle;

procedure setDCPixelFormat(hdc:HDC);
var pixelFormat:INteger;
begin
  pixelFormat := ChoosePixelFormat(hdc, @pfd);
  SetPixelFormat(hdc, pixelFormat, @pfd);
  DescribePixelFormat(hdc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), pfd);
  if(pfd.dwFlags and PFD_NEED_PALETTE>0) then
  	  MessageBox(NULL, 'Palette mode not supported!', 'Error', MB_OK);
end;

// При создании формы
procedure TF.FormCreate(Sender: TObject);
var dc:HDC;
    wnd:HWND;
begin
  DecimalSeparator:='.';
  Canvas.Brush.Style := bsClear;
  dc := F.GetDeviceContext(wnd);
  { установка "правильного" формата битовой плоскости окна }
  setDCPixelFormat(dc);
  { создание контекста OpenGL }
  hglrc := wglCreateContext(dc);
  ReleaseDC(wnd, dc);
  { Инициализация точек }
  ConvertToTable;
  SetupPointsGrid;
  FCreatePolygon.Go(5);
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

procedure TF.ConvertFromTable;
var i:Integer;
begin
  Points_num := PointsGrid_.RowCount-1;
  with PointsGrid_ do
    for i:=1 to PointsGrid_.RowCount-1 do with points[i] do begin
      x:=StrToFloat(cells[1,i]);
      y:=StrToFloat(cells[2,i]);
    end;
end;

procedure TF.ConvertToTable;
var i:Integer;
begin
  PointsGrid_.RowCount := Points_num+1;
  with PointsGrid_ do
    for i:=1 to PointsGrid_.RowCount-1 do
      with points[i] do begin
        cells[0,i]:=IntToStr(i);
        cells[1,i]:=FloatToStr(x);
        cells[2,i]:=FloatToStr(y);
      end;
end;


procedure TF.SetupPointsGrid;
var i:Integer;
begin
  with PointsGrid_ do begin
    RowCount:=2; ColCount:=3;
    ColWidths[0]:=Trunc(0.15*ClientWidth);
    ColWidths[1]:=Trunc(0.425*ClientWidth)-1;
    ColWidths[2]:=Trunc(0.425*ClientWidth)-1;
    FixedRows:=1; FixedCols:=1;
    Cells[0,0]:='№';
    Cells[1,0]:='X';
    Cells[2,0]:='Y';
    for i:=1 to PointsGrid_.RowCount-1 do Cells[0,i]:=IntToStr(i);
  end;
end;

procedure TF.PointsGrid_SetEditText(Sender: TObject; ACol,
  ARow: Integer; const Value: String);
begin
  try
    with Points[Arow] do
      if ACol=1 then x:=StrToFloatDef(Value, x)
             else y:=StrToFloatDef(Value, y);
    Invalidate;
  except
    // Игнорируем ошибку и позволяем пользователю исправить координаты
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
    i:integer;
    s:AnsiString;
begin
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);

  { Рисуем нулевые оси }
  glLineWidth(0.5);

  glBegin(1);  {  1-GL_LINES }

    glColor(0.0,0.0,0.0); { RGB - Чёрный цвет }
    glVertex2d(-1, 0);
    glVertex2d(1, 0);
    glVertex2d(0, -1);
    glVertex2d(0, 1);

  glEnd();

  { Установка параметров примитивов }
  glPointSize(sePointSize.Value);
  glLineWidth(seLineWidth.Value);

  glBegin(cbStyle.ItemIndex);
  for i:=1 to PointsGrid_.RowCount-1 do
    with points[i] do begin
      glColor(r,g,b);
      glVertex2d(x, y);
    end;
  glEnd();

  glFlush();

  SwapBuffers(dc);
  if cbNums.Checked then
    for i:=1 to PointsGrid_.RowCount-1 do with points[i] do begin
      s := IntToStr(i);
      // TODO: вывод через OpenGL
      TextOut(dc,
        Panel1.Width+Trunc((x+1)*(ClientWidth-Panel1.Width)/2)-Length(s)*4-7,
        Trunc((1-y)*(ClientHeight-StatusBar1.Height-2)/2)-18,
        @s[1], Length(s));
    end;

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

procedure TF.BitBtn1Click(Sender: TObject);
begin
  inc(points_num);
  with points[points_num] do begin
    x := 0;
    y := 0;
    r := random(100)/100;
    g := random(100)/100;
    b := random(100)/100;
  end;
  // ------
  ConvertToTable;
  // ------
  Invalidate;
end;

procedure TF.N2Click(Sender: TObject);
begin
  FCreatePolygon.Show;
end;

procedure TF.N3Click(Sender: TObject);
begin
  FCreateRandom.Show;
end;

procedure TF.N4Click(Sender: TObject);
begin
  FAbout.Show;
end;

end.
