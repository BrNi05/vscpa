#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "../consts/consts.h"
#include "../data/configFile.h"

namespace UI
{
    // Sets the console title
    void setConsoleTitle(std::string title = UI::consoleDefaultTitle);

    // Clears the console
    void clearConsole();

    // Warns the user about first run and lack of admin privileges
    void warnFirstRun();

//
// Functions realted to setup sequence //
//

    // Starts setup sequence, called from main.cpp in normal circumstances
    bool setupSequence();

    // Immediately loads default config and generates files
    bool startDefaultMode(ConfigFile* config);

    // Guides user through the process of creating a new config
    bool startEditMode();

//
// Functions realted to modifications // 
//

    // Resets the fast setup flag
    bool resetFastSetup();

}

#endif // UI_H