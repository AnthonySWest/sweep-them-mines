/* **************************************************************************
App.cpp
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
#include "App.h"
//---------------------------------------------------------------------------
#include <mutex>
#include <shlobj.h> //for CSIDL_LOCAL_APPDATA and the like
#include <windows.h>
//---------------------------------------------------------------------------
#include "AppTool.h"
#include "ASWTools_Common.h"
#include "ConsoleHelper.h"
#include "PathTool.h"
#include "StringTool.h"
//---------------------------------------------------------------------------
using namespace ASWTools;
//---------------------------------------------------------------------------

namespace SweepThemMines
{

/////////////////////////////////////////////////////////////////////////////
// TApp
/////////////////////////////////////////////////////////////////////////////

TApp::EExitCode TApp::ExitCode = TApp::EExitCode::Success;
bool TApp::m_AppTerminating = false;
std::mutex TApp::m_locMutex_msgOut;

//---------------------------------------------------------------------------
TApp::TApp()
    : AppRunningMutexHandle(nullptr),
      AnotherInstanceAlreadyRunning(false),
      AppGlobalMutexName("Global\\ASWSoftware_SweepThemMines"),
      m_AppStartedByCommandLine(false)
{
}

//---------------------------------------------------------------------------
TApp::~TApp()
{
    FreeAppRunningMutex();
    Cleanup();
}

//---------------------------------------------------------------------------
// Static
int TApp::Callback_Log(void* thisP, const std::string& msg)
{
#if defined(USE_ELOG)
    ELogMsgLevel logLevel = static_cast<ELogMsgLevel>(thisP->LogLevel);
    return ELog.fputs(logLevel, msg.c_str());
#else
    return 0;
#endif
}

//---------------------------------------------------------------------------
// Static
void TApp::Callback_MessageOut(void* thisP, const std::string& msg, bool& cancel)
{
    m_locMutex_msgOut.lock();

    std::cout << msg;
    fflush(stdout);
#if defined(USE_ELOG)
    ELog.fprintf(ELogMsgLevel::LML_Debug, "%s", msg.c_str());
#endif

    if (m_AppTerminating)
        cancel = true;
    m_locMutex_msgOut.unlock();
}

//---------------------------------------------------------------------------
void TApp::Cleanup()
{
}

//---------------------------------------------------------------------------
bool TApp::CreateAppRunningMutex()
{
#if defined(USE_ELOG)
    const char* codeSectStr = "TApp::CreateAppRunningMutex";
#endif
    std::string mutexName = AppGlobalMutexName;
    DWORD lastErr;

    AnotherInstanceAlreadyRunning = false;

    if (nullptr != AppRunningMutexHandle)
    {
        AnotherInstanceAlreadyRunning = true;
        return true;
    }

    AppRunningMutexHandle = ::CreateMutexA(nullptr, FALSE, mutexName.c_str());
    if (nullptr == AppRunningMutexHandle)
    {
#if defined(USE_ELOG)
        lastErr = ::GetLastError();
        if (ERROR_ACCESS_DENIED == lastErr)
        {

            ELog.fprintf(ELogMsgLevel::LML_Light,
                "%s: Error: CreateMutexA() failed. WinErr: %u (0x%08lx) (ERROR_ACCESS_DENIED): \"%s\".\n",
                codeSectStr, lastErr, lastErr, TStrTool::GetWindowsLastErrorCodeAsStringA(lastErr).c_str());
        }
        else
        {
            ELog.fprintf(ELogMsgLevel::LML_Light, "%s: Error: CreateMutexA() failed. WinErr: %u (0x%08lx): \"%s\".\n",
                codeSectStr, lastErr, lastErr, TStrTool::GetWindowsLastErrorCodeAsStringA(lastErr).c_str());
        }
#endif // #if defined(USE_ELOG)

        return false;
    }

    // Find out if the mutex already existed (another instance already running)
    lastErr = ::GetLastError();
    if (ERROR_ALREADY_EXISTS == lastErr)
        AnotherInstanceAlreadyRunning = true;

    return true;
}

//---------------------------------------------------------------------------
void TApp::FreeAppRunningMutex()
{
    ::CloseHandle(AppRunningMutexHandle);
    AppRunningMutexHandle = nullptr;
}

//---------------------------------------------------------------------------
TVersion const* TApp::GetAppVer()
{
    return &m_AppVer;
}

//---------------------------------------------------------------------------
// Static
std::string TApp::GetHelpStr()
{
    std::string hlpStr = "";

    hlpStr.append("\n****************** Sweep Them Mines - Help *******************\n");
    hlpStr.append("\n Note: An ini file of the same base name as the app is expected to be in\n");
    hlpStr.append("     the exe dir, or under the appdata local dir (ASWSoftware\\SweepThemMines\\).\n");
    hlpStr.append("\n Note: Each command supports switches \"/\" or \"--\"\n");
    hlpStr.append("\n  /?.....................This help.");
#if defined(USE_ELOG)
    hlpStr.append("\n  /d1 /d2 /d3 or /d4.....Sets log level. d1 = errors/warnings. d4 = verbose.");
#endif
    //hlpStr.append(L"\n      Note: blah blah.\n");

    return hlpStr;
}

//---------------------------------------------------------------------------
TApp& TApp::GetInstance()
{
#if __cplusplus >= 201103L

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif // #ifdef __clang__
    // Meyers' Singleton - Thread safe and cleans up on exit since C++ 11
    static TApp singletonInstance;
#ifdef __clang__
#pragma clang diagnostic pop
#endif // #ifdef __clang__

    return singletonInstance;
#else
    // Old way - does not clean up on program exit, but is thread safe
    static TApp* sInstance = nullptr;

    if (nullptr == sInstance)
    {
        TApp* lp = new TApp();
        if (InterlockedCompareExchangePointer(reinterpret_cast <void* volatile*> (&sInstance), lp, nullptr) != nullptr)
            delete lp;
    }

    return *sInstance;
#endif // #if __cplusplus >= 201103L
}

//---------------------------------------------------------------------------
// "searchPhrase" supports Windows search wild cards (i.e. "logPrefix*")
// Intentionally not recursive
bool TApp::DeleteOldLogs(
    std::string const& dirLogs, DWORD numDaysToRetain, std::string const& searchPhrase, bool logSteps)
{
#if defined(USE_ELOG)
    const char* codeSectStr = "TApp::DeleteOldLogs";
#endif
    int numDeleted = 0;

    if (logSteps)
    {
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Medium, "%s: Enter: Days: %u, searchPhrase: \"%s\", dirLogs: \"%s\"\n",
            codeSectStr, numDaysToRetain, searchPhrase.c_str(), dirLogs.c_str());
#endif
    }

    if (0 == numDaysToRetain)
    {
        if (logSteps)
        {
#if defined(USE_ELOG)
            ELog.fprintf(ELogMsgLevel::LML_Medium,
                "%s: numDaysToRetain=0 - retaining forever. No logs deleted.\n", codeSectStr);
#endif
        }

        return true;     //retain forever
    }

    if (!RemoveAllFilesOlderThanNDays(dirLogs, numDaysToRetain, searchPhrase, numDeleted))
    {
        if (logSteps)
        {
#if defined(USE_ELOG)
            ELog.fprintf(ELogMsgLevel::LML_Light,
                "%s: Error: RemoveAllFilesOlderThanNDays() failed. Num old files deleted: %d\n",
                codeSectStr, numDeleted);
#endif
        }

        return false;
    }

    if (logSteps)
    {
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light, "%s: Exit: Num old files deleted: %d\n", codeSectStr, numDeleted);
#endif
    }

    return true;
}

//---------------------------------------------------------------------------
void TApp::Initialize(
    bool singleInstanceApp, std::string const& appSubDirPath, int argc, TCommandCharType const* const argv[])
{
    size_t nArgs = static_cast<size_t>(argc);
    TCommandCharType const* const* argvP = argv;

    for (size_t i = 0; i < nArgs; i++)
    {
        TCommandCharType const* cmd = argvP[i];
        TCommandArg cmdStr = cmd;

        m_CommandArgList.push_back(cmdStr);
    }

    Initialize(singleInstanceApp, appSubDirPath);
}

//---------------------------------------------------------------------------
void TApp::Initialize(bool singleInstanceApp, std::string const& appSubDirPath)
{
#if defined(USE_ELOG)
    const char* codeSectStr = "TApp::Initialize";
#endif
    unsigned short major = 0, minor = 0, build = 0, revision = 0;
    const size_t buffer_size = 999;
    char buffer[buffer_size];
    std::string tmpStr;

    m_AppStartedByCommandLine = TConsole::IsStandardInAConsole();
    //std::wcout << L"Is console: " << m_AppStartedByCommandLine << std::endl;

    SetInitialPaths(appSubDirPath);

    // get the app version
    if (TAppTool::GetAppVersion(AppPath.c_str(), &major, &minor, &build, &revision))
        m_AppVer.SetVersion(major, minor, build, revision);

    // set log title
    sprintf_s(buffer, buffer_size, "SweepThemMines - Version %d.%d.%d.%d", major, minor, build, revision);
    tmpStr = buffer;
#ifdef _DEBUG
    tmpStr.append(" - Debug");
#endif
    LogTitle = tmpStr;

    // set initial console title - will change later if the ini provides a sub title
    ::SetConsoleTitleA(LogTitle.c_str());

    // Pre-process the command line and early exit if applicable
    PreProcessCommandline();
    if (m_AppTerminating)
        return;

    // load INI and pre-process settings before processing command line
    if (!LoadAppSettings())
    {
        m_AppTerminating = true;
        ExitCode = EExitCode::Err_FailedLoadSettings;
        std::cout << "LoadAppSettings failed. Exiting with error." << std::endl;
        return;
    }

    if (Settings.Gen_ImagesPath.length() > 0 && BasicINI::TBasicINI::Dir_Exists_WinAPI(DirImages))
        DirImages = Settings.Gen_ImagesPath;

    // init the logger
    if (!InitLogger() && m_AppTerminating)
        return;

    // create app running mutex before processing the command line
    if (!CreateAppRunningMutex())
    {
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light, "%s: Error: AppMain::CreateAppRunningMutex() failed.\n", codeSectStr);
#endif
    }

    // fully process the command line
    ProcessCommandline();
    if (m_AppTerminating)
        return;

    // now that the command line was considered, terminate this instance if already running
    if (AnotherInstanceAlreadyRunning && singleInstanceApp)
    {
        m_AppTerminating = true;
        ::CloseHandle(TApp::AppRunningMutexHandle);
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light,
            "%s: Another instance of this app is already running. Terminating this instance.\n", codeSectStr);

        //clean up old log files even though the app is going to terminate early
        ELog.fprintf(ELogMsgLevel::LML_Light,
            "%s: Calling DeleteOldLogs() (early terminate - already running)...\n", codeSectStr);
#endif // #if defined(USE_ELOG)
        DeleteOldLogs(DirLogs, Settings.Gen_NDaysRetainLogs, TAppSettings::Default_LogPrefix + std::string("*"), true);

        printf_s("Another instance is already running. Terminating.\n");
        //printf_s("Press any key to exit...");
        //system("pause");
#if defined(USE_ELOG)
        ELog.Finalize("Exiting app...");
#endif // #if defined(USE_ELOG)
        return;
    }

    // /////////////// App initialized - do cool stuff - try to take over the world (but not really) ///////////////
}

//---------------------------------------------------------------------------
bool TApp::InitLogger()
{
#if defined(USE_ELOG)
    const wchar_t* codeSectStrW = L"TApp::InitLogger";
#endif
    std::string logPrefix = TAppSettings::Default_LogPrefix;
    std::string tmpStr;

    // get log dir
    tmpStr = Settings.Gen_DirLogs.length() > 0 ? Settings.Gen_DirLogs : TAppSettings::Default_DirLogs;

    if (!TPathTool::ExpandEnvironmentVars(tmpStr, DirLogs))
    {
        m_AppTerminating = true;
        ExitCode = EExitCode::Err_FailedSetupLog;
        std::cout << "Error: Failed attempt to expand log dir." << std::endl;
        return false;
    }

    if (TPathTool::IsRelative(DirLogs))
        DirLogs = TPathTool::Combine(DirApp, DirLogs);

    // set log name
    if (Settings.Gen_LogPrefix.length() > 0)
        logPrefix = Settings.Gen_LogPrefix;

    FileLog = DirLogs + logPrefix + TStrTool::GetDateTimeStr_LocalA(true) + ".txt";

    // std::cout << "Creating log dir and initialzing log..." << std::endl;

    // create log directory and initialize the logger
    if (BasicINI::TBasicINI::Dir_CreateDirWithSubs(DirLogs))
    {
#if defined(USE_ELOG)
        ELog.Initialize(FileLog.c_str(), LogTitle.c_str(), Settings.Gen_LogLevel);
        ELog.SetWriteThreadID(true);  // turns on thread ID, since this is a multi-threaded app

        ELog.fprintf(ELogMsgLevel::LML_Light, "AppPath:\t\"%s\"\n", AppPath.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "ADLocalBase: \"%s\"\n", DirAppDataLocalBase.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "TempBase:\t\"%s\"\n", DirTempBase.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "Temp:\t\"%s\"\n", DirTemp.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "PDataBase:  \"%s\"\n", DirProgramDataBase.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "ProgData:\t\"%s\"\n", DirProgramData.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "LogPath:\t\"%s\"\n", FileLog.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "PFBase64:\t\"%s\"\n", DirProgFilesBase64.c_str());
        ELog.fprintf(ELogMsgLevel::LML_Light, "Settings:\t\"%s\"\n", FileAppSettings.c_str());

        // log command line
        std::wstring cmdLineArgsStr;

        for (size_t i = 1; i < m_CommandArgList.size(); i++)
        {
            cmdLineArgsStr.append(m_CommandArgList[i]);
            cmdLineArgsStr.append(_T("\n"));
        }

        ELog.fwprintf(ELogMsgLevel::LML_Light, L"Commands:\n%s\n", cmdLineArgsStr.c_str());
        ELog.fwprintf(ELogMsgLevel::LML_Light, L"Help:\n%s\n\n", GetHelpStr().c_str());

        ELog.fwprintf(ELogMsgLevel::LML_Light, L"AppElevated: %s.\n", m_AppIsElevated ? "Yes" : "No");
        ELog.fwprintf(ELogMsgLevel::LML_Light, L"App started by commandline (maybe?): %s.\n",
            m_AppStartedByCommandLine ? L"Yes" : L"No");

        //std::wcout << L"Log: \"" << FileLogW.c_str() << L"\"" << std::endl;
#endif // #if defined(USE_ELOG)
    }
    else
    {
        // Even if logger fails to initialize, it is designed to fail gracefully.
        std::cout << "Error: Failed to init log: \"" << FileLog.c_str() << "\"" << std::endl;
        return false;
    }

    // clean up old log files
    //std::wcout << L"Deleting old logs..." << std::endl;
#if defined(USE_ELOG)
    ELog.fprintf(ELogMsgLevel::LML_Light, "%s: Calling DeleteOldLogs()...\n", codeSectStr);
#endif // #if defined(USE_ELOG)
    DeleteOldLogs(DirLogs, Settings.Gen_NDaysRetainLogs, logPrefix + "*", true);

    return true;
}

//---------------------------------------------------------------------------
bool TApp::IsAppStartedByCommandLine()
{
    return m_AppStartedByCommandLine;
}

//---------------------------------------------------------------------------
// Static
bool TApp::IsAppTerminating()
{
    return m_AppTerminating;
}

//---------------------------------------------------------------------------
bool TApp::LoadAppSettings()
{
#if defined(USE_ELOG)
    const char* codeSectStrW = "TApp::LoadAppSettings";
#endif
    char assignOperator = BasicINI::TBasicINI::DefaultAssignOperator;
    bool createINI = false;

    if (!BasicINI::TBasicINI::File_Exists_WinAPI(FileAppSettings))
    {
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light,
            "%s: Settings do not exist at: \"%s\"\n", codeSectStr, FileAppSettings.c_str());
#endif
        createINI = true;
    }
    else
    {
        BasicINI::EErrINI resINI = Settings.Load(FileAppSettings, assignOperator);
        if (BasicINI::EErrINI::EI_NoError != resINI)
        {
            //if (EErrINI::EI_FileNotExists == resINI)
            //{
            //    createINI = true; //ini file does not yet exists. Save new one out.
            //}
            //else
            {
#if defined(USE_ELOG)
                ELog.fprintf(ELogMsgLevel::LML_Light, "%s: Error: Settings.Load() failed for: \"%s\"\n",
                    codeSectStr, FileAppSettings.c_str());
#endif
                return false;
            }
        }
        else if (Settings.Sections.size() == 0)
        {
            createINI = true;     //no sections - INI is empty - recreate it.
        }
        else if (Settings.NeedsResaved)
        {
            createINI = true;
        }
    }

    if (createINI)
    {
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light, "%s: Creating/recreating initial INI...\n", codeSectStr);
#endif
        BasicINI::EErrINI resINI = Settings.Save(FileAppSettings, true, assignOperator);

        if (BasicINI::EErrINI::EI_NoError != resINI)
        {
#if defined(USE_ELOG)
            ELog.fprintf(ELogMsgLevel::LML_Light,
                "%s: Error: Settings.Save() failed. Unable to create initial INI.\n", codeSectStr);
#endif
            return false;
        }

#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light, "%s: INI created.\n", codeSectStr);
#endif
    }

    //test save
    //resINI = Settings.Save(FileNameAppSettings + "testCopy.ini", true, assignOperator);

    return true;
}

//---------------------------------------------------------------------------
void TApp::PreProcessCommandline()
{
    for (size_t i = 0, count = m_CommandArgList.size(); i < count; i++)
    {
        TCommandArg const& cmd = m_CommandArgList[i];

        if (TStrTool::Compare(_T("/?"), cmd) == 0 || TStrTool::Compare(_T("-?"), cmd) == 0 ||
            TStrTool::CompareIC(_T("/help"), cmd) == 0 || TStrTool::CompareIC(_T("--help"), cmd) == 0)
        {
            // output standard out, whatever that is (could be file or pipe redirection)
            std::wcout << GetHelpStr().c_str() << std::endl;

            // exit since asking for help commands
            //std::cout << std::endl << "Press any key to continue." << std::endl;
            //system("pause");
            ExitCode = EExitCode::Success;
            m_AppTerminating = true;
        }
    }
}

//---------------------------------------------------------------------------
void TApp::ProcessCommandline()
{
#if defined(USE_ELOG)
    const char* codeSectStr = "TApp::ProcessCommandline";
    ELogMsgLevel oldLogLevel = ELog.GetLogLevel();
    ELogMsgLevel newLogLevel = oldLogLevel;
#endif
    std::string tmpStr;

    // Certain commands can be executed without requiring the instance to be a "running" instance, as in, doesn't need
    // to continue running. Referred to as a "command" only instance. This app presently doesn't have any of these
    // commands, but be ready for it.
    bool terminateAfterProcessingCommand = false;

#if defined(USE_ELOG)
    ELog.fprintf(ELogMsgLevel::LML_Medium, "%s: Processing commandline...\n", codeSectStr);
#endif

    // Note: There are various command line styles. Windows commonly uses forward slash ("/") while other systems use
    // a single dash for a single character command and a double dash for word commands. A single dash can be followed
    // by one or more characters, with each character indicating a separate command. For example:
    // "app.exe -a -b" is the same as "app.exe -ab"
    // "app.exe --eol_console"
    // "app.exe /eol_console"
    //
    // This app is using the slash type of command procesing, but losely supports double dash.
    for (size_t cIdx = 1, count = m_CommandArgList.size(); cIdx < count; cIdx++)
    {
        TCommandArg const& cmd = m_CommandArgList[cIdx];

        //help already pre-checked - don't check here

        //check for debug level commands
#if defined(USE_ELOG)
        if (TStrTool::CompareIC(_T("/d1"), cmd) == 0 || TStrTool::CompareIC(_T("/d2"), cmd) == 0 ||
            TStrTool::CompareIC(_T("/d3"), cmd) == 0 || TStrTool::CompareIC(_T("/d4"), cmd) == 0)
        {
            if (TStrTool::CompareIC(_T("/d1"), cmd) == 0 || TStrTool::CompareIC(_T("--d1"), cmd) == 0)
                newLogLevel = ELogMsgLevel::LML_Light;
            else if (TStrTool::CompareIC(_T("/d2"), cmd) == 0 || TStrTool::CompareIC(_T("--d2"), cmd) == 0)
                newLogLevel = ELogMsgLevel::LML_Medium;
            else if (TStrTool::CompareIC(_T("/d3"), cmd) == 0 || TStrTool::CompareIC(_T("--d3"), cmd) == 0)
                newLogLevel = ELogMsgLevel::LML_Heavy;
            else if (TStrTool::CompareIC(_T("/d4"), cmd) == 0 || TStrTool::CompareIC(_T("--d4"), cmd) == 0)
                newLogLevel = ELogMsgLevel::LML_Debug;

            if (newLogLevel != oldLogLevel)
            {
                oldLogLevel = newLogLevel;
                ELog.SetLogLevel(newLogLevel);
                ELog.fprintf(ELogMsgLevel::LML_Medium, "%s: Per commandline, LogLevel changed to %d.\n",
                    codeSectStr, static_cast<int>(newLogLevel));
            }

            continue;
        }
#endif

        if (TStrTool::CompareIC(_T("/starwars"), cmd) == 0 || TStrTool::CompareIC(_T("--starwars"), cmd) == 0)
        {
            std::cout << "May the force be with you!" << std::endl;
        }

        if (TStrTool::CompareIC(_T("/portal"), cmd) == 0 || TStrTool::CompareIC(_T("--portal"), cmd) == 0)
        {
            std::cout << "The cake is a lie! Don't eat the cake!" << std::endl;
        }
    }

    // If going to terminate after processing some commands, close the mutex now so that a "running" instance is not
    // prevented from executing (in case some commands take a while to complete). One "running" instance is allowed.
    // Multiple "command" instances are allowed.
    if (terminateAfterProcessingCommand)
    {
        //not terminating yet - just free up mutex
        FreeAppRunningMutex();
    }

    // Exit early, if command line dictates
    if (terminateAfterProcessingCommand)
    {
        m_AppTerminating = true;
        tmpStr = "Commandlines processed. Exiting app early...";
        std::cout << tmpStr << std::endl;
    }
}

bool TApp::RemoveAllFilesOlderThanNDays(std::string const& dir, DWORD numDaysToRetainFiles,
    std::string const& searchPhrase, int& outNumFilesDeleted)
{
#if defined(USE_ELOG)
    const char* codeSectStr = "TApp::RemoveAllFilesOlderThanNDays";
#endif
    outNumFilesDeleted = 0;

    //defend against deleting files in a root drive
    if (dir.length() <= 3) //e.g. "C:\"
        return false;

    if (searchPhrase.length() == 0)
        return false;

    SYSTEMTIME currentDateTime;
    FILETIME currentDateTimeAsFileTime;
    const __int64 deleteOlderThanDaysLL = numDaysToRetainFiles;
    __int64 oldTime, newTime;
    std::string dirPath, fileName;

    ::GetLocalTime(&currentDateTime);
    ::SystemTimeToFileTime(&currentDateTime, &currentDateTimeAsFileTime);

    //get # of 100 nanosecond intervals in N days
    newTime = deleteOlderThanDaysLL * 60LL * 60LL * 24LL * 10000000LL;
    memcpy(&oldTime, &currentDateTimeAsFileTime, sizeof(oldTime));
    oldTime -= newTime;

    //ensure that directory ends with slash
    if (dir[dir.length() - 1] != '\\' && dir[dir.length() - 1] != '/')
        dirPath = dir + "\\";
    else
        dirPath = dir;

    //find and delete logs older than n days
    HANDLE hFind = nullptr;
    WIN32_FIND_DATAA findFileData;

    hFind = ::FindFirstFileA((dirPath + searchPhrase).c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        DWORD lastErr = ::GetLastError();

        if (lastErr == ERROR_FILE_NOT_FOUND)
            return true; //not found, nothing to delete
#if defined(USE_ELOG)
        ELog.fprintf(ELogMsgLevel::LML_Light,
            "%sError: FindFirstFileA() failed for \"%s\". WinLastErr: %u (0x%08lx): \"%s\".\n",
            codeSectStr, dir.c_str(), lastErr, lastErr, TStrTool::GetWindowsLastErrorCodeAsStringA(lastErr).c_str());
#endif
        return false;
    }

    bool keepSearching = true, result = true, isFirstFile = true;
    FILETIME lastWriteTime;

    while (keepSearching)
    {
        if (isFirstFile || ::FindNextFileA(hFind, &findFileData))
        {
            isFirstFile = false;

            if (TPathTool::IsDots(findFileData.cFileName))
                continue;

            if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                continue; //found a dir, skip

            fileName = dirPath + findFileData.cFileName;

            if (!BasicINI::TBasicINI::File_GetLastWriteTime(fileName, lastWriteTime))
            {
#if defined(USE_ELOG)
                ELog.fprintf(ELogMsgLevel::LML_Light,
                    "%sWarning: File_GetLastWriteTime() failed for file \"%s\". Skipped.\n",
                    codeSectStr, fileName.c_str());
#endif
                result = false;
                continue;
            }

            memcpy(&newTime, &lastWriteTime, sizeof(newTime));
            if (newTime >= oldTime)
                continue;

            //delete the file
            if (::DeleteFileA(fileName.c_str()) == 0)
            {
                DWORD lastErr = ::GetLastError();
                if (ERROR_FILE_NOT_FOUND != lastErr)
                {
#if defined(USE_ELOG)
                    ELog.fprintf(ELogMsgLevel::LML_Light,
                        "%sError: DeleteFileW() failed for \"%s\". WinLastErr: %u (0x%08lx): \"%s\".\n",
                        codeSectStr, fileName.c_str(), lastErr, lastErr,
                        TStrTool::GetWindowsLastErrorCodeAsStringA(lastErr).c_str());
#endif
                    result = false;
                }
                continue;
            }

#if defined(USE_ELOG)
            ELog.fprintf(ELogMsgLevel::LML_Heavy, "%sDeleted old file \"%s\".\n", codeSectStr, fileName.c_str());
#endif
            outNumFilesDeleted++;
        }
        else
        {
            DWORD lastErr = ::GetLastError();
            //check to make sure that there are no more files in current directory
            if (ERROR_NO_MORE_FILES != lastErr)
            {  //error occurred
#if defined(USE_ELOG)
                ELog.fprintf(ELogMsgLevel::LML_Light,
                    "%sError: FindNextFileW() failed for: \"%s\". WinLastErr: %u (0x%08lx): \"%s\".\n",
                    codeSectStr, fileName.c_str(), lastErr, lastErr,
                    TStrTool::GetWindowsLastErrorCodeAsStringA(lastErr).c_str());
#endif
                result = false;
            }
            keepSearching = false;
        }
    }

    ::FindClose(hFind);
#if defined(USE_ELOG)
    ELog.fprintf(ELogMsgLevel::LML_Debug, "%sNumber of old files deleted: %d\n", codeSectStr, numFilesDeleted);
#endif
    return result;
}

//---------------------------------------------------------------------------
void TApp::SetInitialPaths(std::string const& appSubDirPath)
{
    std::string tmpPath;

    // get full path, not local path, and not DOS path either
    tmpPath = TAppTool::GetAppPathA();
    if (tmpPath.length() == 0)
        tmpPath = TStrTool::UnicodeStrToUtf8(m_CommandArgList[0]); // fall back to local
    AppPath = tmpPath;

    DirApp = TPathTool::ExtractDir(AppPath);

    FileAppSettings_NoPath = TPathTool::ExtractFileName(AppPath, true);

    std::string appExt = TPathTool::GetExtension(FileAppSettings_NoPath);

    if (appExt.length() > 0 && (appExt == ".x86" || appExt == ".x64"))
        FileAppSettings_NoPath = TPathTool::ExtractFileName(FileAppSettings_NoPath, true);

    FileAppSettings_NoPath += ".ini";
    FileAppSettings = TPathTool::Combine(DirApp, FileAppSettings_NoPath);

    // Get app data local dir
    tmpPath = TPathTool::GetSpecialFolderDirA(CSIDL_LOCAL_APPDATA);
    DirAppDataLocalBase = TPathTool::Combine(tmpPath, "\\");
    DirAppData = TPathTool::Combine(tmpPath, appSubDirPath);

    // update the settings file path to be under app data if there isn't one local
    if (!BasicINI::TBasicINI::File_Exists_WinAPI(FileAppSettings))
        FileAppSettings = TPathTool::Combine(DirAppData, FileAppSettings_NoPath);

    // get program data dir
    tmpPath = TPathTool::GetSpecialFolderDirA(CSIDL_COMMON_APPDATA);
    DirProgramDataBase = TPathTool::Combine(tmpPath, "\\");
    DirProgramData = TPathTool::Combine(tmpPath, appSubDirPath);

    tmpPath = "";

    // get program files 64 bit, or 32 bit if running as 32bit
    if (!TPathTool::ExpandEnvironmentVars("%ProgramW6432%", tmpPath) || tmpPath.length() == 0)
    {
        // The environment path does not exist (why??). Use the special folders method which will be correct if this
        // app is 64 bit. However, if this app is 32 bit, it will point to the x86 folder. For 32 bit apps, hard code
        // because the above failed. There doesn't seem to be another way for a 32 bit app to get the 64 bit program
        // files directory.
#ifdef _WIN64
        tmpPath = TPathTool::GetSpecialFolderDirA(CSIDL_PROGRAM_FILES);
#else
        tmpPath = "C:\\Program Files";
#endif
    }

    DirProgFilesBase64 = TPathTool::Combine(tmpPath, "\\");

    // get temp dir
    DirTempBase = TPathTool::GetTempDirA();
    DirTemp = TPathTool::Combine(DirTempBase, appSubDirPath);
}

//---------------------------------------------------------------------------
void TApp::TerminateApp()
{
    m_AppTerminating = true;
}

//---------------------------------------------------------------------------

} // namespace SweepThemMines
