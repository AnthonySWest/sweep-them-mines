/* **************************************************************************
AppTool.h
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

#ifndef AppToolH
#define AppToolH
//---------------------------------------------------------------------------
#include "Version.h"
//---------------------------------------------------------------------------

namespace Subroutines
{

/////////////////////////////////////////////////////////////////////////////
// TAppTool
/////////////////////////////////////////////////////////////////////////////
class TAppTool
{
private:
    typedef TAppTool ThisType;

public:
    ////// only const static variables allowed in this public - see second public for regular stuff //////

    static const size_t MaxLen_UnicodeFileName = 32767;

protected:
    ////// static variables //////

    ////// variables //////

    ////// functions //////

private:
    ////// static variables //////

    ////// variables //////

    ////// functions //////
    void Destroy_Private();
    bool Reset_Private();

public:
    TAppTool();
    ~TAppTool();

    ////// variables //////

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for a constructor to not call a virtual function

    static std::string GetAppPathA();
    static std::wstring GetAppPathW();
    static bool GetAppVersion(const char* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
        WORD* outRevision);
    static bool GetAppVersion(const wchar_t* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
        WORD* outRevision);
};

} // namespace Subroutines

#endif // #ifndef AppToolH
