/* **************************************************************************
ASWTools_BasicINI.h
Author: Anthony S. West - ASW Software

Contains routines for INI reading/writing

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

#ifndef ASWTools_BasicINIH
#define ASWTools_BasicINIH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
//---------------------------------------------------------------------------
#include "ASWTools_Common.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

namespace BasicINI
{

enum class EErrINI
{
    EI_NoError = 0,
    EI_BadParameter = -1,
    EI_FileNotExists = -2,
    EI_FileExists = -3,
    EI_FailOpenRead = -4,
    EI_FailOpenWrite = -5,
    EI_ParseFail = -6,
    EI_ReadError = -7,
    EI_WriteError = -8,
    EI_FailedApplyChanges = -9,
    EI_GeneralError = -100
};


/////////////////////////////////////////////////////////////////////////////
// TKeyVal
/////////////////////////////////////////////////////////////////////////////
class TKeyVal
{
public: // Static variables
    static const char CommentStart1 = ';';
    static const char CommentStart2 = '/';

public:
    std::string Key;
    std::string Value;

public:
    TKeyVal();
#if __cplusplus >= 201103L
    TKeyVal(TKeyVal const&) = default;
#endif
    ~TKeyVal();

#if __cplusplus >= 201103L
    TKeyVal& operator=(TKeyVal const& rhs) = default;
#endif

    void Reset();

    bool IsComment() const;
    bool IsKeyValuePair() const;
    bool IsEmpty() const;
};


/////////////////////////////////////////////////////////////////////////////
// TSection
/////////////////////////////////////////////////////////////////////////////
class TSection
{
public: // Static variables
    static char const SectionStart = '[';
    static char const SectionEnd = ']';
    static size_t const NotFound = static_cast<size_t>(-1);

public:
    std::string Name;
    std::vector<TKeyVal> KeyVals;

public:
    TSection();
#if __cplusplus >= 201103L
    TSection(TSection const&) = default;
#endif
    ~TSection();

    TSection& operator=(TSection const& rhs);

    void Reset();

    bool AddKeyVal(std::string const& key, std::string const& value);
    bool AddKeyVal(TKeyVal const& keyVal);
    bool AddKeyVal_IfNotExists(std::string const& key, std::string const& defaultValue, bool searchIgnoreCase);
    bool AddKeyVal_IfNotExists(TKeyVal const& keyVal, bool searchIgnoreCase);
    bool InsertKeyVal(size_t index, std::string const& key, std::string const& value);
    bool InsertKeyVal(size_t index, TKeyVal const& keyVal);
    bool InsertComment(size_t index, std::string const& comment);
    bool DeleteKeyVal(size_t index);
    size_t FindKey(std::string const& key, bool ignoreCase) const;
    size_t FindOrCreateKey(std::string const& key, bool ignoreCase);
    size_t FindVal(std::string const& value, bool ignoreCase) const;

    bool HasOneOrMoreKeyValuePairs() const;

    bool IsGlobalSection();
    EErrINI Save(FILE* fOut, char const assignOperator, std::string const& paddingAfterOperator = "");
};


/////////////////////////////////////////////////////////////////////////////
// TBasicINI
/////////////////////////////////////////////////////////////////////////////
class TBasicINI
{
public: // Static variables allowed in this public - see second public for regular stuff //////
    static const size_t DefaultMaxLineLength = 8192;
    static const char DefaultAssignOperator = '=';

private:
    void Destroy_Private();
    bool Reset_Private();

public:
    TBasicINI();
    ~TBasicINI();

    ////// variables //////

    std::string PaddingAfterOperator_Write;
    char AlternateAssignOperator_Read;
    std::vector<TSection> Sections;

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function

    virtual bool AddSection();
    virtual bool AddSection(TSection const& section);
    virtual bool InsertSection(size_t index);
    virtual bool InsertSection(size_t index, TSection const& section);
    virtual bool DeleteSection(size_t index);
    virtual size_t FindSection(std::string const& sectionName, bool ignoreCase) const;
    virtual size_t FindOrCreateSection(std::string const& sectionName, bool ignoreCase);

    virtual EErrINI Load(std::string const& fileNameINI, char const assignOperator = DefaultAssignOperator,
        size_t maxLineLen = DefaultMaxLineLength);
    virtual EErrINI Load(FILE* fIn, char const assignOperator = DefaultAssignOperator, size_t maxLineLen = DefaultMaxLineLength);
    virtual EErrINI Save(std::string const& fileNameINI, bool overWrite, char const assignOperator = DefaultAssignOperator);
    virtual EErrINI Save(FILE* fOut, char const assignOperator = DefaultAssignOperator);
};

} // namespace BasicINI

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef ASWTools_BasicINIH
