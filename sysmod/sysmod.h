#ifndef SYSMOD_H
#define SYSMOD_H

namespace sysmod
{
    // Checks Windows compatibility
    bool winSysSupported();
    
    // Detects if the program is ran for the first time
    bool firstRun();

    // Helper function for restarting the program when an error occurs
    void restartApp();

    // Adds the program to the PATH environment variable
    void addSelfToPath();

    // Creates a save lib and default config file (NO INTERNAL CODE, USINFIED PARSING FUNCS)
    void saveLibGen();

    // Factory resets VSCPA
    void factoryReset();
}

#endif // SYSMOD_H