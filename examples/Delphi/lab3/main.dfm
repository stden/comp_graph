object frmMain: TfrmMain
  Left = 63
  Top = 38
  Width = 656
  Height = 597
  AlphaBlendValue = 180
  Caption = #1050#1086#1084#1087#1100#1102#1090#1077#1088#1085#1072#1103' '#1075#1088#1072#1092#1080#1082#1072' - '#1051#1072#1073#1086#1088#1072#1090#1086#1088#1085#1072#1103' '#1088#1072#1073#1086#1090#1072' '#8470'3'
  Color = clWindow
  DefaultMonitor = dmDesktop
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnMouseDown = FormMouseDown
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label7: TLabel
    Left = 32
    Top = 134
    Width = 7
    Height = 13
    Caption = 'X'
  end
  object Label8: TLabel
    Left = 32
    Top = 134
    Width = 7
    Height = 13
    Caption = 'X'
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 137
    Height = 563
    Align = alLeft
    TabOrder = 0
    object ScrollBox1: TScrollBox
      Left = 1
      Top = 1
      Width = 135
      Height = 561
      HorzScrollBar.Smooth = True
      HorzScrollBar.Style = ssFlat
      HorzScrollBar.Tracking = True
      VertScrollBar.Smooth = True
      VertScrollBar.Style = ssFlat
      VertScrollBar.Tracking = True
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      TabOrder = 0
      object ObjColorShape: TShape
        Left = 88
        Top = 355
        Width = 25
        Height = 20
        Brush.Color = clBlue
        OnMouseDown = ObjColorShapeMouseDown
      end
      object Label9: TLabel
        Left = 33
        Top = 53
        Width = 7
        Height = 13
        Caption = 'Y'
      end
      object Label5: TLabel
        Left = 33
        Top = 30
        Width = 7
        Height = 13
        Caption = 'X'
      end
      object Label4: TLabel
        Left = 16
        Top = 8
        Width = 100
        Height = 13
        Caption = #1055#1086#1083#1086#1078#1077#1085#1080#1077' "'#1075#1083#1072#1079#1072'"'
      end
      object Label18: TLabel
        Left = 33
        Top = 285
        Width = 7
        Height = 13
        Caption = 'Z'
      end
      object Label17: TLabel
        Left = 33
        Top = 261
        Width = 7
        Height = 13
        Caption = 'Y'
      end
      object Label16: TLabel
        Left = 33
        Top = 238
        Width = 7
        Height = 13
        Caption = 'X'
      end
      object Label10: TLabel
        Left = 33
        Top = 77
        Width = 7
        Height = 13
        Caption = 'Z'
      end
      object Label15: TLabel
        Left = 16
        Top = 216
        Width = 97
        Height = 13
        Caption = #1053#1072#1087#1088#1072#1074#1083'. '#1074#1077#1088#1090'. '#1086#1089#1080
      end
      object Label14: TLabel
        Left = 33
        Top = 181
        Width = 7
        Height = 13
        Caption = 'Z'
      end
      object Label13: TLabel
        Left = 33
        Top = 157
        Width = 7
        Height = 13
        Caption = 'Y'
      end
      object Label12: TLabel
        Left = 33
        Top = 134
        Width = 7
        Height = 13
        Caption = 'X'
      end
      object Label11: TLabel
        Left = 16
        Top = 112
        Width = 95
        Height = 13
        Caption = #1058#1086#1095#1082#1072' '#1085#1072#1073#1083#1102#1076#1077#1085#1080#1103
      end
      object Label1: TLabel
        Left = 8
        Top = 356
        Width = 73
        Height = 13
        Caption = #1062#1074#1077#1090' '#1086#1073#1098#1077#1082#1090#1072':'
      end
      object Label20: TLabel
        Left = 8
        Top = 544
        Width = 3
        Height = 13
      end
      object edtUpZ: TEdit
        Left = 50
        Top = 282
        Width = 60
        Height = 21
        TabOrder = 0
        Text = '0.0'
        OnChange = edtUpZChange
      end
      object edtUpY: TEdit
        Left = 50
        Top = 258
        Width = 60
        Height = 21
        TabOrder = 1
        Text = '0.0'
        OnChange = edtUpYChange
      end
      object edtUpX: TEdit
        Left = 50
        Top = 234
        Width = 60
        Height = 21
        TabOrder = 2
        Text = '0.0'
        OnChange = edtUpXChange
      end
      object edtEyeZ: TEdit
        Left = 50
        Top = 74
        Width = 60
        Height = 21
        TabOrder = 3
        Text = '-2.0'
        OnChange = edtEyeZChange
      end
      object edtEyeX: TEdit
        Left = 50
        Top = 26
        Width = 60
        Height = 21
        TabOrder = 4
        Text = '-2.0'
        OnChange = edtEyeXChange
      end
      object edtCenterX: TEdit
        Left = 50
        Top = 130
        Width = 60
        Height = 21
        TabOrder = 5
        Text = '0.0'
        OnChange = edtCenterXChange
      end
      object edtCenterZ: TEdit
        Left = 50
        Top = 178
        Width = 60
        Height = 21
        TabOrder = 6
        Text = '0.0'
        OnChange = edtCenterZChange
      end
      object edtCenterY: TEdit
        Left = 50
        Top = 154
        Width = 60
        Height = 21
        TabOrder = 7
        Text = '2.0'
        OnChange = edtCenterYChange
      end
      object cbAnimation: TCheckBox
        Left = 16
        Top = 392
        Width = 97
        Height = 17
        Caption = #1040#1085#1080#1084#1072#1094#1080#1103
        TabOrder = 8
        OnClick = cbAnimationClick
      end
      object edtEyeY: TEdit
        Left = 50
        Top = 50
        Width = 60
        Height = 21
        TabOrder = 9
        Text = '-2.0'
        OnChange = edtEyeYChange
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 1
    OnTimer = Timer1Timer
    Left = 32
    Top = 456
  end
  object ColorDialog1: TColorDialog
    Left = 80
    Top = 456
  end
end
