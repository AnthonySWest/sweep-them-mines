/* **************************************************************************
ASWMS_Engine.h
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

#ifndef ASWMS_EngineH
#define ASWMS_EngineH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "ASWMS_Cell.h"
#include "ASWMS_Grid.h"
#include "ASWMS_Sprites.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

enum class EGameState
{
    NotSet,
    NewGame,
    InProgress,
    GameOver_Boom,
    GameOver_Win,
};


/////////////////////////////////////////////////////////////////////////////
// TMSEngine
/////////////////////////////////////////////////////////////////////////////
class TMSEngine
{
private: // Static vars
    static uint32_t const FrameColor = 0xFF1C69B3;
    static int const NumDigits_MinesRemaining = 4;
    static int const NumDigits_Time = 4;
    static size_t const GridCoord_NotSet = static_cast<size_t>(-1);
    static ULONGLONG const Tick_NotSet = 0;

public: // Static vars
    static size_t const BeginnerRows = 8;
    static size_t const BeginnerCols = 8;
    static int const BeginnerMines = 10;
    static size_t const IntermediateRows = 16;
    static size_t const IntermediateCols = 16;
    static int const IntermediateMines = 40;
    static size_t const ExpertRows = 16;
    static size_t const ExpertCols = 30;
    static int const ExpertMines = 99;

private:
    bool m_firstClick;
    bool m_UseQuestionMarks;
    bool m_Paused;
    EGameState m_GameState;
    TShiftState m_MouseDown_Shift;
    int m_MouseDown_X;
    int m_MouseDown_Y;
    int m_NumMines;
    int m_NumFlaggedMines;
    size_t m_BoomRow;
    size_t m_BoomCol;
    ULONGLONG m_StartTick;
    ULONGLONG m_PauseTick;

public:
    TGrid* Grid;
    TSprites Sprites;

private:
    void AutoClickNeighboringCells(size_t row, size_t col);
    void AutoDiscoverNeighboringCells(TShiftState shift, size_t row, size_t col);
    void CheckForAndSetWin();
    void ClickNeighboringCells(TShiftState shift, size_t row, size_t col);
    void DoClick(TShiftState shift, size_t row, size_t col);
    void DrawCell(TImage* image, size_t row, size_t col, int xPos, int yPos, TShiftState shift, int mouseX, int mouseY);
    void DrawDigits(TImage* image, int value, size_t maxDigits);
    int GetCellDrawHeight();
    int GetCellDrawWidth();
    int GetDrawHeight();
    int GetDrawHeight_MinesRemaining();
    int GetDrawHeight_Time();
    int GetDrawWidth();
    int GetDrawWidth_MinesRemaining();
    int GetDrawWidth_Time();
    int GetNeighboringFlagCount(size_t row, size_t col) const;
    int GetNeighboringMineCount(size_t row, size_t col) const;
    void GridCoordsFromMouse(size_t* col, size_t* row, int x, int y);
    void PopulateMineField(size_t mouseRow, size_t mouseCol);
    void RevealAll();

public:
    static std::vector<int> ExtractDigits(int value, bool reverseOrder);

public: // Getters/Setters
    int GetEllapsedTimeMilliSecs();
    EGameState GetGameState();
    ULONGLONG GetStartedTick64() const;
    bool GetUseQuestionMarks() const;
    void SetUseQuestionMarks(bool useQuestionMarks);

public:
    TMSEngine();
    ~TMSEngine();

    void DrawMap(TImage* image, TShiftState shift, int mouseX, int mouseY);
    void DrawMap(TImage* image);
    void DrawMinesRemaining(TImage* image);
    void DrawTime(TImage* image);
    bool IsGameOver() const;
    bool IsGameRunning() const;
    void MouseDown(TShiftState shift, int x, int y);
    void MouseUp(TShiftState shift, int x, int y);
    void NewGame(size_t nRows, size_t nCols, int nMines, TImage* imgMap, TImage* imgTime,
        TImage* imgMinesRemaining, bool useQuestionMarks);
    void PauseTime();
    void ResumeTime();
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_EngineH
