#ifndef SYSMOD_H
#define SYSMOD_H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace sysmod
{
    // Detects if the program is ran for the first time
    bool firstRun();

    // Restarts the program with elevated privileges requested
    void restartWithAdmin();

    // Detects if the program is restarted after first run with elevated privileges
    bool firstRunWithAdmin();

    // Adds the program to the PATH environment variable
    void addSelfToPath();
}


#endif // SYSMOD_H