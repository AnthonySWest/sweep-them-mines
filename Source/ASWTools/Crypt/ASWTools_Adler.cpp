/* **************************************************************************
ASWTools_Adler.cpp
Author: Anthony S. West - ASW Software

Copyright 2025 Anthony S. West

See header for notes

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
#include "ASWTools_Adler.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

namespace Crypt
{

/////////////////////////////////////////////////////////////////////////////
// TAdler
/////////////////////////////////////////////////////////////////////////////

uint32_t TAdler::Adler32(unsigned char const* data, size_t const dataLenInBytes)
{
    unsigned char const* dataP = data;
    uint32_t sumA = 1; // Sum of all bytes in the stream, plus 1
    uint32_t sumB = 0; // Sum of the individual values of sumA from each loop pass

    // Process each byte of data in order
    for (size_t i = 0; i < dataLenInBytes; i++)
    {
        sumA = (sumA + *dataP++) % MOD_ADLER;
        sumB = (sumB + sumA) % MOD_ADLER;
    }

    return (sumB << 16) | sumA;
}
//---------------------------------------------------------------------------
uint32_t TAdler::Adler32(char const* data, size_t const dataLenInBytes)
{
    return Adler32(reinterpret_cast<unsigned char const*>(data), dataLenInBytes);
}
//---------------------------------------------------------------------------
uint32_t TAdler::Adler32(std::string const& data)
{
    return Adler32(reinterpret_cast<unsigned char const*>(data.c_str()), data.length());
}
//---------------------------------------------------------------------------

} // namespace Crypt

} // namespace ASWTools
