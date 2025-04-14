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
#include <vcl.h>
//---------------------------------------------------------------------------
// Module header
#include "ASWMS_Sprites.h"
//---------------------------------------------------------------------------
#include "ASWTools_Path.h"
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
TSprites::TSpriteList& TSprites::GetDigits()
{
    return m_Digits;
}
//---------------------------------------------------------------------------
void TSprites::LoadDigits(String const& digitsDir)
{
    if (!DirectoryExists(digitsDir))
        throw Exception("Digits directory does not exist: " + digitsDir);

    String filename;

    for (size_t i = 0; i < 10; i++)
    {
        TSprite sprite;
        m_Digits.push_back(sprite);

        filename = "Digit_" + String(i) + ".png";
        filename = TPathTool::Combine(digitsDir.c_str(), filename.c_str());
        m_Digits[i].LoadFromFile(filename);
    }

    TSprite sprite;
    m_Digits.push_back(sprite);
    filename = TPathTool::Combine(digitsDir.c_str(), L"Digit_Blank.png");
    m_Digits[m_Digits.size() - 1].LoadFromFile(filename);
}
//---------------------------------------------------------------------------
void TSprites::LoadSprites(String const& imagesDir)
{
    Reset();

    if (!DirectoryExists(imagesDir))
        throw Exception("Images directory does not exist:\n\n" + imagesDir);

    LoadDigits(TPathTool::Combine(AnsiString(imagesDir).c_str(), "Digits").c_str());
}
//---------------------------------------------------------------------------
void TSprites::Reset()
{
    m_Digits.clear();
}
//---------------------------------------------------------------------------

} // namespace ASWMS
