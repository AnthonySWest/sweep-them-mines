/* **************************************************************************
BasicINI.cpp
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
#include "BasicINI.h"
//---------------------------------------------------------------------------
#include <memory>
//---------------------------------------------------------------------------
#include "PathTool.h"
#include "StringTool.h"
//---------------------------------------------------------------------------

namespace ASWTools
{

namespace BasicINI
{

/////////////////////////////////////////////////////////////////////////////
// TKeyVal
/////////////////////////////////////////////////////////////////////////////

//char TKeyVal::CommentStart1 = ';';
//char TKeyVal::CommentStart2 = '/';

//---------------------------------------------------------------------------
TKeyVal::TKeyVal()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TKeyVal::~TKeyVal()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TKeyVal::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TKeyVal::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TKeyVal::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here
    Key = "";
    Value = "";

    return true;
}
//---------------------------------------------------------------------------
bool TKeyVal::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
/*
    TKeyVal::IsComment()

    Comments are read in as values with an empty Key
*/
bool TKeyVal::IsComment() const
{
    if (Key.length() == 0)
    {
        if (Value.length() > 0 && (Value[0] == CommentStart1 || Value[0] == CommentStart2))
            return true;
    }

    return false;
}
//---------------------------------------------------------------------------
/*
    TKeyVal::IsKeyValuePair

    Returns true if there is a key with a length. A comment will have zero for key length because comment is stored
    entirely in value. Also keyless values will have zero key length.
*/
bool TKeyVal::IsKeyValuePair() const
{
    //If key is empty, then not a key value pair. If value is not empty, then could be a comment.
    if (TStrTool::IsEmptyOrWhiteSpace(Key))
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool TKeyVal::IsEmpty() const
{
    if (TStrTool::IsEmptyOrWhiteSpace(Key) && Value.length() == 0)
        return true;
    return false;
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
// TSection
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TSection::TSection()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
//TSection::TSection(const std::string& name)
//{
//	Reset_Private();
//	Name = name;
//}
//---------------------------------------------------------------------------
TSection::~TSection()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TSection::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TSection::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TSection::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here
    Name = "";
    KeyVals.clear();

    return true;
}
//---------------------------------------------------------------------------
bool TSection::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
bool TSection::IsGlobalSection()
{
    return Name.length() == 0;
}
//---------------------------------------------------------------------------
bool TSection::AddKeyVal(const std::string& key, const std::string& value)
{
    TKeyVal keyVal;
    keyVal.Key = key;
    keyVal.Value = value;
    return AddKeyVal(keyVal);
}
//---------------------------------------------------------------------------
bool TSection::AddKeyVal(const TKeyVal& keyVal)
{
    //don't allow a key to begin with a section start
    //on second thought, it will still probably work, so, let the caller be the boss
    //if (keyVal.Key.length() > 0 && keyVal.Key[0] == SectionStart)
    //	return false;

    KeyVals.push_back(keyVal);
    return true;
}
//---------------------------------------------------------------------------
/*
    TSection::AddKeyVal_IfNotExists

    - If the key does not exist, a new key will be added with the value of defaultValue.
    - If the key does exist, nothing is done.
*/
bool TSection::AddKeyVal_IfNotExists(const std::string& key, const std::string& defaultValue, bool searchIgnoreCase)
{
    if (FindKey(key, searchIgnoreCase) < 0)
        return AddKeyVal(key, defaultValue);
    return true;
}
//---------------------------------------------------------------------------
/*
    TSection::AddKeyVal_IfNotExists

    - If the key does not exist, a new key will be added with the value of defaultValue.
    - If the key does exist, nothing is done.
*/
bool TSection::AddKeyVal_IfNotExists(const TKeyVal& keyVal, bool searchIgnoreCase)
{
    if (FindKey(keyVal.Key, searchIgnoreCase) < 0)
        return AddKeyVal(keyVal);
    return true;
}
//---------------------------------------------------------------------------
bool TSection::InsertKeyVal(size_t index, const std::string& key, const std::string& value)
{
    TKeyVal keyVal;
    keyVal.Key = key;
    keyVal.Value = value;
    return InsertKeyVal(index, keyVal);
}
//---------------------------------------------------------------------------
bool TSection::InsertKeyVal(size_t index, const TKeyVal& keyVal)
{
    if (index >= KeyVals.size())
        KeyVals.push_back(keyVal);
    else
    {
        std::vector<TKeyVal>::iterator itIdx = KeyVals.begin() + index;
        KeyVals.insert(itIdx, keyVal);
    }
    return true;
}
//---------------------------------------------------------------------------
/*
    TSection::InsertComment

    Inserts comment as value with blank key
*/
bool TSection::InsertComment(size_t index, const std::string& comment)
{
    //if comment is blank, insert a comment marker
    if (comment.length() == 0)
        return InsertKeyVal(index, "", std::string(1, TKeyVal::CommentStart1));

    //if comment does not begin with a comment marker, add one
    if (comment[0] != TKeyVal::CommentStart1 && comment[0] != TKeyVal::CommentStart2)
        return InsertKeyVal(index, "", std::string(1, TKeyVal::CommentStart1) + comment);
    return InsertKeyVal(index, "", comment);
}
//---------------------------------------------------------------------------
bool TSection::DeleteKeyVal(size_t index)
{
    if (index >= KeyVals.size())
        return true;

    std::vector<TKeyVal>::iterator itIdx = KeyVals.begin() + index;
    KeyVals.erase(itIdx);
    return true;
}
//---------------------------------------------------------------------------
int TSection::FindKey(const std::string& key, bool ignoreCase) const
{
    for (int i = 0; i < KeyVals.size(); i++)
    {
        const TKeyVal* keyValP = &KeyVals[i];

        if ((key.length() == 0 && keyValP->Key.length() == 0) ||
            (ignoreCase && _stricmp(key.c_str(), keyValP->Key.c_str()) == 0) ||
            (!ignoreCase && strcmp(key.c_str(), keyValP->Key.c_str()) == 0))
            return i;
    }

    return -1;
}
//---------------------------------------------------------------------------
/*
    TSection::FindOrCreateKey

    - On success, returns either the found index of existing key, or index of newly added key.
    - On failure, returns less than zero.
*/
int TSection::FindOrCreateKey(const std::string& key, bool ignoreCase)
{
    int idx = FindKey(key, ignoreCase);

    if (idx < 0)
    {
        KeyVals.push_back(TKeyVal());
        idx = static_cast<int>(KeyVals.size() - 1);
        KeyVals[idx].Key = key;
    }

    return idx;
}
//---------------------------------------------------------------------------
int TSection::FindVal(const std::string& value, bool ignoreCase) const
{
    for (int i = 0; i < KeyVals.size(); i++)
    {
        const TKeyVal* keyValP = &KeyVals[i];

        if ((value.length() == 0 && keyValP->Value.length() == 0) ||
            (ignoreCase && _stricmp(value.c_str(), keyValP->Value.c_str()) == 0) ||
            (!ignoreCase && strcmp(value.c_str(), keyValP->Value.c_str()) == 0))
            return i;
    }

    return -1;
}
//---------------------------------------------------------------------------
bool TSection::HasOneOrMoreKeyValuePairs() const
{
    for (int i = 0; i < KeyVals.size(); i++)
    {
        if (KeyVals[i].IsKeyValuePair())
            return true;
    }
    return false;
}
//---------------------------------------------------------------------------
EErrINI TSection::Save(FILE* fOut, const char assignOperator, const std::string& paddingAfterOperator)
{
    if (nullptr == fOut)
    {
        return EErrINI::EI_BadParameter;
    }

    EErrINI result = EErrINI::EI_NoError;
    bool writeErr = false;

    if (Name.length() > 0)
        writeErr |= -1 == fprintf(fOut, "%s\n", TStrTool::Trim_Copy(Name).c_str());

    for (int i = 0; i < KeyVals.size() && !writeErr; i++)
    {
        TKeyVal* keyValP = &KeyVals[i];
        std::string key = TStrTool::Trim_Copy(keyValP->Key);

        if (key.length() == 0) //if there is no key, write the value as is (could be a comment line, or a value only line
            writeErr |= -1 == fprintf(fOut, "%s\n", keyValP->Value.c_str());
        else
            writeErr |= -1 == fprintf(fOut, "%s%c%s%s\n", key.c_str(), assignOperator, paddingAfterOperator.c_str(), keyValP->Value.c_str());
    }

    if (writeErr)
    {
        result = EErrINI::EI_WriteError;
    }

    return result;
}
//---------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
// TBasicINI
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
TBasicINI::TBasicINI()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TBasicINI::~TBasicINI()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TBasicINI::Destroy_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TBasicINI::Destroy() //virtual
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TBasicINI::Reset_Private()
{
    //code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    //reset class vars here
    PaddingAfterOperator_Write = "";
    AlternateAssignOperator_Read = ':';

    Sections.clear();

    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::Reset() //virtual
{
    return Reset_Private();
}
//---------------------------------------------------------------------------
bool TBasicINI::AddSection()
{
    Sections.push_back(TSection());
    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::AddSection(const TSection& section)
{
    Sections.push_back(section);
    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::InsertSection(size_t index)
{
    return InsertSection(index, TSection());
}
//---------------------------------------------------------------------------
bool TBasicINI::InsertSection(size_t index, const TSection& section)
{
    if (index >= Sections.size())
        Sections.push_back(section);
    else
    {
        std::vector<TSection>::iterator itIdx = Sections.begin() + index;
        Sections.insert(itIdx, section);
    }
    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::DeleteSection(size_t index)
{
    if (index >= Sections.size())
        return true;

    std::vector<TSection>::iterator itIdx = Sections.begin() + index;
    Sections.erase(itIdx);
    return true;
}
//---------------------------------------------------------------------------
// -Static
bool TBasicINI::Dir_Exists_WinAPI(std::string const& dir)
{
    DWORD res = GetFileAttributesA(dir.c_str());
    if (INVALID_FILE_ATTRIBUTES == res)
        return false; //dir is invalid

    if (res & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    return false;
}
//---------------------------------------------------------------------------
// -Static
bool TBasicINI::Dir_Exists_WinAPI(std::wstring const& dir)
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
bool TBasicINI::Dir_CreateDirWithSubs(std::string const& dir)
{
    return Dir_CreateDirWithSubs(TStrTool::Utf8ToUnicodeStr(dir));
}
//---------------------------------------------------------------------------
// -Static
bool TBasicINI::Dir_CreateDirWithSubs(std::wstring const& dir)
{
    //see (free use - modified): http://blog.nuclex-games.com/2012/06/how-to-create-directories-recursively-with-win32/
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TBasicINI::Dir_CreateDirWithSubs";
#endif // #ifdef USE_ELOG
    static const std::wstring separators(L"\\/");
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
bool TBasicINI::File_Exists_WinAPI(std::string const& fileName)
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
bool TBasicINI::File_Exists_WinAPI(std::wstring const& fileName)
{
    DWORD attr = ::GetFileAttributesW(fileName.c_str());

    if (INVALID_FILE_ATTRIBUTES == attr)
        return false; //file does not exist

    if (FILE_ATTRIBUTE_DIRECTORY & attr)
        return false; //path is a directory

    return true;
}
//---------------------------------------------------------------------------
bool TBasicINI::File_Open(std::string const& fileName, FILE*& filePointer, char const* fileType, unsigned char openMode)
{
    return File_Open(
        TStrTool::Utf8ToUnicodeStr(fileName), filePointer, TStrTool::Utf8ToUnicodeStr(fileType).c_str(), openMode);
}
//---------------------------------------------------------------------------
// -Static
bool TBasicINI::File_Open(
    std::wstring const& fileName, FILE*& filePointer, wchar_t const* fileType, unsigned char openMode)
{
    const int openFailWaitMS = 30;
    const int openMaxTries = 30;
    unsigned int lastErrno;
    unsigned int tries;
    DWORD lastError;

    //First, make sure that we can open the file.
    if(filePointer != NULL)
        File_Close(filePointer);

    //If overwriting an existing file, delete the old file first.  This helps
    //Windows figure out that it no longer has the file open.
    if((NULL == wcschr(fileType, L'a')) &&
       (NULL == wcschr(fileType, L'r')) &&
       (NULL == wcschr(fileType, L'+')) &&
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
    if (NULL != wcschr(fileType, L'w'))
    {
        std::wstring path = TPathTool::ExtractDir(fileName);

        if (!Dir_Exists_WinAPI(path))
            Dir_CreateDirWithSubs(path);
    }

    ::SetLastError((DWORD)ERROR_SUCCESS); //start with a clean slate before attempting to open

    //Now we can try to open the file.
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
    while ((NULL == filePointer) &&                          //EACCES = permission denied - value = 5
           (ERROR_SHARING_VIOLATION == lastError || EACCES == lastErrno) &&
           (tries <= openMaxTries));

    //Restore the windows last error.
    //Note: Calling GetLastError() resets the system error to zero. This can cause problems
    //with callers of this function that are trying to find out why the function failed.
    if (NULL == filePointer)
        ::SetLastError(lastError);

    return filePointer != NULL; //Return success/failure
}
//---------------------------------------------------------------------------
// -Static
bool TBasicINI::File_Close(FILE*& file)
{
    bool result = true;

    if (NULL != file)
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
                ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: %s\n", L"TBasicINI::File_Close", msg.c_str());
#endif
            result = false;
        }
    }

    file = NULL;
    return result;
}
//---------------------------------------------------------------------------
// -Static
// -maxWaitMS defaults to 4000 milliseconds - 0 ms is unlimited wait
bool TBasicINI::File_Remove(std::string const& fileName, DWORD maxWaitMS)
{
    std::wstring fileNameW = TStrTool::Utf8ToUnicodeStr(fileName);
    return File_Remove(fileNameW, maxWaitMS);
}
//---------------------------------------------------------------------------
// -Static
// -maxWaitMS defaults to 4000 milliseconds - 0 ms is unlimited wait
bool TBasicINI::File_Remove(std::wstring const& fileName, DWORD maxWaitMS)
{
#ifdef USE_ELOG
    const wchar_t codeSectionStr[] = L"TBasicINI::File_Remove";
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
int TBasicINI::FindSection(const std::string& sectionName, bool ignoreCase) const
{
    for (int i = 0; i < Sections.size(); i++)
    {
        const TSection* secP = &Sections[i];

        if ((sectionName.length() == 0 && secP->Name.length() == 0) ||
            (ignoreCase && _stricmp(sectionName.c_str(), secP->Name.c_str()) == 0) ||
            (!ignoreCase && strcmp(sectionName.c_str(), secP->Name.c_str()) == 0))
            return i;
    }

    return -1;
}
//---------------------------------------------------------------------------
/*
    TBasicINI::FindOrCreateSection

    - On success, returns either the found index of existing section, or index of newly added section.
    - On failure, returns less than zero.
*/
int TBasicINI::FindOrCreateSection(const std::string& sectionName, bool ignoreCase)
{
    int idx = FindSection(sectionName, ignoreCase);

    if (idx < 0)
    {
        Sections.push_back(TSection());
        idx = static_cast<int>(Sections.size() - 1);
        Sections[idx].Name = sectionName;
    }

    return idx;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Load(const std::string& fileNameINI, const char assignOperator, size_t maxLineLen)
{
    Reset();

    if (TStrTool::IsEmptyOrWhiteSpace(fileNameINI))
    {
        return EErrINI::EI_BadParameter;
    }

    if (!File_Exists_WinAPI(fileNameINI))
    {
        return EErrINI::EI_FileNotExists;
    }

    FILE* fIn = nullptr;

    if (!File_Open(TStrTool::Utf8ToUnicodeStr(fileNameINI), fIn, L"r", SH_DENYWR))
    {
        return EErrINI::EI_FailOpenRead;
    }

    EErrINI result = Load(fIn, assignOperator, maxLineLen);
    File_Close(fIn);

    return result;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Load(FILE* fIn, const char assignOperator, size_t maxLineLen)
{
    if (nullptr == fIn)
    {
        return EErrINI::EI_BadParameter;
    }

    EErrINI result = EErrINI::EI_NoError;
    const size_t lineSize = maxLineLen + sizeof('\0') + sizeof('\r') + sizeof('\n');
    char* line = new char[lineSize];
    std::unique_ptr<char[]> auto_line(line);

    //read in one line at a time and parse
    while (!::feof(fIn))
    {
        if (::fgets(line, static_cast<int>(lineSize), fIn) == NULL)
        {
            int fErr = ::ferror(fIn);

            if (0 != fErr)
            {
                result = EErrINI::EI_ReadError;
            }

            break;
        }

        char* lineP = line;

        //skip spaces at start
        while (' ' == *lineP || '\n' == *lineP)
            lineP++;

        if (!*lineP)
            continue; //skip empty lines

        size_t len = strlen(lineP);

        //remove the new line ender if there is one
        if (lineP[len - 1] == '\n')
            lineP[--len] = '\0';

        if (TSection::SectionStart == *lineP)
        {
            TSection section;
            section.Name = lineP;
            Sections.push_back(section);
            continue;
        }

        if (Sections.size() == 0)
        {
            //no section was found, but a key value pair or comment line is coming, so, create a "global" scoped section
            Sections.push_back(TSection());
        }

        //get pointer to current section
        TSection* sectionP = &Sections[Sections.size() - 1];
        TKeyVal keyVal;

        if (*lineP == TKeyVal::CommentStart1 || *lineP == TKeyVal::CommentStart2) //add comment lines as values only (no key)
        {
            keyVal.Value = lineP;
            sectionP->KeyVals.push_back(keyVal);
            continue;
        }

        //add key value pair by finding the assignment operator
        //Note: the first operator will determine the "key" portion. If not found, the whole line will be added as a value without a key
        char* valStartP = lineP;
        char* keyEndP = nullptr;
        bool operatorFound = false;

        while (*valStartP)
        {
            if (' ' == *valStartP || assignOperator == *valStartP || AlternateAssignOperator_Read == *valStartP)
            {
                if (nullptr == keyEndP)
                    keyEndP = valStartP;

                if (' ' != *valStartP)
                {
                    operatorFound = true;
                    //found an operator, skip it
                    valStartP++;
                    //skip white space in order to find the start of the value
                    while (' ' == *valStartP)
                        valStartP++;

                    break; //found the beginning of the value, or an empty value
                }
            }

            valStartP++;
        }

        if (operatorFound)
        {
            *keyEndP = '\0';
            keyVal.Key = lineP;
            keyVal.Value = valStartP;
        }
        else //operator not found - treat the entire line as a value with an empty key
        {
            keyVal.Value = lineP;
        }

        sectionP->KeyVals.push_back(keyVal);
    }

    return result;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Save(const std::string& fileNameINI, bool overWrite, const char assignOperator)
{
    if (TStrTool::IsEmptyOrWhiteSpace(fileNameINI))
    {
        return EErrINI::EI_BadParameter;
    }

    if (!overWrite && File_Exists_WinAPI(fileNameINI))
    {
        return EErrINI::EI_FileExists;
    }

    FILE* fOut = nullptr;

    if (!File_Open(TStrTool::Utf8ToUnicodeStr(fileNameINI), fOut, L"w", SH_DENYWR))
    {
        return EErrINI::EI_FailOpenWrite;
    }

    EErrINI result = Save(fOut, assignOperator);
    File_Close(fOut);

    return result;
}
//---------------------------------------------------------------------------
EErrINI TBasicINI::Save(FILE* fOut, const char assignOperator)
{
    if (nullptr == fOut)
    {
        return EErrINI::EI_BadParameter;
    }

    EErrINI result = EErrINI::EI_NoError;
    bool writeErr = false;

    for (int i = 0; i < Sections.size(); i++)
    {
        TSection* secP = &Sections[i];

        if (i > 0)
            writeErr |= -1 == fprintf(fOut, "\n");

        EErrINI resultTmp = secP->Save(fOut, assignOperator, PaddingAfterOperator_Write);

        if (EErrINI::EI_NoError != result)
        {
            result = resultTmp;
            //break; //save what can be saved
        }
    }

    if (writeErr)
    {
        result = EErrINI::EI_WriteError;
    }

    return result;
}

//---------------------------------------------------------------------------

} // namespace BasicINI

} // namespace ASWTools
