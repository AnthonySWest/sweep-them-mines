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
#include "ASWTools_Path.h"
//---------------------------------------------------------------------------
#include "App.h"
#include "MessageDialog.h"
//---------------------------------------------------------------------------
using namespace ASWTools;
using namespace SweepThemMines;
using namespace System;
//---------------------------------------------------------------------------

// //////////////////////////////////////////////////////////////////////////
// TFormMain class
// //////////////////////////////////////////////////////////////////////////

char const* const TFormMain::BaseFilename_HighScores = "HS.dat";

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
void TFormMain::AddScoresToLines(TStrings* lines, TScores::TScoreList const& scores)
{
    if (0 == scores.size())
    {
        lines->Add("999 seconds\t Anonymous");
        return;
    }

    for (TScores::TScoreList::const_iterator it = scores.begin(); it != scores.end(); it++)
    {
        TScore const& item = *it;
        lines->Add(IntToStr(item.Seconds) + " seconds\t " + item.Name.c_str());
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormClose(TObject* /*sender*/, TCloseAction& /*Action*/)
{
    ExitApp();
}
//---------------------------------------------------------------------------
void TFormMain::ExitApp()
{
    TApp::GetInstance().TerminateApp();
    Application->Terminate();
}
//---------------------------------------------------------------------------
String TFormMain::GetHighScoresFilename()
{
    TApp* app = &TApp::GetInstance();
    return TPathTool::Combine(app->DirAppData, BaseFilename_HighScores).c_str();
}
//---------------------------------------------------------------------------
bool TFormMain::LoadHighScores(TScores* scores)
{
    try
    {
        AnsiString filename = GetHighScoresFilename();
        if (!FileExists(filename))
            return true;

        scores->Load(filename.c_str());

        if (!scores->ValidateCheck())
        {
            String msg = "Error: Best Times data has been modified outisde of " + Application->Title + ".";
            MsgDlg(msg, "Monkey Business", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
            return false;
        }

        return true;
    }
    catch (const std::runtime_error& error)
    {
        String msg = String("Failed to load scores: ") + error.what();
        MsgDlg(msg, "", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }
    catch (...)
    {
        String msg = String("Failed to load scores: Unknown exception.");
        MsgDlg(msg, "", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }

    return false;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuAboutClick(TObject* /*sender*/)
{
    TApp* app = &TApp::GetInstance();
    String msg = "Sweep Them Mines - A nod to Win98 Minesweeper\n\n" +
        String("Version: ") + String(app->GetAppVer()->ToStrVer().c_str());
#if defined(_DEBUG)
    msg += " - Debug";
#endif
    msg += "\n\nCopyright (c) 2025 Anthony S. West\n\n";
    msg += "Source: https://github.com/AnthonySWest/sweep-them-mines";
    MsgDlg(msg, "", TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuBestTimesClick(TObject* /*sender*/)
{
    ShowBestTimes();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuExitClick(TObject* /*sender*/)
{
    ExitApp();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuGameClick(TObject* /*sender*/)
{
    MnuResetBestTimes->Enabled = FileExists(GetHighScoresFilename());
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuNewGameClick(TObject* /*sender*/)
{
    NewGame();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuResetBestTimesClick(TObject* /*sender*/)
{
    ResetBestTimes();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MnuStandardDifficultyClick(TObject* sender)
{
    MnuBeginner->Checked = false;
    MnuIntermediate->Checked = false;
    MnuExpert->Checked = false;
    MnuCustom->Checked = false;

    // Check the selected item
    TMenuItem* item = dynamic_cast<TMenuItem*>(sender);
    if (nullptr != item)
    {
        item->Checked = true;

        if ("MnuCustom" == item->Name)
            ShowCustomDifficulty();
    }
}
//---------------------------------------------------------------------------
void TFormMain::NewGame()
{
    MsgDlg("To do", "", TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
}
//---------------------------------------------------------------------------
void TFormMain::ResetBestTimes()
{
    String filename =  GetHighScoresFilename();
    if (!FileExists(filename))
    {
        MsgDlg("Best scores are the defaults.", "", TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
        return;
    }

    TModalResult mRes = MsgDlg("Reset best scores?", "Best Scores Reset", TMsgDlgType::mtConfirmation,
        TMsgDlgButtons() << TMsgDlgBtn::mbYes << TMsgDlgBtn::mbNo);
    if (mrYes != mRes)
        return;

    DeleteFile(filename);

    MsgDlg("Best scores were reset to defaults.", "", TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
}
//---------------------------------------------------------------------------
void TFormMain::SaveBestScores(TScores& scores)
{
    try
    {
        AnsiString filename = GetHighScoresFilename();
        scores.TrimScores(); // Only keep the top N scores
        scores.Save(filename.c_str(), true);
    }
    catch (const std::runtime_error& error)
    {
        String msg = String("Failed to save the best times: ") + error.what();
        MsgDlg(msg, "", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }
    catch (Exception const& ex)
    {
        String msg = String("Failed to save the best times: ") + ex.Message;
        MsgDlg(msg, "", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }
    catch (...)
    {
        String msg = String("Failed to save the best times: Unknown exception.");
        MsgDlg(msg, "", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }
}
//---------------------------------------------------------------------------
void TFormMain::SaveBestTime_Beginner(int seconds, AnsiString const& name)
{
    TScores scores;
    if (!LoadHighScores(&scores))
        return;
    scores.AddScore(scores.Beginner, seconds, name.c_str());
    SaveBestScores(scores);
}
//---------------------------------------------------------------------------
void TFormMain::SaveBestTime_Expert(int seconds, AnsiString const& name)
{
    TScores scores;
    if (!LoadHighScores(&scores))
        return;
    scores.AddScore(scores.Expert, seconds, name.c_str());
    SaveBestScores(scores);
}
//---------------------------------------------------------------------------
void TFormMain::SaveBestTime_Intermediate(int seconds, AnsiString const& name)
{
    TScores scores;
    if (!LoadHighScores(&scores))
        return;
    scores.AddScore(scores.Intermediate, seconds, name.c_str());
    SaveBestScores(scores);
}
//---------------------------------------------------------------------------
void TFormMain::ShowBestTimes()
{
    TScores scores;
    if (!LoadHighScores(&scores))
        return;

    TStringList* dlgLines = new TStringList();
    std::unique_ptr<TStringList> auto_dlgLines(dlgLines);

    dlgLines->Add("Fastest Mine Sweepers:");

    dlgLines->Add("\n----- Beginner mode -----\n");
    AddScoresToLines(dlgLines, scores.Beginner);

    dlgLines->Add("\n----- Intermediate mode -----\n");
    AddScoresToLines(dlgLines, scores.Intermediate);

    dlgLines->Add("\n----- Expert mode -----\n");
    AddScoresToLines(dlgLines, scores.Expert);

    MsgDlg(dlgLines->Text, "Best Times", TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
}
//---------------------------------------------------------------------------
void TFormMain::ShowCustomDifficulty()
{
    MsgDlg("To do", "", TMsgDlgType::mtInformation, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
}
//---------------------------------------------------------------------------
