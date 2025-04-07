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
#include "App.h"
#include "MessageDialog.h"
//---------------------------------------------------------------------------
using namespace SweepThemMines;
using namespace System;
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormMain class
// //////////////////////////////////////////////////////////////////////////

TFormMain* FormMain;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
    : TForm(Owner)
{
    TApp* app = &TApp::GetInstance();

#if defined(_DEBUG)
    Caption = Caption + " - " + String(app->GetAppVer()->ToStrVer().c_str()) + " - Debug";
#else
    Caption = Caption + " - " + String(app->GetAppVer()->ToStrVer().c_str());
#endif
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDestroy(TObject* /*Sender*/)
{
    // Clean up
}
//---------------------------------------------------------------------------
