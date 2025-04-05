/* **************************************************************************
MainForm.h
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

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
// END OF IDE INCLUDES
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormMain class
// //////////////////////////////////////////////////////////////////////////
class TFormMain : public TForm
{
__published: // IDE-managed Components
    void __fastcall FormDestroy(TObject* Sender);
private: // User declarations

public:  // User declarations
    __fastcall TFormMain(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain* FormMain;

int MsgDlg(UnicodeString const& msg, UnicodeString const& title, TMsgDlgType dlgType, TMsgDlgButtons buttons);
//---------------------------------------------------------------------------

#endif // #ifndef MainFormH
