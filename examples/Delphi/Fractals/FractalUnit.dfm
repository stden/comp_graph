object F: TF
  Left = 110
  Top = 62
  Caption = 'F'
  ClientHeight = 607
  ClientWidth = 833
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object I: TImage
    Left = 208
    Top = 0
    Width = 625
    Height = 606
    Align = alCustom
    OnDragOver = IDragOver
  end
  object Label1: TLabel
    Left = 8
    Top = 328
    Width = 94
    Height = 13
    Caption = #1043#1083#1091#1073#1080#1085#1072' '#1088#1077#1082#1091#1088#1089#1080#1080':'
  end
  object Label2: TLabel
    Left = 8
    Top = 32
    Width = 168
    Height = 13
    Caption = #1058#1086#1095#1082#1080' '#1079#1072#1076#1072#1102#1097#1080#1077' '#1092#1088#1072#1082#1090#1072#1083' ('#1103#1076#1088#1086'):'
  end
  object Label3: TLabel
    Left = 16
    Top = 207
    Width = 73
    Height = 13
    Caption = '"'#1044#1083#1080#1085#1072'" '#1103#1076#1088#1072':'
  end
  object BasicLen: TLabel
    Left = 104
    Top = 207
    Width = 44
    Height = 13
    Caption = 'BasicLen'
  end
  object Start: TShape
    Left = 320
    Top = 424
    Width = 9
    Height = 9
    Brush.Color = clLime
    DragMode = dmAutomatic
    Shape = stCircle
  end
  object Finish: TShape
    Left = 744
    Top = 464
    Width = 9
    Height = 9
    Brush.Color = clSilver
    DragMode = dmAutomatic
    Shape = stCircle
  end
  object Shape1: TShape
    Left = 8
    Top = 539
    Width = 25
    Height = 20
    Brush.Color = 33023
    OnMouseDown = ShapeMouseDown
  end
  object Label5: TLabel
    Left = 16
    Top = 520
    Width = 6
    Height = 13
    Caption = '1'
  end
  object Shape2: TShape
    Left = 40
    Top = 539
    Width = 25
    Height = 20
    Brush.Color = clRed
    OnMouseDown = ShapeMouseDown
  end
  object Label4: TLabel
    Left = 48
    Top = 520
    Width = 6
    Height = 13
    Caption = '2'
  end
  object Shape3: TShape
    Left = 72
    Top = 539
    Width = 25
    Height = 20
    Brush.Color = clPurple
    OnMouseDown = ShapeMouseDown
  end
  object Label6: TLabel
    Left = 80
    Top = 520
    Width = 6
    Height = 13
    Caption = '3'
  end
  object Shape4: TShape
    Left = 104
    Top = 539
    Width = 25
    Height = 20
    Brush.Color = clNavy
    OnMouseDown = ShapeMouseDown
  end
  object Label7: TLabel
    Left = 112
    Top = 520
    Width = 6
    Height = 13
    Caption = '4'
  end
  object Shape5: TShape
    Left = 137
    Top = 539
    Width = 25
    Height = 20
    Brush.Color = clGreen
    OnMouseDown = ShapeMouseDown
  end
  object Label8: TLabel
    Left = 144
    Top = 520
    Width = 6
    Height = 13
    Caption = '5'
  end
  object Shape6: TShape
    Left = 168
    Top = 539
    Width = 25
    Height = 20
    Brush.Color = clMaroon
    OnMouseDown = ShapeMouseDown
  end
  object Label9: TLabel
    Left = 176
    Top = 520
    Width = 6
    Height = 13
    Caption = '6'
  end
  object Shape7: TShape
    Left = 8
    Top = 579
    Width = 25
    Height = 20
    Brush.Color = 4202527
    OnMouseDown = ShapeMouseDown
  end
  object Label10: TLabel
    Left = 16
    Top = 563
    Width = 6
    Height = 13
    Caption = '7'
  end
  object Shape8: TShape
    Left = 40
    Top = 579
    Width = 25
    Height = 20
    Brush.Color = 16709
    OnMouseDown = ShapeMouseDown
  end
  object Label11: TLabel
    Left = 48
    Top = 563
    Width = 6
    Height = 13
    Caption = '8'
  end
  object Shape9: TShape
    Left = 72
    Top = 579
    Width = 25
    Height = 20
    Brush.Color = 4191
    OnMouseDown = ShapeMouseDown
  end
  object Label12: TLabel
    Left = 80
    Top = 563
    Width = 6
    Height = 13
    Caption = '9'
  end
  object Shape10: TShape
    Left = 104
    Top = 579
    Width = 25
    Height = 20
    Brush.Color = 33023
    OnMouseDown = ShapeMouseDown
  end
  object Label13: TLabel
    Left = 112
    Top = 563
    Width = 12
    Height = 13
    Caption = '10'
  end
  object Shape11: TShape
    Left = 136
    Top = 579
    Width = 25
    Height = 20
    Brush.Color = 33023
    OnMouseDown = ShapeMouseDown
  end
  object Label14: TLabel
    Left = 144
    Top = 563
    Width = 12
    Height = 13
    Caption = '11'
  end
  object Shape12: TShape
    Left = 168
    Top = 579
    Width = 25
    Height = 20
    Brush.Color = 33023
    OnMouseDown = ShapeMouseDown
  end
  object Label15: TLabel
    Left = 176
    Top = 563
    Width = 12
    Height = 13
    Caption = '12'
  end
  object DepthLimit: TSpinEdit
    Left = 80
    Top = 344
    Width = 97
    Height = 26
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MaxValue = 20000000
    MinValue = 1
    ParentFont = False
    TabOrder = 0
    Value = 3
    OnChange = DepthLimitChange
  end
  object Base: TStringGrid
    Left = 8
    Top = 48
    Width = 193
    Height = 145
    ColCount = 3
    DefaultRowHeight = 16
    TabOrder = 1
    OnSelectCell = BaseSelectCell
  end
  object LineWidth: TRadioGroup
    Left = 8
    Top = 400
    Width = 185
    Height = 89
    Caption = ' '#1058#1086#1083#1097#1080#1085#1072' '#1086#1090#1088#1077#1079#1082#1086#1074' '
    ItemIndex = 2
    Items.Strings = (
      #1059#1074#1077#1083#1080#1095#1080#1074#1072#1090#1100' '#1074' '#1088#1077#1082#1091#1088#1089#1080#1080
      #1059#1084#1077#1085#1100#1096#1072#1090#1100' '#1074' '#1088#1077#1082#1091#1088#1089#1080#1080
      #1053#1077' '#1080#1079#1084#1077#1085#1103#1090#1100
      #1056#1080#1089#1086#1074#1072#1090#1100' '#1087#1086#1089#1083#1077#1076#1085#1080#1081' '#1096#1072#1075)
    TabOrder = 2
    OnClick = LineWidthClick
  end
  object DrawPoints: TCheckBox
    Left = 8
    Top = 496
    Width = 185
    Height = 17
    Caption = #1050#1088#1091#1078#1086#1095#1082#1080' '#1085#1072' '#1082#1086#1085#1094#1072#1093' '#1086#1090#1088#1077#1079#1082#1086#1074
    TabOrder = 3
    OnClick = DrawPointsClick
  end
  object Edit: TEdit
    Left = 56
    Top = 8
    Width = 73
    Height = 19
    Ctl3D = False
    ParentCtl3D = False
    TabOrder = 4
    Text = 'Edit'
    Visible = False
    OnExit = EditExit
  end
  object XML_Doc: TXMLDocument
    Left = 800
    Top = 16
    DOMVendorDesc = 'MSXML'
  end
  object ColorDialog1: TColorDialog
    Left = 120
    Top = 280
  end
end
