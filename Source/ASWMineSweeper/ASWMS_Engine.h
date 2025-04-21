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
    static ULONGLONG const StartTick_NotSet = static_cast<ULONGLONG>(-1);

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
    EGameState m_GameState;
    TShiftState m_MouseDown_Shift;
    int m_MouseDown_X;
    int m_MouseDown_Y;
    int m_NumMines;
    ULONGLONG m_StartTick;

public:
    TGrid* Grid;
    TSprites Sprites;

private:
    void DoClick(TShiftState shift, size_t row, size_t col);
    void DrawCell(TImage* image, size_t row, size_t col, int xPos, int yPos, TShiftState shift, int mouseX, int mouseY);
    TCell* GetCell(size_t row, size_t col);
    int GetCellDrawHeight();
    int GetCellDrawWidth();
    int GetDrawHeight();
    int GetDrawWidth();
    int GetNeighboringMineCount(size_t row, size_t col) const;
    void GridCoordsFromMouse(size_t* col, size_t* row, int x, int y);
    void PopulateMineField(size_t mouseRow, size_t mouseCol);
    void RevealAll();

public:
    EGameState GetGameState();
    ULONGLONG GetStartedTick64() const;

public:
    TMSEngine();
    ~TMSEngine();

    void DrawMap(TImage* image, TShiftState shift, int mouseX, int mouseY);
    void DrawMap(TImage* image);
    void MouseDown(TShiftState shift, int x, int y);
    void MouseUp(TShiftState shift, int x, int y);
    void NewGame(size_t nRows, size_t nCols, int nMines, TImage* image);
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_EngineH
