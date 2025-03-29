/* **************************************************************************
PathTool.cpp
Author: Anthony S. West - ASW Software

See header for info.

Copyright 2019 Anthony S. West

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
#include "PathTool.h"
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
#include "StringTool.h"

#ifdef USE_ELOG
    #include "EasyLogger.h"
#endif
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif // #ifdef __BORLANDC__

//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TPathTool
/////////////////////////////////////////////////////////////////////////////

const std::string TPathTool::AlphaCharsA  =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::wstring TPathTool::AlphaCharsW = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

//---------------------------------------------------------------------------
TPathTool::TPathTool()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TPathTool::~TPathTool()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TPathTool::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TPathTool::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TPathTool::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here

    return true;
}
//---------------------------------------------------------------------------
bool TPathTool::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
bool TPathTool::IsDots(std::string str)
{
    return (str == "." || str == "..");
}
//---------------------------------------------------------------------------
bool TPathTool::IsDots(std::wstring str)
{
    return (str == L"." || str == L"..");
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if: no drive found, not a network path, and doesn't start with an environment var.
bool TPathTool::Path_IsRelative(std::string const& path)
{
    return (path.find(":") == std::string::npos) && !(path.find("%") == 0) && !(path.find("\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if: no drive found, not a network path, and doesn't start with an environment var.
bool TPathTool::Path_IsRelative(std::wstring const& path)
{
    return (path.find(L":") == std::wstring::npos) && !(path.find(L"%") == 0) && !(path.find(L"\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if environment var symbol found in the path.
bool TPathTool::Path_IsEnvironment(const std::string& path)
{
    return (path.find("%") != std::string::npos);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if environment var symbol found in the path.
bool TPathTool::Path_IsEnvironment(const std::wstring& path)
{
    return (path.find(L"%") != std::wstring::npos);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true path starts with network slashes.
bool TPathTool::Path_IsNetwork(const std::string& path)
{
    return (path.find("\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true path starts with network slashes.
bool TPathTool::Path_IsNetwork(const std::wstring& path)
{
    return (path.find(L"\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// - Combines two paths while properly handling directory separator chars.
std::string TPathTool::Path_Combine(const std::string& path1, const std::string& path2)
{
    //this method doesn't seem to be available
    //std::filesystem::path full_path = path1 / path2;
    //return full_path;

    if (path1.length() == 0)
    {
        return path2;
    }
    else if (path2.length() == 0)
    {
        return path1;
    }
    else if (((path1[path1.length() - 1] == '\\' || path1[path1.length() - 1] == '/') && (path2[0] != '\\' &&
                                                                                          path2[0] != '/')) ||
             ((path1[path1.length() - 1] != '\\' && path1[path1.length() - 1] != '/') && (path2[0] == '\\' ||
                                                                                          path2[0] == '/')))
    {
        return path1 + path2; //one of the paths has a slash in the proper spot, so combine and return
    }
    else if (path1[path1.length() - 1] == '\\' || path1[path1.length() - 1] == '/')
    {
        //path1 ends with slash and path2 begins with slash, return combined with just one slash
        return path1 + path2.substr(1, path2.length() - 1);
    }

    return path1 + "\\" + path2; //neither path has a slash at the join location, return combined with a slash
    //std::string combined = (path1[path1.length() - 1] == '\\' || path1[path1.length() - 1] == '/') ? path1 : path1 + "\\";
    //return combined + ((path2[0] == '\\' || path2[0] == '/') ? path2.substr(1, path2.length() - 1) : path2);
}
//---------------------------------------------------------------------------
// -Static
// - Combines two paths while properly handling directory separator chars.
std::wstring TPathTool::Path_Combine(const std::wstring& path1, const std::wstring& path2)
{
    if (path1.length() == 0)
    {
        return path2;
    }
    else if (path2.length() == 0)
    {
        return path1;
    }
    else if (((path1[path1.length() - 1] == L'\\' || path1[path1.length() - 1] == L'/') && (path2[0] != L'\\' &&
                                                                                            path2[0] != L'/')) ||
             ((path1[path1.length() - 1] != L'\\' && path1[path1.length() - 1] != L'/') && (path2[0] == L'\\' ||
                                                                                            path2[0] == L'/')))
    {
        //one of the paths has a slash in the proper spot, so combine and return
        return path1 + path2;
    }
    else if (path1[path1.length() - 1] == L'\\' || path1[path1.length() - 1] == L'/')
    {
        // path1 ends with slash and path2 begins with slash, return combined with just one slash
        return path1 + path2.substr(1, path2.length() - 1);
    }

    return path1 + L"\\" + path2; //neither path has a slash at the join location, return combined with a slash
}
//---------------------------------------------------------------------------
// -Static
// - Expands the environment variable in the path to the actual path.
bool TPathTool::Path_ExpandEnvironmentVars(const std::string& path, std::string& dest)
{
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TPathTool::Path_ExpandEnvironmentVars(A)";
#endif

    if (path.length() == 0 || !Path_IsEnvironment(path))
    {
        dest = path;
        return true;
    }

    dest = "";

    size_t buffSize = MaxLen_UnicodeFileName;
    char* buff = new char[buffSize + sizeof('\0') + 1];
    std::unique_ptr<char[]> auto_buff(buff);

    DWORD nCharsWritten = ::ExpandEnvironmentStringsA(path.c_str(), buff, static_cast<DWORD>(buffSize));
    if (nCharsWritten > buffSize)
    {
        //buffer too small - make it bigger and try again
        buffSize = static_cast<size_t>(nCharsWritten) + 1;
        auto_buff.reset(new char[buffSize + sizeof('\0') + 1]);
        buff = auto_buff.get();
        nCharsWritten = ::ExpandEnvironmentStringsA(path.c_str(), buff, static_cast<DWORD>(buffSize));
    }

    if (nCharsWritten == 0)
    {
#ifdef USE_ELOG
        DWORD winLastErrTmp = ::GetLastError();
        std::wstring errStr = TStrTool::GetWindowsLastErrorCodeAsStringW(winLastErrTmp);
        ELog.fwprintf(ELogMsgLevel::LML_Light, L"%sError: ExpandEnvironmentStringsA() failed for: \"%s\" (0x%08lx): %s\n",
            codeSectionStr, TStrTool::Utf8ToUnicodeStr(path).c_str(), winLastErrTmp, errStr.c_str());
#endif
        return false;
    }

    dest = buff;
    return true;
}
//---------------------------------------------------------------------------
// -Static
// - Expands the environment variable in the path to the actual path.
bool TPathTool::Path_ExpandEnvironmentVars(const std::wstring& path, std::wstring& dest)
{
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TPathTool::Path_ExpandEnvironmentVars(W)";
#endif

    if (path.length() == 0 || !Path_IsEnvironment(path))
    {
        dest = path;
        return true;
    }

    dest = L"";

    size_t buffSize = MaxLen_UnicodeFileName;
    wchar_t* buff = new wchar_t[buffSize + sizeof(L'\0')];
    std::unique_ptr<wchar_t[]> auto_buff(buff);

    DWORD nCharsWritten = ::ExpandEnvironmentStringsW(path.c_str(), buff, static_cast<DWORD>(buffSize));
    if (nCharsWritten > buffSize)
    {
        //buffer too small - make it bigger and try again
        buffSize = static_cast<size_t>(nCharsWritten) + 1;
        auto_buff.reset(new wchar_t[buffSize + sizeof(L'\0')]);
        buff = auto_buff.get();
        nCharsWritten = ::ExpandEnvironmentStringsW(path.c_str(), buff, static_cast<DWORD>(buffSize));
    }

    if (nCharsWritten == 0)
    {
#ifdef USE_ELOG
        DWORD winLastErrTmp = ::GetLastError();
        std::wstring errStr = TStrTool::GetWindowsLastErrorCodeAsStringW(winLastErrTmp);
        ELog.fwprintf(ELogMsgLevel::LML_Light, L"%sError: ExpandEnvironmentStringsW() failed for: \"%s\" (0x%08lx): %s\n",
            codeSectionStr, path.c_str(), winLastErrTmp, errStr.c_str());
#endif
        return false;
    }

    dest = buff;
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -Returns folder path with trailing backslash, if requested
// -"keepTrailSlash" defaults to true
std::string TPathTool::Path_ExtractDir(const std::string& fileName, bool keepTrailSlash)
{
    size_t splitIdx = fileName.find_last_of("/\\");
    if (splitIdx == std::string::npos)
        return "";
    return fileName.substr(0, splitIdx + (keepTrailSlash ? 1 : 0));
}
//---------------------------------------------------------------------------
// -Static
// -Returns folder path with trailing backslash, if requested
// -"keepTrailSlash" defaults to true
std::wstring TPathTool::Path_ExtractDir(const std::wstring& fileName, bool keepTrailSlash)
{
    size_t splitIdx = fileName.find_last_of(L"/\\");
    if (splitIdx == std::wstring::npos)
        return L"";
    return fileName.substr(0, splitIdx + (keepTrailSlash ? 1 : 0));
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::string TPathTool::Path_ExtractFileName(const std::string& path)
{
    size_t splitIdx = path.find_last_of("/\\");
    if (splitIdx == std::string::npos)
        return path;
    return path.substr(splitIdx + 1, path.length() - splitIdx - 1);
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::wstring TPathTool::Path_ExtractFileName(const std::wstring& path)
{
    size_t splitIdx = path.find_last_of(L"/\\");
    if (splitIdx == std::wstring::npos)
        return path;
    return path.substr(splitIdx + 1, path.length() - splitIdx - 1);
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::string TPathTool::Path_ExtractFileName(const std::string& path, bool removeExtension)
{
    if (!removeExtension)
        return Path_ExtractFileName(path);
    return Path_RemoveExtension(Path_ExtractFileName(path));
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::wstring TPathTool::Path_ExtractFileName(const std::wstring& path, bool removeExtension)
{
    if (!removeExtension)
        return Path_ExtractFileName(path);
    return Path_RemoveExtension(Path_ExtractFileName(path));
}
//---------------------------------------------------------------------------
// -Static
// -Removes the last extension (e.g. ".txt") from the path. If no extension
//  found, returns entire contents of path.
std::string TPathTool::Path_RemoveExtension(const std::string& path)
{
    size_t splitIdx = path.find_last_of(".");
    if (splitIdx == std::string::npos)
        return path;
    return path.substr(0, splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// -Removes the last extension (e.g. ".txt") from the path. If no extension
//  found, returns entire contents of path.
std::wstring TPathTool::Path_RemoveExtension(const std::wstring& path)
{
    size_t splitIdx = path.find_last_of(L".");
    if (splitIdx == std::wstring::npos)
        return path;
    return path.substr(0, splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// -Gets the last extension (e.g. ".txt") from the path. If no extension
//  found, returns empty string.
std::string TPathTool::Path_GetExtension(const std::string& path)
{
    size_t splitIdx = path.find_last_of(".");
    if (splitIdx == std::string::npos)
        return "";
    return path.substr(splitIdx, path.length() - splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// -Gets the last extension (e.g. ".txt") from the path. If no extension
//  found, returns empty string.
std::wstring TPathTool::Path_GetExtension(const std::wstring& path)
{
    size_t splitIdx = path.find_last_of(L".");
    if (splitIdx == std::wstring::npos)
        return L"";
    return path.substr(splitIdx, path.length() - splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// - Generates a random name of given length, using the default charset in AlphaCharsA
std::string TPathTool::Path_GenerateRandomNameA(size_t len)
{
    return Path_GenerateRandomName(len, "");
}
//---------------------------------------------------------------------------
// -Static
// -If parameter charList is zero length, default AlphaCharsA is used.
std::string TPathTool::Path_GenerateRandomName(size_t len, const std::string& charList)
{
    if (0 == len)
        return "";

    const std::string* charListP = charList.length() == 0 ? &AlphaCharsA : &charList;
    std::string randName;
    randName.reserve(len);

    for (size_t i = 0; i < len; i++)
        randName += (*charListP)[rand() % (charListP->length() - 1)];
    return randName;
}
//---------------------------------------------------------------------------
// -Static
// - Generates a random name of given length, using the default charset in AlphaCharsW
std::wstring TPathTool::Path_GenerateRandomNameW(size_t len)
{
    return Path_GenerateRandomName(len, L"");
}
//---------------------------------------------------------------------------
// -Static
// -If parameter charList is zero length, default AlphaCharsW is used.
std::wstring TPathTool::Path_GenerateRandomName(size_t len, const std::wstring& charList)
{
    if (0 == len)
        return L"";

    const std::wstring* charListP = charList.length() == 0 ? &AlphaCharsW : &charList;
    std::wstring randName;
    randName.reserve(len);

    for (size_t i = 0; i < len; i++)
        randName += (*charListP)[rand() % (charListP->length() - 1)];
    return randName;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

} // namespace ASWTools
