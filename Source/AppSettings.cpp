/* **************************************************************************
AppSettings.cpp
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
// Module header
#include "AppSettings.h"
//---------------------------------------------------------------------------
#include "StringTool.h"
//---------------------------------------------------------------------------
using namespace ASWTools;
using namespace BasicINI;
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TAppSettings
/////////////////////////////////////////////////////////////////////////////

char const* TAppSettings::Default_DirLogs = "%localAppData%\\ASWSoftware\\SweepThemMines\\Logs\\";
char const* TAppSettings::Default_LogPrefix = "GeneralSTM_";

char const* TAppSettings::SectionName_General = "[General]";

// Key names - General
char const* TAppSettings::KeyName_Gen_ImagesPath = "ImagesPath";
char const* TAppSettings::KeyName_Gen_DirLogs = "DirLogs";
char const* TAppSettings::KeyName_Gen_LogPrefix = "LogPrefix";
char const* TAppSettings::KeyName_Gen_LogLevel = "LogLevel";
char const* TAppSettings::KeyName_Gen_NDaysRetainLogs = "NDaysRetainLogs";

// Comments
char const* TAppSettings::KeyName_Gen_ImagesPath_Comment = ";Leave blank for default (exe directory).";
char const* TAppSettings::KeyName_Gen_LogLevel_Comment =
    ";Valid range for LogLevel: 0-4. 0=System/forced logs only, 1=errors/warnings, 2=medium, 3=heavy, 4=debug/verbose";
char const* TAppSettings::KeyName_Gen_NDaysRetainLogs_Comment =
    ";Valid value for NDaysRetainLogs: 0 to N days. 0=retain forever.";

//---------------------------------------------------------------------------
TAppSettings::TAppSettings()
{
    Reset_Private();
}
//---------------------------------------------------------------------------
TAppSettings::~TAppSettings()
{
    Destroy_Private();
}
//---------------------------------------------------------------------------
void TAppSettings::Destroy_Private()
{
    // Code specific to this level of inheritance goes here - no virtual functions can be called here
}
//---------------------------------------------------------------------------
void TAppSettings::Destroy() // virtual
{
    Inherited::Destroy();
    Destroy_Private();
}
//---------------------------------------------------------------------------
bool TAppSettings::Reset_Private()
{
    // Code specific to this level of inheritance goes here - no virtual functions can be called here
    Destroy_Private();

    // Reset class vars here
    NeedsResaved = false;

    Gen_ImagesPath = "";
    Gen_DirLogs = Default_DirLogs;
    Gen_LogPrefix = Default_LogPrefix;
    Gen_LogLevel = 0;//ELogMsgLevel::LML_Medium;
    Gen_NDaysRetainLogs = 30;

    return true;
}
//---------------------------------------------------------------------------
bool TAppSettings::Reset() // virtual
{
    if (!Inherited::Reset())
        return false;
    return Reset_Private();
}
//---------------------------------------------------------------------------
bool TAppSettings::ParseSection_General()
{
    std::string sectionName = SectionName_General;
    std::string searchKey, tmpStr;
    TSection* secP;
    TKeyVal* keyValP;
    size_t idx;

    if (Sections.size() == 0)
    {
        // All sections are missing - use defaults
        NeedsResaved = true;
        return true;
    }

    idx = FindSection(sectionName, true);

    if (TSection::NotFound == idx)
    {
        // Section missing - use defaults
        NeedsResaved = true;
        return true;
    }

    // Found section - parse key values
    secP = &Sections[idx];

    searchKey = KeyName_Gen_ImagesPath;
    idx = secP->FindKey(searchKey, true);
    if (TSection::NotFound == idx)
    {
        // Key is missing - use default
        NeedsResaved = true;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        Gen_ImagesPath = TStrTool::Trim_Copy(keyValP->Value);
    }

    searchKey = KeyName_Gen_DirLogs;
    idx = secP->FindKey(searchKey, true);
    if (TSection::NotFound == idx)
    {
        // Key is missing - use default
        NeedsResaved = true;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        tmpStr = TStrTool::Trim_Copy(keyValP->Value);

        if (tmpStr.length() == 0)
        {
            // Invalid value - use default
            NeedsResaved = true;
        }
        else
        {
            Gen_DirLogs = tmpStr;
        }
    }

    searchKey = KeyName_Gen_LogPrefix;
    idx = secP->FindKey(searchKey, true);
    if (TSection::NotFound == idx)
    {
        // Key is missing - use default
        NeedsResaved = true;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        tmpStr = TStrTool::Trim_Copy(keyValP->Value);

        if (tmpStr.length() == 0)
        {
            // Invalid value - use default
            NeedsResaved = true;
        }
        else
        {
            Gen_LogPrefix = tmpStr;
        }
    }

    searchKey = KeyName_Gen_LogLevel;
    idx = secP->FindKey(searchKey, true);
    if (TSection::NotFound == idx)
    {
        // Key is missing - use default
        NeedsResaved = true;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        tmpStr = TStrTool::Trim_Copy(keyValP->Value);

        int valInt = std::atoi(tmpStr.c_str());

        if (tmpStr.length() == 0 || valInt < 0)
        {
//            ELog.fwprintf(ELogMsgLevel::LML_Light,
//                L"%s: Warning: Section \"%s\" has invalid value for key: \"%s\". Valid range: 0 - 4, "
//                    "higher number = heavier logging. Using default.\n",
//                codeSectStr, sectionNameW.c_str(), TStrTool::Utf8ToUnicodeStr(searchKey).c_str());
            NeedsResaved = true;
        }
//        else if (valInt > (int)ELogMsgLevel::LML_Debug)
//        {
//            //max log level exceeded - cap to max
//            Gen_LogLevel = ELogMsgLevel::LML_Debug;
//            NeedsResaved = true;
//        }
        else
        {
//            Gen_LogLevel = static_cast<ELogMsgLevel>(valInt);
            Gen_LogLevel = static_cast<DWORD>(valInt);
        }
    }

    searchKey = KeyName_Gen_NDaysRetainLogs;
    idx = secP->FindKey(searchKey, true);
    if (TSection::NotFound == idx)
    {
//        ELog.fwprintf(ELogMsgLevel::LML_Light, L"%s: Warning: Missing key: \"%s\". Using default.\n",
//            codeSectStr, TStrTool::Utf8ToUnicodeStr(searchKey).c_str());
        NeedsResaved = true;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        tmpStr = TStrTool::Trim_Copy(keyValP->Value);

        int valInt = std::atoi(tmpStr.c_str());

        if (tmpStr.length() == 0 || valInt < 0)
        {
//            ELog.fwprintf(ELogMsgLevel::LML_Light,
//                L"%s: Warning: Section \"%s\" has invalid value for key: \"%s\". Valid range: 0 - N, 0=never delete "
//                    "old logs. Using default.\n",
//                codeSectStr, sectionNameW.c_str(), TStrTool::Utf8ToUnicodeStr(searchKey).c_str());
            NeedsResaved = true;
        }
        else
        {
            Gen_NDaysRetainLogs = static_cast<DWORD>(valInt);
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool TAppSettings::ApplyChanges_General()
{
    std::string sectionName = SectionName_General;
    std::string searchKey;
    bool result = true;
    TSection* secP;
    TKeyVal* keyValP;
    size_t idx;

    idx = FindOrCreateSection(sectionName, true);

    if (TSection::NotFound == idx)
        return false;

    secP = &Sections[idx];
    secP->Name = sectionName;

    // Apply values to the section

    // Images directory
    searchKey = KeyName_Gen_ImagesPath;
    if (TSection::NotFound == (idx = secP->FindOrCreateKey(searchKey, true)))
    {
        result = false;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        keyValP->Key = searchKey;
        keyValP->Value = Gen_ImagesPath;

        // Insert comment if a comment is not already before this element
        if (idx == 0 || !secP->KeyVals[idx - 1].IsComment() ||
            secP->KeyVals[idx - 1].Value != KeyName_Gen_ImagesPath_Comment)
        {
            secP->InsertComment(idx, KeyName_Gen_ImagesPath_Comment);
        }
    }

    //logs directory
    searchKey = KeyName_Gen_DirLogs;
    if (TSection::NotFound == (idx = secP->FindOrCreateKey(searchKey, true)))
    {
        result = false;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        keyValP->Key = searchKey;
        keyValP->Value = Gen_DirLogs;
    }

    //log prefix
    searchKey = KeyName_Gen_LogPrefix;
    if (TSection::NotFound == (idx = secP->FindOrCreateKey(searchKey, true)))
    {
        result = false;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        keyValP->Key = searchKey;
        keyValP->Value = Gen_LogPrefix;
    }

    //log level
    searchKey = KeyName_Gen_LogLevel;
    if (TSection::NotFound == (idx = secP->FindOrCreateKey(searchKey, true)))
    {
        result = false;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        keyValP->Key = searchKey;
        keyValP->Value = std::to_string(static_cast<int>(Gen_LogLevel));

        //insert comment if a comment is not already before this element
        if (idx == 0 || !secP->KeyVals[idx - 1].IsComment() ||
            secP->KeyVals[idx - 1].Value != KeyName_Gen_LogLevel_Comment)
        {
            secP->InsertComment(idx, KeyName_Gen_LogLevel_Comment);
        }
    }

    //retain logs
    searchKey = KeyName_Gen_NDaysRetainLogs;
    if (TSection::NotFound == (idx = secP->FindOrCreateKey(searchKey, true)))
    {
        result = false;
    }
    else
    {
        keyValP = &secP->KeyVals[idx];
        keyValP->Key = searchKey;
        keyValP->Value = std::to_string(Gen_NDaysRetainLogs);

        //insert comment if a comment is not already before this element
        if (idx == 0 || !secP->KeyVals[idx - 1].IsComment() ||
            secP->KeyVals[idx - 1].Value != KeyName_Gen_NDaysRetainLogs_Comment)
        {
            secP->InsertComment(idx, KeyName_Gen_NDaysRetainLogs_Comment);
        }
    }

    return result;
}
//---------------------------------------------------------------------------
EErrINI TAppSettings::Load(const std::string& fileNameINI, const char assignOperator, size_t maxLineLen)
{
    EErrINI result = Inherited::Load(fileNameINI, assignOperator, maxLineLen);

    if (EErrINI::EI_NoError != result)
    {
        return result;
    }

    NeedsResaved = false;

    // Base has loaded the file - parse relevant fields, if present
    if (!ParseSection_General())
    {
        result = EErrINI::EI_ParseFail;
    }

    return result;
}
//---------------------------------------------------------------------------
EErrINI TAppSettings::Save(const std::string& fileNameINI, bool overWrite, const char assignOperator)
{
    EErrINI result = EErrINI::EI_NoError;

    if (!ApplyChanges_General())
    {
        result = EErrINI::EI_FailedApplyChanges;
    }

    // Save to disk now that changes are applied
    result = Inherited::Save(fileNameINI, overWrite, assignOperator);

    if (EErrINI::EI_NoError != result)
    {
        // Future - log error here
    }

    return result;
}
//---------------------------------------------------------------------------

} // namespace SweepThemMines
