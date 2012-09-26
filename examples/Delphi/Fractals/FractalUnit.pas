unit FractalUnit;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Grids, StdCtrls, Spin, ExtCtrls, Math, xmldom, XMLIntf,
  msxmldom, XMLDoc;

type
  TF = class(TForm)
    I: TImage;
    DepthLimit: TSpinEdit;
    Label1: TLabel;
    Base: TStringGrid;
    Label2: TLabel;
    Label3: TLabel;
    BasicLen: TLabel;
    LineWidth: TRadioGroup;
    DrawPoints: TCheckBox;
    Start: TShape;
    Finish: TShape;
    Edit: TEdit;
    XML_Doc: TXMLDocument;
    Shape1: TShape;
    Label5: TLabel;
    Shape2: TShape;
    Label4: TLabel;
    Shape3: TShape;
    Label6: TLabel;
    Shape4: TShape;
    Label7: TLabel;
    Shape5: TShape;
    Label8: TLabel;
    Shape6: TShape;
    Label9: TLabel;
    Shape7: TShape;
    Label10: TLabel;
    Shape8: TShape;
    Label11: TLabel;
    Shape9: TShape;
    Label12: TLabel;
    Shape10: TShape;
    Label13: TLabel;
    Shape11: TShape;
    Label14: TLabel;
    Shape12: TShape;
    Label15: TLabel;
    ColorDialog1: TColorDialog;
    procedure FormCreate(Sender: TObject);
    procedure DepthLimitChange(Sender: TObject);
    procedure LineWidthClick(Sender: TObject);
    procedure DrawPointsClick(Sender: TObject);
    procedure IDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure BaseSelectCell(Sender: TObject; ACol, ARow: Integer;
      var CanSelect: Boolean);
    procedure EditExit(Sender: TObject);
    procedure SelectColor(Var c:TColor);
    procedure ShapeMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    EditRow,EditCol : Integer;
    FileName : String;
  public
    function BaseX( Index:Integer ) : Double;
    function BaseY( Index:Integer ) : Double;
    {}
    procedure Recurse( x1,y1,x2,y2:Double; Depth:Integer );
    procedure Draw( _x1,_y1,_x2,_y2:Double; Depth:Integer );
    procedure RedrawFractal;
    procedure After_Base_Updated;
    procedure Update_Caption;
  end;

var
  F: TF;

implementation

{$R *.dfm}

const eps = 1e-10;

Type
  TPoint = Record
    X,Y : Double;
    constructor Init( _X,_Y:Double  );
  End;

{ Линия Коха }
Const
  Kosh : Array [1..5] of TPoint = (
    ( X:0; Y:0 ),
    ( X:1; Y:0 ),
    ( X:1.5; Y:0.7 ),
    ( X:2; Y:0 ),
    ( X:3; Y:0 ));

{ Кривая Дракона }
{Const
  Dragon : Array [1..5] of TPoint = (
    ( X:0; Y:0 ),
    ( X:0; Y:-1 ),
    ( X:1; Y:-1 ),
    ( X:1; Y:0 ),
    ( X:2; Y:0 )); }

{ Кривая Дракона1 }
Const
  Dragon : Array [1..5] of TPoint = (
    ( X:0; Y:0 ),
    ( X:1; Y:1 ),
    ( X:2; Y:0 ),
    ( X:3; Y:-1 ),
    ( X:4; Y:0 ));


function SX( X:Double ):Integer; { Screen X }
begin
  Result := Round(X);
end;

function SY( Y:Double ):Integer; { Screen Y }
begin
  Result := Round(Y);
end;

function TF.BaseX(Index: Integer): Double;
begin
  assert( (1<=Index) and (Index<Base.RowCount) );
  Result := StrToFloat( Base.Cells[1,Index] );
end;

function TF.BaseY(Index: Integer): Double;
begin
  assert( (1<=Index) and (Index<Base.RowCount) );
  Result := StrToFloat( Base.Cells[2,Index] );
