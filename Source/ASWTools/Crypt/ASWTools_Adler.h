/* **************************************************************************
ASWTools_Adler.h
Author: Anthony S. West - ASW Software

Copyright 2025 Anthony S. West

See: https://en.wikipedia.org/wiki/Adler-32

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

#ifndef ASWTools_AdlerH
#define ASWTools_AdlerH
//---------------------------------------------------------------------------
#include <stdint.h>
#include <string>
//---------------------------------------------------------------------------

namespace ASWTools
{

namespace Crypt
{

/////////////////////////////////////////////////////////////////////////////
// TAdler
/////////////////////////////////////////////////////////////////////////////
class TAdler
{
public: // Static const vars
    static uint32_t const MOD_ADLER = 65521;

private:
    TAdler();
    ~TAdler();

public:
    static uint32_t Adler32(unsigned char const* data, size_t const dataLenInBytes);
    static uint32_t Adler32(char const* data, size_t const dataLenInBytes);
    static uint32_t Adler32(std::string const& data);
};

} // namespace Crypt

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef ASWTools_AdlerH
