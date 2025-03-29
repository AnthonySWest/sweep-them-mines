/* **************************************************************************
AppTool.cpp
Author: Anthony S. West - ASW Software

See header for info.

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

//---------------------------------------------------------------------------
// Module header
#include "AppTool.h"
//---------------------------------------------------------------------------
#include <memory>
#include <windows.h>
#include <winver.h>
//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TAppTool
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TAppTool::TAppTool()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TAppTool::~TAppTool()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TAppTool::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TAppTool::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TAppTool::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here

    return true;
}
//---------------------------------------------------------------------------
bool TAppTool::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
/*
    TAppTool::GetAppPathA - Static

    Get application directory (gets correct dir, even when run from ZIP)
*/
std::string TAppTool::GetAppPathA()
{
    //see: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlongpathnamea
    const size_t maxFileNameLen = MaxLen_UnicodeFileName;
    int numCharsWritten;
    char* appNamePath = new char[maxFileNameLen + 1];
    std::unique_ptr<char[]> auto_appNamePath(appNamePath);
    DWORD lenCopied;
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    *appNamePath = '\0';
    lenCopied = ::GetModuleFileNameA(hInstance, appNamePath, static_cast<DWORD>(maxFileNameLen));

    if (maxFileNameLen == lenCopied || 0 == lenCopied)
        return "";

    char* longDir = new char[maxFileNameLen + 1]; //stores expanded directory path (no ~)
    std::unique_ptr<char[]> auto_longDir(longDir);
    *longDir = '\0';

    //make sure not DOS path
    lenCopied = ::GetLongPathNameA(appNamePath, longDir, static_cast<DWORD>(maxFileNameLen));

    //buffer to small - resize and try again (should never happen, given the original buffer size)
    if (lenCopied >= maxFileNameLen)
    {
        size_t newlongDirSize = lenCopied;
        auto_longDir.reset(new char[newlongDirSize]);
        longDir = auto_longDir.get();
        *longDir = '\0';
        lenCopied = ::GetLongPathNameA(appNamePath, longDir, static_cast<DWORD>(newlongDirSize));
    }

    std::string resultPath;

    if (0 == lenCopied) //error condition
        resultPath = "";
    else
        resultPath = longDir;

    return resultPath;
}
//---------------------------------------------------------------------------
/*
    TAppTool::GetAppPathW - Static

    Get application directory (gets correct dir, even when run from ZIP)
*/
std::wstring TAppTool::GetAppPathW()
{
    //see: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlongpathnamea
    const size_t maxFileNameLen = MaxLen_UnicodeFileName;
    int numCharsWritten;
    wchar_t* appNamePath = new wchar_t[maxFileNameLen + 1];
    std::unique_ptr<wchar_t[]> auto_appNamePath(appNamePath);
    DWORD lenCopied;
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    *appNamePath = L'\0';
    lenCopied = ::GetModuleFileNameW(hInstance, appNamePath, static_cast<DWORD>(maxFileNameLen));

    if (maxFileNameLen == lenCopied || 0 == lenCopied)
        return L"";

    wchar_t* longDir = new wchar_t[maxFileNameLen + 1]; //stores expanded directory path (no ~)
    std::unique_ptr<wchar_t[]> auto_longDir(longDir);
    *longDir = L'\0';

    //make sure not DOS path
    lenCopied = ::GetLongPathNameW(appNamePath, longDir, static_cast<DWORD>(maxFileNameLen));

    //buffer to small - resize and try again (should never happen, given the original buffer size)
    if (lenCopied >= maxFileNameLen)
    {
        size_t newlongDirSize = lenCopied;
        auto_longDir.reset(new wchar_t[newlongDirSize]);
        longDir = auto_longDir.get();
        *longDir = L'\0';
        lenCopied = ::GetLongPathNameW(appNamePath, longDir, static_cast<DWORD>(newlongDirSize));
    }

    std::wstring resultPath;

    if (0 == lenCopied) //error condition
        resultPath = L"";
    else
        resultPath = longDir;

    return resultPath;
}
//---------------------------------------------------------------------------
bool TAppTool::GetAppVersion(const char* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
    WORD* outRevision)
{
    if (nullptr == appOrDLLPath || nullptr == outMajorVer || nullptr == outMinorVer || nullptr == outBuild ||
        nullptr == outRevision)
    {
        return false;
    }

    //Note: code from MSDN, with a few modifications. Uses Winver.h.

    DWORD dwDummy = 0, dwLen;
    UINT bufLen;
    LPVOID lpData;
    VS_FIXEDFILEINFO* pFileInfo;

    dwLen = ::GetFileVersionInfoSizeA(appOrDLLPath, &dwDummy);
    if (!dwLen)
        return false;

    lpData = (LPVOID)malloc(dwLen);
    if (nullptr == lpData)
        return false;
    std::unique_ptr<void, decltype(free) *> auto_lpData(lpData, free);

    bool result = ::GetFileVersionInfoA(appOrDLLPath, 0, dwLen, lpData);

    if(result && ::VerQueryValueA(lpData, "\\", (LPVOID*)&pFileInfo, (PUINT)&bufLen ))
    {
        *outMajorVer = HIWORD(pFileInfo->dwFileVersionMS);
        *outMinorVer = LOWORD(pFileInfo->dwFileVersionMS);
        *outBuild = HIWORD(pFileInfo->dwFileVersionLS);
        *outRevision = LOWORD(pFileInfo->dwFileVersionLS);
    }

    return result;
}
//---------------------------------------------------------------------------
bool TAppTool::GetAppVersion(const wchar_t* appOrDLLPath, WORD* outMajorVer, WORD* outMinorVer, WORD* outBuild,
    WORD* outRevision)
{
    if (nullptr == appOrDLLPath || nullptr == outMajorVer || nullptr == outMinorVer || nullptr == outBuild ||
        nullptr == outRevision)
    {
        return false;
    }

    //Note: code from MSDN, with a few modifications. Uses Winver.h.

    DWORD dwDummy = 0, dwLen;
    UINT bufLen;
    LPVOID lpData;
    VS_FIXEDFILEINFO* pFileInfo;

    dwLen = ::GetFileVersionInfoSizeW(appOrDLLPath, &dwDummy);
    if (!dwLen)
        return false;

    lpData = (LPVOID)malloc(dwLen);
    if (nullptr == lpData)
        return false;
    std::unique_ptr<void, decltype(free) *> auto_lpData(lpData, free);

    bool result = ::GetFileVersionInfoW(appOrDLLPath, 0, dwLen, lpData);

    if(result && ::VerQueryValueW(lpData, L"\\", (LPVOID*)&pFileInfo, (PUINT)&bufLen ))
    {
        *outMajorVer = HIWORD(pFileInfo->dwFileVersionMS);
        *outMinorVer = LOWORD(pFileInfo->dwFileVersionMS);
        *outBuild = HIWORD(pFileInfo->dwFileVersionLS);
        *outRevision = LOWORD(pFileInfo->dwFileVersionLS);
    }

    return result;
}
//---------------------------------------------------------------------------

} // namespace ASWTools
