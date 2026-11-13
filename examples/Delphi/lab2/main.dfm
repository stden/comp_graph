object frmMain: TfrmMain
  Left = 203
  Top = 19
  Width = 748
  Height = 656
  AlphaBlend = False
  AlphaBlendValue = 220
  Caption = #1050#1086#1084#1087#1100#1102#1090#1077#1088#1085#1072#1103' '#1075#1088#1072#1092#1080#1082#1072' - '#1051#1072#1073#1086#1088#1072#1090#1086#1088#1085#1072#1103' '#1088#1072#1073#1086#1090#1072' '#8470'2'
  Color = clWindow
  Constraints.MinHeight = 420
  Constraints.MinWidth = 550
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 205
    Height = 603
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
    object Panel3: TPanel
      Left = 0
      Top = 169
      Width = 205
      Height = 434
      Align = alClient
      TabOrder = 0
      object ScrollBox1: TScrollBox
        Left = 1
        Top = 1
        Width = 203
        Height = 432
        VertScrollBar.Smooth = True
        VertScrollBar.Style = ssFlat
        VertScrollBar.Tracking = True
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        TabOrder = 0
        object Label2: TLabel
          Left = 48
          Top = 192
          Width = 21
          Height = 13
          Caption = 'REF'
        end
        object Label3: TLabel
          Left = 8
          Top = 280
          Width = 126
          Height = 13
          Caption = #1057#1087#1086#1089#1086#1073' '#1089#1084#1077#1096#1077#1085#1080#1103' '#1085#1086#1074#1099#1093
        end
        object Label4: TLabel
          Left = 8
          Top = 328
          Width = 184
          Height = 13
          Caption = #1057#1087#1086#1089#1086#1073' '#1089#1084#1077#1096#1077#1085#1080#1103' '#1091#1078#1077' '#1085#1072#1093#1086#1076#1103#1097#1080#1093#1089#1103
        end
        object Label5: TLabel
          Left = 29
          Top = 56
          Width = 7
          Height = 13
          Caption = 'X'
        end
        object Label6: TLabel
          Left = 125
          Top = 56
          Width = 7
          Height = 13
          Caption = 'Y'
        end
        object Label7: TLabel
          Left = 19
          Top = 86
          Width = 28
          Height = 13
          Caption = 'Width'
        end
        object Label8: TLabel
          Left = 112
          Top = 87
          Width = 31
          Height = 13
          Caption = 'Height'
        end
        object Label9: TLabel
          Left = 24
          Top = 327
          Width = 3
          Height = 13
        end
        object Label10: TLabel
          Left = 40
          Top = 8
          Width = 117
          Height = 13
          Caption = 'GL_SCISSOR_TEST'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label11: TLabel
          Left = 48
          Top = 120
          Width = 103
          Height = 13
          Caption = 'GL_ALPHA_TEST'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label12: TLabel
          Left = 64
          Top = 232
          Width = 65
          Height = 13
          Caption = 'GL_BLEND'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object cbAlpha: TCheckBox
          Left = 8
          Top = 136
          Width = 177
          Height = 17
          Caption = #1056#1072#1079#1088#1077#1096#1080#1090#1100' '#1090#1077#1089#1090' Alpha - '#1082#1072#1085#1072#1083#1072
          TabOrder = 0
          OnClick = cbAlphaClick
        end
        object cbAlphaFunc: TComboBox
          Left = 24
          Top = 160
          Width = 169
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 4
          TabOrder = 1
          Text = 'GL_GREATER'
          OnChange = cbAlphaFuncChange
          Items.Strings = (
            'GL_NEVER     '
            'GL_LESS  '
            'GL_EQUAL'
            'GL_LEQUAL'
            'GL_GREATER'
            'GL_NOTEQUAL'
            'GL_GEQUAL'
            'GL_ALWAYS')
        end
        object edtRef: TEdit
          Left = 74
          Top = 192
          Width = 47
          Height = 21
          TabOrder = 2
          Text = '0.25'
          OnChange = edtRefChange
        end
        object cbSFactor: TComboBox
          Left = 24
          Top = 296
          Width = 169
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 3
          Text = 'GL_ZERO'
          OnChange = cbSFactorChange
          Items.Strings = (
            'GL_ZERO'
            'GL_ONE'
            'GL_DST_COLOR'
            'GL_ONE_MINUS_DST_COLOR'
            'GL_SRC_ALPHA'
            'GL_ONE_MINUS_SRC_ALPHA'
            'GL_DST_ALPHA'
            'GL_ONE_MINUS_DST_ALPHA'
            'GL_SRC_ALPHA_SATURATE')
        end
        object cbDFactor: TComboBox
          Left = 24
          Top = 344
          Width = 169
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 4
          Text = 'GL_ZERO'
          OnChange = cbDFactorChange
          Items.Strings = (
            'GL_ZERO'
            'GL_ONE'
            'GL_SRC_COLOR'
            'GL_ONE_MINUS_SRC_COLOR'
            'GL_SRC_ALPHA'
            'GL_ONE_MINUS_SRC_ALPHA'
            'GL_DST_ALPHA'
            'GL_ONE_MINUS_DST_ALPHA')
        end
        object cbBlend: TCheckBox
          Left = 8
          Top = 256
          Width = 169
          Height = 17
          Hint = 'glEnable(GL_BLEND);'#13#10'glBlendFunc(sfactor,dfactor);'
          Caption = #1056#1072#1079#1088#1077#1096#1080#1090#1100' '#1089#1084#1077#1096#1077#1085#1080#1077' '#1094#1074#1077#1090#1086#1074
          ParentShowHint = False
          ShowHint = True
          TabOrder = 5
          OnClick = cbBlendClick
        end
        object cbScissor: TCheckBox
          Left = 8
          Top = 24
          Width = 177
          Height = 17
          Hint = 
            #1058#1077#1089#1090' '#1086#1090#1089#1077#1095#1077#1085#1080#1103'. '#1056#1077#1078#1080#1084' GL_SCISSOR_TEST '#1088#1072#1079#1088#1077#1096#1072#1077#1090' '#1086#1090#1089#1077#1095#1077#1085#1080#1077' '#13#10#1090#1077#1093' ' +
            #1092#1088#1072#1075#1084#1077#1085#1090#1086#1074' '#1086#1073#1098#1077#1082#1090#1072', '#1082#1086#1090#1086#1088#1099#1077' '#1085#1072#1093#1086#1076#1103#1090#1089#1103' '#1074#1085#1077' '#1087#1088#1103#1084#1086#1091#1075#1086#1083#1100#1085#1080#1082#1072' "'#1074#1099#1088#1077#1079#1082 +
            #1080'".'
          Caption = #1056#1072#1079#1088#1077#1096#1080#1090#1100' '#1090#1077#1089#1090' "'#1086#1090#1089#1077#1095#1077#1085#1080#1103'"'
          ParentShowHint = False
          ShowHint = True
          TabOrder = 6
          OnClick = cbScissorClick
        end
        object edtSx: TEdit
          Left = 50
          Top = 52
          Width = 47
          Height = 21
          TabOrder = 7
          Text = '100'
          OnChange = edtSxChange
        end
        object edtSy: TEdit
          Left = 146
          Top = 52
          Width = 47
          Height = 21
          TabOrder = 8
          Text = '200'
          OnChange = edtSyChange
        end
        object edtSW: TEdit
          Left = 50
          Top = 82
          Width = 47
          Height = 21
          TabOrder = 9
          Text = '300'
          OnChange = edtSWChange
        end
        object edtSh: TEdit
          Left = 146
          Top = 84
          Width = 47
          Height = 21
          TabOrder = 10
          Text = '200'
          OnChange = edtShChange
        end
        object PointsShow: TComboBox
          Left = 24
          Top = 400
          Width = 169
          Height = 21
          Style = csDropDownList
          ItemHeight = 13
          ItemIndex = 0
          TabOrder = 11
          Text = #1053#1080#1095#1077#1075#1086' '#1085#1077' '#1087#1086#1082#1072#1079#1099#1074#1072#1090#1100
          OnChange = cbSFactorChange
          Items.Strings = (
            #1053#1080#1095#1077#1075#1086' '#1085#1077' '#1087#1086#1082#1072#1079#1099#1074#1072#1090#1100
            #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1085#1086#1084#1077#1088#1072' '#1090#1086#1095#1077#1082
            #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' Alpha '#1090#1086#1095#1077#1082)
        end
      end
    end
    object Panel4: TPanel
      Left = 0
      Top = 0
      Width = 205
      Height = 169
      Align = alTop
      TabOrder = 1
      object Label1: TLabel
        Left = 8
        Top = 8
        Width = 93
        Height = 13
        Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1090#1086#1095#1077#1082':'
      end
      object PointsGrid: TStringGrid
        Left = 8
        Top = 24
        Width = 185
        Height = 137
        ColCount = 1
        Ctl3D = True
        DefaultRowHeight = 18
        FixedCols = 0
        RowCount = 1
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ParentCtl3D = False
        ScrollBars = ssNone
        TabOrder = 0
        OnDblClick = PointsGridDblClick
        OnDrawCell = PointsGridDrawCell
        OnExit = PointsGridExit
        OnSelectCell = PointsGridSelectCell
        OnSetEditText = PointsGridSetEditText
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 603
    Width = 740
    Height = 19
    Panels = <
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 544
    Top = 424
  end
end
