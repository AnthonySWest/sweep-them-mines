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
void TMSEngine::DoClick(TShiftState shift, size_t row, size_t col)
{
    size_t nCols = Grid->GetColCount();
    size_t nRows = Grid->GetRowCount();

    if (row >= nRows || col >= nCols)
        return;

    TCell* cell = Grid->GetCell(row, col);

    if (shift.Contains(ssLeft) && shift.Contains(ssRight))
    {

    }
    else if (shift.Contains(ssLeft))
    {
        if (cell->MarkedAsQuestion || cell->MarkedAsMine)
        {
            // Do nothing - don't allow a click to reveal a cell when the user has a marked it sus
        }
        else if (cell->IsMine)
        {
            m_GameState = EGameState::GameOver_Boom;
            RevealAll();
        }
        else if (!cell->Discovered)
        {
            cell->Discovered = true;
            int nMines = GetNeighboringMineCount(row, col);
            if (0 == nMines)
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
        }
    }
    else if (shift.Contains(ssRight))
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
    Graphics::TBitmap* bmpBack = Sprites.Tiles[static_cast<size_t>(ETile::Covered)].Bmp;
    Graphics::TBitmap* bmpFore = nullptr;

    if (cell->Discovered)
    {
        bmpBack = Sprites.Tiles[static_cast<size_t>(ETile::Uncovered)].Bmp;

        if (cell->IsMine)
        {
            bmpFore = Sprites.Mine.Bmp;
        }
        else
        {
            int nMines = GetNeighboringMineCount(row, col);
            if (nMines > 0)
                bmpFore = Sprites.Digits_Proximity[static_cast<size_t>(nMines - 1)].Bmp;
        }
    }
    else if (cell->MarkedAsMine)
    {
        bmpFore = Sprites.Flag.Bmp;
    }
    else if (cell->MarkedAsQuestion)
    {
        bmpFore = Sprites.Question.Bmp;
    }

    if (!cell->Discovered)
    {
        // Is the mouse over the cell
        if (mouseX >= xPos && mouseX < xPos + bmpBack->Width &&
            mouseY >= yPos && mouseY < yPos + bmpBack->Height)
        {
            bmpBack = Sprites.Tiles[static_cast<size_t>(ETile::CoveredLit)].Bmp;

            if (shift.Contains(ssLeft))
            {
                bmpBack = Sprites.Tiles[static_cast<size_t>(ETile::CoveredClicked)].Bmp;
            }
        }
    }

    canvas->Draw(xPos, yPos, bmpBack);

    if (nullptr != bmpFore)
    {
        bmpFore->Transparent = true;
        canvas->Draw(xPos, yPos, bmpFore);
    }

    TRect rect(xPos, yPos, xPos + bmpBack->Width - 1, yPos + bmpBack->Height - 1);
    canvas->Brush->Color = TColor(0xFF1C69B3);
    canvas->FrameRect(rect);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawMap(TImage* image, TShiftState shift, int mouseX, int mouseY)
{
    int cellWidth = GetCellDrawWidth();
    int cellHeight = GetCellDrawHeight();

    size_t nRows = Grid->GetRowCount();
    size_t nCols = Grid->GetColCount();

    for (size_t row = 0; row < nRows; row++)
    {
        int yOffset = static_cast<int>(row) * cellHeight;

        for (size_t col = 0; col < nCols; col++)
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
        *col = static_cast<size_t>(x / GetCellDrawWidth());

    if (nullptr != row)
        *row = static_cast<size_t>(y / GetCellDrawHeight());
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

    int mineCount = 0;
    int mineWhileLoopCount = 0;

    size_t nRows = Grid->GetRowCount();
    size_t nCols = Grid->GetColCount();

    int totalCells = static_cast<int>(nRows * nCols);
    if (0 == totalCells)
        return; // Should never happen

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Defend against an infinite loop
    if (m_NumMines >= totalCells)
        m_NumMines = totalCells - 1;

    // Try to get the best population of mines the first time through the field
    int chance = static_cast<int>(static_cast<double>(m_NumMines) / static_cast<double>(totalCells) * 100.0);
    if (chance < 8)
        chance = 8;

    do
    {
        for (size_t row = 0; row < nRows && mineCount < m_NumMines; row++)
        {
            for (size_t col = 0; col < nCols && mineCount < m_NumMines; col++)
            {
                // Don't populate a mine where the click occurred - give user a break on the first click
                if (row == mouseRow && col == mouseCol)
                    continue;

                bool setMine = ((std::rand() % 100) < chance);
                if (!setMine && m_NumMines < totalCells - 1)
                    continue;

                mineCount++;
                TCell* cell = Grid->GetCell(row, col);
                cell->IsMine = true;
            }
        }

        mineWhileLoopCount++;
    } while (mineCount < m_NumMines && mineWhileLoopCount < maxWhileLoopCounts);

    if (mineCount < m_NumMines)
        m_NumMines = mineCount; // Unexpected unless the mine count to total cells ratio is too high (like 999/1000)
}
//---------------------------------------------------------------------------
void TMSEngine::RevealAll()
{
    size_t nRows = Grid->GetRowCount();
    size_t nCols = Grid->GetColCount();

    for (size_t row = 0; row < nRows; row++)
    {
        for (size_t col = 0; col < nCols; col++)
        {
            TCell* cell = Grid->GetCell(row, col);
            cell->Discovered = true;
        }
    }
}
//---------------------------------------------------------------------------

} // namespace ASWMS
