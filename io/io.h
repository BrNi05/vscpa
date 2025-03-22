#ifndef IO_H
#define IO_H

#include <filesystem>
#include "../data/configFile.h"
#include "../data/common.hpp"

namespace IO
{
    extern bool fastSetup;
    
// Config file related operations //

    // Loads the config file if there is one
    ConfigFile* loadConfigFile();

    // Checks for .c or .cpp files in the folder and sets the auto default config file accordingly (fallback)
    CMode detectCMode();

    // Finds gcc/g++ path
    std::string findCompilerPath();

// JSON operations //
    
    // Saves the config file
    void saveConfigFile(ConfigFile *config);

    // Generates the VS Code Tasks and Launch files
    void generateVSCodeFiles(ConfigFile *config);

    // Checks if the fast setup marking file exists
    bool fastSetupExists();

    // Deletes the fast setup marking file
    void resetFastSetup();

// File operations realted to the programs working //

    // Return the path to the appdata/local folder
    std::filesystem::path getAppdataPath();

    // Checks if a program generated directory exists in appdata/local
    bool ownDirExists();

    // Returns the path to the stored default config file or an empty path if it does not exist
    std::filesystem::path defaultConfigPath();

    // Check if program is started from folder or canonical path
    bool startedFromFolder();
}

#endif // IO_H