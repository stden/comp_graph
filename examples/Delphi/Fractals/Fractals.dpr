program Fractals;

uses
  Forms,
  FractalUnit in 'FractalUnit.pas' {F};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TF, F);
  Application.Run;
end.
