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
      m_GameState(EGameState::NotSet),
      m_MouseDown_Shift(0),
      m_MouseDown_X(-1),
      m_MouseDown_Y(-1),
      m_NumMines(0),
      m_BoomRow(GridCoord_NotSet),
      m_BoomCol(GridCoord_NotSet),
      m_StartTick(StartTick_NotSet),
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
        bmpFlag = Sprites.Flag.Bmp;
    else if (cell->MarkedAsQuestion)
        bmpFlag = Sprites.Question.Bmp;

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
    canvas->Brush->Color = TColor(0xFF1C69B3);
    canvas->FrameRect(rect);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawMap(TImage* image, TShiftState shift, int mouseX, int mouseY)
{
    int cellWidth = GetCellDrawWidth();
    int cellHeight = GetCellDrawHeight();

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
int TMSEngine::GetDrawWidth()
{
    //if (Grid->GetRowCount() == 0 || Grid->GetColCount() == 0)
    //    return 0;

    //TGrid::TRow const* row = &(*Grid->Rows)[0];
    //int result = static_cast<int>(row->size()) * Sprites.Tiles[0].Bmp->Width;
    //return result;
    return static_cast<int>(Grid->GetColCount()) * Sprites.Tiles[0].Bmp->Width;
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
    }

    DoClick(shift, row, col);
    m_firstClick = false;
}
//---------------------------------------------------------------------------
void TMSEngine::NewGame(size_t nRows, size_t nCols, int nMines, TImage* image)
{
    delete Grid;
    Grid = new TGrid(nRows, nCols);

    m_firstClick = true;
    m_StartTick = StartTick_NotSet;
    m_GameState = EGameState::NewGame;
    m_NumMines = std::min(static_cast<int>(nRows * nCols) - 1, nMines);

    m_BoomRow = GridCoord_NotSet;
    m_BoomCol = GridCoord_NotSet;

    Graphics::TBitmap* bmp = image->Picture->Bitmap;
    TCanvas* canvas = bmp->Canvas;
    canvas->Brush->Color = clWhite;
    canvas->Font->Color = clWhite;
    bmp->Width = GetDrawWidth();
    bmp->Height = GetDrawHeight();
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

                bool setMine = ((std::rand() % 100) < chance);
                if (!setMine && m_NumMines < totalCells - 1)
                    continue;

                mineCount++;
                TCell* cell = Grid->GetCell(static_cast<size_t>(row), static_cast<size_t>(col));
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

} // namespace ASWMS
