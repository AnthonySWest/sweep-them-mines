/* **************************************************************************
AppSettings.h
Author: Anthony S. West - ASW Software

Contains routines for saving and loading app settings.

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

#ifndef AppSettingsH
#define AppSettingsH
//---------------------------------------------------------------------------
#include <windows.h>
#include <string>
//---------------------------------------------------------------------------
#include "ASWTools_BasicINI.h"
#include "ASWTools_Version.h"
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TAppSettings
/////////////////////////////////////////////////////////////////////////////
class TAppSettings : public ASWTools::BasicINI::TBasicINI
{
private:
    typedef ASWTools::BasicINI::TBasicINI Inherited;

public: // Static variables
    static char const* const Default_DirLogs;
    static char const* const Default_LogPrefix;

    // Section names
    static char const* SectionName_General;

    // Key names - General
    static char const* const KeyName_Gen_ImagesPath;
    static char const* const KeyName_Gen_EnableCheats;
    static char const* const KeyName_Gen_UseQuestionMarksInit;
    static char const* const KeyName_Gen_DirLogs;
    static char const* const KeyName_Gen_LogPrefix;
    static char const* const KeyName_Gen_LogLevel;
    static char const* const KeyName_Gen_NDaysRetainLogs;

    //ini comments
    static char const* const KeyName_Gen_ImagesPath_Comment;
    static char const* const KeyName_Gen_LogLevel_Comment;
    static char const* const KeyName_Gen_NDaysRetainLogs_Comment;

private:
    void Destroy_Private();
    bool Reset_Private();

private:
    bool m_EnableCheatsWasPresent;

public:
    bool NeedsResaved;

    // General section vars
    std::string Gen_ImagesPath;
    bool Gen_EnableCheats;
    bool Gen_UseQuestionMarksInit;
    std::string Gen_DirLogs;
    std::string Gen_LogPrefix;
//    ELogMsgLevel Gen_LogLevel;
    DWORD Gen_LogLevel;
    DWORD Gen_NDaysRetainLogs;

public:
    TAppSettings();
    ~TAppSettings();

    void Destroy() override;
    bool Reset() override;

    bool ParseSection_General();

    bool ApplyChanges_General();

    ASWTools::BasicINI::EErrINI Load(const std::string& fileNameINI, const char assignOperator = DefaultAssignOperator,
        size_t maxLineLen = DefaultMaxLineLength) override;
    ASWTools::BasicINI::EErrINI Save(
        const std::string& fileNameINI, bool overWrite, const char assignOperator = DefaultAssignOperator) override;
};

} // namespace SweepThemMines

//---------------------------------------------------------------------------
#endif // #ifndef AppSettingsH
