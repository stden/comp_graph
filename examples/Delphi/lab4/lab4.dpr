program lab4;

uses
  Forms,
  main in 'main.pas' {F},
  AboutUnit in 'AboutUnit.pas' {FAbout};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TF, F);
  Application.CreateForm(TFAbout, FAbout);
  Application.Run;
end.
