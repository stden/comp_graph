object F: TF
  Left = 133
  Top = 149
  Caption = #1050#1086#1084#1087#1100#1102#1090#1077#1088#1085#1072#1103' '#1075#1088#1072#1092#1080#1082#1072' - '#1051#1072#1073#1086#1088#1072#1090#1086#1088#1085#1072#1103' '#1088#1072#1073#1086#1090#1072' '#8470'4'
  ClientHeight = 470
  ClientWidth = 609
  Color = clWindow
  Constraints.MinHeight = 480
  Constraints.MinWidth = 600
  DefaultMonitor = dmDesktop
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnMouseMove = FormMouseMove
  OnPaint = FormPaint
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 161
    Height = 451
    Align = alLeft
    TabOrder = 0
    object Label4: TLabel
      Left = 8
      Top = 24
      Width = 82
      Height = 13
      Caption = #1058#1086#1083#1097#1080#1085#1072' '#1083#1080#1085#1080#1080':'
    end
    object seLineWidth: TSpinEdit
      Left = 24
      Top = 40
      Width = 73
      Height = 22
      MaxValue = 100
      MinValue = 1
      TabOrder = 0
      Value = 2
      OnChange = sePointSizeChange
    end
    object L9_Panel: TPanel
      Left = 5
      Top = 98
      Width = 154
      Height = 87
      TabOrder = 1
      object Label1: TLabel
        Left = 8
        Top = 34
        Width = 94
        Height = 13
        Caption = #1043#1083#1091#1073#1080#1085#1072' '#1088#1077#1082#1091#1088#1089#1080#1080':'
      end
      object L9_Depth: TSpinEdit
        Left = 32
        Top = 51
        Width = 73
        Height = 22
        MaxValue = 100
        MinValue = 1
        TabOrder = 0
        Value = 2
        OnChange = L9_DepthChange
      end
      object cbShowL9: TCheckBox
        Left = 8
        Top = 8
        Width = 145
        Height = 17
        Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' 9-'#1083#1086#1084#1072#1085#1085#1091#1102
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = cbShowL9Click
      end
    end
    object T_Panel: TPanel
      Left = 5
      Top = 188
      Width = 154
      Height = 165
      TabOrder = 2
      object Label2: TLabel
        Left = 8
        Top = 34
        Width = 94
        Height = 13
        Caption = #1043#1083#1091#1073#1080#1085#1072' '#1088#1077#1082#1091#1088#1089#1080#1080':'
      end
      object cbTRandomFractal: TCheckBox
        Left = 8
        Top = 78
        Width = 145
        Height = 17
        Caption = #1057#1083#1091#1095#1072#1081#1085#1099#1081' '#1076#1086' '#1075#1083#1091#1073#1080#1085#1099
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = cbShowTClick
      end
      object cbShowT: TCheckBox
        Left = 8
        Top = 14
        Width = 145
        Height = 17
        Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' T-'#1092#1088#1072#1082#1090#1072#1083
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = cbShowTClick
      end
      object T_Depth: TSpinEdit
        Left = 32
        Top = 51
        Width = 73
        Height = 22
        MaxValue = 100
        MinValue = 1
        TabOrder = 2
        Value = 6
        OnChange = L9_DepthChange
      end
      object maxRandomDepth: TSpinEdit
        Left = 32
        Top = 99
        Width = 73
        Height = 22
        MaxValue = 100
        MinValue = 1
        TabOrder = 3
        Value = 4
        OnChange = L9_DepthChange
      end
      object ReDraw: TBitBtn
        Left = 10
        Top = 131
        Width = 135
        Height = 25
        Caption = '&'#1055#1077#1088#1077#1075#1077#1085#1077#1088#1080#1088#1086#1074#1072#1090#1100
        Kind = bkRetry
        NumGlyphs = 2
        TabOrder = 4
        OnClick = ReDrawClick
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 451
    Width = 609
    Height = 19
    Panels = <
      item
        Width = 50
      end>
  end
  object MainMenu1: TMainMenu
    Left = 584
    Top = 8
    object N1: TMenuItem
      Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088#1099
      object N2: TMenuItem
        Caption = #1055#1088#1072#1074#1080#1083#1100#1085#1099#1081' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082
      end
      object N3: TMenuItem
        Caption = #1057#1083#1091#1095#1072#1081#1085#1099#1081' '#1085#1072#1073#1086#1088' '#1090#1086#1095#1077#1082
      end
    end
    object N4: TMenuItem
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      OnClick = N4Click
    end
  end
end
