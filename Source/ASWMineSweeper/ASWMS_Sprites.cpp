/* **************************************************************************
ASWMS_Sprites.cpp
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

//---------------------------------------------------------------------------
// Module header
#include "ASWMS_Sprites.h"
//---------------------------------------------------------------------------
#include "ASWTools_Path.h"
#include "ASWTools_String.h"
//---------------------------------------------------------------------------
using namespace ASWTools;
using namespace System;
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TSprites
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TSprites::TSprites()
{
    Reset();
}
//---------------------------------------------------------------------------
TSprites::~TSprites()
{
}
//---------------------------------------------------------------------------
void TSprites::LoadDigits_Proximity(std::string const& digitsDir)
{
    if (!TPathTool::Dir_Exists_WinAPI(digitsDir))
        throw Exception(("Proximity digits directory does not exist: " + digitsDir).c_str());

    std::string filename;

    for (size_t i = 0; i < 8; i++)
    {
        TSprite sprite;
        Digits_Proximity.push_back(sprite);

#if __cplusplus >= 201103L
        filename = "Proximity_" + std::to_string(i + 1) + ".png";
#else
        filename = "Proximity_" + TStrTool::ToStringA(i + 1) + ".png";
#endif
        filename = TPathTool::Combine(digitsDir, filename);
        Digits_Proximity[i].LoadFromFile(filename);
    }
}
//---------------------------------------------------------------------------
void TSprites::LoadDigits_Score(std::string const& digitsDir)
{
    if (!TPathTool::Dir_Exists_WinAPI(digitsDir))
        throw Exception(("Digits directory does not exist: " + digitsDir).c_str());

    std::string filename;

    for (size_t i = 0; i < 10; i++)
    {
        TSprite sprite;
        Digits_Score.push_back(sprite);

#if __cplusplus >= 201103L
        filename = "Digit_" + std::to_string(i) + ".png";
#else
        filename = "Digit_" + TStrTool::ToStringA(i) + ".png";
#endif
        filename = TPathTool::Combine(digitsDir, filename);
        Digits_Score[i].LoadFromFile(filename);
    }

    TSprite sprite;
    Digits_Score.push_back(sprite);
    filename = TPathTool::Combine(digitsDir, "Digit_Blank.png");
    Digits_Score[Digits_Score.size() - 1].LoadFromFile(filename);
}
//---------------------------------------------------------------------------
void TSprites::LoadGeneralSprites(std::string const& spritesDir)
{
    if (!TPathTool::Dir_Exists_WinAPI(spritesDir))
        throw Exception(("Sprites directory does not exist: " + spritesDir).c_str());

    FaceHappy.LoadFromFile(TPathTool::Combine(spritesDir, "Face_Happy.png"));
    FaceScared.LoadFromFile(TPathTool::Combine(spritesDir, "Face_Scared.png"));
    FaceToast.LoadFromFile(TPathTool::Combine(spritesDir, "Face_Toast.png"));
    FaceWin.LoadFromFile(TPathTool::Combine(spritesDir, "Face_Win.png"));
    Flag.LoadFromFile(TPathTool::Combine(spritesDir, "Flag.png"));
    FlagX.LoadFromFile(TPathTool::Combine(spritesDir, "FlagX.png"));
    Mine.LoadFromFile(TPathTool::Combine(spritesDir, "Mine.png"));
    Question.LoadFromFile(TPathTool::Combine(spritesDir, "Question.png"));
}
//---------------------------------------------------------------------------
void TSprites::LoadSprites(std::string const& imagesDir)
{
    Reset();

    if (!TPathTool::Dir_Exists_WinAPI(imagesDir))
        throw Exception(("Images directory does not exist:\n\n" + imagesDir).c_str());

    LoadGeneralSprites(TPathTool::Combine(imagesDir, "Sprites"));
    LoadDigits_Proximity(TPathTool::Combine(imagesDir, "Sprites"));
    LoadDigits_Score(TPathTool::Combine(imagesDir, "Digits"));
    LoadTiles(TPathTool::Combine(imagesDir, "Tiles"));
}
//---------------------------------------------------------------------------
void TSprites::LoadTiles(std::string const& tilesDir)
{
    if (!TPathTool::Dir_Exists_WinAPI(tilesDir))
        throw Exception(("Tiles directory does not exist: " + tilesDir).c_str());

    TSprite sprite;

    sprite.LoadFromFile(TPathTool::Combine(tilesDir, "Covered.png"));
    Tiles.push_back(sprite);
    sprite.LoadFromFile(TPathTool::Combine(tilesDir, "Covered_Clicked.png"));
    Tiles.push_back(sprite);
    sprite.LoadFromFile(TPathTool::Combine(tilesDir, "Covered_Lit.png"));
    Tiles.push_back(sprite);
    sprite.LoadFromFile(TPathTool::Combine(tilesDir, "Uncovered.png"));
    Tiles.push_back(sprite);
    sprite.LoadFromFile(TPathTool::Combine(tilesDir, "Uncovered_Boom.png"));
    Tiles.push_back(sprite);
}
//---------------------------------------------------------------------------
void TSprites::Reset()
{
    Digits_Proximity.clear();
    Digits_Score.clear();
    Tiles.clear();

    FaceHappy.Reset();
    FaceScared.Reset();
    FaceToast.Reset();
    FaceWin.Reset();
    Flag.Reset();
    FlagX.Reset();
    Mine.Reset();
    Question.Reset();
}
//---------------------------------------------------------------------------

} // namespace ASWMS
