/* **************************************************************************
Scores.cpp
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

#include <vcl.h>
#if !defined(__clang__)
#pragma hdrstop
#endif // #if !defined(__clang__)

//---------------------------------------------------------------------------
// Module header
#include "Scores.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <stdexcept>
//---------------------------------------------------------------------------
#include "PathTool.h"
//---------------------------------------------------------------------------
using namespace ASWTools;
using namespace System;
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TScores
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TScores::TScores()
{
    Reset();
}

//---------------------------------------------------------------------------
TScores::~TScores()
{
}
//---------------------------------------------------------------------------
void TScores::Reset()
{
    Beginner.clear();
    Intermediate.clear();
    Expert.clear();
}
//---------------------------------------------------------------------------
void TScores::Load(std::string filename)
{
    Reset();

    if (!TPathTool::File_Exists_WinAPI(filename))
    {
        throw std::runtime_error("File does not exist: " + filename);
    }

#warning to do - load the score data
    // ZZZ load the data
}
//---------------------------------------------------------------------------
void TScores::Save(std::string filename)
{
#warning to do - save the score data
}
//---------------------------------------------------------------------------

} // namespace SweepThemMines
