/* **************************************************************************
ConsoleHelper.cpp
Author: Anthony S. West - ASW Software

See header for info.

Inspired by: https://stackoverflow.com/questions/191842/how-do-i-get-console-output-in-c-with-a-windows-program

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
#include <windows.h>
#include <iostream>
//---------------------------------------------------------------------------
// Module header
#include "ConsoleHelper.h"
//---------------------------------------------------------------------------
#include <conio.h>
#include <fcntl.h> // for _O_TEXT
#include <io.h>  //isatty()
//---------------------------------------------------------------------------

#ifndef Isatty
#ifdef __BORLANDC__
    #define Isatty isatty
#else
    #define Isatty _isatty
#endif // #ifdef __BORLANDC__
#endif // #ifndef Isatty

//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TConsole
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TConsole::TConsole()
{
}

//---------------------------------------------------------------------------
TConsole::~TConsole()
{
}

//---------------------------------------------------------------------------
bool TConsole::CreateNewConsole(int16_t minLength)
{
    bool result = false;

    // Release any current console and redirect IO to NUL
    ReleaseConsole();

    // Attempt to create new console
    if (::AllocConsole())
    {
        AdjustConsoleBuffer(minLength);
        result = RedirectConsoleIO();
    }

    return result;
}

//---------------------------------------------------------------------------
bool TConsole::ReleaseConsole()
{
    bool result = true;
    FILE* fp;

    // Just to be safe, redirect standard IO to NUL before releasing.

    // Redirect STDIN to NUL
    if (freopen_s(&fp, "NUL:", "r", stdin) != 0)
        result = false;
    else
        setvbuf(stdin, nullptr, _IONBF, 0);

    // Redirect STDOUT to NUL
    if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
        result = false;
    else
        setvbuf(stdout, nullptr, _IONBF, 0);

    // Redirect STDERR to NUL
    if (freopen_s(&fp, "NUL:", "w", stderr) != 0)
        result = false;
    else
        setvbuf(stderr, nullptr, _IONBF, 0);

    // Detach from console
    if (!::FreeConsole())
        result = false;

    return result;
}

//---------------------------------------------------------------------------
bool TConsole::RedirectConsoleIO()
{
    bool result = true;
    FILE* fp;

    // Redirect STDIN if the console has an input handle
    if (::GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONIN$", "r", stdin) != 0)
            result = false;
        else
            setvbuf(stdin, nullptr, _IONBF, 0);
    }

    // Redirect STDOUT if the console has an output handle
    if (::GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
            result = false;
        else
            setvbuf(stdout, nullptr, _IONBF, 0);
    }

    // Redirect STDERR if the console has an error handle
    if (::GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
    {
        if (freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
            result = false;
        else
            setvbuf(stderr, nullptr, _IONBF, 0);
    }

    // Make C++ standard streams point to console as well.
    std::ios::sync_with_stdio(true);

    // Clear the error state for each of the C++ standard streams.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::cin.clear();

    return result;
}

//---------------------------------------------------------------------------
void TConsole::AdjustConsoleBuffer(int16_t minLength)
{
    // Set the screen buffer to be big enough to scroll some text
    CONSOLE_SCREEN_BUFFER_INFO conInfo;
    ::GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
    if (conInfo.dwSize.Y < minLength)
        conInfo.dwSize.Y = minLength;
    ::SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
}

//---------------------------------------------------------------------------
bool TConsole::AttachParentConsole(int16_t minLength)
{
    bool result = false;

    // Release any current console and redirect IO to NUL
    ReleaseConsole();

    // Attempt to attach to parent process's console
    if (::AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AdjustConsoleBuffer(minLength);
        result = RedirectConsoleIO();
    }

    return result;
}

//---------------------------------------------------------------------------
bool TConsole::IsStandardInAConsole()
{
    if (Isatty(_fileno(stdin)))
        return true;

    return false;
}

//---------------------------------------------------------------------------
bool TConsole::IsStandardOutAConsole()
{
    HANDLE consoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);

    if (nullptr == consoleOutput || INVALID_HANDLE_VALUE == consoleOutput)
        return false;

    int systemOutput = _open_osfhandle(intptr_t(consoleOutput), _O_TEXT);

    // check if output is a console and not redirected to a file
    return -1 == systemOutput ? false : Isatty(systemOutput);
}

//---------------------------------------------------------------------------
void TConsole::TestConsole()
{
    int intVal = 0;
    int nVals;

    // Test stdio
    fprintf(stdout, "Test stdout output\n");
    fprintf(stderr, "Test stderr output\n");
    fprintf(stdout, "Test stdin - Enter an integer: ");
#ifdef _MSC_VER
    nVals = scanf_s("%d", &intVal);
#else
    nVals = scanf("%d", &intVal);
#endif
    printf("You entered %d, nVals %d\n", intVal, nVals);

    // Test iostreams
    std::cout << "Test std::cout output" << std::endl;
    std::cerr << "Test std::cerr output" << std::endl;
    std::clog << "Test std::clog output" << std::endl;
    std::cout << "Test std::cin - Enter an integer: ";
    std::cin >> intVal;
    std::cout << "You entered: " << intVal << std::endl;

    std::cout << std::endl << "Press any key to continue..." << std::endl;
    int ch = _getch();
    if ('a' == ch)
    {
    }
}

//---------------------------------------------------------------------------

} //end namespace ASWTools
