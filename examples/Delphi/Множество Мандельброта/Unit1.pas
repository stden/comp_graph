unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls;

type
  TForm1 = class(TForm)
    I: TImage;
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
  private
    { Private declarations }
  public
    procedure Draw;
  end;

var
  Form1: TForm1;

type
  Complex = Record
    Re,Im : Double;
  end;

implementation

{$R *.dfm}

{ Умножение комплексных чисел: }
{ (a+bi)(c+di)=(ac-bd)+(ad+bc)i }
function G( C:Complex ):Integer;
var Z,Zn : Complex;
begin
  Result := 0;
  Z.Re := 0; Z.Im := 0;
  while (Z.Re*Z.Re + Z.Im*Z.Im) < 4 do begin
    Zn.Re := Z.Re*Z.Re - Z.Im*Z.Im + C.Re;
    Zn.Im := 2*Z.Re*Z.Im + C.Im;
    inc(Result);
    Z := Zn;
    if Result > 300 then break;
  end;
end;

procedure TForm1.Draw;
var
  xi,yi,IW,IH : Integer;
  C : Complex;
  B:TBitmap;
  A:PByteArray;
  pal: PLogPalette;
  hpal: HPALETTE;
  n: Integer;
  BW,BH:Integer;
  BR:TRect;
begin
  IW := I.Width;
  IH := I.Height;
  BW:=IW;
  BH:=IH;
  BR:=Rect(0,0,BW-1,BH-1);
  B:=TBitmap.Create;
  With B do
    Begin
      PixelFormat:=pf8bit;
      pal := nil;
      GetMem(pal, sizeof(TLogPalette) + sizeof(TPaletteEntry) * 255);
      pal.palVersion := $300;
      pal.palNumEntries := 256;
      for n := 0 to 255 do
      begin
        pal.palPalEntry[n].peRed := n;
        pal.palPalEntry[n].peGreen := 255-n;
        pal.palPalEntry[n].peBlue := 0;
      end;
      hpal := CreatePalette(pal^);
      if hpal <> 0 then
        Palette := hpal;
      FreeMem(pal);
      Width:=BW;
      Height:=BH;
    End;
  with B do
    for yi:=0 to BH-1 do begin
      A:=ScanLine[yi];
      for xi:=0 to BW-1 do begin
        C.Re := (xi*3)/BW - 2.2;
        C.Im := (yi*2.4)/BH - 1.2;
        A[xi] := (G(C)*8) mod 256{*23};
      end;
    end;
  I.Picture.Assign(B);
  //I.Canvas.CopyRect(BR,B.Canvas,BR);
  B.Free;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  Draw;
end;

procedure TForm1.FormResize(Sender: TObject);
begin
  //Draw;
end;

end.
