/* **************************************************************************
Version.cpp
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
#include "Version.h"
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
    ValueU64 = version.ValueU64;
}
//---------------------------------------------------------------------------
TVersion::TVersion(unsigned long long valueU64)
{
    Reset();
    ValueU64 = valueU64;
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
    ValueU64 = 0;
}
//---------------------------------------------------------------------------
void TVersion::SetVersion(WORD major, WORD minor, WORD build, WORD revision)
{
    Major = major;
    Minor = minor;
    Build = build;
    Revision = revision;
}
//---------------------------------------------------------------------------
bool TVersion::ExtractVersionNumbersFromVersionStr(std::string const& verStr)
{
    int major, minor, build, revision;

    ValueU64 = 0;

    if (ExtractVersionNumbersFromVersionStr(verStr, &major, &minor, &build, &revision))
    {
        Major = (WORD)major;
        Minor = (WORD)minor;
        Build = (WORD)build;
        Revision = (WORD)revision;
        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
bool TVersion::ExtractVersionNumbersFromVersionStr(std::wstring const& verStr)
{
    int major, minor, build, revision;

    ValueU64 = 0;

    if (ExtractVersionNumbersFromVersionStr(verStr, &major, &minor, &build, &revision))
    {
        Major = (WORD)major;
        Minor = (WORD)minor;
        Build = (WORD)build;
        Revision = (WORD)revision;
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
    return std::to_string(Major) + "." + std::to_string(Minor) + "." +
        std::to_string(Build) + "." + std::to_string(Revision);
}
//---------------------------------------------------------------------------
std::wstring TVersion::ToStrVerW() const
{
    return std::to_wstring(Major) + L"." + std::to_wstring(Minor) + L"." +
        std::to_wstring(Build) + L"." + std::to_wstring(Revision);
}

//---------------------------------------------------------------------------
bool TVersion::CopyFrom(ThisType const& otherClass)
{
    if (this == &otherClass)
        return true; //done due to self-assignment

    Reset();

    ValueU64 = otherClass.ValueU64;

    return true;
}
//---------------------------------------------------------------------------
bool TVersion::SameAs(ThisType const& otherClass) const
{
    if (this == &otherClass)
        return true; //self-compare

    if (ValueU64 != otherClass.ValueU64)
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
    return ValueU64 < right.ValueU64;
}
//---------------------------------------------------------------------------
bool TVersion::operator<=(ThisType const& right) const
{
    if (this == &right)
        return true; //self-compare
    return ValueU64 <= right.ValueU64;
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
