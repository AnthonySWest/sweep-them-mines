/* **************************************************************************
CustomFieldForm.h
Author: Anthony S. West - ASW Software

Copyright 2025 Anthony S. West

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************** */

#ifndef CustomFieldFormH
#define CustomFieldFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Samples.Spin.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormCustomField class
// //////////////////////////////////////////////////////////////////////////
class TFormCustomField : public TForm
{
__published: // IDE-managed Components
    TSpinEdit* EditHeight;
    TLabel* LblHeight;
    TLabel* LblWidth;
    TLabel* LblMines;
    TSpinEdit* EditWidth;
    TSpinEdit* EditMines;
    TBitBtn* BtnOK;
    TBitBtn* BtnCancel;
    void __fastcall BtnOKClick(TObject* Sender);

private: // User declarations

public: // User declarations
#if defined(__clang__)
    __fastcall TFormCustomField(TComponent* Owner) override;
#else
    __fastcall TFormCustomField(TComponent* Owner);
#endif
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCustomField* FormCustomField;
//---------------------------------------------------------------------------
#endif // #ifndef CustomFieldFormH
