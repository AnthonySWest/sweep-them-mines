/* **************************************************************************
ASWMS_Grid.h
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

#ifndef ASWMS_GridH
#define ASWMS_GridH
//---------------------------------------------------------------------------
#include <vector>
//---------------------------------------------------------------------------
#include "ASWMS_Cell.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TGrid
/////////////////////////////////////////////////////////////////////////////
class TGrid
{
public:
    typedef std::vector<TCell> TRow;
    typedef std::vector<TRow> TRows;

public:
    TRows* Rows;

public: // Getters/Setters
    TCell* GetCell(size_t row, size_t col);
    size_t GetColCount();
    size_t GetRowCount();

public:
    TGrid(size_t nRows, size_t nCols);
    ~TGrid();
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_GridH
