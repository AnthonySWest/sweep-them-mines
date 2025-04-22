/* **************************************************************************
ASWMS_Sprite.h
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

#ifndef ASWMS_SpriteH
#define ASWMS_SpriteH
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TSprite
/////////////////////////////////////////////////////////////////////////////
class TSprite
{
private:
    Graphics::TBitmap* m_Bmp;
    std::string m_Filename;

    void FreeBitmap();
    std::string GetPictureFileExtension(std::string const& filename);

public:
    TSprite();
    TSprite(TSprite const& other);
    ~TSprite();

    TSprite& operator=(TSprite const& rhs);

    TSprite& CopyFrom(TSprite const& other);
    void LoadFromFile(std::string const& filename);
    void Reset();

public: // Getters/Setters
    Graphics::TBitmap* GetBitmap();
    std::string GetFilename();

    __property Graphics::TBitmap* Bmp = { read = GetBitmap };
    __property std::string Filename = { read = GetFilename };
};

} // namespace ASWMS

//---------------------------------------------------------------------------
#endif // #ifndef ASWMS_SpriteH
