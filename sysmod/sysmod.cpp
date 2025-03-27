#include "sysmod.h"

#include "../io/io.h"
#include "../console/ui.h"

#include <iostream>
#include <fstream>

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

//! G++ add to PATH
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
            if (RegQueryValueExA(hKey, "Path", NULL, &type, NULL, &size) == ERROR_SUCCESS)
            {
                if (type != REG_SZ)
                {
                    UI::errorMsg("The 'Path' registry value is not a string."); //! REMOVE
                    return;
                }
            }

            std::vector<char> currentMPath(size);

            if (RegQueryValueExA(hKey, "Path", NULL, &type, (LPBYTE)currentMPath.data(), &size) == ERROR_SUCCESS)
            {
                // Check if PATH already contains the program
                if (strstr(currentMPath.data(), ownDir.c_str()) != NULL) { return; }

                if (size == 0) { UI::errorMsg("addSelfToPath - RegQueryValueExA str"); }

                //! FINISH! ADD G++ PARENT FOLDER TO PATH
                
                std::string newPath = std::string(currentMPath.begin(), currentMPath.end()) + ";" + ownDir;
                if (RegSetValueExA(hKey, "Path", 0, REG_SZ, (LPBYTE)newPath.c_str(), newPath.size() + 1) == ERROR_SUCCESS)
                {
                    SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, NULL);
                    RegCloseKey(hKey);
                }
                else { UI::errorMsg("addSelfToPath - RegSetValueExA"); }
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

        // Check if PATH already contains the program
        std::ifstream inputFile(shellConfig);
        std::string line;
        while (std::getline(inputFile, line)) { if (line.find(ownDir) != std::string::npos) { inputFile.close(); return; } }
        
        //! FINISH! ADD G++ PARENT FOLDER TO PATH

        std::ofstream outputFile(shellConfig, std::ios::app);
        if (outputFile.is_open())
        {
            std::ofstream outputFile(shellConfig, std::ios::app);
            outputFile << "\nexport PATH=\"" << ownPath << ":$PATH\"\n";
            outputFile.close();
        }
        else { UI::errorMsg("addSelfToPath - ofstream"); }
    #endif

    saveLibGen();
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
        //! remove from PATH
    }
    catch(const std::exception& e)
    {
        UI::errorMsg("factoryReset");
    }
    
}