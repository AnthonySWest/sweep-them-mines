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
#include "CustomFieldForm.h"
#include "MessageDialog.h"
//---------------------------------------------------------------------------
using namespace ASWMS;
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
    : TForm(Owner),
      m_CustomCols(30),
      m_CustomRows(16),
      m_CustomMines(99)
{
    TApp* app = &TApp::GetInstance();

#if defined(_DEBUG)
    Caption = Caption + " - " + String(app->GetAppVer()->ToStrVer().c_str()) + " - Debug";
#else
    Caption = Caption + " - " + String(app->GetAppVer()->ToStrVer().c_str());
#endif

    AnsiString imagesDir = app->DirImages.c_str();

    if (!DirectoryExists(imagesDir))
    {
        String msg = "Images folder not found at:\n\n" + imagesDir +
            "\n\nPlease specifiy an images location in settings file:\n\n" + String(app->FileAppSettings.c_str());
        MsgDlg(msg, "", TMsgDlgType::mtError, TMsgDlgButtons() << TMsgDlgBtn::mbOK);
    }

    m_MineSweeper.Sprites.LoadSprites(imagesDir.c_str());
    BtnReact->Glyph->Assign(m_MineSweeper.Sprites.FaceHappy.Bmp);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormDestroy(TObject* /*sender*/)
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
void __fastcall TFormMain::FormClose(TObject* /*sender*/, TCloseAction& /*action*/)
{
    ExitApp();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormResize(TObject* /*sender*/)
{
    ReCenter();
}
//---------------------------------------------------------------------------
void TFormMain::ExitApp()
{
    TApp::GetInstance().TerminateApp();
    Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormMouseMove(TObject* /*sender*/, TShiftState shift, int /*x*/, int /*y*/)
{
    m_MineSweeper.DrawMap(ImageMap, shift, -1, -1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject* /*sender*/)
{
    NewGame();
}
//---------------------------------------------------------------------------
TPoint TFormMain::GetExtendedImageMapMousePos()
{
    TPoint imagePoint;
    TPoint scrollBoxPoint;

    // Windows limits the position to a short int value of 32,767. Recalculate so that we have a signed int value.
    scrollBoxPoint.x = Mouse->CursorPos.x - ScrollBoxMap->ClientOrigin.x;
    scrollBoxPoint.y = Mouse->CursorPos.y - ScrollBoxMap->ClientOrigin.y;
    imagePoint.x = scrollBoxPoint.x - ImageMap->Left;
    imagePoint.y = scrollBoxPoint.y - ImageMap->Top;

    return imagePoint;
}
//---------------------------------------------------------------------------
String TFormMain::GetHighScoresFilename()
{
    TApp* app = &TApp::GetInstance();
    return TPathTool::Combine(app->DirAppData, BaseFilename_HighScores).c_str();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ImageMapMouseDown(
    TObject* /*sender*/, TMouseButton /*button*/, TShiftState shift, int /*x*/, int /*y*/)
{
    TPoint pos = GetExtendedImageMapMousePos();

    m_MineSweeper.MouseDown(shift, pos.x, pos.y);
    m_MineSweeper.DrawMap(ImageMap, shift, pos.x, pos.y);

    EGameState state = m_MineSweeper.GetGameState();
    if (shift.Contains(ssLeft) && EGameState::GameOver_Boom != state)
        BtnReact->Glyph->Assign(m_MineSweeper.Sprites.FaceScared.Bmp);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ImageMapMouseMove(TObject* /*sender*/, TShiftState shift, int /*x*/, int /*y*/)
{
    TPoint pos = GetExtendedImageMapMousePos();

    m_MineSweeper.DrawMap(ImageMap, shift, pos.x, pos.y);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ImageMapMouseUp(
    TObject* /*sender*/, TMouseButton button, TShiftState shift, int /*x*/, int /*y*/)
{
    if (mbLeft == button)
        shift = shift << ssLeft;
    else if (mbRight == button)
        shift = shift << ssRight;

    TPoint pos = GetExtendedImageMapMousePos();

    m_MineSweeper.MouseUp(shift, pos.x, pos.y);
    m_MineSweeper.DrawMap(ImageMap, shift, pos.x, pos.y);

    EGameState state = m_MineSweeper.GetGameState();
    if (EGameState::GameOver_Boom == state)
        BtnReact->Glyph->Assign(m_MineSweeper.Sprites.FaceToast.Bmp);
    else
        BtnReact->Glyph->Assign(m_MineSweeper.Sprites.FaceHappy.Bmp);

    if (EGameState::GameOver_Win == state)
    {
        ShowMessage("to do - win");
    }
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
    // Check the selected item
    TMenuItem* item = dynamic_cast<TMenuItem*>(sender);
    if (nullptr != item)
    {
        if ("MnuCustom" == item->Name)
        {
            TModalResult mRes = ShowCustomDifficulty();
            if (mrCancel == mRes)
                return;
        }

        MnuBeginner->Checked = false;
        MnuIntermediate->Checked = false;
        MnuExpert->Checked = false;
        MnuCustom->Checked = false;

        item->Checked = true;

        NewGame();
    }
}
//---------------------------------------------------------------------------
void TFormMain::NewGame()
{
    size_t nRows;
    size_t nCols;
    int nMines;

    if (MnuBeginner->Checked)
    {
        nRows = TMSEngine::BeginnerRows;
        nCols = TMSEngine::BeginnerCols;
        nMines = TMSEngine::BeginnerMines;
    }
    else if (MnuIntermediate->Checked)
    {
        nRows = TMSEngine::IntermediateRows;
        nCols = TMSEngine::IntermediateCols;
        nMines = TMSEngine::IntermediateMines;
    }
    else if (MnuExpert->Checked)
    {
        nRows = TMSEngine::ExpertRows;
        nCols = TMSEngine::ExpertCols;
        nMines = TMSEngine::ExpertMines;
    }
    else
    {
        nRows = static_cast<size_t>(m_CustomRows);
        nCols = static_cast<size_t>(m_CustomCols);
        nMines = m_CustomMines;
    }

    m_MineSweeper.NewGame(nRows, nCols, nMines, ImageMap);
    m_MineSweeper.DrawMap(ImageMap);

    ClientWidth = ImageMap->Width +
        ((ImageMap->Left + ScrollBoxMap->Left + ScrollBoxMap->BevelWidth + BorderWidth) * 2) + 4;
    ClientHeight = ImageMap->Height + ScrollBoxMap->Top + ScrollBoxMap->Left +
        ((ImageMap->Top + ScrollBoxMap->BevelWidth + BorderWidth) * 2) + 2;

    if (Width > Screen->Width)
        Width = Screen->Width;

    if (Height > Screen->Height - 100)
        Height = Screen->Height - 100;

    ReCenter();

    BtnReact->Glyph->Assign(m_MineSweeper.Sprites.FaceHappy.Bmp);

    //DrawMinesLeft();
    //DrawElapsedTime();
}
//---------------------------------------------------------------------------
void TFormMain::ReCenter()
{
    BtnReact->Left = (Width / 2) - (BtnReact->Width / 2);
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
TModalResult TFormMain::ShowCustomDifficulty()
{
    TFormCustomField* form = new TFormCustomField(nullptr);
    std::unique_ptr<TFormCustomField> auto_form(form);

    form->EditWidth->Value = m_CustomCols;
    form->EditHeight->Value = m_CustomRows;
    form->EditMines->Value = m_CustomMines;

    TModalResult mRes = form->ShowModal();

    if (mrOk == mRes)
    {
        m_CustomCols = form->EditWidth->Value;
        m_CustomRows = form->EditHeight->Value;
        m_CustomMines = form->EditMines->Value;
    }

    return mRes;
}
//---------------------------------------------------------------------------
