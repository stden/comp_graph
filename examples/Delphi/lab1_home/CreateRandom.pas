unit CreateRandom;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Spin, Buttons;

type
  TFCreateRandom = class(TForm)
    BitBtn1: TBitBtn;
    PointsNumber: TSpinEdit;
    Label1: TLabel;
    procedure BitBtn1Click(Sender: TObject);
  private
    { Private declarations }
  public
    procedure Go( N:Integer );
  end;

var
  FCreateRandom: TFCreateRandom;

implementation

uses main;

{$R *.dfm}

procedure TFCreateRandom.BitBtn1Click(Sender: TObject);
begin
  if PointsNumber.Value <= 0 then begin
    ShowMessage('Количество вершин должно быть больше 0!');
    exit;
  end;
  // Передаём количество точек в процедуру генерации
  Go(PointsNumber.Value);
  // Закрываем окно
  Hide;
end;

procedure TFCreateRandom.Go(N: Integer);
var i:Integer;
begin
  randomize;
  points_num := N;
  for i:=1 to N do with points[i] do begin
    x:=Random(9000)/5000-1;
    y:=Random(9000)/5000-1;
    r:=random(100)/100;
    g:=random(100)/100;
    b:=random(100)/100;
  end;
  // --------------------------------------------------------
  F.ConvertToTable;
  F.Invalidate;
end;

end.
