/* **************************************************************************
CustomFieldForm.cpp
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

#include <vcl.h>
#if !defined(__clang__)
#pragma hdrstop
#endif

//---------------------------------------------------------------------------
// Module header
#include "CustomFieldForm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormCustomField class
// //////////////////////////////////////////////////////////////////////////

TFormCustomField* FormCustomField;

//---------------------------------------------------------------------------
__fastcall TFormCustomField::TFormCustomField(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormCustomField::BtnOKClick(TObject* /*Sender*/)
{
    if (EditWidth->Value > 100)
        EditWidth->Value = 100;

    if (EditHeight->Value > 100)
        EditHeight->Value = 100;

    if (EditMines->Value > 100 * 100)
        EditMines->Value = 100 * 100;
}
//---------------------------------------------------------------------------
