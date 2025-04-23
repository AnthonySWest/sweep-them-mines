/* **************************************************************************
ASWMS_Engine.cpp
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

//---------------------------------------------------------------------------
// Module header
#include "ASWMS_Engine.h"
//---------------------------------------------------------------------------
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <time.h>
//---------------------------------------------------------------------------
#include "ASWMS_Cell.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TMSEngine
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TMSEngine::TMSEngine()
    : m_firstClick(true),
      m_UseQuestionMarks(true),
      m_Paused(false),
      m_GameState(EGameState::NotSet),
      m_MouseDown_Shift(0),
      m_MouseDown_X(-1),
      m_MouseDown_Y(-1),
      m_NumMines(0),
      m_NumFlaggedMines(0),
      m_BoomRow(GridCoord_NotSet),
      m_BoomCol(GridCoord_NotSet),
      m_StartTick(Tick_NotSet),
      m_PauseTick(Tick_NotSet),
      Grid(nullptr)
{
}
//---------------------------------------------------------------------------
TMSEngine::~TMSEngine()
{
    delete Grid;
}
//---------------------------------------------------------------------------
// If flagged count is the same as mine count in neighboring cells, unflagged cells will be auto clicked. If the player
// incorrectly marked a cell, the game is lost if a mine is clicked.
void TMSEngine::AutoClickNeighboringCells(size_t row, size_t col)
{
    int nMines = GetNeighboringMineCount(row, col);
    if (0 == nMines)
        return; // Can't auto click if this cell has no neighboring mines

    int nFlags = GetNeighboringFlagCount(row, col);
    if (nFlags != nMines)
        return; // Flag count must match mine count

    TShiftState shift = TShiftState() << ssLeft;
    ClickNeighboringCells(shift, row, col);
}
//---------------------------------------------------------------------------
// If the current cell has zero mines surrounding it, then neigboring cells are automatically marked as discovered.
void TMSEngine::AutoDiscoverNeighboringCells(TShiftState shift, size_t row, size_t col)
{
    int nMines = GetNeighboringMineCount(row, col);
    if (0 != nMines)
        return;

    ClickNeighboringCells(shift, row, col);
}
//---------------------------------------------------------------------------
void TMSEngine::CheckForAndSetWin()
{
    if (IsGameOver())
        return; // Don't check for a win if the game is already done

    for (size_t row = 0, nRows = Grid->GetRowCount(); row < nRows; row++)
    {
        for (size_t col = 0, nCols = Grid->GetColCount(); col < nCols; col++)
        {
            TCell* cell = Grid->GetCell(row, col);
            if (!cell->IsMine && !cell->Discovered)
                return;
        }
    }

    m_GameState = EGameState::GameOver_Win;
}
//---------------------------------------------------------------------------
void TMSEngine::ClickNeighboringCells(TShiftState shift, size_t row, size_t col)
{
    // Start top left then go clockwise around this cell
    DoClick(shift, row - 1, col - 1);
    DoClick(shift, row - 1, col);
    DoClick(shift, row - 1, col + 1);
    DoClick(shift, row, col - 1);
    DoClick(shift, row, col + 1);
    DoClick(shift, row + 1, col - 1);
    DoClick(shift, row + 1, col);
    DoClick(shift, row + 1, col + 1);
}
//---------------------------------------------------------------------------
void TMSEngine::DoClick(TShiftState shift, size_t row, size_t col)
{
    if (IsGameOver())
        return;

    size_t nCols = Grid->GetColCount();
    size_t nRows = Grid->GetRowCount();

    if (row >= nRows || col >= nCols)
        return;

    TCell* cell = Grid->GetCell(row, col);

    if ((shift.Contains(ssLeft) && shift.Contains(ssRight)) ||
        (m_MouseDown_Shift.Contains(ssLeft) && m_MouseDown_Shift.Contains(ssRight)))
    {
        // Both mouse buttons were held down - perform auto click

        if (cell->Discovered)
        {
            TShiftState oldDownShift = m_MouseDown_Shift;
            m_MouseDown_Shift.Clear();

            AutoClickNeighboringCells(row, col);

            m_MouseDown_Shift = oldDownShift;
        }
    }
    else if (shift.Contains(ssLeft) && !m_MouseDown_Shift.Contains(ssRight))
    {
        if (cell->MarkedAsMine)
        {
            // Do nothing - don't allow a click to reveal a cell when the user has a flagged it.
            // Note: Question marks can still be clicked (in Win98 Minesweeper)
        }
        else if (cell->IsMine)
        {
            m_GameState = EGameState::GameOver_Boom;
            m_BoomRow = row;
            m_BoomCol = col;
            cell->Discovered = true;
            cell->MarkedAsQuestion = false;
            RevealAll();
        }
        else if (!cell->Discovered)
        {
            cell->Discovered = true;
            cell->MarkedAsMine = false;
            cell->MarkedAsQuestion = false;

            CheckForAndSetWin();
            if (EGameState::GameOver_Win == m_GameState)
                return;

            // Recursively discover neighboring cells, if applicable
            AutoDiscoverNeighboringCells(shift, row, col);
        }
    }
    else if (shift.Contains(ssRight) && !m_MouseDown_Shift.Contains(ssLeft) && !cell->Discovered)
    {
        if (cell->MarkedAsMine)
        {
            cell->MarkedAsMine = false;

            if (m_UseQuestionMarks)
                cell->MarkedAsQuestion = true;
        }
        else if (cell->MarkedAsQuestion)
        {
            cell->MarkedAsQuestion = false;
        }
        else
        {
            cell->MarkedAsMine = true;
        }
    }
}
//---------------------------------------------------------------------------
void TMSEngine::DrawCell(
    TImage* image, size_t row, size_t col, int xPos, int yPos, TShiftState shift, int mouseX, int mouseY)
{
    TCell const* cell = Grid->GetCell(row, col);
    Graphics::TBitmap* bmp = image->Picture->Bitmap;
    TCanvas* canvas = bmp->Canvas;
    Graphics::TBitmap* bmpTile = nullptr;
    Graphics::TBitmap* bmpMine = nullptr;
    Graphics::TBitmap* bmpProx = nullptr;
    Graphics::TBitmap* bmpFlag = nullptr;
    Graphics::TBitmap* bmpFlagX = nullptr;

    if (cell->Discovered)
    {
        bmpTile = Sprites.Tiles[static_cast<size_t>(ETile::Uncovered)].Bmp;

        if (cell->IsMine)
        {
            bmpMine = Sprites.Mine.Bmp;

            if (row == m_BoomRow && col == m_BoomCol)
                bmpTile = Sprites.Tiles[static_cast<size_t>(ETile::UncoveredBoom)].Bmp;
        }
        else
        {
            int nMines = GetNeighboringMineCount(row, col);
            if (nMines > 0)
                bmpProx = Sprites.Digits_Proximity[static_cast<size_t>(nMines - 1)].Bmp;

            // Incorrectly marked as a mine - this condition occurs after game is over
            if (cell->MarkedAsMine)
                bmpFlagX = Sprites.FlagX.Bmp;
        }
    }
    else
    {
        bmpTile = Sprites.Tiles[static_cast<size_t>(ETile::Covered)].Bmp;

        size_t mouseRow;
        size_t mouseCol;
        GridCoordsFromMouse(&mouseCol, &mouseRow, mouseX, mouseY);
        TCell const* mouseCell = Grid->GetCell(mouseRow, mouseCol);

        // Is the mouse over the cell
        if (mouseRow == row && mouseCol == col)
        {
            bmpTile = Sprites.Tiles[static_cast<size_t>(ETile::CoveredLit)].Bmp;

            // Note: Allow question marks to be shown as clicking
            if (shift.Contains(ssLeft) && !cell->MarkedAsMine)
            {
                bmpTile = Sprites.Tiles[static_cast<size_t>(ETile::CoveredClicked)].Bmp;
            }
        }

        // Player may be attempting an auto-click for multiple cells - if so, highlight cell, if neighbor
        if (GridCoord_NotSet != mouseRow && GridCoord_NotSet != mouseCol &&
            !cell->MarkedAsMine &&
            mouseCell->Discovered && shift.Contains(ssLeft) && shift.Contains(ssRight))
        {
            int diffCol = std::abs(static_cast<int>(mouseCol) - static_cast<int>(col));
            int diffRow = std::abs(static_cast<int>(mouseRow) - static_cast<int>(row));

            if (diffCol <= 1  && diffRow <= 1)
                bmpTile = Sprites.Tiles[static_cast<size_t>(ETile::CoveredLit)].Bmp;
        }
    }

    if (cell->MarkedAsMine)
    {
        bmpFlag = Sprites.Flag.Bmp;
        m_NumFlaggedMines++;
    }
    else if (cell->MarkedAsQuestion)
    {
        bmpFlag = Sprites.Question.Bmp;
    }

    // Draw tile first (Layer 1)
    if (nullptr != bmpTile)
        canvas->Draw(xPos, yPos, bmpTile);

    // Draw mine or proximity digit (layer 2)
    if (nullptr != bmpMine)
    {
        bmpMine->Transparent = true;
        canvas->Draw(xPos, yPos, bmpMine);
    }
    else if (nullptr != bmpProx)
    {
        bmpProx->Transparent = true;
        canvas->Draw(xPos, yPos, bmpProx);
    }

    // Draw flag/marker (Layer 3)
    if (nullptr != bmpFlag)
    {
        bmpFlag->Transparent = true;
        canvas->Draw(xPos, yPos, bmpFlag);
    }

    // Draw incorrect flag/marker (Layer 4)
    if (nullptr != bmpFlagX)
    {
        bmpFlagX->Transparent = true;
        canvas->Draw(xPos, yPos, bmpFlagX);
    }

    // Draw border (Layer 5)
    TRect rect(xPos, yPos, xPos + bmpTile->Width - 1, yPos + bmpTile->Height - 1);
    canvas->Brush->Color = TColor(FrameColor);
    canvas->FrameRect(rect);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawDigits(TImage* image, int value, size_t maxDigits)
{
    std::vector<int> digits = ExtractDigits(value, true);
    if (digits.size() > maxDigits)
        digits.resize(maxDigits);

    TBitmap* bmp = image->Picture->Bitmap;
    TCanvas* canvas = bmp->Canvas;

    for (size_t i = maxDigits - 1, count = digits.size(), idx = 0; i < maxDigits; i--, idx++)
    {
        int digit = TSprites::BlankScoreDigitIndex;
        if (i < count)
            digit = digits[i];

        TBitmap* bmpDigit = Sprites.Digits_Score[static_cast<size_t>(digit)].Bmp;
        bmpDigit->Transparent = true;
        canvas->Draw(bmpDigit->Width * static_cast<int>(idx), 0, bmpDigit);
    }

    // Draw border
    TRect rect(0, 0, bmp->Width - 1, bmp->Height - 1);
    canvas->Brush->Color = TColor(FrameColor);
    canvas->FrameRect(rect);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawMap(TImage* image, TShiftState shift, int mouseX, int mouseY)
{
    int cellWidth = GetCellDrawWidth();
    int cellHeight = GetCellDrawHeight();

    m_NumFlaggedMines = 0;

    for (size_t row = 0, nRows = Grid->GetRowCount(); row < nRows; row++)
    {
        int yOffset = static_cast<int>(row) * cellHeight;

        for (size_t col = 0, nCols = Grid->GetColCount(); col < nCols; col++)
        {
            int xOffset = static_cast<int>(col) * cellWidth;
            DrawCell(image, row, col, xOffset, yOffset, shift, mouseX, mouseY);
        }
    }
}
//---------------------------------------------------------------------------
void TMSEngine::DrawMap(TImage* image)
{
    TShiftState dummy;
    DrawMap(image, dummy, -1, -1);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawMinesRemaining(TImage* image)
{
    int remaining = m_NumMines - m_NumFlaggedMines;
    if (remaining < 0)
        remaining = 0;

    size_t maxDigits = static_cast<size_t>(NumDigits_Time);
    DrawDigits(image, remaining, maxDigits);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawTime(TImage* image)
{
    int seconds = GetEllapsedTimeMilliSecs() / 1000;
    size_t maxDigits = static_cast<size_t>(NumDigits_Time);
    DrawDigits(image, seconds, maxDigits);
}
//---------------------------------------------------------------------------
std::vector<int> TMSEngine::ExtractDigits(int value, bool reverseOrder)
{
    std::vector<int> result;

    if (0 == value)
    {
        result.push_back(0);
        return result;
    }

    if (value < 0)
        value = -value;

    while (value > 0)
    {
        result.push_back(value % 10);
        value /= 10;
    }

    // If caller wants the reverse order, leave as is
    if (!reverseOrder)
        std::reverse(result.begin(), result.end());
    return result;
}
//---------------------------------------------------------------------------
int TMSEngine::GetCellDrawHeight()
{
    return Sprites.Tiles[0].Bmp->Height;
}
//---------------------------------------------------------------------------
int TMSEngine::GetCellDrawWidth()
{
    return Sprites.Tiles[0].Bmp->Width;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawHeight()
{
    return static_cast<int>(Grid->GetRowCount()) * Sprites.Tiles[0].Bmp->Height;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawHeight_MinesRemaining()
{
    return Sprites.Digits_Score[0].Bmp->Height;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawHeight_Time()
{
    return Sprites.Digits_Score[0].Bmp->Height;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawWidth()
{
    return static_cast<int>(Grid->GetColCount()) * Sprites.Tiles[0].Bmp->Width;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawWidth_MinesRemaining()
{
    return NumDigits_MinesRemaining * Sprites.Digits_Score[0].Bmp->Width;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawWidth_Time()
{
    return NumDigits_Time * Sprites.Digits_Score[0].Bmp->Width;
}
//---------------------------------------------------------------------------
int TMSEngine::GetEllapsedTimeMilliSecs()
{
    if (Tick_NotSet == m_StartTick)
        return 0;
    ULONGLONG offset = (m_Paused ? ::GetTickCount64() - m_PauseTick : 0);
    return static_cast<int>(::GetTickCount64() - (m_StartTick + offset));
}
//---------------------------------------------------------------------------
EGameState TMSEngine::GetGameState()
{
    return m_GameState;
}
//---------------------------------------------------------------------------
int TMSEngine::GetNeighboringFlagCount(size_t row, size_t col) const
{
    int count = 0;
    size_t nCols = Grid->GetColCount();
    size_t nRows = Grid->GetRowCount();

    // Top row
    if (row > 0 && col > 0 && Grid->GetCell(row - 1, col - 1)->MarkedAsMine)
        count++;

    if (row > 0 && Grid->GetCell(row - 1, col)->MarkedAsMine)
        count++;

    if (row > 0 && col + 1 < nCols && Grid->GetCell(row - 1, col + 1)->MarkedAsMine)
        count++;

    // Left and right
    if (col > 0 && Grid->GetCell(row, col - 1)->MarkedAsMine)
        count++;

    if (col + 1 < nCols && Grid->GetCell(row, col + 1)->MarkedAsMine)
        count++;

    // bottom row
    if (row + 1 < nRows && col > 0 && Grid->GetCell(row + 1, col - 1)->MarkedAsMine)
        count++;

    if (row + 1 < nRows && Grid->GetCell(row + 1, col)->MarkedAsMine)
        count++;

    if (row + 1 < nRows && col + 1 < nCols && Grid->GetCell(row + 1, col + 1)->MarkedAsMine)
        count++;

    return count;
}
//---------------------------------------------------------------------------
int TMSEngine::GetNeighboringMineCount(size_t row, size_t col) const
{
    int count = 0;
    size_t nCols = Grid->GetColCount();
    size_t nRows = Grid->GetRowCount();

    // Top row
    if (row > 0 && col > 0 && Grid->GetCell(row - 1, col - 1)->IsMine)
        count++;

    if (row > 0 && Grid->GetCell(row - 1, col)->IsMine)
        count++;

    if (row > 0 && col + 1 < nCols && Grid->GetCell(row - 1, col + 1)->IsMine)
        count++;

    // Left and right
    if (col > 0 && Grid->GetCell(row, col - 1)->IsMine)
        count++;

    if (col + 1 < nCols && Grid->GetCell(row, col + 1)->IsMine)
        count++;

    // bottom row
    if (row + 1 < nRows && col > 0 && Grid->GetCell(row + 1, col - 1)->IsMine)
        count++;

    if (row + 1 < nRows && Grid->GetCell(row + 1, col)->IsMine)
        count++;

    if (row + 1 < nRows && col + 1 < nCols && Grid->GetCell(row + 1, col + 1)->IsMine)
        count++;

    return count;
}
//---------------------------------------------------------------------------
ULONGLONG TMSEngine::GetStartedTick64() const
{
    return m_StartTick;
}
//---------------------------------------------------------------------------
bool TMSEngine::GetUseQuestionMarks() const
{
    return m_UseQuestionMarks;
}
//---------------------------------------------------------------------------
void TMSEngine::GridCoordsFromMouse(size_t* col, size_t* row, int x, int y)
{
    if (nullptr != col)
    {
        if (x < 0)
        {
            *col = GridCoord_NotSet;
        }
        else
        {
            *col = static_cast<size_t>(x / GetCellDrawWidth());
            if (*col > Grid->GetColCount())
                *col = GridCoord_NotSet;
        }
    }

    if (nullptr != row)
    {
        if (y < 0)
        {
            *row = GridCoord_NotSet;
        }
        else
        {
            *row = static_cast<size_t>(y / GetCellDrawHeight());
            if (*row > Grid->GetRowCount())
                *row = GridCoord_NotSet;
        }
    }
}
//---------------------------------------------------------------------------
bool TMSEngine::IsGameOver() const
{
    return EGameState::GameOver_Win == m_GameState || EGameState::GameOver_Boom == m_GameState;
}
//---------------------------------------------------------------------------
bool TMSEngine::IsGameRunning() const
{
    return EGameState::InProgress == m_GameState;
}
//---------------------------------------------------------------------------
void TMSEngine::MouseDown(TShiftState shift, int x, int y)
{
    m_MouseDown_Shift = shift;
    m_MouseDown_X = x;
    m_MouseDown_Y = y;
}
//---------------------------------------------------------------------------
void TMSEngine::MouseUp(TShiftState shift, int x, int y)
{
    if (m_firstClick && !shift.Contains(ssLeft))
        return;

    if (m_firstClick && m_MouseDown_Shift.Contains(ssLeft) && m_MouseDown_Shift.Contains(ssRight))
        return;

    size_t row;
    size_t col;
    GridCoordsFromMouse(&col, &row, x, y);

    if (m_firstClick)
    {
        PopulateMineField(row, col);
        m_GameState = EGameState::InProgress;
        m_StartTick = ::GetTickCount64();
    }

    DoClick(shift, row, col);
    m_firstClick = false;
}
//---------------------------------------------------------------------------
void TMSEngine::NewGame(size_t nRows, size_t nCols, int nMines, TImage* imgMap, TImage* imgTime,
    TImage* imgMinesRemaining, bool useQuestionMarks)
{
    delete Grid;
    Grid = new TGrid(nRows, nCols);

    m_firstClick = true;
    m_StartTick = m_PauseTick = Tick_NotSet;
    m_GameState = EGameState::NewGame;
    m_NumMines = std::min(static_cast<int>(nRows * nCols) - 1, nMines);
    m_NumFlaggedMines = 0;
    m_UseQuestionMarks = useQuestionMarks;
    m_Paused = false;

    m_BoomRow = GridCoord_NotSet;
    m_BoomCol = GridCoord_NotSet;

    // Prep the map image
    Graphics::TBitmap* bmp = imgMap->Picture->Bitmap;
    TCanvas* canvas = bmp->Canvas;
    canvas->Brush->Color = clBlack;
    canvas->Font->Color = clBlack;
    bmp->Width = GetDrawWidth();
    bmp->Height = GetDrawHeight();

    // Prep the time image
    bmp = imgTime->Picture->Bitmap;
    canvas = bmp->Canvas;
    canvas->Brush->Color = clBlack;
    canvas->Font->Color = clBlack;
    bmp->Width = GetDrawWidth_Time();
    bmp->Height = GetDrawHeight_Time();

    // Prep the mines remaining image
    bmp = imgMinesRemaining->Picture->Bitmap;
    canvas = bmp->Canvas;
    canvas->Brush->Color = clBlack;
    canvas->Font->Color = clBlack;
    bmp->Width = GetDrawWidth_MinesRemaining();
    bmp->Height = GetDrawHeight_MinesRemaining();
}
//---------------------------------------------------------------------------
void TMSEngine::PauseTime()
{
    if (m_Paused)
        return;

    if (EGameState::InProgress != m_GameState)
        return;
    m_Paused = true;
    m_PauseTick = ::GetTickCount64();
}
//---------------------------------------------------------------------------
void TMSEngine::PopulateMineField(size_t mouseRow, size_t mouseCol)
{
    static const int maxWhileLoopCounts = 100;
    static const int minChanceAfterFirstPass = 8;

    int mineCount = 0;
    int mineWhileLoopCount = 0;

    int nRows = static_cast<int>(Grid->GetRowCount());
    int nCols = static_cast<int>(Grid->GetColCount());

    int totalCells = static_cast<int>(nRows * nCols);
    if (0 == totalCells)
        return; // Should never happen

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Defend against an infinite loop
    if (m_NumMines >= totalCells)
        m_NumMines = totalCells - 1;

    // Try to get the best population of mines the first time through the field
    int chance = static_cast<int>(static_cast<double>(m_NumMines) / static_cast<double>(totalCells) * 100.0);
    if (chance < 1)
        chance = 1;

    do
    {
        int rowDirection = ((std::rand() % 100) < 50) ? 1 : -1;
        int rowStart = rowDirection > 0 ? 0 : nRows - 1;
        int rowEnd = rowDirection > 0 ? nRows : -1;

        for (int row = rowStart; row != rowEnd && mineCount < m_NumMines; row += rowDirection)
        {
            int colDirection = ((std::rand() % 100) < 50) ? 1 : -1;
            int colStart = colDirection > 0 ? 0 : nCols - 1;
            int colEnd = colDirection > 0 ? nCols : -1;

            for (int col = colStart; col != colEnd && mineCount < m_NumMines; col += colDirection)
            {
                // Don't populate a mine where the click occurred - give user a break on the first click
                if (row == static_cast<int>(mouseRow) && col == static_cast<int>(mouseCol))
                    continue;

                TCell* cell = Grid->GetCell(static_cast<size_t>(row), static_cast<size_t>(col));
                if (cell->IsMine)
                    continue; // already a mine

                bool setMine = ((std::rand() % 100) < chance);
                if (!setMine && m_NumMines < totalCells - 1)
                    continue;

                mineCount++;
                cell->IsMine = true;
            }
        }

        // Give the second pass a greater chance of using up all of the mines
        if (mineCount < m_NumMines && chance < minChanceAfterFirstPass)
            chance = minChanceAfterFirstPass;

        mineWhileLoopCount++;
    } while (mineCount < m_NumMines && mineWhileLoopCount < maxWhileLoopCounts);

    if (mineCount < m_NumMines)
        m_NumMines = mineCount; // Unexpected unless the mine count to total cells ratio is too high (like 999/1000)
}
//---------------------------------------------------------------------------
void TMSEngine::ResumeTime()
{
    if (!m_Paused)
        return;
    m_Paused = false;

    if (EGameState::InProgress != m_GameState)
        return;
    ULONGLONG currentTick = ::GetTickCount64();
    m_StartTick += currentTick - m_PauseTick;
}
//---------------------------------------------------------------------------
void TMSEngine::RevealAll()
{
    for (size_t row = 0, nRows = Grid->GetRowCount(); row < nRows; row++)
    {
        for (size_t col = 0, nCols = Grid->GetColCount(); col < nCols; col++)
        {
            TCell* cell = Grid->GetCell(row, col);
            cell->Discovered = true;
        }
    }
}
//---------------------------------------------------------------------------
void TMSEngine::SetUseQuestionMarks(bool useQuestionMarks)
{
    m_UseQuestionMarks = useQuestionMarks;
}
//---------------------------------------------------------------------------

} // namespace ASWMS
