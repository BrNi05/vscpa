#include "io.h"

#include "../consts/consts.h"

#include <iostream>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    #include <shlobj.h>
    //#include <shlobj_core.h>
#else
    #include <unistd.h>
    #include <cstdlib.h>
#endif


bool IO::fastSetup = false;


// Config file related operations //

bool IO::defaultConfigExists()
{
    
}

ConfigFile* IO::loadConfigFile()
{
    // loads the config file if there is one created
    // returns an empty object if not

    fastSetup = true; // set
}


// JSON operations //

int IO::saveConfigFile()
{

}

int IO::generateVSCodeFiles()
{
    // and .vscode folder as well if not present
    // delete existing content (rmdir forced)
}


// File operations realted to the programs working //

std::filesystem::path IO::getAppdataPath()
{
    #ifdef _WIN32
        PWSTR path = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path)))
        {
            std::filesystem::path appDataPath = path;
            CoTaskMemFree(path);
            return appDataPath;
        }
        else
        {
            //! ERROR HANDLING - message and terminate
        }
    #else
        const char* path = std::getenv("HOME");
        if (path != nullptr)
        {
            return std::filesystem::path(path) / ".local/share";
        }
        else
        {
            //! ERROR HANDLING - message and terminate
        }

    #endif
}

bool IO::ownDirExists()
{
    return std::filesystem::exists(getAppdataPath() / IO::ownDirName) && std::filesystem::is_directory(getAppdataPath() / IO::ownDirName);
}

bool IO::deleteFastSetup()
{
    
}