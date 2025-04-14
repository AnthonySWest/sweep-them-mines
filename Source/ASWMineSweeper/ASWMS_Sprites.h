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
#include <vector>
//---------------------------------------------------------------------------
#include "ASWMS_Sprite.h"
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TSprites
/////////////////////////////////////////////////////////////////////////////
class TSprites
{
public:
    typedef std::vector<TSprite> TSpriteList;

private:
    TSpriteList m_Digits;

private:
    void LoadDigits(System::String const& digitsDir);

public:
    TSprites();
    ~TSprites();

    void LoadSprites(System::String const& imagesDir);
    void Reset();

public: // Getters/Setters
    TSpriteList& GetDigits();

    __property TSpriteList& Digits = { read = GetDigits };
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_SpritesH
