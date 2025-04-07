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
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
// END OF IDE INCLUDES
//---------------------------------------------------------------------------
#include "Scores.h"
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormMain class
// //////////////////////////////////////////////////////////////////////////
class TFormMain : public TForm
{
__published: // IDE-managed Components
    TMainMenu* MainMenu1;
    TMenuItem* MnuGame;
    TMenuItem* MnuHelp;
    TMenuItem* MnuAbout;
    TMenuItem* MnuExit;
    TMenuItem* MnuNewGame;
    TMenuItem* N1;
    TMenuItem* N2;
    TMenuItem* MnuBeginner;
    TMenuItem* MnuIntermediate;
    TMenuItem* MnuExpert;
    TMenuItem* MnuCustom;
    TMenuItem* N3;
    TMenuItem* MnuBestTimes;
    TMenuItem* MnuResetBestTimes;
    void __fastcall FormDestroy(TObject* Sender);
    void __fastcall MnuExitClick(TObject* Sender);
    void __fastcall MnuAboutClick(TObject* Sender);
    void __fastcall MnuBestTimesClick(TObject* Sender);
    void __fastcall MnuNewGameClick(TObject* Sender);
    void __fastcall MnuStandardDifficultyClick(TObject* Sender);
    void __fastcall MnuResetBestTimesClick(TObject* Sender);
    void __fastcall FormClose(TObject* Sender, TCloseAction& Action);
    void __fastcall MnuGameClick(TObject* Sender);
private: // User declarations
    static char const* const BaseFilename_HighScores;

private:
    void AddScoresToLines(System::Classes::TStrings* lines, SweepThemMines::TScores::TScoreList const& scores);
    System::String GetHighScoresFilename();
    void NewGame();
    void ResetBestTimes();
    void ShowBestTimes();
    void ShowCustomDifficulty();

public:  // User declarations
#if defined(__clang__)
    __fastcall TFormMain(TComponent* Owner) override;
#else
    __fastcall TFormMain(TComponent* Owner);
#endif

    void ExitApp();
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain* FormMain;
//---------------------------------------------------------------------------

#endif // #ifndef MainFormH