end;

procedure TF.Draw( _x1,_y1,_x2,_y2:Double; Depth:Integer );
var x1,y1,x2,y2 : Integer;
begin
  x1:=SX(_x1); y1:=SY(_y1);
  x2:=SX(_x2); y2:=SY(_y2);
  {}
  Case Depth of
    1: I.Canvas.Pen.Color := Shape1.Brush.Color;
    2: I.Canvas.Pen.Color := Shape2.Brush.Color;
    3: I.Canvas.Pen.Color := Shape3.Brush.Color;
    4: I.Canvas.Pen.Color := Shape4.Brush.Color;
    5: I.Canvas.Pen.Color := Shape5.Brush.Color;
    6: I.Canvas.Pen.Color := Shape6.Brush.Color;
    7: I.Canvas.Pen.Color := Shape7.Brush.Color;
    8: I.Canvas.Pen.Color := Shape8.Brush.Color;
    9: I.Canvas.Pen.Color := Shape9.Brush.Color;
    10: I.Canvas.Pen.Color := Shape10.Brush.Color;
    11: I.Canvas.Pen.Color := Shape11.Brush.Color;
    12: I.Canvas.Pen.Color := Shape12.Brush.Color;
  End;
  {}
  Case LineWidth.ItemIndex of
    0: I.Canvas.Pen.Width := Depth;
    1: I.Canvas.Pen.Width := DepthLimit.Value-Depth+1;
    2: I.Canvas.Pen.Width := 1;
    3: if Depth<>DepthLimit.Value then exit else I.Canvas.Pen.Width := 1;
  end;
  { Концевые точки }
  if DrawPoints.Checked then begin
    I.Canvas.Ellipse(x1-2,y1-2,x1+2,y1+2);
    I.Canvas.Ellipse(x2-2,y2-2,x2+2,y2+2);
  end;
  { Отрезок }
  I.Canvas.MoveTo(x1,y1);
  I.Canvas.LineTo(x2,y2);
end;

procedure TF.FormCreate(Sender: TObject);
var i : Integer;
begin
  // Десятичный разделитель - точка
  FormatSettings.DecimalSeparator := '.';

  FileName := 'Линия Коха';
  Update_Caption;
  { Расставляем номера точек }
  Base.Cells[0,0] := '№';
  Base.ColWidths[0] := 20;
  { Кооординаты точек }
  Base.Cells[1,0] := 'X'; Base.Cells[2,0] := 'Y';
  Base.RowCount := 6;
  for i:=1 to Base.RowCount-1 do begin
    Base.Cells[0,i] := IntToStr(i);
    Base.Cells[1,i] := FloatToStr(Dragon[i].X); {Kosh}
    Base.Cells[2,i] := FloatToStr(Dragon[i].Y); {Kosh}
  end;
  After_Base_Updated;
end;

function Translate( P:TPoint; dx,dy:Double ) : TPoint;
begin
  Result.X := P.X + dx;
  Result.Y := P.Y + dy;
end;

function Scale( P:TPoint; ScaleFactor:Double ) : TPoint;
begin
  Result.X := P.X * ScaleFactor;
  Result.Y := P.Y * ScaleFactor;
end;

function Rotate( P:TPoint; Phi:Double ) : TPoint;
begin
  Result.X := P.X * cos(Phi) - P.Y * sin(Phi);
  Result.Y := P.X * sin(Phi) + P.Y * cos(Phi);
end;

procedure TF.Recurse(x1, y1, x2, y2: Double; Depth: Integer);
var
  VectorLen,ScaleFactor,a1,a2,phi,dx,dy : Double;
  PCur,P : TPoint;
  i : Integer;
