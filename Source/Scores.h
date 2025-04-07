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
#include <vector>
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

    TScore()
        : Seconds(0)
    {
    }
    TScore(std::string const& name, int seconds)
        : Seconds(seconds),
          Name(name)
    {
    }
};


/////////////////////////////////////////////////////////////////////////////
// TScores
/////////////////////////////////////////////////////////////////////////////
class TScores
{
public:
    typedef std::vector<TScore> TScoreList;

public:
    TScoreList Beginner;
    TScoreList Intermediate;
    TScoreList Expert;

public:
    TScores();
    ~TScores();

    void Reset();

    void Load(std::string filename);
    void Save(std::string filename);
};

} // namespace SweepThemMines

#endif // #ifndef ScoresH
