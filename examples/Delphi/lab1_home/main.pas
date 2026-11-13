unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, Grids, Spin, ComCtrls, Buttons,
  Menus, { -->} OpenGL { <-- подключение библиотеки OpenGL };

type
  TF = class(TForm)
    LeftPanel: TPanel;
    PointsGrid: TStringGrid;
    Label1: TLabel;
    cbStyle: TComboBox;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    sePointSize: TSpinEdit;
    seLineWidth: TSpinEdit;
    StatusBar1: TStatusBar;
    cbNums: TCheckBox;
    AddPointButton: TBitBtn;
    MainMenu1: TMainMenu;
    N1: TMenuItem;
    N2: TMenuItem;
    N3: TMenuItem;
    N4: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormPaint(Sender: TObject);
    procedure PointsGridSetEditText(Sender: TObject; ACol, ARow: Integer;
      const Value: String);
    procedure cbStyleChange(Sender: TObject);
    procedure sePointSizeChange(Sender: TObject);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure cbNumsClick(Sender: TObject);
    procedure AddPointButtonClick(Sender: TObject);
    procedure N2Click(Sender: TObject);
    procedure N3Click(Sender: TObject);
    procedure N4Click(Sender: TObject);
  public
    procedure ConvertFromTable;
    procedure ConvertToTable;
    procedure SetupPointsGrid;
    procedure Draw;
    procedure AddPoint(x,y:extended);
    procedure AddLine(x1,y1,x2,y2:extended);
    procedure AddRectangle(x1,y1,x2,y2:extended);

    procedure CreateHomePicture;
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
  dc := GetDeviceContext(wnd);
  { установка "правильного" формата битовой плоскости окна }
  setDCPixelFormat(dc);
  { создание контекста OpenGL }
  hglrc := wglCreateContext(dc);
  ReleaseDC(wnd, dc);
  { Инициализация точек }
  ConvertToTable;
  SetupPointsGrid;

  CreateHomePicture;

  ConvertToTable;
  Invalidate;
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
  
  glViewport(GLsizei(LeftPanel.Width), StatusBar1.Height,
             GLsizei(ClientWidth-LeftPanel.Width),
             GLsizei(ClientHeight-StatusBar1.Height));

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
  Points_num := PointsGrid.RowCount-1;
  with PointsGrid do
    for i:=1 to PointsGrid.RowCount-1 do with points[i] do begin
      x:=StrToFloat(cells[1,i]);
      y:=StrToFloat(cells[2,i]);
    end;
end;

procedure TF.ConvertToTable;
var i:Integer;
begin
  PointsGrid.RowCount := Points_num+1;
  with PointsGrid do
    for i:=1 to PointsGrid.RowCount-1 do
      with points[i] do begin
        cells[0,i]:=IntToStr(i);
        cells[1,i]:=FloatToStr(x);
        cells[2,i]:=FloatToStr(y);
      end;
end;


procedure TF.SetupPointsGrid;
var i:Integer;
begin
  with PointsGrid do begin
    RowCount:=2; ColCount:=3;
    ColWidths[0]:=Trunc(0.15*ClientWidth);
    ColWidths[1]:=Trunc(0.425*ClientWidth)-1;
    ColWidths[2]:=Trunc(0.425*ClientWidth)-1;
    FixedRows:=1; FixedCols:=1;
    Cells[0,0]:='№';
    Cells[1,0]:='X';
    Cells[2,0]:='Y';
    for i:=1 to PointsGrid.RowCount-1 do Cells[0,i]:=IntToStr(i);
  end;
end;

