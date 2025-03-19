#include "sysmod.h"

#include "../io/io.h"
#include "../console/ui.h"

#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <shellapi.h>
#else
    #include <unistd.h>
#endif


bool sysmod::firstRun()
{
    // No admin rights, just check for own directory
    return IO::ownDirExists();
}

void sysmod::restartWithAdmin()
{
    #ifdef _WIN32
        wchar_t appPath[MAX_PATH];
        GetModuleFileNameW(NULL, appPath, MAX_PATH);

        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas"; // run as admin
        sei.lpFile = appPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (ShellExecuteExW(&sei)) { exit(0); }
        else { UI::errorMsg("restWAdmin - shellexecute"); }

    #else
        char appPath[1024];
        ssize_t pathLength = readlink("/proc/self/exe", appPath, sizeof(appPath) - 1);
        if (pathLength == -1) { UI::errorMsg("restWAdmin - readlink"); }
        appPath[pathLength] = '\0';
        
        std::string command = "sudo " + std::string(appPath);
        system(command.c_str());
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
    // add to PATH
    // create marking dir
}

void sysmod::saveLibGen()
{
    // generate folder in appdata
}