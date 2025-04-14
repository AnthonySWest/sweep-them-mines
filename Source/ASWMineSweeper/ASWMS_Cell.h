/* **************************************************************************
ASWMS_Cell.h
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

#ifndef ASWMS_CellH
#define ASWMS_CellH
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TCell
/////////////////////////////////////////////////////////////////////////////
class TCell
{
public:
    bool IsMine;
    bool MarkedAsMine;
    bool MarkedAsQuestion;

public:
    TCell();
    TCell(bool isMine, bool markedAsMine, bool markedAsQuestion);
    TCell(TCell const&) = default;
    ~TCell();

    TCell& operator=(TCell const& rhs) = default;
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_CellH
