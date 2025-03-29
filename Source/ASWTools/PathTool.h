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

    static const std::string AlphaCharsA;
    static const std::wstring AlphaCharsW;

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

    static bool Path_IsRelative(std::string const& path);
    static bool Path_IsRelative(std::wstring const& path);

    static bool Path_IsEnvironment(const std::string& path);
    static bool Path_IsEnvironment(const std::wstring& path);

    static bool Path_IsNetwork(const std::string& path);
    static bool Path_IsNetwork(const std::wstring& path);

    static std::string Path_Combine(const std::string& path1, const std::string& path2);
    static std::wstring Path_Combine(const std::wstring& path1, const std::wstring& path2);

    static bool Path_ExpandEnvironmentVars(const std::string& path, std::string& dest);
    static bool Path_ExpandEnvironmentVars(const std::wstring& path, std::wstring& dest);

    static std::string Path_ExtractDir(const std::string& fileName, bool keepTrailSlash = true);
    static std::wstring Path_ExtractDir(const std::wstring& fileName, bool keepTrailSlash = true);

    static std::string Path_ExtractFileName(const std::string& path);
    static std::wstring Path_ExtractFileName(const std::wstring& path);
    static std::string Path_ExtractFileName(const std::string& path, bool removeExtension);
    static std::wstring Path_ExtractFileName(const std::wstring& path, bool removeExtension);

    static std::string Path_RemoveExtension(const std::string& path);
    static std::wstring Path_RemoveExtension(const std::wstring& path);

    static std::string Path_GetExtension(const std::string& path);
    static std::wstring Path_GetExtension(const std::wstring& path);

    static std::string Path_GenerateRandomNameA(size_t len);
    static std::string Path_GenerateRandomName(size_t len, const std::string& charList);
    static std::wstring Path_GenerateRandomNameW(size_t len);
    static std::wstring Path_GenerateRandomName(size_t len, const std::wstring& charList);
};

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef PathToolH
