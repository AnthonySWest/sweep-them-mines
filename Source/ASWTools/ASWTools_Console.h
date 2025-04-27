/* **************************************************************************
ASWTools_Console.h
Author: Anthony S. West - ASW Software

Contains functions for creating a console window from a WinMain app.

Every attempt should be made to keep this module at least Windows portable.

Copyright 2022 Anthony S. West

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
#pragma once
//---------------------------------------------------------------------------
#ifndef ASWTools_ConsoleH
#define ASWTools_ConsoleH
//---------------------------------------------------------------------------
#include <cstdint>
//---------------------------------------------------------------------------
#include "ASWTools_Common.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TConsole
/////////////////////////////////////////////////////////////////////////////
class TConsole
{
public: // Static variables
    static int const Default_MinBuffer = 8192;

protected:
    static bool RedirectConsoleIO();

private:
    TConsole();

public:
    static bool CreateNewConsole(int16_t minLength = Default_MinBuffer);
    static bool ReleaseConsole();

    // Attaches to existing console instead of creating a new console
    static bool AttachParentConsole(int16_t minLength = Default_MinBuffer);

    static bool IsStandardInAConsole();
    static bool IsStandardOutAConsole();

    static void AdjustConsoleBuffer(int16_t minLength);
    static void TestConsole();

public:
    ~TConsole();
};

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef ASWTools_ConsoleH
