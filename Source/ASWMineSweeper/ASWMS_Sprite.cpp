/* **************************************************************************
ASWMS_Sprite.cpp
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
#include "ASWMS_Sprite.h"
//---------------------------------------------------------------------------
#include <fstream>
#include <memory>
#include <string>
//---------------------------------------------------------------------------
#include <Vcl.Imaging.GIFImg.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------
using namespace System;
//---------------------------------------------------------------------------

namespace ASWMS
{

/////////////////////////////////////////////////////////////////////////////
// TSprite
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TSprite::TSprite()
    : m_Bmp(nullptr)
{
    Reset();
}
TSprite::TSprite(TSprite const& other)
    : m_Bmp(nullptr)
{
    CopyFrom(other);
}
//---------------------------------------------------------------------------
TSprite::~TSprite()
{
    FreeBitmap();
}
//---------------------------------------------------------------------------
TSprite& TSprite::operator=(TSprite const& rhs)
{
    return CopyFrom(rhs);
}
//---------------------------------------------------------------------------
TSprite& TSprite::CopyFrom(TSprite const& other)
{
    if (this == &other)
        return *this;

    FreeBitmap();
    m_Bmp = new TBitmap();
    m_Bmp->Assign(other.m_Bmp);

    m_Filename = other.m_Filename;

    return *this;
}
//---------------------------------------------------------------------------
void TSprite::FreeBitmap()
{
    delete m_Bmp;
    m_Bmp = nullptr;
}
//---------------------------------------------------------------------------
Graphics::TBitmap* TSprite::GetBitmap()
{
    return m_Bmp;
}
//---------------------------------------------------------------------------
String TSprite::GetFilename()
{
    return m_Filename;
}
//---------------------------------------------------------------------------
String TSprite::GetPictureFileExtension(String const& filename)
{
    // Set the starting extension
    String ext = ExtractFileExt(filename.LowerCase());

    if (!FileExists(filename))
        return ext;

    // Scan the image file to determine actual file extension

    std::ifstream in;
    // May want to set the exception bit in the future. If so, uncomment to throw an exception on open failure, etc.
    //in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    static size_t const idLen = 15;
    unsigned char id[idLen + sizeof('\0')];
    id[idLen] = '\0';

    try
    {
        in.open(AnsiString(filename).c_str(), std::ios::in | std::ios::binary);
        if (in.fail())
            throw Exception("Failed to open file: " + filename);
    }
    catch (std::ifstream::failure const& ex)
    {
        throw Exception(String("Failed to open file: ") + ex.what());
    }

    // Read in first N bytes to determine file type
    in.read(reinterpret_cast<char*>(id), static_cast<std::streamsize>(idLen));
    size_t bytesRead = static_cast<size_t>(in.gcount());
    in.close();
    id[bytesRead] = '\0';

    static size_t const offsetJpg = 6;

    // Override extension if applicable
    if (std::memcmp("BM", id, 2) == 0) // bitmap
        ext = ".bmp";
    else if (std::memcmp("II", id, 2) == 0 || std::memcmp("MM", id, 2) == 0) // TIFF
        ext = ".tif";
    else if (std::memcmp("GIF", id, 3) == 0) // GIF
        ext = ".gif";
    else if (std::memcmp("PNG", id, 3) == 0) // PNG
        ext = ".png";
    else if (std::memcmp("JFIF", id + offsetJpg, 4) == 0) // JPG
        ext = ".jpg";
    else if (id[0] == 0 && id[1] == 0 && id[2] == 1 && id[3] == 0) // icon
        ext = ".ico";
    else if (id[0] == 1 && id[1] == 0 && id[2] == 0 && id[3] == 0) // emf
        ext = ".emf";
    else if ((id[0] == 0xD7 && id[1] == 0xCD && id[2] == 0xC6 && id[3] == 0x9A && id[4] == 0x00 && id[5] == 0x00) ||
             (id[0] == 1 && id[1] == 0 && id[2] == 9 && id[3] == 0 && id[4] == 0 && id[5] == 3))
        ext = ".wmf";

    return ext;
}
//---------------------------------------------------------------------------
void TSprite::LoadFromFile(String const& filename)
{
    Reset();

    if (!FileExists(filename))
        throw Exception("File does not exist: " + filename);

    m_Filename = filename;

    String const ext = GetPictureFileExtension(filename);

    // load the picture
    if (ext == ".ico")
    {
        TIcon* icon = new TIcon();
        std::unique_ptr<TIcon> auto_icon(icon);

        icon->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf8bit;
        m_Bmp->Width = icon->Width;
        m_Bmp->Height = icon->Height;
        m_Bmp->Canvas->Draw(0, 0, icon);
    }
    else if (ext == ".wmf" || ext == ".emf")//metafile
    {
        TMetafile* metaFile = new TMetafile();
        std::unique_ptr<TMetafile> auto_metaFile(metaFile);

        metaFile->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf8bit;
        m_Bmp->Width = metaFile->Width;
        m_Bmp->Height = metaFile->Height;
        m_Bmp->Canvas->Draw(0, 0, metaFile);
    }
    else if (ext == ".jpg" || ext == "jpeg")
    {
        TJPEGImage* jpeg = new TJPEGImage;
        std::unique_ptr<TJPEGImage> auto_jpeg(jpeg);

        jpeg->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf24bit;
        m_Bmp->Width = jpeg->Width;
        m_Bmp->Height = jpeg->Height;
        m_Bmp->Canvas->Draw(0, 0, jpeg);
    }
    else if (ext == ".gif")
    {
        TGIFImage* gifImage = new TGIFImage;
        std::unique_ptr<TGIFImage> auto_gifImage(gifImage);

        gifImage->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf8bit;
        m_Bmp->Width = gifImage->Width;
        m_Bmp->Height = gifImage->Height;
        m_Bmp->Canvas->Draw(0, 0, gifImage);
    }
    else if (ext == ".png")
    {
        TPngImage* imagePng = new TPngImage();
        std::unique_ptr<TPngImage> auto_imagePNG(imagePng);

        imagePng->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf24bit;
        m_Bmp->Width = imagePng->Width;
        m_Bmp->Height = imagePng->Height;
        m_Bmp->Canvas->Draw(0, 0, imagePng);
    }
    else if (ext == ".tif")
    {
        TPicture* tmpPic = new TPicture;
        std::unique_ptr<TPicture> auto_tmpPic(tmpPic);

        tmpPic->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf24bit;
        m_Bmp->Width = tmpPic->Width;
        m_Bmp->Height = tmpPic->Height;
        m_Bmp->Canvas->Draw(0, 0, tmpPic->Graphic);
    }
    else if (ext == ".bmp")
    {
        m_Bmp->LoadFromFile(filename);
    }
    else //try to load whatever it is
    {
        TPicture* tmpPic = new TPicture;
        std::unique_ptr<TPicture> auto_tmpPic(tmpPic);

        tmpPic->LoadFromFile(filename);
        m_Bmp->PixelFormat = pf24bit;
        m_Bmp->Width = tmpPic->Width;
        m_Bmp->Height = tmpPic->Height;
        m_Bmp->Canvas->Draw(0, 0, tmpPic->Graphic);
    }

    m_Bmp->PixelFormat = pf32bit;
}
//---------------------------------------------------------------------------
void TSprite::Reset()
{
    FreeBitmap();
    m_Filename = EmptyStr;
    m_Bmp = new Graphics::TBitmap();
}
//---------------------------------------------------------------------------

} // namespace ASWMS
