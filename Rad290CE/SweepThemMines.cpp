/* **************************************************************************
SweepThemMines.cpp
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
#endif // #if !define(__clang__)
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("..\Source\MainForm.cpp", FormMain);
//---------------------------------------------------------------------------
#include <tchar.h>
//---------------------------------------------------------------------------
#include "App.h"
//---------------------------------------------------------------------------
using namespace SweepThemMines;
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        _TCHAR** argvP = __targv;
        TApp* app = &TApp::GetInstance();
        app->Initialize(false, "ASWSoftware\\SweepThemMines\\", __argc, argvP);
        if (TApp::IsAppTerminating())
            return static_cast<int>(TApp::ExitCode);

        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        TStyleManager::TrySetStyle("Cobalt XEMedia");
        Application->Title = "Sweep Them Mines";
        Application->CreateForm(__classid(TFormMain), &FormMain);
        Application->Run();
    }
    catch (Exception& exception)
    {
        Application->ShowException(&exception);
        TApp::ExitCode = TApp::EExitCode::Err_Exception;
    }
    catch (...)
    {
        TApp::ExitCode = TApp::EExitCode::Err_Exception;
        try
        {
            throw Exception("");
        }
        catch (Exception& exception)
        {
            Application->ShowException(&exception);
        }
    }

    return static_cast<int>(TApp::ExitCode);
}
//---------------------------------------------------------------------------
