/* **************************************************************************
BasicINI.cpp
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
#include "BasicINI.h"
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
#include "PathTool.h"
#include "StringTool.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

namespace BasicINI
{

/////////////////////////////////////////////////////////////////////////////
// TKeyVal
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TKeyVal::TKeyVal()
{
}
//---------------------------------------------------------------------------
TKeyVal::~TKeyVal()
{
}
//---------------------------------------------------------------------------
void TKeyVal::Reset()
{
    Key = "";
    Value = "";
}
//---------------------------------------------------------------------------
/*
    TKeyVal::IsComment()

    Comments are read in as values with an empty Key
*/
bool TKeyVal::IsComment() const
{
    if (Key.length() == 0)
    {
        if (Value.length() > 0 && (Value[0] == CommentStart1 || Value[0] == CommentStart2))
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
/*
    TKeyVal::IsKeyValuePair

    Returns true if there is a key with a length. A comment will have zero for key length because comment is stored
    entirely in value. Also keyless values will have zero key length.
*/
bool TKeyVal::IsKeyValuePair() const
{
    //If key is empty, then not a key value pair. If value is not empty, then could be a comment.
    if (TStrTool::IsEmptyOrWhiteSpace(Key))
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool TKeyVal::IsEmpty() const
{
    if (TStrTool::IsEmptyOrWhiteSpace(Key) && Value.length() == 0)
        return true;
    return false;
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
// TSection
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TSection::TSection()
{
}
//---------------------------------------------------------------------------
TSection::~TSection()
{
}
//---------------------------------------------------------------------------
TSection& TSection::operator=(TSection const& rhs)
{
    Name = rhs.Name;
    KeyVals = rhs.KeyVals;
    return *this;
}
//---------------------------------------------------------------------------
void TSection::Reset()
{
    //reset class vars here
    Name = "";
    KeyVals.clear();
}
//---------------------------------------------------------------------------
bool TSection::IsGlobalSection()
{
    return Name.length() == 0;
}
//---------------------------------------------------------------------------
bool TSection::AddKeyVal(const std::string& key, const std::string& value)
{
    TKeyVal keyVal;
    keyVal.Key = key;
    keyVal.Value = value;
    return AddKeyVal(keyVal);
}
//---------------------------------------------------------------------------
bool TSection::AddKeyVal(const TKeyVal& keyVal)
{
    //don't allow a key to begin with a section start
    //on second thought, it will still probably work, so, let the caller be the boss
    //if (keyVal.Key.length() > 0 && keyVal.Key[0] == SectionStart)
    //	return false;

    KeyVals.push_back(keyVal);
    return true;
}
//---------------------------------------------------------------------------
/*
    TSection::AddKeyVal_IfNotExists

    - If the key does not exist, a new key will be added with the value of defaultValue.
    - If the key does exist, nothing is done.
*/
bool TSection::AddKeyVal_IfNotExists(const std::string& key, const std::string& defaultValue, bool searchIgnoreCase)
{
    if (NotFound == FindKey(key, searchIgnoreCase))
        return AddKeyVal(key, defaultValue);
    return true;
}
//---------------------------------------------------------------------------
/*
    TSection::AddKeyVal_IfNotExists

    - If the key does not exist, a new key will be added with the value of defaultValue.
    - If the key does exist, nothing is done.
*/
bool TSection::AddKeyVal_IfNotExists(const TKeyVal& keyVal, bool searchIgnoreCase)
{
    if (NotFound == FindKey(keyVal.Key, searchIgnoreCase))
        return AddKeyVal(keyVal);
    return true;
}
//---------------------------------------------------------------------------
bool TSection::InsertKeyVal(size_t index, const std::string& key, const std::string& value)
{
    TKeyVal keyVal;
    keyVal.Key = key;
    keyVal.Value = value;
    return InsertKeyVal(index, keyVal);
}
//---------------------------------------------------------------------------
bool TSection::InsertKeyVal(size_t index, const TKeyVal& keyVal)
{
    if (index >= KeyVals.size())
    {
        KeyVals.push_back(keyVal);
    }
    else
    {
        std::vector<TKeyVal>::iterator itIdx = KeyVals.begin() + static_cast<std::ptrdiff_t>(index);
        KeyVals.insert(itIdx, keyVal);
    }
    return true;
}
//---------------------------------------------------------------------------
/*
    TSection::InsertComment

    Inserts comment as value with blank key
*/
bool TSection::InsertComment(size_t index, const std::string& comment)
{
    //if comment is blank, insert a comment marker
    if (comment.length() == 0)
        return InsertKeyVal(index, "", std::string(1, TKeyVal::CommentStart1));

    //if comment does not begin with a comment marker, add one
    if (comment[0] != TKeyVal::CommentStart1 && comment[0] != TKeyVal::CommentStart2)
        return InsertKeyVal(index, "", std::string(1, TKeyVal::CommentStart1) + comment);
    return InsertKeyVal(index, "", comment);
}
//---------------------------------------------------------------------------
bool TSection::DeleteKeyVal(size_t index)
{
    if (index >= KeyVals.size())
        return true;

    std::vector<TKeyVal>::iterator itIdx = KeyVals.begin() + static_cast<std::ptrdiff_t>(index);
    KeyVals.erase(itIdx);
    return true;
}
//---------------------------------------------------------------------------
size_t TSection::FindKey(const std::string& key, bool ignoreCase) const
{
    for (size_t i = 0; i < KeyVals.size(); i++)
    {
        TKeyVal const* keyValP = &KeyVals[i];

        if ((key.length() == 0 && keyValP->Key.length() == 0) ||
            (ignoreCase && _stricmp(key.c_str(), keyValP->Key.c_str()) == 0) ||
            (!ignoreCase && strcmp(key.c_str(), keyValP->Key.c_str()) == 0))
        {
            return i;
        }
    }

    return NotFound;
}
//---------------------------------------------------------------------------
size_t TSection::FindOrCreateKey(const std::string& key, bool ignoreCase)
{
    size_t idx = FindKey(key, ignoreCase);

    if (NotFound == idx)
    {
        KeyVals.push_back(TKeyVal());
        size_t insertIdx = KeyVals.size() - 1;
        KeyVals[insertIdx].Key = key;
        idx = insertIdx;
    }

    return idx;
}
//---------------------------------------------------------------------------
size_t TSection::FindVal(const std::string& value, bool ignoreCase) const
{
    for (size_t i = 0; i < KeyVals.size(); i++)
    {
        const TKeyVal* keyValP = &KeyVals[i];

        if ((value.length() == 0 && keyValP->Value.length() == 0) ||
            (ignoreCase && _stricmp(value.c_str(), keyValP->Value.c_str()) == 0) ||
            (!ignoreCase && strcmp(value.c_str(), keyValP->Value.c_str()) == 0))
        {
            return i;
        }
    }

    return NotFound;
}
//---------------------------------------------------------------------------
bool TSection::HasOneOrMoreKeyValuePairs() const
{
    for (size_t i = 0; i < KeyVals.size(); i++)
    {
        if (KeyVals[i].IsKeyValuePair())
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
EErrINI TSection::Save(FILE* fOut, const char assignOperator, const std::string& paddingAfterOperator)
{
    if (nullptr == fOut)
    {
        return EErrINI::EI_BadParameter;
    }

    EErrINI result = EErrINI::EI_NoError;
    bool writeErr = false;

    if (Name.length() > 0)
        writeErr |= -1 == fprintf(fOut, "%s\n", TStrTool::Trim_Copy(Name).c_str());

    for (size_t i = 0; i < KeyVals.size() && !writeErr; i++)
    {
        TKeyVal* keyValP = &KeyVals[i];
        std::string key = TStrTool::Trim_Copy(keyValP->Key);

        if (key.length() == 0) //if there is no key, write the value as is (could be a comment line, or a value only line
            writeErr |= -1 == fprintf(fOut, "%s\n", keyValP->Value.c_str());
        else
            writeErr |= -1 == fprintf(fOut, "%s%c%s%s\n", key.c_str(), assignOperator, paddingAfterOperator.c_str(), keyValP->Value.c_str());
    }

    if (writeErr)
    {
        result = EErrINI::EI_WriteError;
    }

    return result;
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
// TBasicINI
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TBasicINI::TBasicINI()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TBasicINI::~TBasicINI()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TBasicINI::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TBasicINI::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TBasicINI::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here
    PaddingAfterOperator_Write = "";
    AlternateAssignOperator_Read = ':';

    Sections.clear();

    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
bool TBasicINI::AddSection()
{
    Sections.push_back(TSection());
    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::AddSection(const TSection& section)
{
    Sections.push_back(section);
    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::InsertSection(size_t index)
{
    return InsertSection(index, TSection());
}
//---------------------------------------------------------------------------
bool TBasicINI::InsertSection(size_t index, const TSection& section)
{
    if (index >= Sections.size())
    {
        Sections.push_back(section);
    }
    else
    {
        std::vector<TSection>::iterator itIdx = Sections.begin() + static_cast<std::ptrdiff_t>(index);
        Sections.insert(itIdx, section);
    }

    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::DeleteSection(size_t index)
{
    if (index >= Sections.size())
        return true;

    std::vector<TSection>::iterator itIdx = Sections.begin() + static_cast<std::ptrdiff_t>(index);
    Sections.erase(itIdx);
    return true;
}
//---------------------------------------------------------------------------
size_t TBasicINI::FindSection(const std::string& sectionName, bool ignoreCase) const
{
    for (size_t i = 0; i < Sections.size(); i++)
    {
        TSection const* secP = &Sections[i];

        if ((sectionName.length() == 0 && secP->Name.length() == 0) ||
            (ignoreCase && _stricmp(sectionName.c_str(), secP->Name.c_str()) == 0) ||
            (!ignoreCase && strcmp(sectionName.c_str(), secP->Name.c_str()) == 0))
            return i;
    }

    return TSection::NotFound;
}
//---------------------------------------------------------------------------
/*
    TBasicINI::FindOrCreateSection

    - On success, returns either the found index of existing section, or index of newly added section.
    - On failure, returns less than zero.
*/
size_t TBasicINI::FindOrCreateSection(const std::string& sectionName, bool ignoreCase)
{
    size_t idx = FindSection(sectionName, ignoreCase);

    if (TSection::NotFound == idx)
    {
        Sections.push_back(TSection());
        size_t insertIdx = Sections.size() - 1;
        Sections[insertIdx].Name = sectionName;
        idx = insertIdx;
    }

    return idx;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Load(const std::string& fileNameINI, const char assignOperator, size_t maxLineLen)
{
    Reset();

    if (TStrTool::IsEmptyOrWhiteSpace(fileNameINI))
    {
        return EErrINI::EI_BadParameter;
    }

    if (!TPathTool::File_Exists_WinAPI(fileNameINI))
    {
        return EErrINI::EI_FileNotExists;
    }

    FILE* fIn = nullptr;

    if (!TPathTool::File_Open(TStrTool::Utf8ToUnicodeStr(fileNameINI), fIn, L"r", SH_DENYWR))
    {
        return EErrINI::EI_FailOpenRead;
    }

    EErrINI result = Load(fIn, assignOperator, maxLineLen);
    TPathTool::File_Close(fIn);

    return result;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Load(FILE* fIn, const char assignOperator, size_t maxLineLen)
{
    if (nullptr == fIn)
    {
        return EErrINI::EI_BadParameter;
    }

    EErrINI result = EErrINI::EI_NoError;
    const size_t lineSize = maxLineLen + sizeof('\0') + sizeof('\r') + sizeof('\n');
    char* line = new char[lineSize];
    std::unique_ptr<char[]> auto_line(line);

    //read in one line at a time and parse
    while (!::feof(fIn))
    {
        if (::fgets(line, static_cast<int>(lineSize), fIn) == nullptr)
        {
            int fErr = ::ferror(fIn);

            if (0 != fErr)
            {
                result = EErrINI::EI_ReadError;
            }

            break;
        }

        char* lineP = line;

        //skip spaces at start
        while (' ' == *lineP || '\n' == *lineP)
            lineP++;

        if (!*lineP)
            continue; //skip empty lines

        size_t len = strlen(lineP);

        //remove the new line ender if there is one
        if (lineP[len - 1] == '\n')
            lineP[--len] = '\0';

        if (TSection::SectionStart == *lineP)
        {
            TSection section;
            section.Name = lineP;
            Sections.push_back(section);
            continue;
        }

        if (Sections.size() == 0)
        {
            //no section was found, but a key value pair or comment line is coming, so, create a "global" scoped section
            Sections.push_back(TSection());
        }

        //get pointer to current section
        TSection* sectionP = &Sections[Sections.size() - 1];
        TKeyVal keyVal;

        if (*lineP == TKeyVal::CommentStart1 || *lineP == TKeyVal::CommentStart2) //add comment lines as values only (no key)
        {
            keyVal.Value = lineP;
            sectionP->KeyVals.push_back(keyVal);
            continue;
        }

        //add key value pair by finding the assignment operator
        //Note: the first operator will determine the "key" portion. If not found, the whole line will be added as a value without a key
        char* valStartP = lineP;
        char* keyEndP = nullptr;
        bool operatorFound = false;

        while (*valStartP)
        {
            if (' ' == *valStartP || assignOperator == *valStartP || AlternateAssignOperator_Read == *valStartP)
            {
                if (nullptr == keyEndP)
                    keyEndP = valStartP;

                if (' ' != *valStartP)
                {
                    operatorFound = true;
                    //found an operator, skip it
                    valStartP++;
                    //skip white space in order to find the start of the value
                    while (' ' == *valStartP)
                        valStartP++;

                    break; //found the beginning of the value, or an empty value
                }
            }

            valStartP++;
        }

        if (operatorFound)
        {
            *keyEndP = '\0';
            keyVal.Key = lineP;
            keyVal.Value = valStartP;
        }
        else //operator not found - treat the entire line as a value with an empty key
        {
            keyVal.Value = lineP;
        }

        sectionP->KeyVals.push_back(keyVal);
    }

    return result;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Save(const std::string& fileNameINI, bool overWrite, const char assignOperator)
{
    if (TStrTool::IsEmptyOrWhiteSpace(fileNameINI))
    {
        return EErrINI::EI_BadParameter;
    }

    if (!overWrite && TPathTool::File_Exists_WinAPI(fileNameINI))
    {
        return EErrINI::EI_FileExists;
    }

    FILE* fOut = nullptr;

    if (!TPathTool::File_Open(TStrTool::Utf8ToUnicodeStr(fileNameINI), fOut, L"w", SH_DENYWR))
    {
        return EErrINI::EI_FailOpenWrite;
    }

    EErrINI result = Save(fOut, assignOperator);
    TPathTool::File_Close(fOut);

    return result;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Save(FILE* fOut, const char assignOperator)
{
    if (nullptr == fOut)
    {
        return EErrINI::EI_BadParameter;
    }

    EErrINI result = EErrINI::EI_NoError;
    bool writeErr = false;

    for (size_t i = 0; i < Sections.size(); i++)
    {
        TSection* secP = &Sections[i];

        if (i > 0)
            writeErr |= -1 == fprintf(fOut, "\n");

        EErrINI resultTmp = secP->Save(fOut, assignOperator, PaddingAfterOperator_Write);

        if (EErrINI::EI_NoError != result)
        {
            result = resultTmp;
            //break; //save what can be saved
        }
    }

    if (writeErr)
    {
        result = EErrINI::EI_WriteError;
    }

    return result;
}

//---------------------------------------------------------------------------

} // namespace BasicINI

} // namespace ASWTools
