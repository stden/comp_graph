object FCreateRandom: TFCreateRandom
  Left = 461
  Top = 300
  BorderStyle = bsDialog
  Caption = #1057#1083#1091#1095#1072#1081#1085#1099#1081' '#1085#1072#1073#1086#1088' '#1090#1086#1095#1077#1082
  ClientHeight = 116
  ClientWidth = 296
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 32
    Width = 103
    Height = 13
    Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1074#1077#1088#1096#1080#1085':'
  end
  object BitBtn1: TBitBtn
    Left = 152
    Top = 72
    Width = 121
    Height = 25
    Caption = #1057#1086#1079#1076#1072#1090#1100
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
    OnClick = BitBtn1Click
  end
  object PointsNumber: TSpinEdit
    Left = 128
    Top = 24
    Width = 81
    Height = 26
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxValue = 0
    MinValue = 0
    ParentFont = False
    TabOrder = 1
    Value = 20
  end
end
