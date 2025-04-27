object FormCustomField: TFormCustomField
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Custom Field'
  ClientHeight = 231
  ClientWidth = 284
  Color = clBtnFace
  ParentFont = True
  Position = poScreenCenter
  Scaled = False
  DesignSize = (
    284
    231)
  TextHeight = 15
  object LblHeight: TLabel
    Left = 23
    Top = 49
    Width = 39
    Height = 15
    Caption = '&Height:'
  end
  object LblWidth: TLabel
    Left = 23
    Top = 88
    Width = 35
    Height = 15
    Caption = '&Width:'
  end
  object LblMines: TLabel
    Left = 23
    Top = 125
    Width = 35
    Height = 15
    Caption = '&Mines:'
  end
  object LblLimit: TLabel
    Left = 8
    Top = 8
    Width = 145
    Height = 15
    Caption = 'Limits: 100x100, 9999 mines'
  end
  object EditHeight: TSpinEdit
    Left = 88
    Top = 46
    Width = 100
    Height = 24
    MaxValue = 1000
    MinValue = 1
    TabOrder = 0
    Value = 16
  end
  object EditWidth: TSpinEdit
    Left = 88
    Top = 85
    Width = 100
    Height = 24
    MaxValue = 1000
    MinValue = 1
    TabOrder = 1
    Value = 30
  end
  object EditMines: TSpinEdit
    Left = 88
    Top = 122
    Width = 130
    Height = 24
    MaxValue = 9999
    MinValue = 1
    TabOrder = 2
    Value = 1
  end
  object BtnOK: TBitBtn
    Left = 112
    Top = 188
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = BtnOKClick
  end
  object BtnCancel: TBitBtn
    Left = 201
    Top = 188
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
end
