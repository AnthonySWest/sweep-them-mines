/* **************************************************************************
ASWTools_Path.h
Author: Anthony S. West - ASW Software

Contains routines for file/folder path manipulation

Every attempt should be made to keep this module at least Windows portable.

  Visual Studio NOTE: '__cplusplus' is not defined to the latest unless '/Zc:__cplusplus' is added to
  'Additional Options' because Microsoft likes to make things difficult. If using C++ 11 and up, add
  this option to: Properties-> C/C++ -> All Options -> Additional Options

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

#ifndef ASWTools_PathH
#define ASWTools_PathH
//---------------------------------------------------------------------------
#include <windows.h>
#include <share.h>
#include <string>
//---------------------------------------------------------------------------
#include "ASWTools_Common.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TPathTool
/////////////////////////////////////////////////////////////////////////////
class TPathTool
{
public: // const static variables

    static size_t const MaxLen_UnicodeFileName = 32767;

    static char const* const AlphaCharsA;
    static wchar_t const* const AlphaCharsW;

protected:

private:
    TPathTool();
    ~TPathTool();

public: // Misc. path related methods
    static bool IsDots(std::string str);
    static bool IsDots(std::wstring str);

    static bool IsRelative(std::string const& path);
    static bool IsRelative(std::wstring const& path);

    static bool IsEnvironment(std::string const& path);
    static bool IsEnvironment(std::wstring const& path);

    static bool IsNetwork(std::string const& path);
    static bool IsNetwork(std::wstring const& path);

    static std::string Combine(std::string const& path1, std::string const& path2);
    static std::wstring Combine(std::wstring const& path1, std::wstring const& path2);

    static bool ExpandEnvironmentVars(std::string const& path, std::string& dest);
    static bool ExpandEnvironmentVars(std::wstring const& path, std::wstring& dest);

    static std::string ExtractDir(std::string const& fileName, bool keepTrailSlash = true);
    static std::wstring ExtractDir(std::wstring const& fileName, bool keepTrailSlash = true);

    static std::string ExtractFileName(std::string const& path);
    static std::wstring ExtractFileName(std::wstring const& path);
    static std::string ExtractFileName(std::string const& path, bool removeExtension);
    static std::wstring ExtractFileName(std::wstring const& path, bool removeExtension);

    static std::string RemoveExtension(std::string const& path);
    static std::wstring RemoveExtension(std::wstring const& path);

    static std::string GetExtension(std::string const& path);
    static std::wstring GetExtension(std::wstring const& path);

    static std::string GenerateRandomNameA(size_t len);
    static std::string GenerateRandomName(size_t len, std::string const& charList);
    static std::wstring GenerateRandomNameW(size_t len);
    static std::wstring GenerateRandomName(size_t len, std::wstring const& charList);

    static std::string GetDocumentsDirA();
    static std::wstring GetDocumentsDirW();
    static std::string GetPicturesDirA();
    static std::wstring GetPicturesDirW();
    static std::string GetSpecialFolderDirA(int folderCSIDL);
    static std::wstring GetSpecialFolderDirW(int folderCSIDL);
    static std::string GetTempDirA();
    static std::wstring GetTempDirW();

public: // Methods for directory operations
    static bool Dir_Exists_WinAPI(std::string const& dir);
    static bool Dir_Exists_WinAPI(std::wstring const& dir);
    static bool Dir_CreateDirWithSubs(std::string const& dir);
    static bool Dir_CreateDirWithSubs(std::wstring const& dir);

public: // Methods for file operations
    static bool File_Exists_WinAPI(std::string const& fileName);
    static bool File_Exists_WinAPI(std::wstring const& fileName);
    static bool File_GetLastWriteTime(std::string const& fn, FILETIME& lastwritetime);
    static bool File_GetLastWriteTime(std::wstring const& fn, FILETIME& lastwritetime);
    static bool File_Open(std::string const& fileName, FILE*& filePointer, char const* fileType,
        unsigned char openMode = SH_DENYNO); //default is share deny none
    static bool File_Open(std::wstring const& fileName, FILE*& filePointer, wchar_t const* fileType,
        unsigned char openMode = SH_DENYNO); //default is share deny none
    static bool File_Close(FILE*& file);
    static bool File_Remove(std::string const& fileName, DWORD maxWaitMS = 4000);
    static bool File_Remove(std::wstring const& fileName, DWORD maxWaitMS = 4000);
};

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef ASWTools_PathH
