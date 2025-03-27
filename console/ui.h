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
// Functions for console management an error messages //  

    // Sets the console title
    void setConsoleTitle(std::string_view title = UI::CONSOLE_DEFAULT_TITLE);

    // Clears the console
    void clearConsole();

    // Warns the user about first run and lack of admin privileges
    void warnFirstRun();

    // Generic error message function
    void errorMsg(std::string where);

    // Generic information message function
    void infoMsg(std::string_view msg, int delay = 2);

    // Exits the app after a delay
    void exitDelayed(int delay);

// Functions realted to setup sequence //

    // Starts setup sequence, called from main.cpp in normal circumstances
    bool setupSequence();

    // Immediately loads default config and generates files
    bool startDefaultMode(ConfigFile* config);

    // Guides user through the process of creating a new config
    bool startEditMode();
}

#endif // UI_H