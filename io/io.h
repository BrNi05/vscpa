#ifndef IO_H
#define IO_H

#include "../data/configFile.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace IO
{
    extern bool fastSetup;
    
    // 
    // Config file related operations //
    //

    // Checks if the default config file (and directories) exists
    bool defaultConfigExists();

    // Loads the config file if there is one
    ConfigFile* loadConfigFile();

    //
    // JSON operations //
    //
    
    // Saves the config file
    int saveConfigFile();

    // Generates the VS Code Tasks and Launch files
    int generateVSCodeFiles();

    //
    // File operations realted to the programs working //
    //

    // Deletes the fast setup marking file
    bool deleteFastSetup();
}

#endif // IO_H