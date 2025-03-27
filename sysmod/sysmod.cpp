#include "sysmod.h"

#include "../io/io.h"
#include "../console/ui.h"

#include <iostream>
#include <fstream>
#include <thread>

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
        OSVERSIONINFOEXW osvi = { sizeof(OSVERSIONINFOEXW), 10, 0, 22621 };
        DWORDLONG conditionMask = VerSetConditionMask(0, VER_BUILDNUMBER, VER_GREATER_EQUAL);
        return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, conditionMask);
    #else
        return true;
    #endif
}

bool sysmod::firstRun()
{
    // No admin rights, just check for own directory
    return !IO::ownDirExists();
}

void sysmod::restartWithAdmin()
{
    restartApp(true);
}

void sysmod::restartApp(bool withAdmin)
{
    #ifdef _WIN32
        wchar_t appPath[MAX_PATH];
        GetModuleFileNameW(NULL, appPath, MAX_PATH);

        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        if (withAdmin) { sei.lpVerb = L"runas"; }
        sei.lpFile = L"wt.exe";
        sei.lpParameters = appPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (ShellExecuteExW(&sei)) { exit(0); }
        else { UI::errorMsg("restWAdmin - shellexecute"); }
    #else
        char appPath[1024];
        ssize_t pathLength = readlink("/proc/self/exe", appPath, sizeof(appPath) - 1);
        if (pathLength == -1) { UI::errorMsg("restWAdmin - readlink"); }
        appPath[pathLength] = '\0';
        
        if (withAdmin) { execlp("sudo", "sudo", appPath, NULL); }
        else { execlp(appPath, appPath, NULL); }

        //std::string command = withAdmin ? "sudo " + std::string(appPath) : std::string(appPath);
        //system(command.c_str());
        exit(0);
    #endif
}

bool sysmod::firstRunWithAdmin()
{
    if (!firstRun()) { return false; }
    else
    {
        #ifdef _WIN32
            bool isAdmin = false;
            HANDLE hToken = NULL;
            if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
            {
                TOKEN_ELEVATION Elevation;
                DWORD cbSize = sizeof(TOKEN_ELEVATION);
                if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize))
                {
                    isAdmin = Elevation.TokenIsElevated;
                }
            }
            if (hToken) { CloseHandle(hToken); }

            if (isAdmin) { return true; }
            else { return false; }
        #else
            return geteuid() == 0;
        #endif
    }
}

void sysmod::addSelfToPath()
{
    #ifdef _WIN32
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        std::string ownDir = std::string(exePath);
        ownDir = ownDir.substr(0, ownDir.find_last_of("\\"));
    
        HKEY hKey;
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_SET_VALUE | KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
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
                if (pathQuery.find(compilerPath) == std::string::npos) { pathQuery += (";" + ownDir + ";" + compilerPath + "\\"); }
                else { pathQuery += (";" + ownDir + "\\"); }

                if (RegSetValueExA(hKey, "Path", 0, REG_SZ, (LPBYTE)pathQuery.c_str(), pathQuery.size() + 1) == ERROR_SUCCESS) { RegCloseKey(hKey); }
                else { RegCloseKey(hKey); UI::errorMsg("addSelfToPath - RegSetValueExA"); }
            }
            else { UI::errorMsg("addSelfToPath - RegQueryValueExA"); }
        }
        else { UI::errorMsg("addSelfToPath - RegOpenKeyExA"); }

    #else
        char exePath[1024] = {0};

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

        // Check if PATH already contains the program and g++
        bool compilerInPath = false;
        std::ifstream inputFile(shellConfig);
        std::string line;
        while (std::getline(inputFile, line))
        {
            if (line.find(ownDir) != std::string::npos) { inputFile.close(); return; }
            if (line.find("g++") != std::string::npos) { compilerInPath = true; }
        }
        
        std::string compilerPath = IO::findCompilerPath(nullptr).parent_path().string();

        std::ofstream outputFile(shellConfig, std::ios::app);
        if (outputFile.is_open())
        {
            std::ofstream outputFile(shellConfig, std::ios::app);
            outputFile << "\nexport PATH=\"" << ownDir << ":$PATH\"\n";
            if (!compilerInPath) { outputFile << "export PATH=\"" << compilerPath << ":$PATH\"\n"; }
            outputFile.close();
        }
        else { UI::errorMsg("addSelfToPath - ofstream"); }
    #endif

    saveLibGen();
}

void sysmod::addSelfToPathHelper()
{
    #ifdef _WIN32
        std::this_thread::sleep_for(std::chrono::seconds(1));
        system("taskkill /f /im explorer.exe");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        system("start explorer.exe");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    #else
        
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
        UI::infoMsg(UI::FACTORY_RESET_EXTRA);;
    }
    catch(const std::exception& e)
    {
        UI::errorMsg("factoryReset");
    }
}