/* **************************************************************************
ASWTools_App.h
Author: Anthony S. West - ASW Software

Contains routines for working with apps.

Every attempt should be made to keep this module at least Windows portable.

Copyright 2023 Anthony S. West

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

#ifndef ASWTools_AppH
#define ASWTools_AppH
//---------------------------------------------------------------------------
#include <string>
#include <windows.h>
//---------------------------------------------------------------------------
#include "ASWTools_Common.h"
#include "ASWTools_Version.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TAppTool
/////////////////////////////////////////////////////////////////////////////
class TAppTool
{
private:
    typedef TAppTool ThisType;

public: // const static variables
    static size_t const MaxLen_UnicodeFileName = 32767;

private:
    void Destroy_Private();
    bool Reset_Private();

public:
    TAppTool();
    ~TAppTool();

    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for a constructor to not call a virtual function

    static std::string GetAppPathA();
    static std::wstring GetAppPathW();
    static bool GetAppVersion(char const* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
        WORD* outRevision);
    static bool GetAppVersion(wchar_t const* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
        WORD* outRevision);
};

} // namespace ASWTools

#endif // #ifndef ASWTools_AppH
