/* **************************************************************************
Scores.cpp
Author: Anthony S. West - ASW Software

Copyright 2025 Anthony S. West

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

#include <vcl.h>
#if !defined(__clang__)
#pragma hdrstop
#endif // #if !defined(__clang__)

//---------------------------------------------------------------------------
// Module header
#include "Scores.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
#include <algorithm>
#include <string>
//---------------------------------------------------------------------------
#include "ASWTools_Adler.h"
#include "ASWTools_Path.h"
#include "ASWTools_String.h"
//---------------------------------------------------------------------------
using namespace ASWTools;
using namespace ASWTools::BasicINI;
using namespace System;
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TScores
/////////////////////////////////////////////////////////////////////////////

char const* TScores::SectionName_General = "[General]";
char const* TScores::SectionName_Scores = "[Scores]";

// Key names - General
char const* TScores::KeyName_Gen_Check = "Check";

// Key names - Scores
char const* TScores::KeyName_Scores_Beginner = "Beginner";
char const* TScores::KeyName_Scores_Intermediate = "Intermediate";
char const* TScores::KeyName_Scores_Expert = "Expert";

//---------------------------------------------------------------------------
TScores::TScores()
    : m_Check(Check_NotSetVal)
{
    Reset();
}

//---------------------------------------------------------------------------
TScores::~TScores()
{
}
//---------------------------------------------------------------------------
bool TScores::Reset()
{
    if (!Inherited::Reset())
        return false;

    m_Check = Check_NotSetVal;
    Beginner.clear();
    Intermediate.clear();
    Expert.clear();
    return true;
}
//---------------------------------------------------------------------------
void TScores::AddScore(TScoreList& list, TScore const& score)
{
    list.push_back(score);
}
//---------------------------------------------------------------------------
void TScores::AddScore(TScoreList& list, int seconds, std::string const& name)
{
    TScore score;
    score.Seconds = seconds;
    score.Name = name;
    score.TimeUtcStr = TStrTool::DateTime_GetUTCNow_ISO8601();
    AddScore(list, score);
}
//---------------------------------------------------------------------------
bool TScores::ApplyChanges_General()
{
    std::string sectionName = SectionName_General;
    std::string searchKey;
    bool result = true;
    TSection* secP;
    TKeyVal* keyValP;
    size_t idx;

    idx = FindOrCreateSection(sectionName, true);

    if (TSection::NotFound == idx)
        return false;

    secP = &Sections[idx];
    secP->Name = sectionName;

    // Apply values to the section

    // Check hash value
    searchKey = KeyName_Gen_Check;
    if (TSection::NotFound == (idx = secP->FindOrCreateKey(searchKey, true)))
    {
        result = false;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        keyValP->Key = searchKey;
        m_Check = CalcCheckHash();
#if __cplusplus >= 201103L
        keyValP->Value = std::to_string(m_Check);
#else
        keyValP->Value = TStrTool::ToStringA(m_Check);
#endif
    }

    return result;
}
//---------------------------------------------------------------------------
bool TScores::ApplyChanges_Scores()
{
    std::string sectionName = SectionName_Scores;
    bool result = true;
    TSection* secP;
    size_t idx;

    idx = FindOrCreateSection(sectionName, true);

    if (TSection::NotFound == idx)
        return false;

    secP = &Sections[idx];
    secP->Name = sectionName;

    // Apply values to the section

    // Clear underyling key value pairs because each list will be re-added fresh.
    secP->KeyVals.clear();

    // Beginner score(s)
    for (TScores::TScoreList::const_iterator it = Beginner.begin(); it != Beginner.end(); it++)
    {
        TScore const& score = *it;
        secP->AddKeyVal(KeyName_Scores_Beginner, EncodeScoreToB64(score));
    }

    // Intermediate score(s)
    for (TScores::TScoreList::const_iterator it = Intermediate.begin(); it != Intermediate.end(); it++)
    {
        TScore const& score = *it;
        secP->AddKeyVal(KeyName_Scores_Intermediate, EncodeScoreToB64(score));
    }

    // Expert score(s)
    for (TScores::TScoreList::const_iterator it = Expert.begin(); it != Expert.end(); it++)
    {
        TScore const& score = *it;
        secP->AddKeyVal(KeyName_Scores_Expert, EncodeScoreToB64(score));
    }

    return result;
}
//---------------------------------------------------------------------------
uint32_t TScores::CalcCheckHash()
{
    uint64_t adlerBeginner = GetAdler32(Beginner);
    uint64_t adlerIntermediate = GetAdler32(Intermediate);
    uint64_t adlerExpert = GetAdler32(Expert);

    return static_cast<uint32_t>(adlerBeginner + adlerIntermediate + adlerExpert);
}
//---------------------------------------------------------------------------
std::string TScores::EncodeScoreToB64(TScore const& score) const
{
#if __cplusplus >= 201103L
    std::string delim = std::to_string(score.Seconds) + ScoreSplitChar + score.Name + ScoreSplitChar + score.TimeUtcStr;
#else
    std::string delim =
        TStrTool::ToStringA(score.Seconds) + ScoreSplitChar + score.Name + ScoreSplitChar + score.TimeUtcStr;
#endif
    return TStrTool::EncodeStrToBase64Str(delim, false);
}
//---------------------------------------------------------------------------
// On success, and if score is not null, score is populated with the delimited values from 'b64'.
bool TScores::DecodeScoreFromB64(std::string b64, TScore* score) const
{
    static size_t const expectedElementCount = 3;

    if (nullptr != score)
        score->Reset();

    std::string scoreAndName = TStrTool::DecodeBase64ToStrA(b64);
    std::vector<std::string> elements = TStrTool::Split(scoreAndName, ScoreSplitChar);

    if (elements.size() < expectedElementCount)
        return false; // User tampering with scores?

    int seconds = 0;
    if (!TStrTool::TryStrToInt32(elements[0], &seconds))
        return false; // User tampering with scores?

    if (nullptr != score)
    {
        score->Seconds = seconds;
        score->Name = elements[1];
        score->TimeUtcStr = elements[2];
    }

    return true;
}
//---------------------------------------------------------------------------
uint32_t TScores::GetAdler32(TScoreList const& scores)
{
    std::string data;

    for (TScores::TScoreList::const_iterator it = scores.begin(); it != scores.end(); it++)
    {
        TScore const& item = *it;
#if __cplusplus >= 201103L
        data += (std::to_string(item.Seconds) + "|" + item.Name + "|" + item.TimeUtcStr + "\n");
#else
        data += (TStrTool::ToStringA(item.Seconds) + "|" + item.Name + "|" + item.TimeUtcStr + "\n");
#endif
    }

    return Crypt::TAdler::Adler32(data);
}
//---------------------------------------------------------------------------
bool TScores::ParseSection_General()
{
    std::string sectionName = SectionName_General;
    std::string searchKey, tmpStr;
    TSection* secP;
    TKeyVal* keyValP;
    size_t idx;

    if (Sections.size() == 0)
    {
        // All sections are missing - use defaults
        return true;
    }

    idx = FindSection(sectionName, true);

    if (TSection::NotFound == idx)
    {
        // Section missing - use defaults
        return true;
    }

    // Found section - parse key values
    secP = &Sections[idx];

    searchKey = KeyName_Gen_Check;
    idx = secP->FindKey(searchKey, true);
    if (TSection::NotFound == idx)
    {
        // Key is missing - use default
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        tmpStr = TStrTool::Trim_Copy(keyValP->Value);

        uint32_t valUInt32 = 0;
        if (tmpStr.length() == 0 || !TStrTool::TryStrToUInt32(tmpStr, &valUInt32))
        {
//            ELog.fwprintf(ELogMsgLevel::LML_Light,
//                L"%s: Warning: Section \"%s\" has invalid value for key: \"%s\".\n",
//                codeSectStr, sectionNameW.c_str(), TStrTool::Utf8ToUnicodeStr(searchKey).c_str());
        }
        else
        {
            m_Check = valUInt32;
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool TScores::ParseSection_Scores()
{
    std::string sectionName = SectionName_Scores;
    TSection* secP;
    size_t idx;

    if (Sections.size() == 0)
    {
        // All sections are missing - use defaults
        return true;
    }

    idx = FindSection(sectionName, true);

    if (TSection::NotFound == idx)
    {
        // Section missing - use defaults
        return true;
    }

    // Found section - parse key values
    secP = &Sections[idx];

    // Read in scores
    for (size_t i = 0, count = secP->KeyVals.size(); i < count; i++)
    {
        TKeyVal const* keyValP = &secP->KeyVals[i];
        // Ignore empty keys, such as comments
        if (!keyValP->IsKeyValuePair())
            continue;

        TScore score;
        if (!DecodeScoreFromB64(keyValP->Value, &score))
            continue;

        if (TStrTool::CompareIC(KeyName_Scores_Beginner, keyValP->Key) == 0)
            Beginner.push_back(score);
        else if (TStrTool::CompareIC(KeyName_Scores_Intermediate, keyValP->Key) == 0)
            Intermediate.push_back(score);
        else if (TStrTool::CompareIC(KeyName_Scores_Expert, keyValP->Key) == 0)
            Expert.push_back(score);
    }

    return true;
}
//---------------------------------------------------------------------------
EErrINI TScores::Load(const std::string& fileNameINI, const char assignOperator, size_t maxLineLen)
{
    EErrINI result = Inherited::Load(fileNameINI, assignOperator, maxLineLen);

    if (EErrINI::EI_NoError != result)
    {
        return result;
    }

    // Base has loaded the file - parse relevant fields, if present
    if (!ParseSection_General())
    {
        result = EErrINI::EI_ParseFail;
    }

    if (!ParseSection_Scores())
    {
        result = EErrINI::EI_ParseFail;
    }

    SortScores();

    return result;
}
//---------------------------------------------------------------------------
EErrINI TScores::Save(const std::string& fileNameINI, bool overWrite, const char assignOperator)
{
    EErrINI result = EErrINI::EI_NoError;

    SortScores();

    if (!ApplyChanges_General())
    {
        result = EErrINI::EI_FailedApplyChanges;
    }

    if (!ApplyChanges_Scores())
    {
        result = EErrINI::EI_FailedApplyChanges;
    }

    // Save to disk now that changes are applied
    if (EErrINI::EI_NoError == result)
        result = Inherited::Save(fileNameINI, overWrite, assignOperator);

    if (EErrINI::EI_NoError != result)
    {
        // Future - log error here
    }

    return result;
}
//---------------------------------------------------------------------------
void TScores::SortScores()
{
    std::sort(Beginner.begin(), Beginner.end(), TScore::CompareAsc);
    std::sort(Intermediate.begin(), Intermediate.end(), TScore::CompareAsc);
    std::sort(Expert.begin(), Expert.end(), TScore::CompareAsc);
}
//---------------------------------------------------------------------------
// Sorts the scores by best time and keeps no more than 'maxScoresToKeepPerCategory' scores per category.
void TScores::TrimScores(size_t maxScoresToKeepPerCategory)
{
    SortScores();

    if (maxScoresToKeepPerCategory < Beginner.size())
        Beginner.resize(maxScoresToKeepPerCategory);

    if (maxScoresToKeepPerCategory < Intermediate.size())
        Intermediate.resize(maxScoresToKeepPerCategory);

    if (maxScoresToKeepPerCategory < Expert.size())
        Expert.resize(maxScoresToKeepPerCategory);
}
//---------------------------------------------------------------------------
// Used for checking against high scores tampering
bool TScores::ValidateCheck()
{
    uint32_t const check = CalcCheckHash();
    return check == m_Check;
}
//---------------------------------------------------------------------------

} // namespace SweepThemMines
