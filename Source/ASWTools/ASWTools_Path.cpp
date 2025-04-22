/* **************************************************************************
ASWTools_Path.cpp
Author: Anthony S. West - ASW Software

See header for info.

Copyright 2019 Anthony S. West

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
#include "ASWTools_Path.h"
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
#include <shlobj.h>
//---------------------------------------------------------------------------
#include "ASWTools_String.h"

#ifdef USE_ELOG
    #include "EasyLogger.h"
#endif
//---------------------------------------------------------------------------

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif // #ifdef __BORLANDC__

//---------------------------------------------------------------------------

namespace ASWTools
{

/////////////////////////////////////////////////////////////////////////////
// TPathTool
/////////////////////////////////////////////////////////////////////////////

char const* const TPathTool::AlphaCharsA  =  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
wchar_t const* const TPathTool::AlphaCharsW = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

//---------------------------------------------------------------------------
TPathTool::TPathTool()
{
}
//---------------------------------------------------------------------------
TPathTool::~TPathTool()
{
}
//---------------------------------------------------------------------------
bool TPathTool::IsDots(std::string str)
{
    return (str == "." || str == "..");
}
//---------------------------------------------------------------------------
bool TPathTool::IsDots(std::wstring str)
{
    return (str == L"." || str == L"..");
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if: no drive found, not a network path, and doesn't start with an environment var.
bool TPathTool::IsRelative(std::string const& path)
{
    return (path.find(":") == std::string::npos) && !(path.find("%") == 0) && !(path.find("\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if: no drive found, not a network path, and doesn't start with an environment var.
bool TPathTool::IsRelative(std::wstring const& path)
{
    return (path.find(L":") == std::wstring::npos) && !(path.find(L"%") == 0) && !(path.find(L"\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if environment var symbol found in the path.
bool TPathTool::IsEnvironment(std::string const& path)
{
    return (path.find("%") != std::string::npos);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true if environment var symbol found in the path.
bool TPathTool::IsEnvironment(std::wstring const& path)
{
    return (path.find(L"%") != std::wstring::npos);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true path starts with network slashes.
bool TPathTool::IsNetwork(std::string const& path)
{
    return (path.find("\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// -Returns true path starts with network slashes.
bool TPathTool::IsNetwork(std::wstring const& path)
{
    return (path.find(L"\\\\") == 0);
}
//---------------------------------------------------------------------------
// -Static
// - Combines two paths while properly handling directory separator chars.
std::string TPathTool::Combine(std::string const& path1, std::string const& path2)
{
    //this method doesn't seem to be available
    //std::filesystem::path full_path = path1 / path2;
    //return full_path;

    if (path1.length() == 0)
    {
        return path2;
    }
    else if (path2.length() == 0)
    {
        return path1;
    }
    else if (((path1[path1.length() - 1] == '\\' || path1[path1.length() - 1] == '/') && (path2[0] != '\\' &&
                                                                                          path2[0] != '/')) ||
             ((path1[path1.length() - 1] != '\\' && path1[path1.length() - 1] != '/') && (path2[0] == '\\' ||
                                                                                          path2[0] == '/')))
    {
        return path1 + path2; //one of the paths has a slash in the proper spot, so combine and return
    }
    else if (path1[path1.length() - 1] == '\\' || path1[path1.length() - 1] == '/')
    {
        //path1 ends with slash and path2 begins with slash, return combined with just one slash
        return path1 + path2.substr(1, path2.length() - 1);
    }

    return path1 + "\\" + path2; //neither path has a slash at the join location, return combined with a slash
    //std::string combined = (path1[path1.length() - 1] == '\\' || path1[path1.length() - 1] == '/') ? path1 : path1 + "\\";
    //return combined + ((path2[0] == '\\' || path2[0] == '/') ? path2.substr(1, path2.length() - 1) : path2);
}
//---------------------------------------------------------------------------
// -Static
// - Combines two paths while properly handling directory separator chars.
std::wstring TPathTool::Combine(std::wstring const& path1, std::wstring const& path2)
{
    if (path1.length() == 0)
    {
        return path2;
    }
    else if (path2.length() == 0)
    {
        return path1;
    }
    else if (((path1[path1.length() - 1] == L'\\' || path1[path1.length() - 1] == L'/') && (path2[0] != L'\\' &&
                                                                                            path2[0] != L'/')) ||
             ((path1[path1.length() - 1] != L'\\' && path1[path1.length() - 1] != L'/') && (path2[0] == L'\\' ||
                                                                                            path2[0] == L'/')))
    {
        //one of the paths has a slash in the proper spot, so combine and return
        return path1 + path2;
    }
    else if (path1[path1.length() - 1] == L'\\' || path1[path1.length() - 1] == L'/')
    {
        // path1 ends with slash and path2 begins with slash, return combined with just one slash
        return path1 + path2.substr(1, path2.length() - 1);
    }

    return path1 + L"\\" + path2; //neither path has a slash at the join location, return combined with a slash
}
//---------------------------------------------------------------------------
bool TPathTool::Dir_Exists_WinAPI(std::string const& dir)
{
    DWORD res = GetFileAttributesA(dir.c_str());
    if (INVALID_FILE_ATTRIBUTES == res)
        return false; //dir is invalid

    if (res & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}
//---------------------------------------------------------------------------
bool TPathTool::Dir_Exists_WinAPI(std::wstring const& dir)
{
    DWORD res = GetFileAttributesW(dir.c_str());
    if (INVALID_FILE_ATTRIBUTES == res)
        return false; //dir is invalid

    if (res & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}
//---------------------------------------------------------------------------
// -Overload
bool TPathTool::Dir_CreateDirWithSubs(std::string const& dir)
{
    return Dir_CreateDirWithSubs(TStrTool::Utf8ToUnicodeStr(dir));
}
//---------------------------------------------------------------------------
// -Static
bool TPathTool::Dir_CreateDirWithSubs(std::wstring const& dir)
{
    //see (free use - modified): http://blog.nuclex-games.com/2012/06/how-to-create-directories-recursively-with-win32/
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TPathTool::Dir_CreateDirWithSubs";
#endif // #ifdef USE_ELOG
    static wchar_t const* separators = L"\\/";
    DWORD lastErr;
    std::wstring directory;

    // ASW - Remove ending slash
    if (dir.length() > 0 && (dir[dir.length() - 1] == L'\\' || dir[dir.length() - 1] == L'/'))
        directory = dir.substr(0, dir.length() - 1);
    else
        directory = dir;

    DWORD fileAttributes = ::GetFileAttributesW(directory.c_str());

    if(INVALID_FILE_ATTRIBUTES == fileAttributes)
    {
        // Recursively do it all again for the parent directory, if any
        std::size_t slashIndex = directory.find_last_of(separators);
        if(slashIndex != std::wstring::npos)
        {
            if (!Dir_CreateDirWithSubs(directory.substr(0, slashIndex)))
            {
                //don't log - recursive call already did
                return false;
            }
        }

        // Create the last directory on the path (the recursive calls will have taken
        // care of the parent directories by now)
        BOOL result = ::CreateDirectoryW(directory.c_str(), nullptr);
        if(result == FALSE &&
           (lastErr = ::GetLastError()) != ERROR_ALREADY_EXISTS) //this shouldn't happen, given the attributes check above, but doesn't hurt
        {
#ifdef USE_ELOG
            ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: Failed to create dir: \"%s\". WinLastErr: %u (0x%08lx): \"%s\".\n",
                codeSectionStr, directory.c_str(), lastErr, lastErr, TStrTool::GetWindowsLastErrorCodeAsStringW(lastErr).c_str());
#endif
            //throw std::runtime_error("Could not create directory");
            return false;
        }
    }
    else
    { // Specified directory name already exists as a file or directory

        bool isDirectoryOrJunction = ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
            ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if(!isDirectoryOrJunction)
        {
#ifdef USE_ELOG
            ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Error: Failed to create dir: \"%s\". A file with the same name exists.\n",
                codeSectionStr, directory.c_str());
#endif
            //throw std::runtime_error("Could not create directory because a file with the same name exists");
            return false;
        }

    }

    return true;
}
//---------------------------------------------------------------------------
// -Static
// - Expands the environment variable in the path to the actual path.
bool TPathTool::ExpandEnvironmentVars(std::string const& path, std::string& dest)
{
#ifdef USE_ELOG
    wchar_t const codeSectionStr[] = L"TPathTool::ExpandEnvironmentVars(A)";
#endif

    if (path.length() == 0 || !IsEnvironment(path))
    {
        dest = path;
        return true;
    }

    dest = "";

    size_t buffSize = MaxLen_UnicodeFileName;
    char* buff = new char[buffSize + sizeof('\0') + 1];
    std::unique_ptr<char[]> auto_buff(buff);

    DWORD nCharsWritten = ::ExpandEnvironmentStringsA(path.c_str(), buff, static_cast<DWORD>(buffSize));
    if (nCharsWritten > buffSize)
    {
        //buffer too small - make it bigger and try again
        buffSize = static_cast<size_t>(nCharsWritten) + 1;
        auto_buff.reset(new char[buffSize + sizeof('\0') + 1]);
        buff = auto_buff.get();
        nCharsWritten = ::ExpandEnvironmentStringsA(path.c_str(), buff, static_cast<DWORD>(buffSize));
    }

    if (nCharsWritten == 0)
    {
#ifdef USE_ELOG
        DWORD winLastErrTmp = ::GetLastError();
        std::wstring errStr = TStrTool::GetWindowsLastErrorCodeAsStringW(winLastErrTmp);
        ELog.fwprintf(ELogMsgLevel::LML_Light, L"%sError: ExpandEnvironmentStringsA() failed for: \"%s\" (0x%08lx): %s\n",
            codeSectionStr, TStrTool::Utf8ToUnicodeStr(path).c_str(), winLastErrTmp, errStr.c_str());
#endif
        return false;
    }

    dest = buff;
    return true;
}
//---------------------------------------------------------------------------
// -Static
// - Expands the environment variable in the path to the actual path.
bool TPathTool::ExpandEnvironmentVars(std::wstring const& path, std::wstring& dest)
{
#ifdef USE_ELOG
    wchar_t const codeSectionStr[] = L"TPathTool::ExpandEnvironmentVars(W)";
#endif

    if (path.length() == 0 || !IsEnvironment(path))
    {
        dest = path;
        return true;
    }

    dest = L"";

    size_t buffSize = MaxLen_UnicodeFileName;
    wchar_t* buff = new wchar_t[buffSize + sizeof(L'\0')];
    std::unique_ptr<wchar_t[]> auto_buff(buff);

    DWORD nCharsWritten = ::ExpandEnvironmentStringsW(path.c_str(), buff, static_cast<DWORD>(buffSize));
    if (nCharsWritten > buffSize)
    {
        //buffer too small - make it bigger and try again
        buffSize = static_cast<size_t>(nCharsWritten) + 1;
        auto_buff.reset(new wchar_t[buffSize + sizeof(L'\0')]);
        buff = auto_buff.get();
        nCharsWritten = ::ExpandEnvironmentStringsW(path.c_str(), buff, static_cast<DWORD>(buffSize));
    }

    if (nCharsWritten == 0)
    {
#ifdef USE_ELOG
        DWORD winLastErrTmp = ::GetLastError();
        std::wstring errStr = TStrTool::GetWindowsLastErrorCodeAsStringW(winLastErrTmp);
        ELog.fwprintf(ELogMsgLevel::LML_Light, L"%sError: ExpandEnvironmentStringsW() failed for: \"%s\" (0x%08lx): %s\n",
            codeSectionStr, path.c_str(), winLastErrTmp, errStr.c_str());
#endif
        return false;
    }

    dest = buff;
    return true;
}
//---------------------------------------------------------------------------
// -Static
// -Returns folder path with trailing backslash, if requested
// -"keepTrailSlash" defaults to true
std::string TPathTool::ExtractDir(std::string const& fileName, bool keepTrailSlash)
{
    size_t splitIdx = fileName.find_last_of("/\\");
    if (splitIdx == std::string::npos)
        return "";
    return fileName.substr(0, splitIdx + (keepTrailSlash ? 1 : 0));
}
//---------------------------------------------------------------------------
// -Static
// -Returns folder path with trailing backslash, if requested
// -"keepTrailSlash" defaults to true
std::wstring TPathTool::ExtractDir(std::wstring const& fileName, bool keepTrailSlash)
{
    size_t splitIdx = fileName.find_last_of(L"/\\");
    if (splitIdx == std::wstring::npos)
        return L"";
    return fileName.substr(0, splitIdx + (keepTrailSlash ? 1 : 0));
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::string TPathTool::ExtractFileName(std::string const& path)
{
    size_t splitIdx = path.find_last_of("/\\");
    if (splitIdx == std::string::npos)
        return path;
    return path.substr(splitIdx + 1, path.length() - splitIdx - 1);
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::wstring TPathTool::ExtractFileName(std::wstring const& path)
{
    size_t splitIdx = path.find_last_of(L"/\\");
    if (splitIdx == std::wstring::npos)
        return path;
    return path.substr(splitIdx + 1, path.length() - splitIdx - 1);
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::string TPathTool::ExtractFileName(std::string const& path, bool removeExtension)
{
    if (!removeExtension)
        return ExtractFileName(path);
    return RemoveExtension(ExtractFileName(path));
}
//---------------------------------------------------------------------------
// -Static
// -Extracts file name portion from a slash delimited path. If no slashes
//  are found, returns entire contents of path.
std::wstring TPathTool::ExtractFileName(std::wstring const& path, bool removeExtension)
{
    if (!removeExtension)
        return ExtractFileName(path);
    return RemoveExtension(ExtractFileName(path));
}
//---------------------------------------------------------------------------
// -Static
bool TPathTool::File_Exists_WinAPI(std::string const& fileName)
{
    DWORD attr = ::GetFileAttributesA(fileName.c_str());

    if (INVALID_FILE_ATTRIBUTES == attr)
        return false; //file does not exist

    if (FILE_ATTRIBUTE_DIRECTORY & attr)
        return false; //path is a directory

    return true;
}
//---------------------------------------------------------------------------
// -Static
bool TPathTool::File_Exists_WinAPI(std::wstring const& fileName)
{
    DWORD attr = ::GetFileAttributesW(fileName.c_str());

    if (INVALID_FILE_ATTRIBUTES == attr)
        return false; //file does not exist

    if (FILE_ATTRIBUTE_DIRECTORY & attr)
        return false; //path is a directory

    return true;
}
//---------------------------------------------------------------------------
bool TPathTool::File_GetLastWriteTime(std::string const& fn, FILETIME& lastwritetime)
{
    HANDLE h = nullptr;
    BY_HANDLE_FILE_INFORMATION info;

    lastwritetime.dwLowDateTime   = 0;
    lastwritetime.dwHighDateTime  = 0;

    // get a file handle for fn
    // Note: FILE_SHARE_WRITE is needed so that a shared file that is being appended to is processed correctly.
    DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    h = ::CreateFileA(fn.c_str(), GENERIC_READ, shareMode, nullptr, OPEN_EXISTING, 0, nullptr);
    if (INVALID_HANDLE_VALUE == h)
        return false;
    std::unique_ptr<void, decltype(& CloseHandle)> auto_h(h, CloseHandle);

    if (!::GetFileInformationByHandle(h, &info))
        return false;

    lastwritetime = info.ftLastWriteTime;
    return true;
}
//---------------------------------------------------------------------------
bool TPathTool::File_GetLastWriteTime(std::wstring const& fn, FILETIME& lastwritetime)
{
    HANDLE h = nullptr;
    BY_HANDLE_FILE_INFORMATION info;

    lastwritetime.dwLowDateTime = 0;
    lastwritetime.dwHighDateTime = 0;

    // get a file handle for fn
    // Note: FILE_SHARE_WRITE is needed so that a shared file that is being appended to is processed correctly.
    DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    h = ::CreateFileW(fn.c_str(), GENERIC_READ, shareMode, nullptr, OPEN_EXISTING, 0, nullptr);
    if (INVALID_HANDLE_VALUE == h)
        return false;
    std::unique_ptr<void, decltype(& CloseHandle)> auto_h(h, CloseHandle);

    if (!::GetFileInformationByHandle(h, &info))
        return false;

    lastwritetime = info.ftLastWriteTime;
    return true;
}
//---------------------------------------------------------------------------
bool TPathTool::File_Open(std::string const& fileName, FILE*& filePointer, char const* fileType, unsigned char openMode)
{
    return File_Open(
        TStrTool::Utf8ToUnicodeStr(fileName), filePointer, TStrTool::Utf8ToUnicodeStr(fileType).c_str(), openMode);
}
//---------------------------------------------------------------------------
// -Static
bool TPathTool::File_Open(
    std::wstring const& fileName, FILE*& filePointer, wchar_t const* fileType, unsigned char openMode)
{
    int const openFailWaitMS = 30;
    int const openMaxTries = 30;
    int lastErrno;
    unsigned int tries;
    DWORD lastError;

    //First, make sure that we can open the file.
    if(nullptr != filePointer)
        File_Close(filePointer);

    //If overwriting an existing file, delete the old file first.  This helps
    //Windows figure out that it no longer has the file open.
    if((nullptr == wcschr(fileType, L'a')) &&
       (nullptr == wcschr(fileType, L'r')) &&
       (nullptr == wcschr(fileType, L'+')) &&
       File_Exists_WinAPI(fileName))
    {
        //File exists, try deleting it.
        if (openMode == SH_DENYNO)
        {
            File_Remove(fileName);
            Sleep(0); //Turn remaining time slice back to windows - give it a bit of a chance to delete.
        }
    }

    //If the file is being opened for write, make sure that the path to the file exists.
    if (nullptr != wcschr(fileType, L'w'))
    {
        std::wstring path = TPathTool::ExtractDir(fileName);

        if (!Dir_Exists_WinAPI(path))
            Dir_CreateDirWithSubs(path);
    }

    ::SetLastError(ERROR_SUCCESS); //start with a clean slate before attempting to open

    // Now we can try to open the file.
    tries = 0;
    do
    {
        //clear the last windows error
        errno = 0;
        //Try to open/create file.  Try 30 times, with 30 milliseconds between each try.
        filePointer = _wfsopen(fileName.c_str(), fileType, openMode);
        lastError = ::GetLastError();
        lastErrno = errno; //tracking alternate windows 'errno' value because GetLastError() doesn't
                           //always return an error when there is one.
        tries++;

        if(lastError != 0 || lastErrno != 0)
            Sleep(openFailWaitMS); //wait
        Sleep(0); //Turn remaining time slice back to windows - give windows a chance to process closures/opens.
    }
    //Try opening the file again if it not open yet, or the last error is that the
    //file is open in another process (ERROR_SHARING_VIOLATION = 32), and we have
    //tried fewer than 30 times.
    while ((nullptr == filePointer) &&                          //EACCES = permission denied - value = 5
           (ERROR_SHARING_VIOLATION == lastError || EACCES == lastErrno) &&
           (tries <= openMaxTries));

    //Restore the windows last error.
    //Note: Calling GetLastError() resets the system error to zero. This can cause problems
    //with callers of this function that are trying to find out why the function failed.
    if (nullptr == filePointer)
        ::SetLastError(lastError);

    return filePointer != nullptr; //Return success/failure
}
//---------------------------------------------------------------------------
// -Static
bool TPathTool::File_Close(FILE*& file)
{
    bool result = true;

    if (nullptr != file)
    {
        int flushError = fflush(file); //Note that fflush has no affect on streams opened for reading
                                       //but it does solve issues with files not being closed all
                                       //the way when fclose calls fflush and gets an error.
        fclose(file);
        Sleep(0); //Turn remaining time slice back to windows - give it a bit of a chance to close.

        if (EOF == flushError)
        {
#ifdef USE_ELOG
            std::wstring msg = L"File flush error. Errno: " + std::to_wstring(errno);
            if (ELog.Initialized())
                ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: %s\n", L"TPathTool::File_Close", msg.c_str());
#endif
            result = false;
        }
    }

    file = nullptr;
    return result;
}
//---------------------------------------------------------------------------
// -Static
// -maxWaitMS defaults to 4000 milliseconds - 0 ms is unlimited wait
bool TPathTool::File_Remove(std::string const& fileName, DWORD maxWaitMS)
{
    std::wstring fileNameW = TStrTool::Utf8ToUnicodeStr(fileName);
    return File_Remove(fileNameW, maxWaitMS);
}
//---------------------------------------------------------------------------
// -Static
// -maxWaitMS defaults to 4000 milliseconds - 0 ms is unlimited wait
bool TPathTool::File_Remove(std::wstring const& fileName, DWORD maxWaitMS)
{
#ifdef USE_ELOG
    wchar_t const codeSectionStr[] = L"TPathTool::File_Remove";
#endif

    errno = 0; //clear windows error

    if (!File_Exists_WinAPI(fileName))
        return true;

    if(::DeleteFileW(fileName.c_str()))
    {
        //DWORD startTick = ::GetTickCount();
        //DWORD currentTick;
        ULONGLONG startTick = ::GetTickCount64();
        ULONGLONG currentTick;

        do
        {
            //currentTick = ::GetTickCount();
            currentTick = ::GetTickCount64();

            if (maxWaitMS != 0 && ((currentTick - startTick) >= maxWaitMS))
            {
#ifdef USE_ELOG
                ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Max wait reached for delete file: %s\n",
                    codeSectionStr, fileName.c_str());
#endif
                return false;
            }

            ::Sleep(0); //turn remaining time slice back to windows
        } while (File_Exists_WinAPI(fileName));

        return true;
    }

    //Store the windows last error and restore it after logprintf.
    //Note: Calling GetLastError() resets the system error to zero. This can cause problems
    //with callers of this function that are trying to find out why the function failed.
#ifdef USE_ELOG
    DWORD winLastErrTmp = ::GetLastError();
    std::wstring errStr = TStrTool::GetWindowsLastErrorCodeAsStringW(winLastErrTmp);
    ELog.fwprintf(ELogMsgLevel::LML_Light, L"%sError: Failed to delete file: \"%s\" (0x%08lx): %s\n",
        codeSectionStr, fileName.c_str(), winLastErrTmp, errStr.c_str());
    ::SetLastError(winLastErrTmp); //restor last error in case caller needs to peek at it
#endif

    return false;
}
//---------------------------------------------------------------------------
// -Static
// -Removes the last extension (e.g. ".txt") from the path. If no extension
//  found, returns entire contents of path.
std::string TPathTool::RemoveExtension(std::string const& path)
{
    size_t splitIdx = path.find_last_of(".");
    if (splitIdx == std::string::npos)
        return path;
    return path.substr(0, splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// -Removes the last extension (e.g. ".txt") from the path. If no extension
//  found, returns entire contents of path.
std::wstring TPathTool::RemoveExtension(std::wstring const& path)
{
    size_t splitIdx = path.find_last_of(L".");
    if (splitIdx == std::wstring::npos)
        return path;
    return path.substr(0, splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// -Gets the last extension (e.g. ".txt") from the path. If no extension
//  found, returns empty string.
std::string TPathTool::GetExtension(std::string const& path)
{
    size_t splitIdx = path.find_last_of(".");
    if (splitIdx == std::string::npos)
        return "";
    return path.substr(splitIdx, path.length() - splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// -Gets the last extension (e.g. ".txt") from the path. If no extension
//  found, returns empty string.
std::wstring TPathTool::GetExtension(std::wstring const& path)
{
    size_t splitIdx = path.find_last_of(L".");
    if (splitIdx == std::wstring::npos)
        return L"";
    return path.substr(splitIdx, path.length() - splitIdx);
}
//---------------------------------------------------------------------------
// -Static
// - Generates a random name of given length, using the default charset in AlphaCharsA
std::string TPathTool::GenerateRandomNameA(size_t len)
{
    return GenerateRandomName(len, AlphaCharsA);
}
//---------------------------------------------------------------------------
// -Static
// -If parameter charList is zero length, default AlphaCharsA is used.
std::string TPathTool::GenerateRandomName(size_t len, std::string const& charList)
{
    if (0 == len)
        return "";

    int randMax = static_cast<int>(charList.length() - 1);
    std::string randName;
    randName.reserve(len);

    for (size_t i = 0; i < len; i++)
        randName += charList[static_cast<size_t>(rand() % randMax)];
    return randName;
}
//---------------------------------------------------------------------------
// -Static
// - Generates a random name of given length, using the default charset in AlphaCharsW
std::wstring TPathTool::GenerateRandomNameW(size_t len)
{
    return GenerateRandomName(len, AlphaCharsW);
}
//---------------------------------------------------------------------------
// -Static
// -If parameter charList is zero length, default AlphaCharsW is used.
std::wstring TPathTool::GenerateRandomName(size_t len, std::wstring const& charList)
{
    if (0 == len)
        return L"";

    int randMax = static_cast<int>(charList.length() - 1);
    std::wstring randName;
    randName.reserve(len);

    for (size_t i = 0; i < len; i++)
        randName += charList[static_cast<size_t>(rand() % randMax)];
    return randName;
}
//---------------------------------------------------------------------------
std::string TPathTool::GetDocumentsDirA()
{
    return GetSpecialFolderDirA(CSIDL_PERSONAL);
}
//---------------------------------------------------------------------------
std::wstring TPathTool::GetDocumentsDirW()
{
    return GetSpecialFolderDirW(CSIDL_PERSONAL);
}
//---------------------------------------------------------------------------
std::string TPathTool::GetPicturesDirA()
{
    return GetSpecialFolderDirA(CSIDL_MYPICTURES);
}
//---------------------------------------------------------------------------
std::wstring TPathTool::GetPicturesDirW()
{
    return GetSpecialFolderDirW(CSIDL_MYPICTURES);
}
//---------------------------------------------------------------------------
// -Static
std::string TPathTool::GetSpecialFolderDirA(int folderCSIDL)
{
    size_t tempDirSize = MAX_PATH;
    char* tempDir = new char[tempDirSize];
    std::unique_ptr<char[]> auto_tempDir(tempDir);

    if (!SUCCEEDED(::SHGetFolderPathA(nullptr, folderCSIDL, nullptr, SHGFP_TYPE_CURRENT, tempDir)))
        return "";

    size_t longDirSize = 1024; // resized if needed below, but should be plenty
    char* longDir = new char[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<char[]> auto_longDir(longDir);

    // make sure not a DOS path
    size_t copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return "";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new char[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return "";
    }

    return std::string(longDir);
}
//---------------------------------------------------------------------------
// -Static
std::wstring TPathTool::GetSpecialFolderDirW(int folderCSIDL)
{
    size_t tempDirSize = MAX_PATH; // 32767 is max unicode path length
    wchar_t* tempDir = new wchar_t[tempDirSize];
    std::unique_ptr<wchar_t[]> auto_tempDir(tempDir);

    if (!SUCCEEDED(::SHGetFolderPathW(nullptr, folderCSIDL, nullptr, SHGFP_TYPE_CURRENT, tempDir)))
        return L"";

    size_t longDirSize = 32767; // 32767 is max unicode path length
    wchar_t* longDir = new wchar_t[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<wchar_t[]> auto_longDir(longDir);

    // make sure not a DOS path
    size_t copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return L"";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new wchar_t[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return L"";
    }

    return std::wstring(longDir);
}
//---------------------------------------------------------------------------
// -Static
std::string TPathTool::GetTempDirA()
{
    size_t tempDirLen = MAX_PATH + 1; // initial length - resized if needed
    char* tempDir = new char[tempDirLen + sizeof('\0')];
    std::unique_ptr<char[]> auto_tempDir(tempDir);

    size_t copiedLen = ::GetTempPathA(static_cast<DWORD>(tempDirLen), tempDir);
    if (0 == copiedLen)
        return "";

    if (copiedLen > tempDirLen)
    {
        // Buffer is too small - reallocate
        tempDirLen = copiedLen + sizeof('\0');
        auto_tempDir.reset(new char[tempDirLen + sizeof('\0')]);
        tempDir = auto_tempDir.get();

        copiedLen = ::GetTempPathA(static_cast<DWORD>(tempDirLen), tempDir);
        if (0 == copiedLen)
            return "";
    }

    tempDir[tempDirLen] = '\0'; // For safety

    size_t longDirSize = tempDirLen + 32767 + sizeof('\0'); // 32767 is max unicode path length
    char* longDir = new char[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<char[]> auto_longDir(longDir);

    // make sure not a DOS path
    copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return "";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new char[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameA(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return "";
    }

    return std::string(longDir);
}
//---------------------------------------------------------------------------
// -Static
std::wstring TPathTool::GetTempDirW()
{
    size_t tempDirLen = MAX_PATH + 1;
    wchar_t* tempDir = new wchar_t[tempDirLen + sizeof('\0')];
    std::unique_ptr<wchar_t[]> auto_tempDir(tempDir);

    size_t copiedLen = ::GetTempPathW(static_cast<DWORD>(tempDirLen), tempDir);
    if (0 == copiedLen)
        return L"";

    if (copiedLen > tempDirLen)
    {
        // Buffer is too small - reallocate
        tempDirLen = copiedLen + sizeof('\0');
        auto_tempDir.reset(new wchar_t[tempDirLen + sizeof('\0')]);
        tempDir = auto_tempDir.get();

        copiedLen = ::GetTempPathW(static_cast<DWORD>(tempDirLen), tempDir);
        if (0 == copiedLen)
            return L"";
    }

    tempDir[tempDirLen] = L'\0'; // For safety

    size_t longDirSize = tempDirLen + 32767 + sizeof('\0'); // 32767 is max unicode path length
    wchar_t* longDir = new wchar_t[longDirSize]; // stores expanded directory path (no ~)
    std::unique_ptr<wchar_t[]> auto_longDir(longDir);

    // make sure not a DOS path
    copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
    if (0 == copiedLen)
        return L"";

    if (copiedLen > longDirSize)
    {
        longDirSize = copiedLen;
        auto_longDir.reset(new wchar_t[longDirSize]);
        longDir = auto_longDir.get();

        copiedLen = ::GetLongPathNameW(tempDir, longDir, static_cast<DWORD>(longDirSize));
        if (0 == copiedLen)
            return L"";
    }

    return std::wstring(longDir);
}
//---------------------------------------------------------------------------

} // namespace ASWTools
