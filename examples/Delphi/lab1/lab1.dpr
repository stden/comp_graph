program lab1;

uses
  Forms,
  main in 'main.pas' {F},
  CreatePolygon in 'CreatePolygon.pas' {FCreatePolygon},
  CreateRandom in 'CreateRandom.pas' {FCreateRandom},
  AboutUnit in 'AboutUnit.pas' {FAbout};

{$R *.res}

begin
  Application.Initialize; 
  Application.CreateForm(TF, F);
  Application.CreateForm(TFCreatePolygon, FCreatePolygon);
  Application.CreateForm(TFCreateRandom, FCreateRandom);
  Application.CreateForm(TFAbout, FAbout);
  Application.Run;
end.
