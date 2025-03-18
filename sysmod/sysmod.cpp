#include "sysmod.h"

#include "../io/io.h"

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


    #else


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