/* **************************************************************************
ASWTools_String.cpp
Author: Anthony S. West - ASW Software

See header for info.

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

//---------------------------------------------------------------------------
// Module header
#include "ASWTools_String.h"
//---------------------------------------------------------------------------
#include <algorithm>

#if __cplusplus >= 201103L
#   include <codecvt>
#else
#   include <functional>
#   include <stdarg.h> // va_start
#   include <wctype.h>
#endif // #if __cplusplus >= 201103L

//#include <cstdarg> //va_start
//#include <cctype>
#include <limits>
#include <locale>
#include <stdexcept>
#include <wctype.h>
//---------------------------------------------------------------------------

#ifndef strcmpI
#ifdef __BORLANDC__
    #define strcmpI stricmp
#else
    #define strcmpI _stricmp
#endif // #ifdef __BORLANDC__
#endif // #ifndef strcmpI

#ifdef _MSC_VER
//See: https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019
    #if (_MSC_VER >= 1910) // VStudio 2017 or higher
//This will use the VS safe string functions to avoid the "may be unsafe" warnings.
//Note that _CRT_SECURE_NO_WARNINGS can also be defined in order to ignore the warnings.
    #define USE_SAFESTR_FUNCS //tells logger to use the Microsoft secure char/wchar manipulation functions
#endif // #if (_MSC_VER >= 1910)
#endif // #ifdef _MSC_VER


//---------------------------------------------------------------------------
#ifdef _WIN64
    #ifdef _MSC_VER
        #pragma comment(lib, "rpcrt4.lib") //for UUID stuff for VS
    #else

    #endif
#else
    #pragma comment(lib, "rpcrt4.lib") //for UUID stuff for VS
#endif

//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TStrTool
/////////////////////////////////////////////////////////////////////////////

const char TStrTool::HEX_UPPER[NUM_HEX_CHARS] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
const char TStrTool::HEX_LOWER[NUM_HEX_CHARS] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

//---------------------------------------------------------------------------
TStrTool::TStrTool()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TStrTool::~TStrTool()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TStrTool::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TStrTool::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TStrTool::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here

    return true;
}
//---------------------------------------------------------------------------
bool TStrTool::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
// - Static
std::string TStrTool::UnicodeStrToUtf8(std::wstring const& str)
{
    if (str.empty())
        return "";

#if __cplusplus >= 201103L

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    std::wstring_convert<std::codecvt_utf8<wchar_t> > myconv;
    return myconv.to_bytes(str);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#else
    int utf16Len = str.length();
    int utf8Len = ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), utf16Len, nullptr, 0, nullptr, nullptr);
    if (0 == utf8Len)
        return "";

    std::string utf8Str(utf8Len, '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), utf16Len, &utf8Str[0], utf8Len, nullptr, nullptr);

    return utf8Str;
#endif // #if __cplusplus >= 201103L
}
//---------------------------------------------------------------------------
// -Static
std::wstring TStrTool::Utf8ToUnicodeStr(const std::string& str)
{
    if (str.empty())
        return L"";

#if __cplusplus >= 201103L

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    std::wstring_convert<std::codecvt_utf8<wchar_t> > myconv;
    return myconv.from_bytes(str);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#else
    int wideCharLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (0 == wideCharLen)
        return L"";

    std::wstring strW(wideCharLen, L'\0');
    ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &strW[0], wideCharLen);
    return strW;
//    std::vector<wchar_t> wideCharBuffer(wideCharLen);
//    ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wideCharBuffer.data(), wideCharLen);
//    return std::wstring(wideCharBuffer.begin(), wideCharBuffer.end() - 1);
#endif // #if __cplusplus >= 201103L
}
//---------------------------------------------------------------------------
// - Static
std::wstring TStrTool::Utf8ToUnicodeStr(char const* utf8Bytes, size_t length)
{
    if (0 == length || nullptr == utf8Bytes)
        return L"";

#if __cplusplus >= 201103L

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
    std::wstring_convert<std::codecvt_utf8<wchar_t> > myconv;
    // Note that from_bytes does not include the character pointed to by "last", the 2nd parameter
    return myconv.from_bytes(utf8Bytes, utf8Bytes + length);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
#else
    int wideCharLen = ::MultiByteToWideChar(CP_UTF8, 0, utf8Bytes, length, nullptr, 0);
    if (0 == wideCharLen)
        return L"";

    std::wstring strW(wideCharLen, L'\0');
    ::MultiByteToWideChar(CP_UTF8, 0, utf8Bytes, length, &strW[0], wideCharLen);
    return strW;
#endif // #if __cplusplus >= 201103L
}
//---------------------------------------------------------------------------
// -Static
// -Parameter 'winLastErr' must be the value of API call GetLastError()
std::string TStrTool::GetWindowsLastErrorCodeAsStringA(DWORD const winLastErr)
{
    LPSTR error = nullptr;
    std::string errorStr;

    //translate the windows error code to string
    if (::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr, winLastErr, 0, reinterpret_cast<LPSTR>(&error), 0, nullptr) == 0)
    {
        //if failed to get the error string, show the error hex number instead
        const size_t error2Size = 96;
        char error2[error2Size];
        sprintf_s(error2, error2Size, "Unknown error: %u (0x%08lx)", winLastErr, winLastErr);
        errorStr = error2;
    }
    else if (error != nullptr)
    {
        errorStr = error;
        ::LocalFree(error);
    }

    //A trailing newline is sometimes present. Remove.
    TrimRight(errorStr);
    return errorStr;
}
//---------------------------------------------------------------------------
// -Static
// -Parameter 'winLastErr' must be the value of API call GetLastError()
std::wstring TStrTool::GetWindowsLastErrorCodeAsStringW(DWORD const winLastErr)
{
    LPWSTR error = nullptr;
    std::wstring errorStr;

    //translate the windows error code to string
    if (::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr, winLastErr, 0, reinterpret_cast<LPWSTR>(&error), 0, nullptr) == 0)
    {
        //if failed to get the error string, show the error hex number instead
        size_t const error2Size = 96;
        wchar_t error2[error2Size];
        swprintf_s(error2, error2Size, L"Unknown error: %u (0x%08lx)", winLastErr, winLastErr);
        errorStr = error2;
    }
    else if (error != nullptr)
    {
        errorStr = error;
        ::LocalFree(error);
    }

    //A trailing newline is sometimes present. Remove.
    TrimRight(errorStr);
    return errorStr;
}
//---------------------------------------------------------------------------
// - Static
std::string TStrTool::DateTime_GetUTCNow_ISO8601()
{
    //C++11
//  std::chrono::system_clock::time_point nowTP = std::chrono::system_clock::now();
//  std::time_t nowTT = std::chrono::system_clock::to_time_t(nowTP);
//  std::ostringstream ss;
//  ss << std::put_time(gmtime(&nowTT), "%FT%TZ"); //C++ builder appears to not work for the date part
//  return ss.str();
    time_t now;
    time(&now);
    char buf[sizeof "0001-01-01T00:00:00Z" + 1];
    //strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now)); //C++ builder appears to not work for the date part
    strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    return buf;
}
//---------------------------------------------------------------------------
// - Static
std::string TStrTool::DateTime_GetMinDate_ISO8601()
{
//  std::time_t minTime = std::numeric_limits<time_t>::min();
//  std::ostringstream ss;
//  ss << std::put_time(gmtime(&minTime), "%FT%TZ");
//  return ss.str();
    return "0001-01-01T00:00:00Z"; //minimum value that C# will parse
}
//---------------------------------------------------------------------------
// -Static
// -Returns a string formatted according to ISO8601 with "Z" (UTC) for timezone.
std::string TStrTool::SystemTimeToStr_ISO8601(SYSTEMTIME const& sysTime)
{
    std::ostringstream ssDt;

    ssDt << std::setfill('0');
    ssDt << sysTime.wYear <<  "-" << std::setw(2) << sysTime.wMonth << "-"
    << std::setw(2) << sysTime.wDay << "T" << std::setw(2) << sysTime.wHour << ":"
    << std::setw(2) << sysTime.wMinute << ":" << std::setw(2) << sysTime.wSecond << "."
    << std::setw(3) << sysTime.wMilliseconds
    << "Z"; //assume utc, since no timezone info for this function

    return ssDt.str();
}
//---------------------------------------------------------------------------
// -Static
// -Returns a string formatted according to ISO8601, including timezone offset info.
// -Example usage:
//    SYSTEMTIME dateTime;
//    ::GetLocalTime(&dateTime);
//
//    TIME_ZONE_INFORMATION tzInfo;
//    ::GetTimeZoneInformation(&tzInfo);
//
//    std::string timeStr = SystemTimeToStr_ISO8601(dateTime, tzInfo);
std::string TStrTool::SystemTimeToStr_ISO8601(SYSTEMTIME const& sysTime, TIME_ZONE_INFORMATION const& tZoneInfo)
{
    std::ostringstream ssDt;

    ssDt << std::setfill('0');
    ssDt << sysTime.wYear <<  "-" << std::setw(2) << sysTime.wMonth << "-"
    << std::setw(2) << sysTime.wDay << "T" << std::setw(2) << sysTime.wHour << ":"
    << std::setw(2) << sysTime.wMinute << ":" << std::setw(2) << sysTime.wSecond << "."
    << std::setw(3) << sysTime.wMilliseconds
    << "Z"; //assume utc, since no timezone info for this function

    //Get to UTC by computing localtime + bias. Bias is in minutes.
    LONG utcOffset_hours = tZoneInfo.Bias / 60;
    LONG utcOffset_minutes = std::abs(tZoneInfo.Bias - (utcOffset_hours * 60));
    char offsetSign = tZoneInfo.Bias <= 0 ? '+' : '-';

    //append timezone offset info
    ssDt << offsetSign << std::setw(2) << std::abs(utcOffset_hours) << ":" << utcOffset_minutes;

    return ssDt.str();
}
//---------------------------------------------------------------------------
// - Static
// - Sets all non-null out parameters.
void TStrTool::DateTime_SetComponents(int y, int month, int d, int h, int min, int s, int ms, int tzH, int tzM,
    int* outYear, int* outMonth, int* outDay, int* outHour, int* outMin, int* outSec, int* outMS,
    int* outTZOffsetH, int* outTZOffsetM)
{
    if (outYear)
        *outYear = y;

    if (outMonth)
        *outMonth = month;

    if (outDay)
        *outDay = d;

    if (outHour)
        *outHour = h;

    if (outMin)
        *outMin = min;

    if (outSec)
        *outSec = s;

    if (outMS)
        *outMS = ms;

    if (outTZOffsetH)
        *outTZOffsetH = tzH;

    if (outTZOffsetM)
        *outTZOffsetM = tzM;
}
//---------------------------------------------------------------------------
// - Static
// - If iso8601Str is zero length, the out values will be set to the minimum date.
// - All out parameters can be null if not needed.
// - If both outTZOffsetHours and outTZOffsetMin are zero, then time is assumed to be UTC.
// - Intended to parse inputs like:
//  "2021-08-10T15:45:36.806Z"		//includes milliseconds, and is UTC (TZ offset zero)
//  "2021-08-10T15:45:36.806-7:00"	//includes milliseconds and is offset -7 (Arizona time)
//  "2021-08-10T15:45:36Z"			//does not include milliseconds, and is UTC (TZ offset zero)
//  "2021-08-10T15:45:36-7:00"		//does not include milliseconds, and is offset -7 (Arizona time)
bool TStrTool::DateTime_Parse_ISO8601(std::string const& iso8601Str,
    int* outYear, int* outMonth, int* outDay, int* outHour, int* outMin, int* outSec, int* outMS,
    int* outTZOffsetH, int* outTZOffsetM)
{
    //set initial values to minimum supported by C# DateTime, for compatibility
    DateTime_SetComponents(1, 1, 1, 0, 0, 0, 0, 0, 0,
        outYear, outMonth, outDay, outHour, outMin, outSec, outMS, outTZOffsetH, outTZOffsetM);

    if (iso8601Str.length() == 0)
        return true; //nothing to parse

    int const nValsIfNoOffset_isUTC = 6; //y+M+d+h+m+s (e.g. "2021-08-10T15:45:36Z")
    int y = 1, month = 1, d = 1, h = 0, min = 0, s = 0, ms = 0, tzH = 0, tzM = 0;
    float secsF = 0.0f;

    //Variation of: https://visdap.blogspot.com/2018/12/how-do-i-parse-iso-8601-date-with.html
    //but cleaned, added error checking and a correction for float/double base10 to base2 issue.
    int nScanned = sscanf(iso8601Str.c_str(), "%d-%d-%dT%d:%d:%f%d:%dZ", &y, &month, &d, &h, &min, &secsF, &tzH, &tzM);

    if (EOF == nScanned)
        return false;

    //if the num items scanned is greater than 6, there is a time zone offset in the string
    if (nScanned > nValsIfNoOffset_isUTC)
    {
        //fix minutes offset if hours is negative
        if (tzH < 0)
            tzM = -tzM;
    }

    //extract seconds from float
    s = static_cast<int>(secsF);

    //extract milliseconds from float

    //Note: sscanf() will scan 36.806 and store it as 36.8059998, due to floats/doubles being
    //incapable of storing in base 2 certain rational numbers exactly represented in base 10.
    //Since the milliseconds portion, if present, is range 0-999, add a correction for this
    //situation in the 4th decimal place.
    float correction = 0.0001F;
    ms = static_cast<int>((secsF + correction - static_cast<float>(s)) * 1000);

    DateTime_SetComponents(y, month, d, h, min, s, ms, tzH, tzM,
        outYear, outMonth, outDay, outHour, outMin, outSec, outMS, outTZOffsetH, outTZOffsetM);

    return true;
}
//---------------------------------------------------------------------------
std::wstring TStrTool::GetDateTimeStr_LocalW(bool fileNameFriendly)
{
    SYSTEMTIME time; //has milliseconds
    size_t const bufferSize = 128;
    wchar_t buffer[bufferSize];

    //get current time
    GetLocalTime(&time);

    if (fileNameFriendly)
    {
        swprintf(buffer, bufferSize, L"%04d%02d%02d_%02d%02d%02d.%03d",
            time.wYear, time.wMonth, time.wDay,
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    }
    else
    {
        swprintf(buffer, bufferSize, L"%04d/%02d/%02d::%02d:%02d:%02d.%03d",
            time.wYear, time.wMonth, time.wDay,
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    }

    buffer[bufferSize - 1] = L'\0'; //for safety
    return buffer;
}
//---------------------------------------------------------------------------
std::string TStrTool::GetDateTimeStr_LocalA(bool fileNameFriendly)
{
    SYSTEMTIME time; //has milliseconds
    size_t const bufferSize = 128;
    char buffer[bufferSize];

    //get current time
    GetLocalTime(&time);

    if (fileNameFriendly)
    {
        snprintf(buffer, bufferSize, "%04d%02d%02d_%02d%02d%02d.%03d",
            time.wYear, time.wMonth, time.wDay,
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    }
    else
    {
        snprintf(buffer, bufferSize, "%04d/%02d/%02d::%02d:%02d:%02d.%03d",
            time.wYear, time.wMonth, time.wDay,
            time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    }

    buffer[bufferSize - 1] = '\0'; //for safety
    return buffer;
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to first null terminator, not exceeding
//  destSize_words, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_words is zero.
// -Ignores locale.
bool TStrTool::StrCpyW(wchar_t* dest, size_t destSize_words, wchar_t const* src)
{
    if (nullptr == dest || destSize_words == 0)
        return false;

    wchar_t* destWalker = dest;
    wchar_t const* srcWalker = src;
    size_t count = 0, maxCopy = destSize_words - sizeof('\0');

    while (count < maxCopy && *srcWalker)
    {
        *destWalker++ = *srcWalker++;
        count++;
    }

    *destWalker = L'\0';
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to first null terminator, not exceeding
//  destSize_bytes, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_bytes is zero.
// -Ignores locale.
bool TStrTool::StrCpyA(char* dest, size_t destSize_bytes, char const* src)
{
    if (nullptr == dest || destSize_bytes == 0)
        return false;

    char* destWalker = dest;
    char const* srcWalker = src;
    size_t count = 0, maxCopy = destSize_bytes - sizeof('\0');

    while (count < maxCopy && *srcWalker)
    {
        *destWalker++ = *srcWalker++;
        count++;
    }

    *destWalker = '\0';
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -Performs a copy by calling either StrCpyW() or StrCpyA(), depending on
//  the project TCHAR type (unicode or not).
// -Copies what will fit from src into dest, up to first null terminator, not exceeding
//  destArrayLen, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destArrayLen is zero.
// -Ignores locale.
bool TStrTool::StrCpyT(TCHAR* dest, size_t destArrayLen, TCHAR const* src)
{
#ifdef _UNICODE
    return StrCpyW(dest, destArrayLen, src);
#else
    return StrCpyA(dest, destArrayLen, src);
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to maxCount, or first null terminator, whichever
//  comes first, not exceeding destSize_words, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_words is zero.
bool TStrTool::StrNCpy_safeW(wchar_t* dest, size_t destSize_words, wchar_t const* src, size_t maxCount)
{
    if (nullptr == dest || destSize_words == 0)
        return false;

    //Ensure that maxCount does not exceed the destination buffer size with null terminator.
    if (maxCount >= destSize_words)
        maxCount = destSize_words - sizeof('\0');

    if (0 == maxCount || nullptr == src)
    {
        *dest = L'\0';
        return true; //nothing to copy
    }

#ifdef USE_SAFESTR_FUNCS
    //wcsncpy_s(dest, bufferLen, src, _TRUNCATE);
    return (0 == wcsncpy_s(dest, destSize_words, src, maxCount));
#else
    wcsncpy(dest, src, maxCount);
    dest[maxCount] = L'\0';
    return true;
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Copies what will fit from src into dest, up to maxCount, or first null terminator, whichever
//  comes first, not exceeding destSize_bytes, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destSize_bytes is zero.
bool TStrTool::StrNCpy_safeA(char* dest, size_t destSize_bytes, char const* src, size_t maxCount)
{
    if (nullptr == dest || destSize_bytes == 0)
        return false;

    //Ensure that maxCount does not exceed the destination buffer size with null terminator.
    if (maxCount >= destSize_bytes)
        maxCount = destSize_bytes - sizeof('\0');

    if (0 == maxCount || nullptr == src)
    {
        *dest = '\0';
        return true; //nothing to copy
    }

#ifdef USE_SAFESTR_FUNCS
    //strncpy_s(dest, bufferLen, src, _TRUNCATE);
    return (0 == strncpy_s(dest, destSize_bytes, src, maxCount));
#else
    strncpy(dest, src, maxCount);
    dest[maxCount] = '\0';
    return true;
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Performs a copy by calling either StrNCpy_safeW() or StrNCpy_safeA(), depending on
//  the project TCHAR type (unicode or not).
// -Copies what will fit from src into dest, up to maxCount, or first null terminator, whichever
//  comes first, not exceeding destArrayLen, and ensures that dest is null terminated.
// -Returns false for failure. Will fail if dest is null or destArrayLen is zero.
bool TStrTool::StrNCpy_safeT(TCHAR* dest, size_t destArrayLen, TCHAR const* src, size_t maxCount)
{
#ifdef _UNICODE
    return StrNCpy_safeW(dest, destArrayLen, src, maxCount);
#else
    return StrNCpy_safeA(dest, destArrayLen, src, maxCount);
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimLeft(std::string& s)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    s.erase(s.begin(),
        std::find_if(s.begin(), s.end(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        })
    );
#else
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
#endif
}


//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
void TStrTool::TrimLeft(std::string& s, const unsigned char trimChar, bool trim_isspace)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    s.erase(s.begin(),
        std::find_if(s.begin(), s.end(), [trimChar, trim_isspace](unsigned char ch)
        {
            return trim_isspace ? (!std::isspace(ch) && trimChar != ch) : trimChar != ch;
        })
    );
#else
    std::size_t i = 0;
    std::size_t end = s.size();
    while (i < end && (trim_isspace ? std::isspace(s[i]) || trimChar == s[i] : trimChar == s[i]))
        i++;
    s = s.substr(i);
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimLeft(std::wstring& s)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    s.erase(s.begin(),
        std::find_if(s.begin(), s.end(), [](wchar_t ch)
        {
            return !std::iswspace(ch);
        })
    );
#else
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun(static_cast<int (*)(wchar_t)>(std::iswspace)))));
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
void TStrTool::TrimLeft(std::wstring& s, const wchar_t trimChar, bool trim_iswspace)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    s.erase(s.begin(),
        std::find_if(s.begin(), s.end(), [trimChar, trim_iswspace](wchar_t ch)
        {
            return trim_iswspace ? (!std::iswspace(ch) && trimChar != ch) : trimChar != ch;
        })
    );
#else
    std::size_t i = 0;
    std::size_t end = s.size();
    while (i < end && (trim_iswspace ? std::iswspace(s[i]) || trimChar == s[i] : trimChar == s[i]))
        i++;
    s = s.substr(i);
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimRight(std::string& s)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
        {
            return !std::isspace(ch);
        }).base(), s.end());
#else
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
void TStrTool::TrimRight(std::string& s, unsigned char const trimChar, bool trim_isspace)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    //Note: Can use [trimChar, trim_isspace] in the lambda, or just [=] to get access to
    //trimChar and trim_isspace.
    s.erase(std::find_if(s.rbegin(), s.rend(), [ = ](unsigned char ch)
        {
            return trim_isspace ? (!std::isspace(ch) && trimChar != ch) : trimChar != ch;
        }).base(), s.end());
#else
    while (!s.empty() &&
           (trim_isspace ? std::isspace(s[s.size() - 1]) || trimChar == s[s.size() - 1] : trimChar == s[s.size() - 1]))
    {
        s.erase(s.size() - 1);
    }
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
void TStrTool::TrimRight(std::wstring& s)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch)
        {
            return !std::iswspace(ch);
        }).base(), s.end());
#else
    s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun(static_cast<int (*)(wchar_t)>(std::iswspace)))).base(), s.end());
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
void TStrTool::TrimRight(std::wstring& s, wchar_t const trimChar, bool trim_iswspace)
{
    if (s.empty())
        return;

#if __cplusplus >= 201103L
    //Note: Can use [trimChar, trim_iswspace] in the lambda, or just [=] to get access to
    //trimChar and trim_iswspace.
    s.erase(std::find_if(s.rbegin(), s.rend(), [ = ](wchar_t ch)
        {
            return trim_iswspace ? (!std::iswspace(ch) && trimChar != ch) : trimChar != ch;
        }).base(), s.end());
#else
    size_t lc = s.size() - 1;
    while (!s.empty() &&
           (trim_iswspace ? std::iswspace(s[lc]) || trimChar == s[lc] : trimChar == s[lc]))
    {
        s.erase(lc);
        lc = s.size() - 1;
    }
#endif
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
void TStrTool::Trim(std::string& s)
{
    TrimLeft(s);
    TrimRight(s);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
void TStrTool::Trim(std::string& s, unsigned char const trimChar, bool trim_isspace)
{
    TrimLeft(s, trimChar, trim_isspace);
    TrimRight(s, trimChar, trim_isspace);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
void TStrTool::Trim(std::wstring& s)
{
    TrimLeft(s);
    TrimRight(s);
}
//---------------------------------------------------------------------------
// -Static
// -Trims in place, left and right
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
void TStrTool::Trim(std::wstring& s, wchar_t const trimChar, bool trim_iswspace)
{
    TrimLeft(s, trimChar, trim_iswspace);
    TrimRight(s, trimChar, trim_iswspace);
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::string TStrTool::TrimLeft_Copy(std::string s)
{
    TrimLeft(s);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
std::string TStrTool::TrimLeft_Copy(std::string s, unsigned char const trimChar, bool trim_isspace)
{
    TrimLeft(s, trimChar, trim_isspace);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::wstring TStrTool::TrimLeft_Copy(std::wstring s)
{
    TrimLeft(s);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
std::wstring TStrTool::TrimLeft_Copy(std::wstring s, wchar_t const trimChar, bool trim_iswspace)
{
    TrimLeft(s, trimChar, trim_iswspace);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::string TStrTool::TrimRight_Copy(std::string s)
{
    TrimRight(s);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
std::string TStrTool::TrimRight_Copy(std::string s, unsigned char const trimChar, bool trim_isspace)
{
    TrimRight(s, trimChar, trim_isspace);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::wstring TStrTool::TrimRight_Copy(std::wstring s)
{
    TrimRight(s);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
std::wstring TStrTool::TrimRight_Copy(std::wstring s, wchar_t const trimChar, bool trim_iswspace)
{
    TrimRight(s, trimChar, trim_iswspace);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::string TStrTool::Trim_Copy(std::string s)
{
    Trim(s);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_issspace', if true, will also trim spaces.
std::string TStrTool::Trim_Copy(std::string s, unsigned char const trimChar, bool trim_isspace)
{
    Trim(s, trimChar, trim_isspace);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
std::wstring TStrTool::Trim_Copy(std::wstring s)
{
    Trim(s);
    return s;
}
//---------------------------------------------------------------------------
// -Static
// -Returns a trimmed copy, not affecting original
// -'trimChar' is a character, other than a space, to trim.
// -'trim_iswsspace', if true, will also trim spaces.
std::wstring TStrTool::Trim_Copy(std::wstring s, wchar_t const trimChar, bool trim_iswspace)
{
    Trim(s, trimChar, trim_iswspace);
    return s;
}
//---------------------------------------------------------------------------
int TStrTool::Compare(std::string const& s1, std::string const& s2)
{
    return s1.compare(s2);
}
//---------------------------------------------------------------------------
int TStrTool::Compare(std::wstring const& s1, std::wstring const& s2)
{
    return s1.compare(s2);
}
//---------------------------------------------------------------------------
int TStrTool::CompareIC(std::string const& s1, std::string const& s2)
{
    unsigned char const* us1 = reinterpret_cast<unsigned char const*> (s1.c_str());
    unsigned char const* us2 = reinterpret_cast<unsigned char const*> (s2.c_str());
    int c1, c2;

    if (us1 == us2)
        return 0;

    do
    {
        c1 = std::tolower(*us1++);
        c2 = std::tolower(*us2++);
    } while (c1 && c1 == c2);

    return c1 - c2;
}
//---------------------------------------------------------------------------
int TStrTool::CompareIC(std::wstring const& s1, std::wstring const& s2)
{
    wchar_t const* us1 = s1.c_str();
    wchar_t const* us2 = s2.c_str();
    wchar_t c1, c2;

    if (us1 == us2)
        return 0;

    do
    {
        c1 = std::towlower(*us1++);
        c2 = std::towlower(*us2++);
    } while (c1 && c1 == c2);

    return static_cast<int> (c1) - static_cast<int> (c2);
}
//---------------------------------------------------------------------------}
int32_t TStrTool::StrToInt32(std::string const& str)
{
    try
    {
#if __cplusplus >= 201103L
        long val = std::stol(str);
#else
        long val = std::atol(str.c_str());
#endif

#if LONG_MAX > INT32_MAX
        if (val > std::numeric_limits<int32_t>::max())
            throw std::out_of_range("Value exceeds int32_t range");
#endif
        return static_cast<int32_t>(val);
    }
    catch (const std::invalid_argument& e)
    {
        throw std::invalid_argument("String is not a signed 32-bit int: " + std::string(e.what()));
    }
    catch (const std::out_of_range& e)
    {
        throw std::out_of_range("Out of range: " + std::string(e.what()));
    }
}
//---------------------------------------------------------------------------}
int64_t TStrTool::StrToInt64(std::string const& str)
{
    try
    {
#if __cplusplus >= 201103L
        long long val = std::stoll(str);
#else
        long long val = std::atoll(str.c_str());
#endif

#if LONGLONG_MAX > INT64_MAX
        if (val > std::numeric_limits<int64_t>::max())
            throw std::out_of_range("Value exceeds int64_t range");
#endif
        return static_cast<int64_t>(val);
    }
    catch (const std::invalid_argument& e)
    {
        throw std::invalid_argument("String is not a signed 64-bit int: " + std::string(e.what()));
    }
    catch (const std::out_of_range& e)
    {
        throw std::out_of_range("Out of range: " + std::string(e.what()));
    }
}
//---------------------------------------------------------------------------}
uint32_t TStrTool::StrToUInt32(std::string const& str)
{
    try
    {
#if __cplusplus >= 201103L
        unsigned long val = std::stoul(str);
#else
        char* endP = NULL;
        unsigned long val = std::strtoul(str.c_str(), &endP, 10);
#endif

#if ULONG_MAX > UINT32_MAX
        if (val > std::numeric_limits<uint32_t>::max())
        {   // should not happen on Windows (unsigned long == uint32_t)
            throw std::out_of_range("Value exceeds uint32_t range");
        }
#endif
        return static_cast<uint32_t>(val);
    }
    catch (std::invalid_argument const& e)
    {
        throw std::invalid_argument("String is not an unsigned 32-bit int: " + std::string(e.what()));
    }
    catch (std::out_of_range const& e)
    {
        throw std::out_of_range("Out of range: " + std::string(e.what()));
    }
}
//---------------------------------------------------------------------------}
uint64_t TStrTool::StrToUInt64(std::string const& str)
{
    try
    {
#if __cplusplus >= 201103L
        unsigned long long val = std::stoull(str);
#else
        char* endP = NULL;
        unsigned long long val = std::strtoull(str.c_str(), &endP, 10);
#endif

#if ULONGLONG_MAX > UINT64_MAX
        if (val > std::numeric_limits<uint64_t>::max())
        {   // should not happen on Windows (unsigned long == uint32_t)
            throw std::out_of_range("Value exceeds uint64_t range");
        }
#endif
        return static_cast<uint64_t>(val);
    }
    catch (std::invalid_argument const& e)
    {
        throw std::invalid_argument("String is not an unsigned 64-bit int: " + std::string(e.what()));
    }
    catch (std::out_of_range const& e)
    {
        throw std::out_of_range("Out of range: " + std::string(e.what()));
    }
}
//---------------------------------------------------------------------------
bool TStrTool::ToBool(std::string const& str)
{
    std::string lowerStr = Trim_Copy(ToLower(str));

    if (lowerStr.length() == 0 || "false" == lowerStr || "no" == lowerStr || "0" == lowerStr)
        return false;

    return true;
}
//---------------------------------------------------------------------------
bool TStrTool::ToBool(std::wstring const& str)
{
    std::wstring lowerStr = Trim_Copy(ToLower(str));

    if (lowerStr.length() == 0 || L"false" == lowerStr || L"no" == lowerStr || L"0" == lowerStr)
        return false;

    return true;
}
//---------------------------------------------------------------------------
std::string TStrTool::ToLower(std::string const& str)
{
    if (str.empty())
        return "";

    std::string result = str;
    result.resize(str.size());
#if __cplusplus >= 201103L
    std::transform(str.begin(), str.end(), result.begin(), [](unsigned char c){
            return std::tolower(c);
        });
#else
    std::transform(str.begin(), str.end(), result.begin(), (int (*)(int)) std::tolower);
#endif
    return result;
}
//---------------------------------------------------------------------------
std::wstring TStrTool::ToLower(std::wstring const& str)
{
    if (str.empty())
        return L"";

    std::wstring result = str;
    result.resize(str.size());
#if __cplusplus >= 201103L
    std::transform(str.begin(), str.end(), result.begin(), [](wchar_t c){
            return std::towlower(c);
        });
#else
    std::transform(str.begin(), str.end(), result.begin(), (int (*)(wchar_t)) std::towlower);
#endif
    return result;
}
//---------------------------------------------------------------------------
std::string TStrTool::ToUpper(std::string const& str)
{
    if (str.empty())
        return "";

    std::string result = str;
    result.resize(str.size());
#if __cplusplus >= 201103L
    std::transform(str.begin(), str.end(), result.begin(), [](unsigned char c){
            return std::toupper(c);
        });
#else
    std::transform(str.begin(), str.end(), result.begin(), (int (*)(int)) std::toupper);
#endif
    return result;
}
//---------------------------------------------------------------------------
std::wstring TStrTool::ToUpper(std::wstring const& str)
{
    if (str.empty())
        return L"";

    std::wstring result = str;
    result.resize(str.size());
#if __cplusplus >= 201103L
    std::transform(str.begin(), str.end(), result.begin(), [](wchar_t c){
            return std::towupper(c);
        });
#else
    std::transform(str.begin(), str.end(), result.begin(), (int (*)(wchar_t)) std::towupper);
#endif
    return result;
}
//---------------------------------------------------------------------------
// On success, 'outVal', if not null, will be set to the converted value.
bool TStrTool::TryStrToInt32(std::string const& str, int32_t* outVal)
{
    try
    {
        int32_t val = StrToInt32(str);
        if (nullptr != outVal)
            *outVal = val;
        return true;
    }
    catch (...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
// On success, 'outVal', if not null, will be set to the converted value.
bool TStrTool::TryStrToInt64(std::string const& str, int64_t* outVal)
{
    try
    {
        int64_t val = StrToInt64(str);
        if (nullptr != outVal)
            *outVal = val;
        return true;
    }
    catch (...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
// On success, 'outVal', if not null, will be set to the converted value.
bool TStrTool::TryStrToUInt32(std::string const& str, uint32_t* outVal)
{
    try
    {
        uint32_t val = StrToUInt32(str);
        if (nullptr != outVal)
            *outVal = val;
        return true;
    }
    catch (...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
// On success, 'outVal', if not null, will be set to the converted value.
bool TStrTool::TryStrToUInt64(std::string const& str, uint64_t* outVal)
{
    try
    {
        uint64_t val = StrToUInt64(str);
        if (nullptr != outVal)
            *outVal = val;
        return true;
    }
    catch (...)
    {
        return false;
    }
}
//---------------------------------------------------------------------------
// -Static
// -Fast check for chars considered to be a space, as specified at:
//  https://cplusplus.com/reference/cctype/isspace/
// -Does not factor in locale. If locale is important, see isspace variant in "locale" header.
//  Note that the locale variant is slower, hence the existence of this function.
bool TStrTool::IsSpace(int c)
{
    //if the character is outside the range of "is space" characters, return early
    if (c > 0x20 || c < 0x09)
        return false;

    //see: https://cplusplus.com/reference/cctype/isspace/
    return ' ' == c || //0x20
        '\t' == c || //0x09
        '\n' == c || //0x0a
        '\r' == c || //0x0d
        '\v' == c || //0x0b
        '\f' == c;    //0x0c
}
//---------------------------------------------------------------------------
// -Static
bool TStrTool::IsEmptyOrWhiteSpace(std::string const& s)
{
    if (s.length() == 0)
        return true;

    if (TStrTool::Trim_Copy(s).length() == 0)
        return true;

    return false;
}
//---------------------------------------------------------------------------
// -Static
bool TStrTool::IsEmptyOrWhiteSpace(std::wstring const& s)
{
    if (s.length() == 0)
        return true;

    if (TStrTool::Trim_Copy(s).length() == 0)
        return true;

    return false;
}
//---------------------------------------------------------------------------
// -'path' will be wrapped in double quotes, unless already starts with a double quote.
// -Caller is responsible for ensuring that 'args' is a proper command argument list, or blank.
std::string TStrTool::CombinePathAndArgs(std::string const& path, std::string const& args)
{
    std::string pathAndArgs;

    if (path[0] == '\"')
        pathAndArgs = path;
    else
        pathAndArgs = "\"" + path + "\"";

    if (args.length() > 0)
        pathAndArgs += " " + args;

    return pathAndArgs;
}
//---------------------------------------------------------------------------
// -'path' will be wrapped in double quotes, unless already starts with a double quote.
// -Caller is responsible for ensuring that 'args' is a proper command argument list, or blank.
std::wstring TStrTool::CombinePathAndArgs(std::wstring const& path, std::wstring const& args)
{
    std::wstring pathAndArgs;

    if (path[0] == L'\"')
        pathAndArgs = path;
    else
        pathAndArgs = L"\"" + path + L"\"";

    if (args.length() > 0)
        pathAndArgs += L" " + args;

    return pathAndArgs;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::string> TStrTool::Split(std::string const& str, char const sep)
{
    std::string::size_type pos = 0, prevPos = 0;
    std::vector<std::string> list;

    while((pos = str.find(sep, pos)) != std::string::npos)
    {
        list.push_back(str.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }

    //get last item
    list.push_back(str.substr(prevPos, pos - prevPos));
    return list;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::wstring> TStrTool::Split(std::wstring const& str, wchar_t const sep)
{
    std::wstring::size_type pos = 0, prevPos = 0;
    std::vector<std::wstring> list;

    while((pos = str.find(sep, pos)) != std::wstring::npos)
    {
        list.push_back(str.substr(prevPos, pos - prevPos));
        prevPos = ++pos;
    }

    //get last item
    list.push_back(str.substr(prevPos, pos - prevPos));
    return list;
}
//---------------------------------------------------------------------------
// -Static
// -Inserts a delim character wherever there is a delim character found.
std::string TStrTool::DelimStr_Escape(std::string const& strIn, char const delim)
{
    if (strIn.length() == 0)
        return "";

    size_t outBuffSize = (strIn.length() * 2) + sizeof('\0');
    char* outBuff = new char[outBuffSize];
    std::unique_ptr<char[]> auto_outBuff(outBuff);
    char const* walkerIn = strIn.c_str();
    char* walkerOut = outBuff;

    //copy in string to out string while inserting a delim wherever a delim is found
    while (*walkerIn)
    {
        if (delim == *walkerIn)
            *walkerOut++ = delim;
        *walkerOut++ = *walkerIn++;
    }

    *walkerOut = '\0';
    std::string strOut = outBuff;
    return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::wstring TStrTool::DelimStr_Escape(std::wstring const& strIn, wchar_t delim)
{
    if (strIn.length() == 0)
        return L"";

    size_t outBuffSize = (strIn.length() * 2) + sizeof('\0');
    wchar_t* outBuff = new wchar_t[outBuffSize];
    std::unique_ptr<wchar_t[]> auto_outBuff(outBuff);
    wchar_t const* walkerIn = strIn.c_str();
    wchar_t* walkerOut = outBuff;

    //copy in string to out string while inserting a delim wherever a delim is found
    while (*walkerIn)
    {
        if (delim == *walkerIn)
            *walkerOut++ = delim;
        *walkerOut++ = *walkerIn++;
    }

    *walkerOut = L'\0';
    std::wstring strOut = outBuff;
    return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::string TStrTool::DelimStr_UnEscape(std::string const& strIn, char delim)
{
    if (strIn.length() == 0)
        return "";

    size_t outBuffSize = strIn.length() + sizeof('\0');
    char* outBuff = new char[outBuffSize];
    std::unique_ptr<char[]> auto_outBuff(outBuff);
    char const* walkerIn = strIn.c_str();
    char* walkerOut = outBuff;

    //copy in string to out string while skipping escaped delims wherever a delim is found
    while (*walkerIn)
    {
        if (delim == *walkerIn && delim == *(walkerIn + 1))
            walkerIn++; //skip a delim
        *walkerOut++ = *walkerIn++;
    }

    *walkerOut = '\0';
    std::string strOut = outBuff;
    return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::wstring TStrTool::DelimStr_UnEscape(std::wstring const& strIn, wchar_t delim)
{
    if (strIn.length() == 0)
        return L"";

    size_t outBuffSize = strIn.length() + sizeof('\0');
    wchar_t* outBuff = new wchar_t[outBuffSize];
    std::unique_ptr<wchar_t[]> auto_outBuff(outBuff);
    wchar_t const* walkerIn = strIn.c_str();
    wchar_t* walkerOut = outBuff;

    //copy in string to out string while inserting a delim wherever a delim is found
    while (*walkerIn)
    {
        if (delim == *walkerIn && delim == *(walkerIn + 1))
            walkerIn++; //skip a delim
        *walkerOut++ = *walkerIn++;
    }

    *walkerOut = L'\0';
    std::wstring strOut = outBuff;
    return strOut;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::string> TStrTool::CombineLists_Unique(std::vector<std::string> const& list1,
    std::vector<std::string> const& list2, bool ignoreBlank, bool ignoreCase)
{
    std::vector<std::string> dest;

    for (size_t i = 0; i < list1.size() || i < list2.size(); i++)
    {
        char const* val1P = i < list1.size() ? list1[i].c_str() : "";
        char const* val2P = i < list2.size() ? list2[i].c_str() : "";
        bool addVal1 = (!ignoreBlank || *val1P != '\0') && i < list1.size();
        bool addVal2 = (!ignoreBlank || *val2P != '\0') && i < list2.size();

        if (!addVal1 && !addVal2)
            continue;

        //if val1 and val2 are still eligible, and if val1 and val2 are the same, don't bother with val2
        if (addVal1 && addVal2)
        {
            if ((ignoreCase && strcmpI(val1P, val2P) == 0) || (!ignoreCase && strcmp(val1P, val2P) == 0))
                addVal2 = false;
        }

        //check new list for existing items
        for (size_t j = 0; j < dest.size(); j++)
        {
            char const* destP = dest[j].c_str();

            if (addVal1)
            {
                if ((ignoreCase && strcmpI(destP, val1P) == 0) || (!ignoreCase && strcmp(destP, val1P) == 0))
                    addVal1 = false;
            }

            if (addVal2)
            {
                if ((ignoreCase && strcmpI(destP, val2P) == 0) || (!ignoreCase && strcmp(destP, val2P) == 0))
                    addVal2 = false;
            }

            if (!addVal1 && !addVal2)
                break; //stop searching if it has been determined to not add either one
        }

        if (addVal1)
            dest.push_back(val1P);

        if (addVal2)
            dest.push_back(val2P);
    }

    return dest;
}
//---------------------------------------------------------------------------
// -Static
std::vector<std::wstring> TStrTool::CombineLists_Unique(std::vector<std::wstring> const& list1,
    std::vector<std::wstring> const& list2, bool ignoreBlank, bool ignoreCase)
{
    std::vector<std::wstring> dest;

    for (size_t i = 0; i < list1.size() || i < list2.size(); i++)
    {
        wchar_t const* val1P = i < list1.size() ? list1[i].c_str() : L"";
        wchar_t const* val2P = i < list2.size() ? list2[i].c_str() : L"";
        bool addVal1 = (!ignoreBlank || *val1P != L'\0') && i < list1.size();
        bool addVal2 = (!ignoreBlank || *val2P != L'\0') && i < list2.size();

        if (!addVal1 && !addVal2)
            continue;

        //if val1 and val2 are still eligible, and if val1 and val2 are the same, don't bother with val2
        if (addVal1 && addVal2)
        {
            if ((ignoreCase && _wcsicmp(val1P, val2P) == 0) || (!ignoreCase && wcscmp(val1P, val2P) == 0))
                addVal2 = false;
        }

        //check new list for existing items
        for (size_t j = 0; j < dest.size(); j++)
        {
            wchar_t const* destP = dest[j].c_str();

            if (addVal1)
            {
                if ((ignoreCase && _wcsicmp(destP, val1P) == 0) || (!ignoreCase && wcscmp(destP, val1P) == 0))
                    addVal1 = false;
            }

            if (addVal2)
            {
                if ((ignoreCase && _wcsicmp(destP, val2P) == 0) || (!ignoreCase && wcscmp(destP, val2P) == 0))
                    addVal2 = false;
            }

            if (!addVal1 && !addVal2)
                break; //stop searching if it has been determined to not add either one (i.e. they already exist in dest)
        }

        if (addVal1)
            dest.push_back(val1P);

        if (addVal2)
            dest.push_back(val2P);
    }

    return dest;
}
//---------------------------------------------------------------------------
// - Static
// - Returns a copy of str, with replacements of all found "find" values with "replaceWith" values.
std::string TStrTool::ReplaceAll(std::string const& str, std::string const& find, std::string const& replaceWith)
{
    std::string resultStr = str;

    if(find.empty()) //no value provided
        return resultStr;

    size_t startPos = 0, foundPos = 0;

    while((foundPos = resultStr.find(find, startPos)) != std::string::npos)
    {
        resultStr.replace(foundPos, find.length(), replaceWith);
        startPos = foundPos + replaceWith.length(); //skip past what was just replaced
    }

    return resultStr;
}
//---------------------------------------------------------------------------
// - Static
// - Returns a copy of str, with replacements of all found "find" values with "replaceWith" values.
std::string TStrTool::ReplaceAll(std::string const& str, char find, char replaceWith)
{
    return ReplaceAll(str, std::string(1, find), std::string(1, replaceWith));
}
//---------------------------------------------------------------------------
// - Static
// - Returns a copy of str, with replacements of all found "find" values with "replaceWith" values.
std::wstring TStrTool::ReplaceAll(std::wstring const& str, std::wstring const& find, std::wstring const& replaceWith)
{
    std::wstring resultStr = str;

    if(find.empty()) //no value provided
        return resultStr;

    size_t startPos = 0, foundPos = 0;

    while((foundPos = resultStr.find(find, startPos)) != std::wstring::npos)
    {
        resultStr.replace(foundPos, find.length(), replaceWith);
        startPos = foundPos + replaceWith.length(); //skip past what was just replaced
    }

    return resultStr;
}
//---------------------------------------------------------------------------
// - Static
// - Returns a copy of str, with replacements of all found "find" values with "replaceWith" values.
std::wstring TStrTool::ReplaceAll(std::wstring const& str, wchar_t find, wchar_t replaceWith)
{
    return ReplaceAll(str, std::wstring(1, find), std::wstring(1, replaceWith));
}
//---------------------------------------------------------------------------
// -Static
// -Converts an uppercase or lowercase hex character to a byte.
// -Caller must join the low and high of a hex pair with a bit shift. ( "b = (high << 4) | low;" )
bool TStrTool::HexSingleToByte(char const hexSingle, BYTE* out)
{
    if (hexSingle <= '9' && hexSingle >= '0')
        *out = static_cast<BYTE>(hexSingle - '0');
    else if (hexSingle <= 'F' && hexSingle >= 'A')
        *out = static_cast<BYTE>(hexSingle - 'A' + 10);
    else if (hexSingle <= 'f' && hexSingle >= 'a')
        *out = static_cast<BYTE>(hexSingle - 'a' + 10);
    else
        return false; //invalid hex character

    return true;
}
//---------------------------------------------------------------------------
// -Static
// -Converts an uppercase or lowercase hex character to a byte.
// -Caller must join the low and high of a hex pair with a bit shift. ( "b = (high << 4) | low;" )
// -Returns less than zero for invalid hex characters.
int TStrTool::HexSingleToByte(char const hexSingle)
{
    if (hexSingle <= '9' && hexSingle >= '0')
        return hexSingle - '0';

    if (hexSingle <= 'F' && hexSingle >= 'A')
        return hexSingle - 'A' + 10;

    if (hexSingle <= 'f' && hexSingle >= 'a')
        return hexSingle - 'a' + 10;

    return -1; //invalid char
}
//---------------------------------------------------------------------------
// -Static
std::string TStrTool::EncodeStrToBase16Hex(std::string const& strUtf8, bool upperCase)
{
    size_t length = strUtf8.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase16Hex(reinterpret_cast<BYTE const*>(strUtf8.c_str()), length, upperCase);
}
//---------------------------------------------------------------------------
// -Static
// -Set 'length' to zero to use the length of 'strUtf8'. Otherwise, length will be used, allowing
//  for just a portion of the string to be converted.
std::string TStrTool::EncodeStrToBase16Hex(std::string const& strUtf8, size_t length, bool upperCase)
{
    if (0 == length || length > strUtf8.length())
        length = strUtf8.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase16Hex(reinterpret_cast<BYTE const*>(strUtf8.c_str()), length, upperCase);
}
//---------------------------------------------------------------------------
// -Static
std::string TStrTool::EncodeStrToBase16Hex(std::wstring const& strW, bool upperCase)
{
    size_t length = strW.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase16Hex(reinterpret_cast<BYTE const*>(strW.c_str()), length * sizeof(wchar_t), upperCase);
}
//---------------------------------------------------------------------------
// -Static
// -Set 'length' to zero to use the length of 'strW'. Otherwise, length will be used, allowing
//  for just a portion of the string to be converted.
std::string TStrTool::EncodeStrToBase16Hex(std::wstring const& strW, size_t length, bool upperCase)
{
    if (0 == length || length > strW.length())
        length = strW.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase16Hex(reinterpret_cast<BYTE const*>(strW.c_str()), length * sizeof(wchar_t), upperCase);
}
//---------------------------------------------------------------------------
// -Static
// -Converts bytes, up to bytesLen, to a hex string.
std::string TStrTool::EncodeToBase16Hex(BYTE const* bytes, size_t bytesLen, bool upperCase)
{
    static const char hexValuesUpper[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    static const char hexValuesLower[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    const char* hexVals = upperCase ? hexValuesUpper : hexValuesLower;

    if (nullptr == bytes || 0 == bytesLen)
        return "";

    BYTE const* bytesP = bytes;
    char* hexBuff = new char[bytesLen * 2 + sizeof('\0')];
    std::unique_ptr<char[]> auto_hexBuff(hexBuff);
    char* hexBuffP = hexBuff;

    for (size_t i = 0; i < bytesLen; i++)
    {
        //encryption can be done here, for other variants of this function

        //convert character to hex by extracting left and right values
        *hexBuffP++ = hexVals[((*bytesP >> 4) & 0xF)];//left
        *hexBuffP++ = hexVals[(*bytesP++) & 0x0F];//right
    }

    *hexBuffP = '\0';
    std::string hexResult = hexBuff;
    return hexResult;
}
//---------------------------------------------------------------------------
// -Static
// -Assumes that 'inHex' contains utf8 text.
std::string TStrTool::DecodeBase16HexToStrA(std::string const& inHex)
{
    if (inHex.length() == 0)
        return "";

    size_t buffSize = (inHex.length() / 2);
    BYTE* buff = new BYTE[buffSize + sizeof('\0')];
    std::unique_ptr<BYTE[]> auto_buff(buff);
    int64_t bytesWritten;

    buff[buffSize] = '\0'; //safety - not needed, but can help with debugging
    DecodeBase16HexToBytes(inHex, buff, buffSize, &bytesWritten);

    if (0 == bytesWritten)
    {
        return "";
    }

    std::string resultStr(reinterpret_cast<char*>(buff), static_cast<size_t>(bytesWritten));
    return resultStr;
}
//---------------------------------------------------------------------------
// -Static
// -Assumes that 'inHex' contains wide char text.
std::wstring TStrTool::DecodeBase16HexToStrW(std::string const& inHex)
{
    if (inHex.length() == 0)
        return L"";

    size_t buffSize = (inHex.length() / 2);
    BYTE* buff = new BYTE[buffSize + sizeof('\0')];
    std::unique_ptr<BYTE[]> auto_buff(buff);
    int64_t bytesWritten;

    buff[buffSize] = '\0'; //safety - not needed, but can help with debugging
    DecodeBase16HexToBytes(inHex, buff, buffSize, &bytesWritten);

    if (0 == bytesWritten)
    {
        return L"";
    }

    std::wstring resultStr(reinterpret_cast<wchar_t*>(buff), static_cast<size_t>(bytesWritten) / sizeof(wchar_t));
    return resultStr;
}
//---------------------------------------------------------------------------
// -Static
// -Parameters:
//      1. "inHex": Content can be mixed case and can optionally start with "0x" or "0X" and can
//         optionally be wrapped in double quotes. Input is expected to be comprised of paired
//         characters with each pair representing a single byte value
//         (e.g "CF4" will produce an error - instead, pass in "0CF4").
//         Spaces (isspace) are ignored between pairs, but each pair must be consecutive
//         (e.g. "FF AA" = ok, "F F A A" = error).
//         Examples of supported hex formats:
//         "0xFa05", "FbAa", ""FAFF"", ""0XFAFb"", "0xFA 0x56", "F8 F9 AF", "01 50 0x47"
//      2. "outBuff": If null or "buffSize" is 0, on success, the needed buffer size will be
//         estimated and returned. The estimation may be bigger than actually needed if the input
//         hex contains spaces, "0x" markers, newlines, etc.
//      3. "outBytesWritten": If not null and "outBuff" has size, this is set to the count of
//         bytes written to "outBuff". This value is populated if any bytes were written, even
//         if the function fails. For example, "CF4" will result in one byte ("CF") being written
//         and an error returned pointing to the "4" as the problem.
//
// -On success, returns greater than or equal to zero, with the following considerations:
//      1. If content of "inHex" fits within "outBuff", the count of bytes written is returned.
//         (if "outBytesWritten" is not null, its value will match the return value)
//      2. If "buffSize" is not big enough, the estimated needed buffer size is returned.
//         (if "outBytesWritten" is not null, its value will NOT match the return value)
//         (this behavior allows for partially extracting header data, for example, without having
//          to entirely process the contents of "inHex")
//      3. The return value can be compared with "outBytesWritten". If they match, all content
//         was processed.
//
// -On failure, returns less than zero, with the following considerations:
//      1. The bitwise complement of where the error occurred is returned.
//         (convert back to an index with the "~" operator)
//      2. If the return value is 0 (after converting back to an index), then either the "inHex" has
//         an issue at the very beginning, or something else went wrong.
int64_t TStrTool::DecodeBase16HexToBytes(std::string const& inHex, BYTE* outBuff, size_t buffSize, int64_t* outBytesWritten)
{
    size_t srcLen = inHex.length();

    if (nullptr != outBytesWritten)
        *outBytesWritten = 0;

    //get the hex string length
    int64_t hexLen = static_cast<int64_t>(srcLen);

    if (hexLen < 0)
    {
        //this should never happen, but defend against an input length exceeding a signed long long
        //ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: inHex length is longer than supported input size.\n", codeSectionStr);
        return ~0LL;
    }

    size_t hexStartOffset = 0;

    //defend against double quotes - subtract from the length
    for (size_t i = 0; i < srcLen && inHex[i] == '"'; i++)
    {
        hexLen--;
        hexStartOffset++;
    }

    //check right side double quote, ignoring first character on the left side (already checked above)
    for (size_t i = srcLen - 1; i > 0 && inHex[i] == '"'; i--)
        hexLen--;

    //defend against a start 0x or 0X - subtract from length if exists
    if (hexLen >= 2 &&
        (inHex[hexStartOffset + 1] == 'x' || inHex[hexStartOffset + 1] == 'X'))
    {
        hexLen -= 2;
        hexStartOffset += 2;
    }

    if (0 == hexLen)
        return 0; //nothing to do

    if (hexLen < 2)
    {
        //ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: inHex length is invalid.\n", codeSectionStr);
        return ~(static_cast<int64_t>(0));
    }

    bool lengthDivisibleBy2 = ((hexLen % 2) == 0);
    size_t requiredSize = static_cast<size_t>(hexLen / 2);

    if (!lengthDivisibleBy2)
        requiredSize++;

    //if outBuff is NULL, set the size that it needs to be and we are done
    if (nullptr == outBuff || 0 == buffSize)
        return static_cast<int64_t>(requiredSize);

    //Don't overrun output buffer. Don't error if buffer is insufficient size because the caller
    //may just need the header, for example, when the data is binary.
    size_t maxFill = buffSize;

    BYTE* outBuffP = outBuff;
    char const* inHexStartP = inHex.c_str();
    char const* inHexP = inHexStartP + hexStartOffset;
    char charHigh, charLow;
    int intHigh, intLow;
    int64_t filledCount = 0;

    while (*inHexP && static_cast<size_t>(filledCount) < maxFill)
    {
        charHigh = *inHexP;

        //ignore double quotes and spaces, newlines, etc.
        if (charHigh == '"' || IsSpace(charHigh))
        {
            inHexP++;
            continue;
        }

        charLow = *(inHexP + 1);

        //ignore 0x/X prefixes, in case data is of the format "0xFF 0xFA 0xAA"
        if ('x' == charLow || 'X' == charLow)
        {
            inHexP += 2;
            continue;
        }

        intHigh = HexSingleToByte(charHigh);
        intLow = HexSingleToByte(charLow);

        //abort if invalid hex pair detected (intLow could be pointint to null terminator if source isn't divisible by 2)
        if (intLow < 0 || intHigh < 0)
        {
            if (nullptr != outBytesWritten)
                *outBytesWritten = filledCount;
            //return bitwise complement of hex pair index where error occurred
            return ~(static_cast<int64_t>(inHexP - inHexStartP));
        }

        //combine high and low parts into a byte value
        *outBuffP++ = static_cast<BYTE>((intHigh << 4) | intLow);

        filledCount++;
        inHexP += 2;
    }

    if (nullptr != outBytesWritten)
        *outBytesWritten = filledCount;

    //if there is more in the input hex, and the filled count is less than the estimated required size
    if (*inHexP && static_cast<size_t>(filledCount) < requiredSize)
    {
        //output buffer is not big enough - return the size that the buffer needs to be
        return static_cast<int64_t>(requiredSize);
    }

    return filledCount;
}
//---------------------------------------------------------------------------
// - Static
bool TStrTool::IsValidBase64(char const* str, bool isUrlSafe)
{
    bool foundEqual = false;
    char const* b64Chars = isUrlSafe ? "-_" : "+/";
    char const* walker = str;

    if (nullptr == str || *str == '\0')
        return false;

    while(*walker)
    {
        if(*walker == '=')
        {
            foundEqual = true;
        }
        else if(isdigit(*walker) ||
                isalpha(*walker) ||
                (strchr(b64Chars, *walker) != nullptr))
        {
            if(foundEqual)
                return false;
        }
        else
        {
            return false;
        }

        walker++;
    }

    return true;
}
//---------------------------------------------------------------------------
// - Static, overload
bool TStrTool::IsValidBase64(std::string const& str, bool isUrlSafe)
{
    if (str.length() == 0)
        return false;

    return IsValidBase64(str.c_str(), isUrlSafe);
}
//---------------------------------------------------------------------------
// -Static
// -Set 'length' to zero to use the length of 'strUtf8'. Otherwise, length will be used, allowing
//  for just a portion of the string to be converted.
std::string TStrTool::EncodeStrToBase64Str(std::string const& strUtf8, bool makeWebFriendly)
{
    size_t length = strUtf8.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase64Str_Native(reinterpret_cast<BYTE const*>(strUtf8.c_str()), length, makeWebFriendly);
}
//---------------------------------------------------------------------------
// -Static
// -Set 'length' to zero to use the length of 'strUtf8'. Otherwise, length will be used, allowing
//  for just a portion of the string to be converted.
std::string TStrTool::EncodeStrToBase64Str(std::string const& strUtf8, size_t length, bool makeWebFriendly)
{
    if (0 == length || length > strUtf8.length())
        length = strUtf8.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase64Str_Native(reinterpret_cast<BYTE const*>(strUtf8.c_str()), length, makeWebFriendly);
}
//---------------------------------------------------------------------------
// -Static
std::string TStrTool::EncodeStrToBase64Str(std::wstring const& strW, bool makeWebFriendly)
{
    size_t length = strW.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase64Str_Native(reinterpret_cast<BYTE const*>(strW.c_str()), length * sizeof(wchar_t), makeWebFriendly);
}
//---------------------------------------------------------------------------
// -Static
// -Set 'length' to zero to use the length of 'strW'. Otherwise, length will be used, allowing
//  for just a portion of the string to be converted.
std::string TStrTool::EncodeStrToBase64Str(std::wstring const& strW, size_t length, bool makeWebFriendly)
{
    if (0 == length || length > strW.length())
        length = strW.length();

    if (0 == length)
        return ""; //nothing to do

    return EncodeToBase64Str_Native(reinterpret_cast<BYTE const*>(strW.c_str()), length * sizeof(wchar_t), makeWebFriendly);
}
//---------------------------------------------------------------------------
//-Static
//-See: http://www.zedwood.com/article/cpp-convert-to-base64-function
//-Better optimized than other routines that use Windows function
std::string TStrTool::EncodeToBase64Str_Native(BYTE const* bytes, size_t bytesLen, bool makeWebFriendly)
{
    static unsigned char const* base64_chars_std =
        reinterpret_cast<unsigned char const*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    static unsigned char const* base64_chars_web =
        reinterpret_cast<unsigned char const*>("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_");
    unsigned char const* base64_chars = makeWebFriendly ? base64_chars_web : base64_chars_std;
    size_t i = 0, ix = 0, leng = bytesLen;
//  std::stringstream q;
//
//  for(i=0,ix=leng - leng%3; i<ix; i+=3)
//  {
//      q<< base64_chars[ (bytes[i] & 0xfc) >> 2 ];
//      q<< base64_chars[ ((bytes[i] & 0x03) << 4) + ((bytes[i+1] & 0xf0) >> 4)  ];
//      q<< base64_chars[ ((bytes[i+1] & 0x0f) << 2) + ((bytes[i+2] & 0xc0) >> 6)  ];
//      q<< base64_chars[ bytes[i+2] & 0x3f ];
//  }
//
//  if (ix<leng)
//  {
//      q<< base64_chars[ (bytes[ix] & 0xfc) >> 2 ];
//      q<< base64_chars[ ((bytes[ix] & 0x03) << 4) + (ix+1<leng ? (bytes[ix+1] & 0xf0) >> 4 : 0)];
//      q<< (ix+1<leng ? base64_chars[ ((bytes[ix+1] & 0x0f) << 2) ] : '=');
//      q<< '=';
//  }
//  return q.str().c_str();

    //ASW - no longer using a stream - optimized for speed

    //get needed buffer size
//  size_t bufferSize = 0;
//
//  for(i=0,ix=leng - leng%3; i<ix; i+=3)
//      bufferSize+=4;
//
//  if (ix<leng)
//      bufferSize+=4;

    size_t bufferSize = (((4ULL * leng / 3ULL) + 3ULL) & ~3ULL) + sizeof('\0');
    //bufferSize += sizeof('\0');

    //allocate exact size of buffer and populate with base 64
    unsigned char* buffer = new unsigned char[bufferSize];
    std::unique_ptr<unsigned char[]> auto_buffer(buffer);
    unsigned char* walker = buffer;

    for(i = 0, ix = leng - leng % 3; i<ix; i += 3)
    {
        *walker++ = base64_chars[ (bytes[i] & 0xfc) >> 2 ];
        *walker++ = base64_chars[ ((bytes[i] & 0x03) << 4) + ((bytes[i + 1] & 0xf0) >> 4)  ];
        *walker++ = base64_chars[ ((bytes[i + 1] & 0x0f) << 2) + ((bytes[i + 2] & 0xc0) >> 6)  ];
        *walker++ = base64_chars[ bytes[i + 2] & 0x3f ];
    }

    if (ix<leng)
    {
        *walker++ = base64_chars[ (bytes[ix] & 0xfc) >> 2 ];
        *walker++ = base64_chars[ ((bytes[ix] & 0x03) << 4) + (ix + 1<leng ? (bytes[ix + 1] & 0xf0) >> 4 : 0)];
        *walker++ = (ix + 1<leng ? base64_chars[ ((bytes[ix + 1] & 0x0f) << 2) ] : '=');
        *walker++ = '=';
    }

    *walker = '\0';

    std::string result = reinterpret_cast<char*>(buffer);
    return result;
}
//---------------------------------------------------------------------------
// -Static
// -Assumes that 'inB64' contains utf8 text data.
std::string TStrTool::DecodeBase64ToStrA(std::string const& inB64)
{
    if (inB64.length() == 0)
        return "";

    //get the size of the buffer needed to stor the decoded data
    size_t buffSize = 0;
    int decodeRes = DecodeBase64ToBytes_Native(inB64.c_str(), nullptr, &buffSize);

    if (decodeRes < 0)
        return "";

    //now that we know the buffer size, allocate and decode
    BYTE* buff = new BYTE[buffSize + sizeof('\0')];
    std::unique_ptr<BYTE[]> auto_buff(buff);

    buff[buffSize] = '\0'; //safety - not needed, but can help with debugging

    int bytesWritten = DecodeBase64ToBytes_Native(inB64.c_str(), buff, &buffSize);

    if (bytesWritten <= 0)
    {
        return "";
    }

    std::string resultStr(reinterpret_cast<char*>(buff), static_cast<size_t>(bytesWritten));
    return resultStr;
}
//---------------------------------------------------------------------------
// -Static
// -Assumes that 'inB64' contains wide char text data.
std::wstring TStrTool::DecodeBase64ToStrW(std::string const& inB64)
{
    if (inB64.length() == 0)
        return L"";

    //get the size of the buffer needed to stor the decoded data
    size_t buffSize = 0;
    int decodeRes = DecodeBase64ToBytes_Native(inB64.c_str(), nullptr, &buffSize);

    if (decodeRes < 0)
        return L"";

    //now that we know the buffer size, allocate and decode
    BYTE* buff = new BYTE[buffSize + sizeof('\0')];
    std::unique_ptr<BYTE[]> auto_buff(buff);

    buff[buffSize] = '\0'; //safety - not needed, but can help with debugging

    int bytesWritten = DecodeBase64ToBytes_Native(inB64.c_str(), buff, &buffSize);

    if (bytesWritten <= 0)
    {
        return L"";
    }

    std::wstring resultStr(reinterpret_cast<wchar_t*>(buff), static_cast<size_t>(bytesWritten) / sizeof(wchar_t));
    return resultStr;
}
//---------------------------------------------------------------------------
// -Pass null for "destBytes" to calculate the needed buffer size, which, on success, will be
//	written to "destBytesSize" and zero will be returned.
// -If "destBytes" is not null, on success, returns the number of bytes written. If "destBytes" is
//	not big enough, then only what can fit will be written.
// -Returns less than zero for failure.
int TStrTool::DecodeBase64ToBytes_Native(char const* src, BYTE* destBytes, size_t* destBytesSize)
{
// #pragma warning(push , 0)
#if __cplusplus >= 201103L
    constexpr BYTE n1 = static_cast<BYTE>(-1);
#else
    static const BYTE n1 = static_cast<BYTE>(-1);
#endif
    static BYTE const base64_reverse[] = {
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, 62, n1, n1, n1, 63, 52, 53,
        54, 55, 56, 57, 58, 59, 60, 61, n1, n1,
        n1, n1, n1, n1, n1,  0,  1,  2,  3,  4,
        5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, n1, n1, n1, n1, n1, n1, 26, 27, 28,
        29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
        49, 50, 51, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1, n1, n1, n1, n1,
        n1, n1, n1, n1, n1, n1 };
// #pragma warning(pop)
    size_t srcLen = (nullptr == src ? 0 : strlen(src));

//  std::stringstream q;
//  if ((leng%4)==0)
//  {
//      leng = s[leng-1]=='=' ? leng-1 : leng;
//      leng = s[leng-1]=='=' ? leng-1 : leng;
//
//      for(i=0,ix=leng-(leng%4); i<ix; i+=4)
//      {
//          q<< (char)((base64_reverse[ s[i] ] << 2)           + ((base64_reverse[ s[i+1] ] & 0x30) >> 4));
//          q<< (char)(((base64_reverse[ s[i+1] ] & 0xf) << 4) + ((base64_reverse[ s[i+2] ] & 0x3c) >> 2));
//          q<< (char)(((base64_reverse[ s[i+2] ] & 0x3) << 6) + base64_reverse[ s[i+3] ]);
//      }
//      if (ix<leng)
//      {
//          q<< (char)( (base64_reverse[ s[ix] ] << 2)          + (ix+1<leng ? (base64_reverse[ s[ix+1] ] & 0x30) >> 4 : 0)   );
//          q<< (char)( ix+1<leng ? ((base64_reverse[ s[ix+1] ] & 0xf) << 4) + (ix+2<leng ? (base64_reverse[ s[ix+2] ] & 0x3c) >> 2 :0 ) : '\0' );
//          q<< (char)( ix+2<leng ? (base64_reverse[ s[ix+2] ] & 0x3) << 6  : '\0' );
//      }
//  }
//    return q.str();

    //ASW - removed stream - optimized for speed

    if (nullptr == destBytesSize)
    {
        //ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: destBytesSize is null.\n", codeSectionStr);
        return -1;
    }

    if (nullptr == src)
    {
        if (nullptr == destBytes)
            *destBytesSize = 0;
        //ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: src is null.\n", codeSectionStr);
        return -2;
    }

    if (0 == srcLen)
    {
        if (nullptr == destBytes)
            *destBytesSize = 0;
        return 0; //nothing to do
    }

    if (srcLen < 2)
    {
        if (nullptr == destBytes)
            *destBytesSize = 0;
        //ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: src is invalid.\n", codeSectionStr);
        return -3;
    }

    size_t pad1 = srcLen % 4 || src[srcLen - 1] == '=';
    size_t pad2 = pad1 && (srcLen % 4 > 2 || src[srcLen - 2] != '=');
    size_t const last = (srcLen - pad1) / 4 << 2;
    size_t requiredSize = (last / 4 * 3) + pad1 + pad2;

//      for(i=0,ix=leng-(leng%4); i<ix; i+=4)
//          requiredSize += 3;
//
//      if (ix<leng)
//      {
//          requiredSize++;
//          requiredSize += (ix+1 < leng ? 1 : 0);
//          requiredSize += (ix+2 < leng ? 1 : 0);
//      }

    //if destBytes is NULL, set the size that it needs to be and we are done
    if (nullptr == destBytes)
    {
        *destBytesSize = requiredSize;
        return 0;
    }

    //Don't overrun output buffer. Don't error if buffer is insufficient size because the caller
    //may just need the header, for example, when the data is binary.
    size_t maxFill = *destBytesSize < requiredSize ? *destBytesSize : requiredSize;

    size_t i = 0, ix = 0, leng = srcLen;
    BYTE* walker = destBytes;
    size_t filledCount = 0;

    leng = src[leng - 1] == '=' ? leng - 1 : leng;
    leng = src[leng - 1] == '=' ? leng - 1 : leng;

    for (i = 0, ix = leng - (leng % 4); i < ix; i += 4)
    {
        //if there is room for the next triple, get at once, for speed
        if (filledCount + 3 <= maxFill)
        {
            *walker++ = static_cast<BYTE>((base64_reverse[static_cast<size_t>(src[i])] << 2) +
                ((base64_reverse[static_cast<size_t>(src[i + 1])] & 0x30) >> 4));
            *walker++ = static_cast<BYTE>(((base64_reverse[static_cast<size_t>(src[i + 1])] & 0xf) << 4) +
                ((base64_reverse[static_cast<size_t>(src[i + 2])] & 0x3c) >> 2));
            *walker++ = static_cast<BYTE>(((base64_reverse[static_cast<size_t>(src[i + 2])] & 0x3) << 6) +
                base64_reverse[static_cast<size_t>(src[i + 3])]);
            filledCount += 3;
        }
        else
        {
            //destination buffer is insufficient size for a triple - top off
            if (filledCount < maxFill)
            {
                *walker++ = static_cast<BYTE>((base64_reverse[static_cast<size_t>(src[i])] << 2) +
                    ((base64_reverse[static_cast<size_t>(src[i + 1])] & 0x30) >> 4));
                filledCount++;
            }

            if (filledCount < maxFill)
            {
                *walker++ = static_cast<BYTE>(((base64_reverse[static_cast<size_t>(src[i + 1])] & 0xf) << 4) +
                    ((base64_reverse[static_cast<size_t>(src[i + 2])] & 0x3c) >> 2));
                filledCount++;
            }

            break; //destination buffer full, stop reading
        }
    }

    //get remainder, if any, and if destination buffer is big enough
    if (ix < leng && filledCount < maxFill)
    {
        *walker++ = static_cast<BYTE>((base64_reverse[static_cast<size_t>(src[ix])] << 2) +
            (ix + 1 < leng ? (base64_reverse[static_cast<size_t>(src[ix + 1])] & 0x30) >> 4 : 0));
        filledCount++;

        if(filledCount < maxFill)
        {
            *walker++ = static_cast<BYTE>(ix + 1 < leng ?
                    ((base64_reverse[static_cast<size_t>(src[ix + 1])] & 0xf) << 4) +
                    (ix + 2 < leng ? (base64_reverse[static_cast<size_t>(src[ix + 2])] & 0x3c) >> 2 :0 ) :
                    '\0' );
            filledCount++;
        }

        if(filledCount < maxFill)
        {
            *walker++ =
                static_cast<BYTE>(ix + 2 < leng ? (base64_reverse[static_cast<size_t>(src[ix + 2])] & 0x3) << 6 : '\0');
            filledCount++;
        }
    }

    return static_cast<int>(filledCount);
}
//---------------------------------------------------------------------------
// -Static
// -Splits the host and path portion from a URL. If URL has not path (just a host) then
//  pathUtf8 will be set to "/".
bool TStrTool::URL_Split(std::string const& url, std::string* hostUtf8, std::string* pathUtf8)
{
    std::string const httpPrefix = "http://";
    std::string const httpsPrefix = "https://";

    if (nullptr != hostUtf8)
        *hostUtf8 = "";

    if (nullptr != pathUtf8)
        *pathUtf8 = "";

    if (url.length() == 0)
        return false;

    char const* urlStart = url.c_str();
    char const* walker = urlStart;

    //skip http/s prefix, if there is one
#if defined(_MSC_VER) || (defined(__clang__) && __clang_major__ >= 15) // Win64x
    if (_strnicmp(walker, httpPrefix.c_str(), httpPrefix.length()) == 0)
        walker += httpPrefix.length();
    else if (_strnicmp(walker, httpsPrefix.c_str(), httpsPrefix.length()) == 0)
        walker += httpsPrefix.length();
#else
    if (strncmpi(walker, httpPrefix.c_str(), httpPrefix.length()) == 0)
        walker += httpPrefix.length();
    else if (strncmpi(walker, httpsPrefix.c_str(), httpsPrefix.length()) == 0)
        walker += httpsPrefix.length();
#endif

    //find "/", which marks the end of the host part
    while (*walker && *walker != '/' && *walker != '\\')
        walker++;

    if (!*walker) //no slash was found - at end of string
    {
        //there is only a host portion, no path, so set path to just a slash
        if (nullptr != hostUtf8)
            *hostUtf8 = url;

        if (nullptr != pathUtf8)
            *pathUtf8 = "/";
        return true;
    }

    size_t slashIdx = static_cast<size_t>(walker - urlStart);

    //slash was found - set host and path
    if (nullptr != hostUtf8)
        *hostUtf8 = url.substr(0, slashIdx);

    if (nullptr != pathUtf8)
        *pathUtf8 = url.substr(slashIdx, std::string::npos);
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -Converts utf8 string to url encoded escaped "%" values
std::string TStrTool::URL_EncodeUtf8(std::string const& valueUtf8, bool useUpperCaseHex)
{
    if (valueUtf8.length() == 0)
        return "";

    char const* hexP = useUpperCaseHex ? HEX_UPPER : HEX_LOWER;

    size_t const maxEncodedCharWidth = 3; //e.g. "%FF"
    size_t const buffSize = (valueUtf8.length() * maxEncodedCharWidth) + sizeof('\0');
    char* buff = new char[buffSize];
    std::unique_ptr<char[]> auto_buff(buff);
    char* buffP = buff;
    char const* walker = valueUtf8.c_str();

    while (*walker)
    {
        if (isalnum(*walker) ||
            *walker == '.'  || *walker == '-' || *walker == '_' || *walker == '~')
        {
            *buffP++ = *walker++;
        }
        else
        {
            *buffP++ = URL_HEX_ESCAPE;
            //convert char to byte so that utf8 works correctly, then convert to hex
            BYTE byteVal = static_cast<BYTE>(*walker++);
            *buffP++ = hexP[((byteVal >> 4) & 0xF)];//left
            *buffP++ = hexP[(byteVal) & 0x0F];//right
        }
    }

    //terminate, copy, cleanup and return
    *buffP = '\0';
    std::string encStr = buff;
    return encStr;
}
//---------------------------------------------------------------------------
// -Static
// -Decodes url encoded/escaped string back to a utf8 string.
std::string TStrTool::URL_DecodeUtf8(std::string const& encodedStr, bool* invalidHexEncountered)
{
    if (encodedStr.length() == 0)
        return "";

    if (nullptr != invalidHexEncountered)
        *invalidHexEncountered = false;

    size_t const buffSize = encodedStr.length() + sizeof('\0');
    char* buff = new char[buffSize];
    std::unique_ptr<char[]> auto_buff(buff);
    char* buffP = buff;
    char const* walker = encodedStr.c_str();

    while (*walker)
    {
        if (*walker == URL_HEX_ESCAPE)
        {
            walker++;

            BYTE high, low;

            //convert hex pair to high and low parts
            if (!HexSingleToByte(*walker, &high) || !HexSingleToByte(*(walker + 1), &low))
            {
                //be tolerant of invalid encoded URL strings - preserve and move on
                *buffP++ = URL_HEX_ESCAPE;
                if (nullptr != invalidHexEncountered)
                    *invalidHexEncountered = true;
                continue;
            }

            //hex singles are valid - combine into a byte
            *buffP++ = static_cast<char>((high << 4) | low);
            walker += 2;
        }
        else if (*walker == '+')
        {
            *buffP++ = ' ';
            walker++;
        }
        else
        {
            *buffP++ = *walker++;
        }
    }

    //terminate, copy, cleanup and return
    *buffP = '\0';
    std::string valUtf8Str = buff;
    return valUtf8Str;
}
//---------------------------------------------------------------------------
std::string TStrTool::Fmt_printf(char const* format, ...)
{
    size_t const initialBufferSize = 1024;
    size_t bufferSize = initialBufferSize;
    int len;
    va_list args;

    //create a decent size buffer that may need to be reallocated later
    char* text = new char[bufferSize];
    std::unique_ptr<char[]> auto_text(text);

    va_start(args, format);

    //Get the needed length of the format string - if allocated size is enough,
    //then were done because the printf worked.
    len = vsnprintf(text, bufferSize, format, args) + 1; //add 1 for null char

    if (len > static_cast<int>(bufferSize))
    {
        bufferSize = static_cast<size_t>(len);
        auto_text.reset(new char[bufferSize]);
        text = auto_text.get();
        //get the string again now that there is enough room for it
        vsnprintf(text, bufferSize, format, args);
    }

    va_end(args);
    std::string result = text;
    return result;
}
//---------------------------------------------------------------------------
std::wstring TStrTool::Fmt_printf(wchar_t const* format, ...)
{
    size_t const initialBufferSize = 1024;
    size_t bufferSize = initialBufferSize;
    int len;
    va_list args;

    //create a decent size buffer that may need to be reallocated later
    wchar_t* text = new wchar_t[bufferSize];
    std::unique_ptr<wchar_t[]> auto_text(text);

    va_start(args, format);

    //Get the needed length of the format string - if allocated size is enough,
    //then were done because the printf worked.
#ifdef __BORLANDC__
    len = vsnwprintf(text, bufferSize, format, args) + 1; //add 1 for null char
#else
    #ifdef USE_SAFESTR_FUNCS
    len = _vsnwprintf_s(text, bufferSize, _TRUNCATE, format, args) + 1; //add 1 for null char
    #else
    len = _vsnwprintf(text, bufferSize, format, args) + 1; //add 1 for null char
    #endif
#endif

    if (len > static_cast<int>(bufferSize))
    {
        bufferSize = static_cast<size_t>(len);
        auto_text.reset(new wchar_t[bufferSize]);
        text = auto_text.get();
        //get the string again now that there is enough room for it
#ifdef __BORLANDC__
        vsnwprintf(text, bufferSize, format, args);
#else
    #ifdef USE_SAFESTR_FUNCS
        _vsnwprintf_s(text, bufferSize, _TRUNCATE, format, args);
    #else
        _vsnwprintf(text, bufferSize, format, args);
    #endif
#endif
    }

    va_end(args);
    std::wstring result = text;
    return result;
}
//---------------------------------------------------------------------------
// -Static
// -If idStr is empty or null, guid is set to zeros
bool TStrTool::StrToGUID(char const* idStr, GUID& guid)
{
    size_t idStrLen = nullptr == idStr ? 0 : strlen(idStr);

    if (idStrLen == 0)
    {
#if __cplusplus >= 201103L
        guid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }; //same as "{00000000-0000-0000-0000-000000000000}"
#else
        guid.Data1 = 0;
        guid.Data2 = 0;
        guid.Data3 = 0;
        memset(guid.Data4, 0, sizeof(guid.Data4));
#endif
        return true;
    }

    //UuidFromString does not take a const string, for some reason? Microsoft? So, make a duplicate
    char* nonConstStr = new char[idStrLen + 1];
    std::unique_ptr<char[]> auto_nonConstStr(nonConstStr);

    char const* srcWalker = idStr;
    char* destWalker = nonConstStr;

    while (*srcWalker)
        *destWalker++ = *srcWalker++;
    *destWalker = '\0';

    HRESULT hr = ::UuidFromStringA(reinterpret_cast<RPC_CSTR>(nonConstStr), &guid);

    if (hr != RPC_S_OK)
        return false;
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -If idStr is empty or null, guid is set to zeros
bool TStrTool::StrToGUID(wchar_t const* idStr, GUID& guid)
{
    size_t idStrLen = nullptr == idStr ? 0 : wcslen(idStr);

    if (idStrLen == 0)
    {
#if __cplusplus >= 201103L
        guid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }; //same as "{00000000-0000-0000-0000-000000000000}"
#else
        guid.Data1 = 0;
        guid.Data2 = 0;
        guid.Data3 = 0;
        memset(guid.Data4, 0, sizeof(guid.Data4));
#endif
        return true;
    }

    //UuidFromString does not take a const string, for some reason? Microsoft? So, make a duplicate
    wchar_t* nonConstStr = new wchar_t[idStrLen + 1];
    std::unique_ptr<wchar_t[]> auto_nonConstStr(nonConstStr);

    wchar_t const* srcWalker = idStr;
    wchar_t* destWalker = nonConstStr;

    while (*srcWalker)
        *destWalker++ = *srcWalker++;
    *destWalker = '\0';

    HRESULT hr = ::UuidFromStringW(reinterpret_cast<RPC_WSTR>(nonConstStr), &guid);

    if (hr != RPC_S_OK)
        return false;
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -If guid is null, a zero filled guid string is returned.
// -An zero length string is returned for errors.
std::string TStrTool::StrFromGUID(GUID const* guid)
{
    RPC_CSTR szUuid = nullptr;

    if (nullptr == guid)
        return "00000000-0000-0000-0000-000000000000";

    if (::UuidToStringA(guid, &szUuid) == RPC_S_OK)
    {
        std::string guidStr = reinterpret_cast<char*>(szUuid);
        ::RpcStringFreeA(&szUuid);
        return guidStr;
    }

    return "";
}
//---------------------------------------------------------------------------
// -Static
// -If guid is null, a zero filled guid string is returned.
// -An zero length string is returned for errors.
std::wstring TStrTool::StrFromGUIDW(GUID const* guid)
{
    RPC_WSTR szUuid = nullptr;

    if (nullptr == guid)
        return L"00000000-0000-0000-0000-000000000000";

    if (::UuidToStringW(guid, &szUuid) == RPC_S_OK)
    {
        std::wstring guidStr = reinterpret_cast<wchar_t*>(szUuid);
        ::RpcStringFreeW(&szUuid);
        return guidStr;
    }

    return L"";
}
//---------------------------------------------------------------------------
// -Static
// -Returns -1 if guid1 is less than guid2.
// -Returns 0 if guid1 is equal to guid2.
// -Returns 1 if guid1 is greater than guid2.
// -Returns -2 for errors.
int TStrTool::CompareGUID(GUID* guid1, GUID* guid2)
{
    RPC_STATUS status;
    int result = ::UuidCompare(guid1, guid2, &status);

    if (status != RPC_S_OK)
        return -2;

    return result;
}
//---------------------------------------------------------------------------
// -Static
// -Returns -1 if guid1 is less than guid2.
// -Returns 0 if guid1 is equal to guid2.
// -Returns 1 if guid1 is greater than guid2.
// -Returns -2 for errors.
int TStrTool::CompareGUID(GUID const& guid1, GUID const& guid2)
{
    //The underlying Uuid functions don't support const, so copy
    GUID guid1Copy = guid1;
    GUID guid2Copy = guid2;

    return CompareGUID(&guid1Copy, &guid2Copy);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if GUID is all zeros, or null
bool TStrTool::IsEmptyGUID(GUID* guid)
{
    if (nullptr == guid)
        return true;

    GUID guidEmpty = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }; //same as "{00000000-0000-0000-0000-000000000000}"

    return (CompareGUID(guid, &guidEmpty) == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if GUID is all zeros, or null
bool TStrTool::IsEmptyGUID(GUID const& guid)
{
    GUID guidEmpty = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }; //same as "{00000000-0000-0000-0000-000000000000}"
    //The underlying Uuid functions don't support const, so copy
    GUID guidCopy = guid;

    return (CompareGUID(&guidCopy, &guidEmpty) == 0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

} // namespace ASWTools
