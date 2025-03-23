#ifndef SYSMOD_H
#define SYSMOD_H

#include <string>

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

    // Helper function for restarting the program with elevated privileges and error handling
    void restartApp(bool withAdmin = false);

    // Detects if the program is restarted after first run with elevated privileges
    bool firstRunWithAdmin();

    // Adds the program to the PATH environment variable
    void addSelfToPath();

    // Creates a save lib and default config file (NO INTERNAL CODE, USINFIED PARSING FUNCS)
    void saveLibGen();
}

#endif // SYSMOD_H