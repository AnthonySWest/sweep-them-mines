/* **************************************************************************
MainForm.cpp
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
#include "MainForm.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// MsgDlg
// A messagebox wrapper
// //////////////////////////////////////////////////////////////////////////
int MsgDlg(UnicodeString const& msg, UnicodeString const& title, TMsgDlgType dlgType, TMsgDlgButtons buttons)
{
    TForm* msgDlg = CreateMessageDialog(msg, dlgType, buttons);
    TModalResult mRes;

    msgDlg->Color = clWindow;

    if (title.Length() > 0)
        msgDlg->Caption = title;
    else
        msgDlg->Caption = Application->Title;

//    msgDlg->GlassFrame->Bottom = 40;
//    msgDlg->GlassFrame->Enabled = true;
//    msgDlg->Font->Name = "Verdana";
//    msgDlg->Font->Size = 10;
//    msgDlg->Canvas->Font = msgDlg->Font;
//    msgDlg->Scaled = false;

    mRes = msgDlg->ShowModal();
    delete msgDlg;
    return mRes;
}
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormMain class
// //////////////////////////////////////////////////////////////////////////

TFormMain* FormMain;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDestroy(TObject* /*Sender*/)
{
    // Clean up
}
//---------------------------------------------------------------------------
