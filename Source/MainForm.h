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
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
// END OF IDE INCLUDES
//---------------------------------------------------------------------------
#include "ASWMS_Engine.h"
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
    TScrollBox* ScrollBoxMap;
    TImage* ImageMap;
    TBitBtn* BtnReact;
    TImage* ImageMinesRemaining;
    TImage* ImageTime;
    TTimer* TimerScoreboard;
    TApplicationEvents* ApplicationEvents;
    TMenuItem* N4;
    TMenuItem* MnuQuestionMarks;
    TMenuItem* N5;
    TMenuItem* MnuRules;
    TMenuItem* MnuHints;
    void __fastcall FormDestroy(TObject* Sender);
    void __fastcall MnuExitClick(TObject* Sender);
    void __fastcall MnuAboutClick(TObject* Sender);
    void __fastcall MnuBestTimesClick(TObject* Sender);
    void __fastcall MnuNewGameClick(TObject* Sender);
    void __fastcall MnuStandardDifficultyClick(TObject* Sender);
    void __fastcall MnuResetBestTimesClick(TObject* Sender);
    void __fastcall FormClose(TObject* Sender, TCloseAction& Action);
    void __fastcall MnuGameClick(TObject* Sender);
    void __fastcall FormShow(TObject* Sender);
    void __fastcall ImageMapMouseMove(TObject* Sender, TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject* Sender, TShiftState Shift, int X, int Y);
    void __fastcall ImageMapMouseDown(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall FormResize(TObject* Sender);
    void __fastcall ImageMapMouseUp(TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall TimerScoreboardTimer(TObject* Sender);
    void __fastcall ApplicationEventsRestore(TObject* Sender);
    void __fastcall ApplicationEventsMinimize(TObject* Sender);
    void __fastcall MnuQuestionMarksClick(TObject* Sender);
    void __fastcall FormKeyDown(TObject* Sender, WORD& Key, TShiftState Shift);
    void __fastcall MnuRulesClick(TObject* Sender);
    void __fastcall MnuHintsClick(TObject* Sender);
private: // User declarations
    static char const* const BaseFilename_HighScores;

    int m_CustomCols;
    int m_CustomRows;
    int m_CustomMines;
    bool m_MouseDownOnImage;

    System::String m_BaseFormCaption;

    ASWMS::TMSEngine m_MineSweeper;

private:
    void AddScoresToLines(System::Classes::TStrings* lines, SweepThemMines::TScores::TScoreList const& scores);
    void DrawScoreboards();
    TPoint GetExtendedImageMapMousePos();
    System::String GetHighScoresFilename();
    bool LoadHighScores(SweepThemMines::TScores* scores);
    void NewGame();
    void ReCenter();
    void ResetBestTimes();
    void ResizeFormToImageMap();
    void SaveBestScores(SweepThemMines::TScores& scores);
    void SaveBestTime_Beginner(int seconds, AnsiString const& name);
    void SaveBestTime_Expert(int seconds, AnsiString const& name);
    void SaveBestTime_Intermediate(int seconds, AnsiString const& name);
    void ShowBestTimes();
    void ShowHints();
    void ShowRules();
    TModalResult ShowCustomDifficulty();

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
