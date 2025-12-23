object F: TF
  Left = 133
  Top = 149
  Width = 663
  Height = 524
  Caption = #1050#1086#1084#1087#1100#1102#1090#1077#1088#1085#1072#1103' '#1075#1088#1072#1092#1080#1082#1072' - '#1051#1072#1073#1086#1088#1072#1090#1086#1088#1085#1072#1103' '#1088#1072#1073#1086#1090#1072' '#8470'1'
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
    Width = 185
    Height = 451
    Align = alLeft
    TabOrder = 0
    DesignSize = (
      185
      451)
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 96
      Height = 13
      Caption = #1050#1086#1086#1088#1076#1080#1085#1072#1090#1099' '#1090#1086#1095#1077#1082':'
    end
    object Label2: TLabel
      Left = 8
      Top = 308
      Width = 130
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = #1043#1088#1072#1092#1080#1095#1077#1089#1082#1080#1077' '#1087#1088#1080#1084#1080#1090#1080#1074#1099':'
    end
    object Label3: TLabel
      Left = 8
      Top = 364
      Width = 73
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = #1056#1072#1079#1084#1077#1088' '#1090#1086#1095#1082#1080':'
    end
    object Label4: TLabel
      Left = 96
      Top = 364
      Width = 82
      Height = 13
      Anchors = [akLeft, akBottom]
      Caption = #1058#1086#1083#1097#1080#1085#1072' '#1083#1080#1085#1080#1080':'
    end
    object PointsGrid_: TStringGrid
      Left = 8
      Top = 24
      Width = 169
      Height = 245
      Anchors = [akLeft, akTop, akBottom]
      ColCount = 1
      DefaultRowHeight = 18
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
      ScrollBars = ssVertical
      TabOrder = 0
      OnSetEditText = PointsGrid_SetEditText
    end
    object cbStyle: TComboBox
      Left = 8
      Top = 324
      Width = 169
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akBottom]
      DropDownCount = 11
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 1
      Text = 'GL_POINTS'
      OnChange = cbStyleChange
      Items.Strings = (
        'GL_POINTS'
        'GL_LINES'
        'GL_LINE_STRIP'
        'GL_LINE_LOOP'
        'GL_TRIANGLES'
        'GL_TRIANGLE_STRIP'
        'GL_TRIANGLE_FAN'
        'GL_QUADS'
        'GL_QUAD_STRIP'
        'GL_POLYGON')
    end
    object sePointSize: TSpinEdit
      Left = 8
      Top = 380
      Width = 73
      Height = 22
      Anchors = [akLeft, akBottom]
      MaxValue = 16
      MinValue = 1
      TabOrder = 2
      Value = 2
      OnChange = sePointSizeChange
    end
    object seLineWidth: TSpinEdit
      Left = 96
      Top = 380
      Width = 73
      Height = 22
      Anchors = [akLeft, akBottom]
      MaxValue = 100
      MinValue = 1
      TabOrder = 3
      Value = 2
      OnChange = sePointSizeChange
    end
    object cbNums: TCheckBox
      Left = 8
      Top = 412
      Width = 169
      Height = 17
      Anchors = [akLeft, akBottom]
      Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1085#1086#1084#1077#1088#1072' '#1090#1086#1095#1077#1082
      TabOrder = 4
      OnClick = cbNumsClick
    end
    object BitBtn1: TBitBtn
      Left = 32
      Top = 275
      Width = 121
      Height = 25
      Anchors = [akLeft, akBottom]
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1090#1086#1095#1082#1091
      TabOrder = 5
      OnClick = BitBtn1Click
      Kind = bkOK
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 451
    Width = 655
    Height = 19
    Panels = <
      item
        Width = 50
      end>
  end
  object MainMenu1: TMainMenu
    Left = 352
    Top = 65520
    object N1: TMenuItem
      Caption = #1043#1077#1085#1077#1088#1072#1090#1086#1088#1099
      object N2: TMenuItem
        Caption = #1055#1088#1072#1074#1080#1083#1100#1085#1099#1081' '#1084#1085#1086#1075#1086#1091#1075#1086#1083#1100#1085#1080#1082
        OnClick = N2Click
      end
      object N3: TMenuItem
        Caption = #1057#1083#1091#1095#1072#1081#1085#1099#1081' '#1085#1072#1073#1086#1088' '#1090#1086#1095#1077#1082
        OnClick = N3Click
      end
    end
    object N4: TMenuItem
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      OnClick = N4Click
    end
  end
end
