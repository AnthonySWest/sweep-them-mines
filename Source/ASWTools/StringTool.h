/* **************************************************************************
StringTool.h
Author: Anthony S. West - ASW Software

Contains routines for string manipulation

Every attempt should be made to keep this module at least Windows portable.

Copyright 2016 Anthony S. West

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

#ifndef StringToolH
#define StringToolH
//---------------------------------------------------------------------------
#include <windows.h>
#include <cstdint> //int64_t
#include <iomanip>  //for GUID stuff
#include <string>
#include <vector>
//---------------------------------------------------------------------------
#include "ASWTools_Common.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TStrTool
/////////////////////////////////////////////////////////////////////////////
class TStrTool
{
public:
    ////// only const static variables allowed in this public - see second public for regular stuff //////

    static const size_t NUM_HEX_CHARS = 16;
    static const char HEX_UPPER[NUM_HEX_CHARS];
    static const char HEX_LOWER[NUM_HEX_CHARS];
    static const char URL_HEX_ESCAPE = '%';

    static const char Base64_Slash = '/'; //base64 can contain '/' - use this to replace if in a URL
    static const char Base64_Plus = '+'; //base64 can contain '+' - use this to replace if in a URL
    static const char Base64_URLSafe_Slash = '_'; //base64 can contain '/' - use this to replace if in a URL
    static const char Base64_URLSafe_Plus = '-'; //base64 can contain '+' - use this to replace if in a URL

protected:
    ////// static variables //////

    ////// variables //////

    ////// functions //////

    static void DateTime_SetComponents(int y, int month, int d, int h, int min, int s, int ms, int tzH, int tzM,
        int* outYear, int* outMonth, int* outDay, int* outHour, int* outMin, int* outSec, int* outMS,
        int* outTZOffsetH, int* outTZOffsetM);

private:
    ////// static variables //////

    ////// variables //////

    ////// functions //////
    void Destroy_Private();
    bool Reset_Private();

public:
    TStrTool();
    ~TStrTool();

    ////// variables //////

    ////// functions //////
    virtual void Destroy(); //calls Destroy_Private()
    virtual bool Reset(); //calls Reset_Private() - It is good practice for a constructor to not call a virtual function

    static std::string UnicodeStrToUtf8(std::wstring const& str);
    static std::wstring Utf8ToUnicodeStr(std::string const& str);
    static std::wstring Utf8ToUnicodeStr(char const* utf8Bytes, size_t length);

    static std::string GetWindowsLastErrorCodeAsStringA(const DWORD winLastErr);
    static std::wstring GetWindowsLastErrorCodeAsStringW(const DWORD winLastErr);

    static std::string DateTime_GetUTCNow_ISO8601();
    static std::string DateTime_GetMinDate_ISO8601();
    static std::string SystemTimeToStr_ISO8601(SYSTEMTIME const& sysTime);
    static std::string SystemTimeToStr_ISO8601(SYSTEMTIME const& sysTime, const TIME_ZONE_INFORMATION& tZoneInfo);
    static bool DateTime_Parse_ISO8601(std::string const& iso8601Str,
        int* outYear, int* outMonth, int* outDay, int* outHour, int* outMin, int* outSec, int* outMS,
        int* outTZOffsetH, int* outTZOffsetM);

    static std::wstring GetDateTimeStr_LocalW(bool fileNameFriendly = false);
    static std::string GetDateTimeStr_LocalA(bool fileNameFriendly = false);

    static bool StrCpyW(wchar_t* dest, size_t destSize_words, wchar_t const* src);
    static bool StrCpyA(char* dest, size_t destSize_bytes, char const* src);
    static bool StrCpyT(TCHAR* dest, size_t destArrayLen, TCHAR const* src);

    static bool StrNCpy_safeW(wchar_t* dest, size_t destSize_words, wchar_t const* src, size_t maxCount);
    static bool StrNCpy_safeA(char* dest, size_t destSize_bytes, char const* src, size_t maxCount);
    static bool StrNCpy_safeT(TCHAR* dest, size_t destArrayLen, TCHAR const* src, size_t maxCount);

    //For trim functions, see: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    static void TrimLeft(std::string& s);
    static void TrimLeft(std::string& s, const unsigned char trimChar, bool trim_isspace);
    static void TrimLeft(std::wstring& s);
    static void TrimLeft(std::wstring& s, const wchar_t trimChar, bool trim_iswspace);
    static void TrimRight(std::string& s);
    static void TrimRight(std::string& s, const unsigned char trimChar, bool trim_isspace);
    static void TrimRight(std::wstring& s);
    static void TrimRight(std::wstring& s, const wchar_t trimChar, bool trim_iswspace);
    static void Trim(std::string& s);
    static void Trim(std::string& s, const unsigned char trimChar, bool trim_isspace);
    static void Trim(std::wstring& s);
    static void Trim(std::wstring& s, const wchar_t trimChar, bool trim_iswspace);
    static std::string TrimLeft_Copy(std::string s);
    static std::string TrimLeft_Copy(std::string s, const unsigned char trimChar, bool trim_isspace);
    static std::wstring TrimLeft_Copy(std::wstring s);
    static std::wstring TrimLeft_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace);
    static std::string TrimRight_Copy(std::string s);
    static std::string TrimRight_Copy(std::string s, const unsigned char trimChar, bool trim_isspace);
    static std::wstring TrimRight_Copy(std::wstring s);
    static std::wstring TrimRight_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace);
    static std::string Trim_Copy(std::string s);
    static std::string Trim_Copy(std::string s, const unsigned char trimChar, bool trim_isspace);
    static std::wstring Trim_Copy(std::wstring s);
    static std::wstring Trim_Copy(std::wstring s, const wchar_t trimChar, bool trim_iswspace);

    static int Compare(std::string const& s1, std::string const& s2);
    static int Compare(std::wstring const& s1, std::wstring const& s2);
    static int CompareIC(std::string const& s1, std::string const& s2);
    static int CompareIC(std::wstring const& s1, std::wstring const& s2);

    static bool IsSpace(int c);

    static bool IsEmptyOrWhiteSpace(std::string const& s);
    static bool IsEmptyOrWhiteSpace(std::wstring const& s);

    static std::string CombinePathAndArgs(std::string const& path, std::string const& args);
    static std::wstring CombinePathAndArgs(std::wstring const& path, std::wstring const& args);

    static std::vector<std::string> Split(std::string const& str, const char sep);
    static std::vector<std::wstring> Split(std::wstring const& str, const wchar_t sep);

    static std::string DelimStr_Escape(std::string const& strIn, const char delim);
    static std::wstring DelimStr_Escape(std::wstring const& strIn, const wchar_t delim);
    static std::string DelimStr_UnEscape(std::string const& strIn, const char delim);
    static std::wstring DelimStr_UnEscape(std::wstring const& strIn, const wchar_t delim);

    static std::vector<std::string> CombineLists_Unique(const std::vector<std::string>& list1,
        const std::vector<std::string>& list2, bool ignoreBlank, bool ignoreCase);
    static std::vector<std::wstring> CombineLists_Unique(const std::vector<std::wstring>& list1,
        const std::vector<std::wstring>& list2, bool ignoreBlank, bool ignoreCase);

    static std::string ReplaceAll(std::string const& str, const std::string& find, std::string const& replaceWith);
    static std::string ReplaceAll(std::string const& str, char find, char replaceWith);
    static std::wstring ReplaceAll(std::wstring const& str, std::wstring const& find, std::wstring const& replaceWith);
    static std::wstring ReplaceAll(std::wstring const& str, wchar_t find, wchar_t replaceWith);

    static bool HexSingleToByte(const char hexSingle, BYTE* out);
    static int HexSingleToByte(const char hexSingle);
    static std::string EncodeStrToBase16Hex(std::string const& strUtf8, bool upperCase);
    static std::string EncodeStrToBase16Hex(std::string const& strUtf8, size_t length, bool upperCase);
    static std::string EncodeStrToBase16Hex(std::wstring const& strW, bool upperCase);
    static std::string EncodeStrToBase16Hex(std::wstring const& strW, size_t length, bool upperCase);
    static std::string EncodeToBase16Hex(const BYTE* bytes, size_t bytesLen, bool upperCase);
    static std::string DecodeBase16HexToStrA(std::string const& inHex);
    static std::wstring DecodeBase16HexToStrW(std::string const& inHex);
    static int64_t DecodeBase16HexToBytes(std::string const& inHex, BYTE* outBuff, size_t buffSize,
        int64_t* outBytesWritten);

    static bool IsValidBase64(const std::string& str, bool isUrlSafe);
    static bool IsValidBase64(const char* str, bool isUrlSafe);
    static std::string EncodeStrToBase64Str(std::string const& strUtf8, bool makeWebFriendly);
    static std::string EncodeStrToBase64Str(std::string const& strUtf8, size_t length, bool makeWebFriendly);
    static std::string EncodeStrToBase64Str(std::wstring const& strW, bool makeWebFriendly);
    static std::string EncodeStrToBase64Str(std::wstring const& strW, size_t length, bool makeWebFriendly);
    static std::string EncodeToBase64Str_Native(const BYTE* bytes, size_t bytesLen, bool makeWebFriendly);
    static std::string DecodeBase64ToStrA(std::string const& inB64);
    static std::wstring DecodeBase64ToStrW(std::string const& inB64);
    static int DecodeBase64ToBytes_Native(const char* src, BYTE* destBytes, size_t* destBytesSize);

    static bool URL_Split(std::string const& url, std::string* hostUtf8, std::string* pathUtf8);
    static std::string URL_EncodeUtf8(std::string const& valueUtf8, bool useUpperCaseHex = true);
    static std::string URL_DecodeUtf8(std::string const& encodedStr, bool* invalidHexEncountered);

    static std::string Fmt_printf(char const* format, ...);
    static std::wstring Fmt_printf(wchar_t const* format, ...);

    static bool StrToGUID(char const* idStr, GUID& guid);
    static bool StrToGUID(wchar_t const* idStr, GUID& guid);
    static std::string StrFromGUID(GUID const* guid);
    static std::wstring StrFromGUIDW(GUID const* guid);
    static int CompareGUID(GUID* guid1, GUID* guid2);
    static int CompareGUID(GUID const& guid1, GUID const& guid2);
    static bool IsEmptyGUID(GUID* guid);
    static bool IsEmptyGUID(GUID const& guid);
};

} // namespace ASWTools

//---------------------------------------------------------------------------
#endif // #ifndef StringToolH
