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
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "ASWMS_Grid.h"
#include "ASWMS_Sprites.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TMSEngine
/////////////////////////////////////////////////////////////////////////////
class TMSEngine
{
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
    int m_NumMines;

public:
    TGrid* Grid;
    TSprites Sprites;

private:
    void DrawCell(TImage* image, size_t row, size_t col, int xPos, int yPos, int mouseX, int mouseY);
    int GetCellDrawHeight();
    int GetCellDrawWidth();
    int GetDrawHeight();
    int GetDrawWidth();

public:
    TMSEngine();
    ~TMSEngine();

    void DrawMap(TImage* image, int mouseX, int mouseY);
    void NewGame(size_t nRows, size_t nCols, int nMines, TImage* image);
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_EngineH