begin
  Draw(x1,y1,x2,y2,Depth);
  {}
  if Depth >= DepthLimit.Value then Exit;
  {}
  { Перемещаение начальной точки }
  dx := x1-BaseX(1);
  dy := y1-BaseY(1);
  { Коэффициент масштабирования = Длина вектора / Длина ядра }
  VectorLen := Sqrt( Sqr(x1-x2) + Sqr(y1-y2) );
  ScaleFactor := VectorLen / StrToFloat(BasicLen.Caption);
  { Угол поворота = угол между двумя векторами }
  a1 := Arctan2( x2-x1, y2-y1 );
  a2 := Arctan2( BaseX(Base.RowCount-1)-BaseX(1), BaseY(Base.RowCount-1)-BaseY(1) );
  phi := a2-a1;
  {}
  PCur.Init( x1, y1 );
  for i:=2 to Base.RowCount-1 do begin
    P.Init( BaseX(i)-BaseX(1), BaseY(i)-BaseY(1) );
    P := Translate( Scale( Rotate(P, phi), ScaleFactor ), dx, dy );
    Recurse( PCur.X, PCur.Y, P.X, P.Y, Depth+1 );
    PCur := P; { Точка становится предыдущей }
  end;
end;

{ TPoint }

constructor TPoint.Init( _X,_Y: Double);
begin
  X := _X;
  Y := _Y;
end;

procedure TF.RedrawFractal;
begin
  I.Canvas.FillRect(Rect(0,0,I.Width-1,I.Height-1));
  Recurse( Start.Left  - I.Left + Start.Height div 2,
           Start.Top   - I.Top  + Start.Width div 2,
           Finish.Left - I.Left + Finish.Height div 2,
           Finish.Top  - I.Top  + Finish.Width div 2,
           1 );
end;

procedure TF.DepthLimitChange(Sender: TObject);
begin
  RedrawFractal;
end;

procedure TF.LineWidthClick(Sender: TObject);
begin
  RedrawFractal;
end;

procedure TF.DrawPointsClick(Sender: TObject);
begin
  RedrawFractal;
end;

procedure TF.IDragOver(Sender, Source: TObject; X, Y: Integer;
  State: TDragState; var Accept: Boolean);
begin
  if Source is TShape then begin
    Accept := True;
    TShape(Source).Top  := I.Top+Y;
    TShape(Source).Left := I.Left+X;
    RedrawFractal;
  end;
end;

procedure TF.BaseSelectCell(Sender: TObject; ACol, ARow: Integer;
  var CanSelect: Boolean);
var R : TRect;
begin
  R := Base.CellRect(ACol,ARow);
  Edit.Top    := R.Top+Base.Top+1;
  Edit.Left   := R.Left+Base.Left+1;
  Edit.Width  := R.Right-R.Left+2;
  Edit.Height := R.Bottom-R.Top+2;
  Edit.Text   := Base.Cells[ACol,ARow];
  EditRow := ARow;
  EditCol := ACol;
  Edit.Visible := True;
  Edit.SetFocus;
end;

procedure TF.EditExit(Sender: TObject);
var
  Err : Integer;
  Value : Double;
begin
  Val( Edit.Text, Value, Err );
  if Err=0 then begin
    Base.Cells[EditCol,EditRow] := Edit.Text;
    After_Base_Updated
  end;
end;

procedure TF.After_Base_Updated;
begin
  { Вычисляем длину ядра }
  BasicLen.Caption := FloatToStr(
     Sqrt( Sqr(BaseX(1)-BaseX(Base.RowCount-1) ) + Sqr(BaseY(1)-BaseY(Base.RowCount-1) )));
  { DRAW }
  RedrawFractal;
end;

procedure TF.Update_Caption;
begin
  F.Caption := 'Построение фракталов - '+FileName;
end;

procedure TF.SelectColor(var c: TColor);
begin
  with ColorDialog1 do
   if Execute then c:=Color;
end;

procedure TF.ShapeMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  Color : TColor;
begin
  SelectColor(Color);
  (Sender as TShape).Brush.Color:=Color;
  RedrawFractal;
end;

end.

