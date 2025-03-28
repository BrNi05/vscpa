#ifndef IO_H
#define IO_H

#include <filesystem>
#include <vector>

#include "../data/configFile.h"
#include "../data/common.hpp"

using Path = std::filesystem::path;

namespace IO
{
    extern Path ownDirPath;
    extern Path ownDirProfilesPath;
    extern Path fastSetupFilePath;
    
// Config file related operations //

    // Loads the config file if there is one
    ConfigFile* loadConfigFile();

    // Saves the config file
    void saveConfigFile(ConfigFile *config);

    // Checks for .c or .cpp files in the folder and sets the auto default config file accordingly (fallback)
    CMode detectCMode();

    // Finds gdb path
    Path findDebuggerPath();

    // Finds compiler path
    Path findCompilerPath(ConfigFile *config = nullptr);

    // Helper function for the two above
    Path pathFinder(std::vector<Path> possiblePaths, std::string fileName);

    // (Fallback) helper function for pathFinder
    Path pathFinderFallback(std::string compiler);

// JSON and save file operations //

    // Generates the VS Code Tasks and Launch files
    void generateVSCodeFiles(ConfigFile *config);

    // Checks if the fast setup marking file exists
    bool fastSetupExists();

    // Deletes the fast setup marking file
    void resetFastSetup();

// File operations realted to the programs working //

    // Return the path to the appdata/local folder
    Path getAppdataPath();

    // Checks if a program generated directory exists in appdata/local
    bool ownDirExists();

    // Returns the path to the stored default config file or an empty path if it does not exist
    Path defaultConfigPath();

    // Check if program is started from folder or canonical path
    bool startedFromFolder();
}

#endif // IO_H