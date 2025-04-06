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
#include <shlobj.h>
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

char const* const TPathTool::AlphaCharsA  =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
wchar_t const* const TPathTool::AlphaCharsW = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

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
bool TPathTool::IsRelative(std::string const& path)
{
    return (path.find(":") == std::string::npos) && !(path.find("%") == 0) && !(path.find("\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if: no drive found, not a network path, and doesn't start with an environment var.
bool TPathTool::IsRelative(std::wstring const& path)
{
    return (path.find(L":") == std::wstring::npos) && !(path.find(L"%") == 0) && !(path.find(L"\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if environment var symbol found in the path.
bool TPathTool::IsEnvironment(const std::string& path)
{
    return (path.find("%") != std::string::npos);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if environment var symbol found in the path.
bool TPathTool::IsEnvironment(const std::wstring& path)
{
    return (path.find(L"%") != std::wstring::npos);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true path starts with network slashes.
bool TPathTool::IsNetwork(const std::string& path)
{
    return (path.find("\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true path starts with network slashes.
bool TPathTool::IsNetwork(const std::wstring& path)
{
    return (path.find(L"\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// - Combines two paths while properly handling directory separator chars.
std::string TPathTool::Combine(const std::string& path1, const std::string& path2)
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
std::wstring TPathTool::Combine(const std::wstring& path1, const std::wstring& path2)
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
bool TPathTool::ExpandEnvironmentVars(const std::string& path, std::string& dest)
{
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TPathTool::ExpandEnvironmentVars(A)";
#endif

    if (path.length() == 0 || !IsEnvironment(path))
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
bool TPathTool::ExpandEnvironmentVars(const std::wstring& path, std::wstring& dest)
{
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TPathTool::ExpandEnvironmentVars(W)";
#endif

    if (path.length() == 0 || !IsEnvironment(path))
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
std::string TPathTool::ExtractDir(const std::string& fileName, bool keepTrailSlash)
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
std::wstring TPathTool::ExtractDir(const std::wstring& fileName, bool keepTrailSlash)
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
std::string TPathTool::ExtractFileName(const std::string& path)
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
std::wstring TPathTool::ExtractFileName(const std::wstring& path)
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
std::string TPathTool::ExtractFileName(const std::string& path, bool removeExtension)
{
    if (!removeExtension)
        return ExtractFileName(path);
    return RemoveExtension(ExtractFileName(path));
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::wstring TPathTool::ExtractFileName(const std::wstring& path, bool removeExtension)
{
    if (!removeExtension)
        return ExtractFileName(path);
    return RemoveExtension(ExtractFileName(path));
}
//---------------------------------------------------------------------------
// -Static
// -Removes the last extension (e.g. ".txt") from the path. If no extension
//  found, returns entire contents of path.
std::string TPathTool::RemoveExtension(const std::string& path)
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
std::wstring TPathTool::RemoveExtension(const std::wstring& path)
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
std::string TPathTool::GetExtension(const std::string& path)
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
std::wstring TPathTool::GetExtension(const std::wstring& path)
{
    size_t splitIdx = path.find_last_of(L".");
    if (splitIdx == std::wstring::npos)
        return L"";
    return path.substr(splitIdx, path.length() - splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// - Generates a random name of given length, using the default charset in AlphaCharsA
std::string TPathTool::GenerateRandomNameA(size_t len)
{
    return GenerateRandomName(len, AlphaCharsA);
}
//---------------------------------------------------------------------------
// -Static
// -If parameter charList is zero length, default AlphaCharsA is used.
std::string TPathTool::GenerateRandomName(size_t len, std::string const& charList)
{
    if (0 == len)
        return "";

    int randMax = static_cast<int>(charList.length() - 1);
    std::string randName;
    randName.reserve(len);

    for (size_t i = 0; i < len; i++)
        randName += charList[static_cast<size_t>(rand() % randMax)];
    return randName;
}
//---------------------------------------------------------------------------
// -Static
// - Generates a random name of given length, using the default charset in AlphaCharsW
std::wstring TPathTool::GenerateRandomNameW(size_t len)
{
    return GenerateRandomName(len, AlphaCharsW);
}
//---------------------------------------------------------------------------
// -Static
// -If parameter charList is zero length, default AlphaCharsW is used.
std::wstring TPathTool::GenerateRandomName(size_t len, std::wstring const& charList)
{
    if (0 == len)
        return L"";

    int randMax = static_cast<int>(charList.length() - 1);
    std::wstring randName;
    randName.reserve(len);

    for (size_t i = 0; i < len; i++)
        randName += charList[static_cast<size_t>(rand() % randMax)];
    return randName;
}
//---------------------------------------------------------------------------
std::string TPathTool::GetDocumentsDirA()
{
    return GetSpecialFolderDirA(CSIDL_PERSONAL);
}
//---------------------------------------------------------------------------
std::wstring TPathTool::GetDocumentsDirW()
{
    return GetSpecialFolderDirW(CSIDL_PERSONAL);
}
//---------------------------------------------------------------------------
std::string TPathTool::GetPicturesDirA()
{
    return GetSpecialFolderDirA(CSIDL_MYPICTURES);
}
//---------------------------------------------------------------------------
std::wstring TPathTool::GetPicturesDirW()
{
    return GetSpecialFolderDirW(CSIDL_MYPICTURES);
}
//---------------------------------------------------------------------------
// -Static
std::string TPathTool::GetSpecialFolderDirA(int folderCSIDL)
{
    size_t tempDirSize = MAX_PATH;
    char* tempDir = new char[tempDirSize];
    std::unique_ptr<char[]> auto_tempDir(tempDir);

    if (!SUCCEEDED(::SHGetFolderPathA(nullptr, folderCSIDL, nullptr, SHGFP_TYPE_CURRENT, tempDir)))
        return "";

    size_t longDirSize = 1024; // resized if needed below, but should be plenty
    char* longDir = new char[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<char[]> auto_longDir(longDir);

    // make sure not a DOS path
    size_t copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return "";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new char[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return "";
    }

    return std::string(longDir);
}
//---------------------------------------------------------------------------
// -Static
std::wstring TPathTool::GetSpecialFolderDirW(int folderCSIDL)
{
    size_t tempDirSize = MAX_PATH; // 32767 is max unicode path length
    wchar_t* tempDir = new wchar_t[tempDirSize];
    std::unique_ptr<wchar_t[]> auto_tempDir(tempDir);

    if (!SUCCEEDED(::SHGetFolderPathW(nullptr, folderCSIDL, nullptr, SHGFP_TYPE_CURRENT, tempDir)))
        return L"";

    size_t longDirSize = 32767; // 32767 is max unicode path length
    wchar_t* longDir = new wchar_t[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<wchar_t[]> auto_longDir(longDir);

    // make sure not a DOS path
    size_t copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return L"";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new wchar_t[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return L"";
    }

    return std::wstring(longDir);
}
//---------------------------------------------------------------------------
// -Static
std::string TPathTool::GetTempDirA()
{
    size_t tempDirLen = MAX_PATH + 1; // initial length - resized if needed
    char* tempDir = new char[tempDirLen + sizeof('\0')];
    std::unique_ptr<char[]> auto_tempDir(tempDir);

    size_t copiedLen = ::GetTempPathA(static_cast<DWORD>(tempDirLen), tempDir);
    if (0 == copiedLen)
        return "";

    if (copiedLen > tempDirLen)
    {
        // Buffer is too small - reallocate
        tempDirLen = copiedLen + sizeof('\0');
        auto_tempDir.reset(new char[tempDirLen + sizeof('\0')]);
        tempDir = auto_tempDir.get();

        copiedLen = ::GetTempPathA(static_cast<DWORD>(tempDirLen), tempDir);
        if (0 == copiedLen)
            return "";
    }

    tempDir[tempDirLen] = '\0'; // For safety

    size_t longDirSize = tempDirLen + 32767 + sizeof('\0'); // 32767 is max unicode path length
    char* longDir = new char[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<char[]> auto_longDir(longDir);

    // make sure not a DOS path
    copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return "";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new char[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return "";
    }

    return std::string(longDir);
}
//---------------------------------------------------------------------------
// -Static
std::wstring TPathTool::GetTempDirW()
{
    size_t tempDirLen = MAX_PATH + 1;
    wchar_t* tempDir = new wchar_t[tempDirLen + sizeof('\0')];
    std::unique_ptr<wchar_t[]> auto_tempDir(tempDir);

    size_t copiedLen = ::GetTempPathW(static_cast<DWORD>(tempDirLen), tempDir);
    if (0 == copiedLen)
        return L"";

    if (copiedLen > tempDirLen)
    {
        // Buffer is too small - reallocate
        tempDirLen = copiedLen + sizeof('\0');
        auto_tempDir.reset(new wchar_t[tempDirLen + sizeof('\0')]);
        tempDir = auto_tempDir.get();

        copiedLen = ::GetTempPathW(static_cast<DWORD>(tempDirLen), tempDir);
        if (0 == copiedLen)
            return L"";
    }

    tempDir[tempDirLen] = L'\0'; // For safety

    size_t longDirSize = tempDirLen + 32767 + sizeof('\0'); // 32767 is max unicode path length
    wchar_t* longDir = new wchar_t[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<wchar_t[]> auto_longDir(longDir);

    // make sure not a DOS path
    copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return L"";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new wchar_t[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return L"";
    }

    return std::wstring(longDir);
}
//---------------------------------------------------------------------------

} // namespace ASWTools
