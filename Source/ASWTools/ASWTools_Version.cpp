/* **************************************************************************
ASWTools_Version.cpp
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
#include "ASWTools_Version.h"
//---------------------------------------------------------------------------
#include "ASWTools_String.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TVersion
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TVersion::TVersion()
{
    Reset();
}
//---------------------------------------------------------------------------
TVersion::TVersion(TVersion const& version)
{
    Reset();
    VersionU64 = version.VersionU64;
}
//---------------------------------------------------------------------------
TVersion::TVersion(unsigned __int64 valueU64)
{
    Reset();
    VersionU64 = valueU64;
}
//---------------------------------------------------------------------------
TVersion::TVersion(std::string const& verStr)
{
    Reset();
    ExtractVersionNumbersFromVersionStr(verStr);
}
//---------------------------------------------------------------------------
TVersion::TVersion(std::wstring const& verStr)
{
    Reset();
    ExtractVersionNumbersFromVersionStr(verStr);
}
//---------------------------------------------------------------------------
TVersion::~TVersion()
{
}
//---------------------------------------------------------------------------
void TVersion::Reset()
{
    VersionU64 = 0;
}
//---------------------------------------------------------------------------
void TVersion::SetVersion(WORD major, WORD minor, WORD build, WORD revision)
{
    Version.Major = major;
    Version.Minor = minor;
    Version.Build = build;
    Version.Revision = revision;
}
//---------------------------------------------------------------------------
bool TVersion::ExtractVersionNumbersFromVersionStr(std::string const& verStr)
{
    int major, minor, build, revision;

    VersionU64 = 0;

    if (ExtractVersionNumbersFromVersionStr(verStr, &major, &minor, &build, &revision))
    {
        Version.Major = static_cast<WORD>(major);
        Version.Minor = static_cast<WORD>(minor);
        Version.Build = static_cast<WORD>(build);
        Version.Revision = static_cast<WORD>(revision);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool TVersion::ExtractVersionNumbersFromVersionStr(std::wstring const& verStr)
{
    int major, minor, build, revision;

    VersionU64 = 0;

    if (ExtractVersionNumbersFromVersionStr(verStr, &major, &minor, &build, &revision))
    {
        Version.Major = static_cast<WORD>(major);
        Version.Minor = static_cast<WORD>(minor);
        Version.Build = static_cast<WORD>(build);
        Version.Revision = static_cast<WORD>(revision);
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
//  -Static
bool TVersion::ExtractVersionNumbersFromVersionStr(std::string const& verStr, int* major,
    int* minor, int* build, int* revision)
{
    *major = *minor = *build = *revision = 0;

    if (verStr.length() == 0)
        return false;

    const char* walker = verStr.c_str();

    //get major version number
    *major = atoi(walker);

    //get minor version number
    while (*walker && *walker != '.')
        walker++;

    if (*walker)
        *minor = atoi(++walker);

    //get build number
    while (*walker && *walker != '.')
        walker++;

    if (*walker)
        *build = atoi(++walker);

    //get revision number
    while (*walker && *walker != '.')
        walker++;

    if (*walker)
        *revision = atoi(++walker);

    return true;
}
//---------------------------------------------------------------------------
//  -Static
bool TVersion::ExtractVersionNumbersFromVersionStr(std::wstring const& verStr, int* major,
    int* minor, int* build, int* revision)
{
    *major = *minor = *build = *revision = 0;

    if (verStr.length() == 0)
        return false;

    const wchar_t* walker = verStr.c_str();

    //get major version number
    *major = _wtoi(walker);

    //get minor version number
    while (*walker && *walker != L'.')
        walker++;

    if (*walker)
        *minor = _wtoi(++walker);

    //get build number
    while (*walker && *walker != L'.')
        walker++;

    if (*walker)
        *build = _wtoi(++walker);

    //get revision number
    while (*walker && *walker != L'.')
        walker++;

    if (*walker)
        *revision = _wtoi(++walker);

    return true;
}
//---------------------------------------------------------------------------
std::string TVersion::ToStrVer() const
{
#if __cplusplus >= 201103L
    return std::to_string(Version.Major) + "." + std::to_string(Version.Minor) + "." +
        std::to_string(Version.Build) + "." + std::to_string(Version.Revision);
#else
    return TStrTool::ToStringA(Version.Major) + "." + TStrTool::ToStringA(Version.Minor) + "." +
        TStrTool::ToStringA(Version.Build) + "." + TStrTool::ToStringA(Version.Revision);
#endif
}
//---------------------------------------------------------------------------
std::wstring TVersion::ToStrVerW() const
{
#if __cplusplus >= 201103L
    return std::to_wstring(Version.Major) + L"." + std::to_wstring(Version.Minor) + L"." +
        std::to_wstring(Version.Build) + L"." + std::to_wstring(Version.Revision);
#else
    return TStrTool::ToStringW(Version.Major) + L"." + TStrTool::ToStringW(Version.Minor) + L"." +
        TStrTool::ToStringW(Version.Build) + L"." + TStrTool::ToStringW(Version.Revision);
#endif
}

//---------------------------------------------------------------------------
bool TVersion::CopyFrom(ThisType const& otherClass)
{
    if (this == &otherClass)
        return true; //done due to self-assignment

    Reset();

    VersionU64 = otherClass.VersionU64;

    return true;
}
//---------------------------------------------------------------------------
bool TVersion::SameAs(ThisType const& otherClass) const
{
    if (this == &otherClass)
        return true; //self-compare

    if (VersionU64 != otherClass.VersionU64)
    {
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
TVersion& TVersion::operator=(ThisType const& right)
{
    //avoid a self assignment
    if (this != &right)
        this->CopyFrom(right);
    return *this;
}
//---------------------------------------------------------------------------
bool TVersion::operator==(ThisType const& right) const
{
    if (this == &right)
        return true; //self-compare
    return this->SameAs(right);
}
//---------------------------------------------------------------------------
bool TVersion::operator!=(ThisType const& right) const
{
    return !(*this == right);
}
//---------------------------------------------------------------------------
bool TVersion::operator<(ThisType const& right) const
{
    if (this == &right)
        return false; //self-compare
    return VersionU64 < right.VersionU64;
}
//---------------------------------------------------------------------------
bool TVersion::operator<=(ThisType const& right) const
{
    if (this == &right)
        return true; //self-compare
    return VersionU64 <= right.VersionU64;
}
//---------------------------------------------------------------------------
bool TVersion::operator>(ThisType const& right) const
{
    return !(*this <= right);
}
//---------------------------------------------------------------------------
bool TVersion::operator>=(ThisType const& right) const
{
    return !(*this < right);
}
//---------------------------------------------------------------------------

} // namespace ASWTools
