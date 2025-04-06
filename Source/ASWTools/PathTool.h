/* **************************************************************************
PathTool.h
Author: Anthony S. West - ASW Software

Contains routines for file/folder path manipulation

Every attempt should be made to keep this module at least Windows portable.

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

#ifndef PathToolH
#define PathToolH
//---------------------------------------------------------------------------
#include <windows.h>
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
public:
    ////// only const static variables allowed in this public - see second public for regular stuff //////

    static const size_t MaxLen_UnicodeFileName = 32767;

    static char const* const AlphaCharsA;
    static wchar_t const* const AlphaCharsW;

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
    TPathTool();
    ~TPathTool();

    ////// variables //////

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for a constructor to not call a virtual function

    static bool IsDots(std::string str);
    static bool IsDots(std::wstring str);

    static bool IsRelative(std::string const& path);
    static bool IsRelative(std::wstring const& path);

    static bool IsEnvironment(const std::string& path);
    static bool IsEnvironment(const std::wstring& path);

    static bool IsNetwork(const std::string& path);
    static bool IsNetwork(const std::wstring& path);

    static std::string Combine(const std::string& path1, const std::string& path2);
    static std::wstring Combine(const std::wstring& path1, const std::wstring& path2);

    static bool ExpandEnvironmentVars(const std::string& path, std::string& dest);
    static bool ExpandEnvironmentVars(const std::wstring& path, std::wstring& dest);

    static std::string ExtractDir(const std::string& fileName, bool keepTrailSlash = true);
    static std::wstring ExtractDir(const std::wstring& fileName, bool keepTrailSlash = true);

    static std::string ExtractFileName(const std::string& path);
    static std::wstring ExtractFileName(const std::wstring& path);
    static std::string ExtractFileName(const std::string& path, bool removeExtension);
    static std::wstring ExtractFileName(const std::wstring& path, bool removeExtension);

    static std::string RemoveExtension(const std::string& path);
    static std::wstring RemoveExtension(const std::wstring& path);

    static std::string GetExtension(const std::string& path);
    static std::wstring GetExtension(const std::wstring& path);

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
};

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef PathToolH
