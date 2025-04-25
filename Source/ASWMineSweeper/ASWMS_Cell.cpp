/* **************************************************************************
ASWMS_Cell.cpp
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
#include "ASWMS_Cell.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TCell
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TCell::TCell()
    : Discovered(false),
      IsMine(false),
      MarkedAsMine(false),
      MarkedAsQuestion(false),
      LastDrawHash(0)
{
}
//---------------------------------------------------------------------------
TCell::TCell(bool discovered, bool isMine, bool markedAsMine, bool markedAsQuestion, uint32_t lastDrawHash)
    : Discovered(discovered),
      IsMine(isMine),
      MarkedAsMine(markedAsMine),
      MarkedAsQuestion(markedAsQuestion),
      LastDrawHash(lastDrawHash)
{
}
//---------------------------------------------------------------------------
TCell::~TCell()
{
}
//---------------------------------------------------------------------------

} // namespace ASWMS
