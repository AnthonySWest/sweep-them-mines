object FormCustomField: TFormCustomField
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Custom Field'
  ClientHeight = 211
  ClientWidth = 284
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poScreenCenter
  Scaled = False
  TextHeight = 21
  object LblHeight: TLabel
    Left = 24
    Top = 25
    Width = 49
    Height = 21
    Caption = '&Height:'
  end
  object LblWidth: TLabel
    Left = 24
    Top = 64
    Width = 45
    Height = 21
    Caption = '&Width:'
  end
  object LblMines: TLabel
    Left = 24
    Top = 101
    Width = 45
    Height = 21
    Caption = '&Mines:'
  end
  object EditHeight: TSpinEdit
    Left = 96
    Top = 22
    Width = 121
    Height = 31
    MaxValue = 1000
    MinValue = 1
    TabOrder = 0
    Value = 16
  end
  object EditWidth: TSpinEdit
    Left = 96
    Top = 61
    Width = 121
    Height = 31
    MaxValue = 1000
    MinValue = 1
    TabOrder = 1
    Value = 30
  end
  object EditMines: TSpinEdit
    Left = 96
    Top = 98
    Width = 121
    Height = 31
    MaxValue = 9999
    MinValue = 1
    TabOrder = 2
    Value = 0
  end
  object BtnOK: TBitBtn
    Left = 112
    Top = 168
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = BtnOKClick
  end
  object BtnCancel: TBitBtn
    Left = 201
    Top = 168
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
end
