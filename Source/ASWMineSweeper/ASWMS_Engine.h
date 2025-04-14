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
public:
    TGrid Grid;
    TSprites Sprites;

public:
    TMSEngine();
    ~TMSEngine();
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_EngineH
