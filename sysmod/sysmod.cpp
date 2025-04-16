#include "sysmod.h"

#include "../io/io.h"
#include "../console/ui.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #include <shlobj.h>
    #include <shellapi.h>
#elif __APPLE__
    #include <mach-o/dyld.h>
    #include <unistd.h>
#else
    #include <unistd.h>
#endif


bool sysmod::winSysSupported()
{
    #ifdef _WIN32
        #ifndef _WIN64
            return false;
        #endif
        OSVERSIONINFOEXW osvi = { sizeof(OSVERSIONINFOEXW), 10, 0, 10240 };
        DWORDLONG conditionMask = VerSetConditionMask(0, VER_BUILDNUMBER, VER_GREATER_EQUAL);
        return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, conditionMask);
    #else
        return true;
    #endif
}

bool sysmod::firstRun()
{
    return !IO::ownDirExists();
}

void sysmod::restartApp()
{
    #ifdef _WIN32
        wchar_t appPath[Sysmod::_MAX_PATH_];
        GetModuleFileNameW(NULL, appPath, MAX_PATH);

        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpFile = L"wt.exe";
        sei.lpParameters = appPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (ShellExecuteExW(&sei)) { exit(0); }
        else { UI::errorMsg("restart - shellexecute"); }
    #elif __APPLE__
        char appPath[Sysmod::_MAX_PATH_] = {0};
        uint32_t size = sizeof(appPath);
        if (_NSGetExecutablePath(appPath, &size) != 0) { UI::errorMsg("restart - _NSGetExecutablePath"); }
        execl(appPath, appPath, NULL);
        exit(0);
    #else
        char appPath[Sysmod::_MAX_PATH_] = {0};
        ssize_t pathLength = readlink("/proc/self/exe", appPath, sizeof(appPath) - 1);
        if (pathLength == -1) { UI::errorMsg("restart - readlink"); }
    
        execl(appPath, appPath, NULL);
        exit(0);
    #endif
}

void sysmod::addSelfToPath()
{
    #ifdef _WIN32
        char exePath[Sysmod::_MAX_PATH_];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        std::string ownDir = std::string(exePath);
        ownDir = ownDir.substr(0, ownDir.find_last_of("\\"));
    
        HKEY hKey;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0, KEY_SET_VALUE | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
        {
            DWORD size;
            DWORD type;

            //Determine size
            if (!RegQueryValueExA(hKey, "Path", NULL, &type, NULL, &size) == ERROR_SUCCESS) { UI::errorMsg("addSelfToPath - RegQueryValueExA size"); }

            std::vector<char> currentMPath(size);

            if (RegQueryValueExA(hKey, "Path", NULL, &type, (LPBYTE)currentMPath.data(), &size) == ERROR_SUCCESS)
            {
                // Check if PATH already contains the program
                std::string pathQuery(currentMPath.begin(), currentMPath.end() - 1);
                if (pathQuery.find(ownDir) != std::string::npos) { return; }

                if (size == 0) { UI::errorMsg("addSelfToPath - RegQueryValueExA str"); }

                std::string compilerPath = IO::findCompilerPath(nullptr).parent_path().string();
                if (pathQuery.find(compilerPath) == std::string::npos) { pathQuery += (";" + ownDir + "\\" + ";" + compilerPath + "\\"); }
                else { pathQuery += (";" + ownDir + "\\"); }

                if (RegSetValueExA(hKey, "Path", 0, REG_EXPAND_SZ, (LPBYTE)pathQuery.c_str(), pathQuery.size() + 1) == ERROR_SUCCESS) { RegCloseKey(hKey); }
                else { RegCloseKey(hKey); UI::errorMsg("addSelfToPath - RegSetValueExA"); }
            }
            else { UI::errorMsg("addSelfToPath - RegQueryValueExA"); }
        }
        else { UI::errorMsg("addSelfToPath - RegOpenKeyExA"); }

        SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 4000, nullptr);

    #else
        char exePath[Sysmod::_MAX_PATH_] = {0};

        #ifdef __APPLE__
            uint32_t size = sizeof(exePath);
            if (_NSGetExecutablePath(exePath, &size) != 0) { UI::errorMsg("addSelfToPath - _NSGetExecutablePath"); }
        #elif __linux__
            ssize_t pathLength = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
            if (pathLength == -1) { throw std::runtime_error("addSelfToPath - readlink failed"); }
            exePath[pathLength] = '\0';
        #endif

        std::string ownDir = std::string(exePath);
        ownDir = ownDir.substr(0, ownDir.find_last_of("/"));

        std::string homeDir = std::string(getenv("HOME"));
        if (homeDir.empty()) { UI::errorMsg("addSelfToPath - getenv-HOME"); }

        std::string shellConfig;
        const char* shell = getenv("SHELL");
        if (shell && strstr(shell, "zsh")) { shellConfig = homeDir + "/.zshrc"; }
        else { shellConfig = homeDir + "/.bashrc"; }

        // Read the shell config
        std::ifstream inputFile(shellConfig);
        std::string content;
        if(inputFile)
        {
            std::string line;
            while (std::getline(inputFile, line)) { content += line + "\n"; }
            inputFile.close();
        }
        else { UI::errorMsg("addSelfToPath - ifstream"); }

        // Check if PATH already contains the program
        if (content.find(ownDir) != std::string::npos) { return; }

        std::ofstream outputFile(shellConfig, std::ios::trunc);
        if (outputFile)
        {
            outputFile << "export PATH=\"" << ownDir << ":$PATH\"\n";
            outputFile << content; // original content writeback
            outputFile.close();
        }
        else { UI::errorMsg("addSelfToPath - ofstream"); }
    #endif
}

void sysmod::saveLibGen()
{
    try
    {
        std::filesystem::create_directory(IO::ownDirPath);
        std::filesystem::create_directory(IO::ownDirProfilesPath);
    }
    catch(const std::exception& e)
    {
        UI::errorMsg("saveLibGen - create_directory");
    }
}

void sysmod::factoryReset()
{
    try
    {
        std::filesystem::remove_all(IO::ownDirPath);
        UI::infoMsg(UI::FACTORY_RESET_EXTRA, 1500);
    }
    catch(const std::exception& e)
    {
        UI::errorMsg("factoryReset");
    }
}