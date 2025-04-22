/* **************************************************************************
ASWMS_Sprites.h
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

#ifndef ASWMS_SpritesH
#define ASWMS_SpritesH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
//---------------------------------------------------------------------------
#include "ASWMS_Sprite.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

enum class ETile
{
    Covered,
    CoveredClicked,
    CoveredLit,
    Uncovered,
    UncoveredBoom,
};


/////////////////////////////////////////////////////////////////////////////
// TSprites
/////////////////////////////////////////////////////////////////////////////
class TSprites
{
public: // Static vars
    static size_t const BlankScoreDigitIndex = 10;

public:
    typedef std::vector<TSprite> TSpriteList;

private:
    void LoadDigits_Proximity(std::string const& digitsDir);
    void LoadDigits_Score(std::string const& digitsDir);
    void LoadGeneralSprites(std::string const& spritesDir);
    void LoadTiles(std::string const& tilesDir);

public:
    TSprites();
    ~TSprites();

    void LoadSprites(std::string const& imagesDir);
    void Reset();

public:
    TSprite FaceHappy;
    TSprite FaceScared;
    TSprite FaceToast;
    TSprite FaceWin;
    TSprite Flag;
    TSprite FlagX;
    TSprite Mine;
    TSprite Question;

    TSpriteList Digits_Proximity;
    TSpriteList Digits_Score;
    TSpriteList Tiles;
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_SpritesH
