/* **************************************************************************
App.h
Author: Anthony S. West - ASW Software

Contains app specific namespaced variables for use throughout the app

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

#ifndef AppH
#define AppH
//---------------------------------------------------------------------------
#include <mutex>
#include <string>
//---------------------------------------------------------------------------
#include "ASWTools_Version.h"
//---------------------------------------------------------------------------
#include "AppSettings.h"
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TApp
//
// Singleton class
/////////////////////////////////////////////////////////////////////////////
class TApp
{
public:
    typedef wchar_t TCommandCharType;
    typedef std::wstring TCommandArg;
    typedef std::vector<TCommandArg> TCommandArgList;

public:
    enum class EExitCode : int
    { //error codes 1 - 199 are reserved for errors
        //don't change existing values - add only - other apps might rely on these values
        NotSet = -1,
        Success = 0,
        Err_EnumStart = 1, //used for getting error start range

        Err_Init = 1,
        Err_FailedLoadSettings = 2,
        Err_FailedSetupLog = 3,

        Err_Exception = 198,
        Err_General = 199,

        Err_EnumEnd = 199 ///used for getting error end range
    };

public: // Static vars
    static EExitCode ExitCode;

private: // Static vars
    static bool m_AppTerminating;
    static std::mutex m_locMutex_msgOut;

public:
    HANDLE AppRunningMutexHandle;
    bool AnotherInstanceAlreadyRunning;
    std::string AppGlobalMutexName;

public: // Paths and files
    std::string AppPath;
    std::string LogTitle;

    std::string DirApp;
    std::string DirAppDataLocalBase;
    std::string DirAppData;
    std::string DirImages;
    std::string DirLogs;
    std::string DirProgramDataBase;
    std::string DirProgramData;
    std::string DirProgFilesBase64;
    std::string DirTempBase;
    std::string DirTemp;

    std::string FileAppSettings;
    std::string FileAppSettings_NoPath;
    std::string FileLog;

public:
    TAppSettings Settings;

private:
    TApp(); // Prevent creating instances outside this class

    bool m_AppStartedByCommandLine;

    ASWTools::TVersion m_AppVer;
    TCommandArgList m_CommandArgList;

    void Cleanup();
    void FreeAppRunningMutex();
    bool InitLogger();
    void PreProcessCommandline();
    void ProcessCommandline();

public:
    ~TApp();
    // Prevent copying by deleting the copy contructor and assignment operator
    TApp(const TApp&) = delete;
    TApp& operator=(const TApp&) = delete;

    bool CreateAppRunningMutex();
    ASWTools::TVersion const* GetAppVer();
    void Initialize(bool singleInstanceApp, std::string const& appSubDirPath);
    void Initialize(
        bool singleInstanceApp, std::string const& appSubDirPath, int argc, TCommandCharType const* const argv[]);
    bool IsAppStartedByCommandLine();
    bool LoadAppSettings();
    void SetInitialPaths(std::string const& appSubDirPath);
    void TerminateApp();

public: // Static functions
    static int Callback_Log(void* thisP, const std::string& msg);
    static void Callback_MessageOut(void* thisP, const std::string& msg, bool& cancel);
    static bool DeleteOldLogs(
        std::string const& dirLogs, DWORD numDaysToRetain, std::string const& searchPhrase, bool logSteps);
    static std::string GetHelpStr();
    static TApp& GetInstance();
    static bool IsAppTerminating();
    static bool RemoveAllFilesOlderThanNDays(std::string const& dir, DWORD numDaysToRetainFiles,
        std::string const& searchPhrase, int& outNumFilesDeleted);
};

} // namespace SweepThemMines

#endif // #ifndef AppH