procedure TF.PointsGridSetEditText(Sender: TObject; ACol,
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
  // Отрисовка графики через OpenGL

  // Получаем окно для рисования
  dc := GetDeviceContext(wnd);
  wglMakeCurrent(dc, hglrc);

  // Очищаем окно белым цветом
  //  Red Green Blue Alpha
  // В диапазоне 0..1.0
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  { Рисуем оси координат OXY }
  glLineWidth(0.5); { Толщина линии }

  // Начинаем рисовать примитив OpenGL
  glBegin(GL_LINES);  {  1-GL_LINES }

    // Выбираем цвет для рисования
    //  0.0 - число типа double
    // Интесивность компоненты цвета: 0.0..1.0
    glColor(0.0, 0.0, 0.0); { RGB - Чёрный цвет }

    // Поскольку режим GL_LINES
    // каждая пара точек означает один отрезок

    // Ось OX
    glVertex2d(0, -1);
    glVertex2d(0, 1);

    // Ось OY
    glVertex2d(-1, 0);
    glVertex2d(1, 0);

  glEnd();

  { Установка параметров примитивов }
  glPointSize(sePointSize.Value);
  glLineWidth(seLineWidth.Value);

  glBegin(cbStyle.ItemIndex);
  for i:=1 to PointsGrid.RowCount-1 do
    with points[i] do begin
      glColor(r,g,b);
      glVertex2d(x, y);
    end;
  glEnd();
  glFlush();

  SwapBuffers(dc);

  if cbNums.Checked then
    for i:=1 to PointsGrid.RowCount-1 do with points[i] do begin
      s := IntToStr(i);
      // TODO: вывод через OpenGL
      TextOut(dc,
        LeftPanel.Width+Trunc((x+1)*(ClientWidth-LeftPanel.Width)/2)-Length(s)*4-7,
        Trunc((1-y)*(ClientHeight-StatusBar1.Height-2)/2)-18,
        @s[1], Length(s));
    end;

  ReleaseDC(wnd, dc);

  wglMakeCurrent(0, 0);
end;

procedure TF.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  OpenGLWindowHeight : Integer;
  ModelY : extended;
begin
  // Окно занимает всё пространство кроме StatusBar'а внизу
  OpenGLWindowHeight := ClientHeight-StatusBar1.Height;
  // Экранные координаты:
  //   0..OpenGLWindowHeight и ось направлена вниз
  // Координаты "внутри" картинки
  //   -1..1   и ось направлена вверх
  // Масштабирование
  ModelY := (y / OpenGLWindowHeight) * 2;
  // Теперь у нас координаты в диапазоне 0..2 и ось направлена вниз
  // переворачиваем ось и сдвигаем на 1
  ModelY := - ModelY + 1;


  with StatusBar1.Panels[0] do
    text:='X='+FloatToStrF((x-LeftPanel.Width)/(ClientWidth-LeftPanel.Width)*2-1,ffFixed,1,2)+', Y='+
          FloatToStrF(ModelY,ffFixed,1,2);
end;

procedure TF.cbNumsClick(Sender: TObject);
begin
  Draw;
end;

procedure TF.AddPointButtonClick(Sender: TObject);
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

procedure TF.AddPoint(x, y: extended);
begin
  Inc(points_num);
  points[points_num].x := x;
  points[points_num].y := y;
end;

// Добавляем отрезок (line)
procedure TF.AddLine(x1, y1, x2, y2: extended);
begin
  // Для этого добавляем 2 точки
  AddPoint(x1,y1);
  AddPoint(x2,y2);
end;

procedure TF.CreateHomePicture;
begin
  { Начальный рисунок: "Домик с трубой" }
  // Основа - корпус домика
  AddLine(-0.5, -0.5, -0.5,  0.5);
  AddLine(-0.5, 0.5, 0.5, 0.5);
  AddLine(0.5,  0.5, 0.5,  -0.5);
  AddLine(0.5, -0.5, -0.5, -0.5);

  // Крыша
  AddLine(-0.7, 0.5, 0.7, 0.5);
  AddLine(-0.7, 0.5,   0, 0.8);
  AddLine(0, 0.8,   0.7, 0.5);

  // Труба
  AddLine(0.12, 0.60, 0.12, 0.89);
  AddLine(0.12, 0.89, 0.25, 0.89);
  AddLine(0.25, 0.89, 0.25, 0.60);

  // Окно
  AddRectangle(-0.4, -0.1, 0.4, 0.3);
end;

// Прямоугольник
procedure TF.AddRectangle(x1, y1, x2, y2: extended);
begin
  AddLine(x1,y1,x1,y2);
  AddLine(x1,y2,x2,y2);
  AddLine(x2,y2,x2,y1);
  AddLine(x2,y1,x1,y1);
end;

end.
