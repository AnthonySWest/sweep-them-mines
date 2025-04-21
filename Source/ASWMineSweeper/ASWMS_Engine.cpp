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
      m_NumMines(0),
      Grid(nullptr)
{
}
//---------------------------------------------------------------------------
TMSEngine::~TMSEngine()
{
    delete Grid;
}
//---------------------------------------------------------------------------
void TMSEngine::DrawCell(TImage* image, size_t row, size_t col, int xPos, int yPos, int mouseX, int mouseY)
{
    TCell const* cell = &(*Grid->Rows)[row][col];
    Graphics::TBitmap* bmp = image->Picture->Bitmap;
    TCanvas* canvas = bmp->Canvas;
    Graphics::TBitmap* bmpCell = nullptr;

    if (cell->Discovered)
    {
        if (cell->IsMine)
            bmpCell = Sprites.Mine.Bmp;
        else
            bmpCell = Sprites.Tiles[static_cast<size_t>(ETile::Uncovered)].Bmp;
    }
    else if (cell->MarkedAsMine)
    {
        bmpCell = Sprites.Flag.Bmp;
    }
    else if (cell->MarkedAsQuestion)
    {
        bmpCell = Sprites.Question.Bmp;
    }
    else
    {
        bmpCell = Sprites.Tiles[static_cast<size_t>(ETile::Covered)].Bmp;

        // Is the mouse over the cell
        if (mouseX >= xPos && mouseX < xPos + bmpCell->Width &&
            mouseY >= yPos && mouseY < yPos + bmpCell->Height)
        {
            bmpCell = Sprites.Tiles[static_cast<size_t>(ETile::CoveredLit)].Bmp;
        }
    }

    canvas->Draw(xPos, yPos, bmpCell);

    TRect rect(xPos, yPos, xPos + bmpCell->Width - 1, yPos + bmpCell->Height - 1);
    canvas->Brush->Color = TColor(0xFF1C69B3);
    canvas->FrameRect(rect);
}
//---------------------------------------------------------------------------
void TMSEngine::DrawMap(TImage* image, int mouseX, int mouseY)
{
    int cellWidth = GetCellDrawWidth();
    int cellHeight = GetCellDrawHeight();
    size_t nRows = Grid->Rows->size();

    if (0 == nRows)
        return; // Should never happen

    size_t nCols = (*Grid->Rows)[0].size();

    if (0 == nCols)
        return; // Should never happen

    for (size_t row = 0; row < nRows; row++)
    {
        int yOffset = static_cast<int>(row) * cellHeight;

        for (size_t col = 0; col < nCols; col++)
        {
            int xOffset = static_cast<int>(col) * cellWidth;
            DrawCell(image, row, col, xOffset, yOffset, mouseX, mouseY);
        }
    }
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
    if (Grid->Rows->size() == 0)
        return 0;

    int result = static_cast<int>(Grid->Rows->size()) * Sprites.Tiles[0].Bmp->Height;
    return result;
}
//---------------------------------------------------------------------------
int TMSEngine::GetDrawWidth()
{
    if (Grid->Rows->size() == 0 || Grid->Rows[0].size() == 0)
        return 0;

    TGrid::TRow const* row = &(*Grid->Rows)[0];
    int result = static_cast<int>(row->size()) * Sprites.Tiles[0].Bmp->Width;
    return result;
}
//---------------------------------------------------------------------------
void TMSEngine::NewGame(size_t nRows, size_t nCols, int nMines, TImage* image)
{
    delete Grid;
    Grid = new TGrid(nRows, nCols);

    m_firstClick = true;
    m_NumMines = std::min(static_cast<int>(nRows * nCols) - 1, nMines);

    Graphics::TBitmap* bmp = image->Picture->Bitmap;
    TCanvas* canvas = bmp->Canvas;
    canvas->Brush->Color = clWhite;
    canvas->Font->Color = clWhite;
    bmp->Width = GetDrawWidth();
    bmp->Height = GetDrawHeight();
}
//---------------------------------------------------------------------------

} // namespace ASWMS
