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
#include <windows.h>
#include <share.h>
#include <string>
#include <vector>
//---------------------------------------------------------------------------

namespace Subroutines
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
public:
    ////// only static variables allowed in this public - see second public for regular stuff //////
    static const char CommentStart1 = ';';
    static const char CommentStart2 = '/';

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
    TKeyVal();
    ~TKeyVal();

    ////// variables //////
    std::string Key;
    std::string Value;

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function

    bool IsComment() const;
    bool IsKeyValuePair() const;
    bool IsEmpty() const;
};


/////////////////////////////////////////////////////////////////////////////
// TSection
/////////////////////////////////////////////////////////////////////////////
class TSection
{
public:
    ////// only static variables allowed in this public - see second public for regular stuff //////
    static const char SectionStart = '[';
    static const char SectionEnd = ']';

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
    TSection();
    //TSection(const std::string& name);
    ~TSection();

    ////// variables //////
    std::string Name;

    std::vector<TKeyVal> KeyVals;

    virtual bool AddKeyVal(const std::string& key, const std::string& value);
    virtual bool AddKeyVal(const TKeyVal& keyVal);
    virtual bool AddKeyVal_IfNotExists(const std::string& key, const std::string& defaultValue, bool searchIgnoreCase);
    virtual bool AddKeyVal_IfNotExists(const TKeyVal& keyVal, bool searchIgnoreCase);
    virtual bool InsertKeyVal(size_t index, const std::string& key, const std::string& value);
    virtual bool InsertKeyVal(size_t index, const TKeyVal& keyVal);
    virtual bool InsertComment(size_t index, const std::string& comment);
    virtual bool DeleteKeyVal(size_t index);
    virtual int FindKey(const std::string& key, bool ignoreCase) const;
    virtual int FindOrCreateKey(const std::string& key, bool ignoreCase);
    virtual int FindVal(const std::string& value, bool ignoreCase) const;

    virtual bool HasOneOrMoreKeyValuePairs() const;

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for the constructor to never call a virtual function
    bool IsGlobalSection();
    EErrINI Save(FILE* fOut, const char assignOperator, const std::string& paddingAfterOperator = "");
};


/////////////////////////////////////////////////////////////////////////////
// TBasicINI
/////////////////////////////////////////////////////////////////////////////
class TBasicINI
{
public:
    ////// only static variables allowed in this public - see second public for regular stuff //////
    static const size_t DefaultMaxLineLength = 8192;
    static const char DefaultAssignOperator = '=';

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
    static bool Dir_Exists_WinAPI(std::string const& dir);
    static bool Dir_Exists_WinAPI(std::wstring const& dir);
    static bool Dir_CreateDirWithSubs(std::string const& dir);
    static bool Dir_CreateDirWithSubs(std::wstring const& dir);

public:
    static bool File_Exists_WinAPI(std::string const& fileName);
    static bool File_Exists_WinAPI(std::wstring const& fileName);
    static bool File_Open(std::string const& fileName, FILE*& filePointer, char const* fileType,
        unsigned char openMode = SH_DENYNO); //default is share deny none
    static bool File_Open(std::wstring const& fileName, FILE*& filePointer, wchar_t const* fileType,
        unsigned char openMode = SH_DENYNO); //default is share deny none
    static bool File_Close(FILE*& file);
    static bool File_Remove(std::string const& fileName, DWORD maxWaitMS = 4000);
    static bool File_Remove(std::wstring const& fileName, DWORD maxWaitMS = 4000);

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
    virtual int FindSection(const std::string& sectionName, bool ignoreCase) const;
    virtual int FindOrCreateSection(const std::string& sectionName, bool ignoreCase);

    virtual EErrINI Load(const std::string& fileNameINI, const char assignOperator = DefaultAssignOperator,
        size_t maxLineLen = DefaultMaxLineLength);
    virtual EErrINI Load(FILE* fIn, const char assignOperator = DefaultAssignOperator, size_t maxLineLen = DefaultMaxLineLength);
    virtual EErrINI Save(const std::string& fileNameINI, bool overWrite, const char assignOperator = DefaultAssignOperator);
    virtual EErrINI Save(FILE* fOut, const char assignOperator = DefaultAssignOperator);
};

} // namespace BasicINI

} // namespace Subroutines

//---------------------------------------------------------------------------
#endif // #ifndef BasicINIH
