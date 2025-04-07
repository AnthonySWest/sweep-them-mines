/* **************************************************************************
BasicINI.h
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

#ifndef BasicINIH
#define BasicINIH
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
    TKeyVal(TKeyVal const&) = default;
    ~TKeyVal();

    TKeyVal& operator=(TKeyVal const& rhs) = default;

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
    static const char SectionStart = '[';
    static const char SectionEnd = ']';
    static size_t const NotFound = static_cast<size_t>(-1);

public:
    std::string Name;
    std::vector<TKeyVal> KeyVals;

public:
    TSection();
    TSection(TSection const&) = default;
    ~TSection();

    TSection& operator=(TSection const& rhs);

    void Reset();

    bool AddKeyVal(const std::string& key, const std::string& value);
    bool AddKeyVal(const TKeyVal& keyVal);
    bool AddKeyVal_IfNotExists(const std::string& key, const std::string& defaultValue, bool searchIgnoreCase);
    bool AddKeyVal_IfNotExists(const TKeyVal& keyVal, bool searchIgnoreCase);
    bool InsertKeyVal(size_t index, const std::string& key, const std::string& value);
    bool InsertKeyVal(size_t index, const TKeyVal& keyVal);
    bool InsertComment(size_t index, const std::string& comment);
    bool DeleteKeyVal(size_t index);
    size_t FindKey(const std::string& key, bool ignoreCase) const;
    size_t FindOrCreateKey(const std::string& key, bool ignoreCase);
    size_t FindVal(const std::string& value, bool ignoreCase) const;

    bool HasOneOrMoreKeyValuePairs() const;

    bool IsGlobalSection();
    EErrINI Save(FILE* fOut, const char assignOperator, const std::string& paddingAfterOperator = "");
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
    virtual bool AddSection(const TSection& section);
    virtual bool InsertSection(size_t index);
    virtual bool InsertSection(size_t index, const TSection& section);
    virtual bool DeleteSection(size_t index);
    virtual size_t FindSection(const std::string& sectionName, bool ignoreCase) const;
    virtual size_t FindOrCreateSection(const std::string& sectionName, bool ignoreCase);

    virtual EErrINI Load(const std::string& fileNameINI, const char assignOperator = DefaultAssignOperator,
        size_t maxLineLen = DefaultMaxLineLength);
    virtual EErrINI Load(FILE* fIn, const char assignOperator = DefaultAssignOperator, size_t maxLineLen = DefaultMaxLineLength);
    virtual EErrINI Save(const std::string& fileNameINI, bool overWrite, const char assignOperator = DefaultAssignOperator);
    virtual EErrINI Save(FILE* fOut, const char assignOperator = DefaultAssignOperator);
};

} // namespace BasicINI

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef BasicINIH
