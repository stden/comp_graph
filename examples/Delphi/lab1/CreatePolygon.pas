// Генерация правильного многоугольника
unit CreatePolygon;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Spin, Buttons;

type
  TFCreatePolygon = class(TForm)
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
  FCreatePolygon: TFCreatePolygon;

implementation

uses main;

{$R *.dfm}

procedure TFCreatePolygon.BitBtn1Click(Sender: TObject);
begin
  // Проверка ограничений и вывод сообшений об ошибках
  if PointsNumber.Value <= 0 then begin
    ShowMessage('Количество вершин должно быть больше 0!');
    exit;
  end;
  // Вызываем процедуру генерации
  Go(PointsNumber.Value);
  // Закрываем форму
  Hide;
end;

procedure TFCreatePolygon.Go(N: Integer);
var i:Integer;
    a:Extended;
begin
  points_num := N;
  for i:=1 to N do
    with points[i] do begin
      // Угол
      a := 2*pi*(i-1)/N;
      // Координаты по углу
      x := Int(cos(a)*90)/100;
      y := Int(sin(a)*90)/100;
      // Цвет точки RGB
      r := random(100)/100;
      g := random(100)/100;
      b := random(100)/100;
    end;
  // --------------------------------------------------------
  F.ConvertToTable;
  F.Invalidate;
end;

end.
