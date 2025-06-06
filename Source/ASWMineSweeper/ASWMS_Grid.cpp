/* **************************************************************************
ASWMS_Grid.cpp
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
#include "ASWMS_Grid.h"
//---------------------------------------------------------------------------
#include "ASWTools_Common.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TGrid
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TGrid::TGrid(size_t nRows, size_t nCols)
    : Rows(nullptr)
{
    Rows = new TRows(nRows, TRow(nCols));
}
//---------------------------------------------------------------------------
TGrid::~TGrid()
{
    delete Rows;
}
//---------------------------------------------------------------------------
TCell* TGrid::GetCell(size_t row, size_t col)
{
    return &(*Rows)[row][col];
}
//---------------------------------------------------------------------------
size_t TGrid::GetColCount()
{
    if (nullptr == Rows || Rows->size() == 0)
        return 0;
    return (*Rows)[0].size();
}
//---------------------------------------------------------------------------
size_t TGrid::GetRowCount()
{
    return (nullptr == Rows ? 0 : Rows->size());
}
//---------------------------------------------------------------------------

} // namespace ASWMS
