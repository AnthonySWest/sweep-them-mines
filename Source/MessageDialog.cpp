/* **************************************************************************
MessageDialog.cpp
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
#endif // #if !defined(__clang__)

//---------------------------------------------------------------------------
// Module header
#include "MessageDialog.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
using namespace System;
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/*
    MsgDlg

    A messagebox wrapper
*/
int MsgDlg(String const& msg, String const& title, TMsgDlgType dlgType, TMsgDlgButtons buttons)
{
    TForm* msgDlg = CreateMessageDialog(msg, dlgType, buttons);
    std::unique_ptr<TForm> auto_msgDlg(msgDlg);
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
    msgDlg->Scaled = false;

    mRes = msgDlg->ShowModal();
    return mRes;
}
//---------------------------------------------------------------------------

} // namespace SweepThemMines
