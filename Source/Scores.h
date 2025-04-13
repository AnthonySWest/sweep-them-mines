/* **************************************************************************
Scores.h
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

#ifndef ScoresH
#define ScoresH
//---------------------------------------------------------------------------
#include <stdint.h>
#include <vector>
//---------------------------------------------------------------------------
#include "ASWTools_BasicINI.h"
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TScore
/////////////////////////////////////////////////////////////////////////////
struct TScore
{
    int Seconds;
    std::string Name;
    std::string TimeUtcStr;

    TScore()
        : Seconds(0)
    {
    }
    TScore(int seconds, std::string const& name, std::string const& timeUtc)
        : Seconds(seconds),
          Name(name),
          TimeUtcStr(timeUtc)
    {
    }

    void Reset()
    {
        Seconds = 0;
        Name = "";
        TimeUtcStr = "";
    }

    static bool CompareAsc(TScore const& a, TScore const& b)
    {
        if (a.Seconds != b.Seconds)
            return a.Seconds < b.Seconds; // Sort by Score ascending
        else if (a.Name != b.Name)
            return a.Name < b.Name; // Sort by Name alphabetically
        else
            return a.TimeUtcStr < b.TimeUtcStr; // Keep oldest time first
    }
};


/////////////////////////////////////////////////////////////////////////////
// TScores
/////////////////////////////////////////////////////////////////////////////
class TScores : public ASWTools::BasicINI::TBasicINI
{
private:
    typedef ASWTools::BasicINI::TBasicINI Inherited;

public:
    typedef std::vector<TScore> TScoreList;

public: // Static variables
    static char const ScoreSplitChar = '|';
    static unsigned int const Check_NotSetVal = static_cast<unsigned int>(-1);
    static size_t const Default_MaxScoresToKeep = 5;

    // Section names
    static char const* SectionName_General;
    static char const* SectionName_Scores;

    // Key names - General
    static char const* KeyName_Gen_Check;

    // Key names - Scores
    static char const* KeyName_Scores_Beginner;
    static char const* KeyName_Scores_Intermediate;
    static char const* KeyName_Scores_Expert;

private:
    uint32_t m_Check;

public:
    TScoreList Beginner;
    TScoreList Intermediate;
    TScoreList Expert;

private:
    bool ParseSection_General();
    bool ParseSection_Scores();

    bool ApplyChanges_General();
    bool ApplyChanges_Scores();

private:
    std::string EncodeScoreToB64(TScore const& score) const;
    bool DecodeScoreFromB64(std::string b64, TScore* score) const;
    uint32_t GetAdler32(TScoreList const& scores);
    uint32_t CalcCheckHash();

public:
    TScores();
    ~TScores();

    bool Reset() override;

    ASWTools::BasicINI::EErrINI Load(const std::string& fileNameINI, const char assignOperator = DefaultAssignOperator,
        size_t maxLineLen = DefaultMaxLineLength) override;
    ASWTools::BasicINI::EErrINI Save(
        const std::string& fileNameINI, bool overWrite, const char assignOperator = DefaultAssignOperator) override;

    void SortScores();
    void TrimScores(size_t maxScoresToKeepPerCategory = Default_MaxScoresToKeep);
    bool ValidateCheck();

public:
    static void AddScore(TScoreList& list, TScore const& score);
    static void AddScore(TScoreList& list, int seconds, std::string const& name);
};

} // namespace SweepThemMines

//---------------------------------------------------------------------------
#endif // #ifndef ScoresH
