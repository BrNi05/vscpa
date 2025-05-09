#ifndef UI_H
#define UI_H

#include <string>
#include <string_view>

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

    // Generic error message function
    void errorMsg(std::string where);

    // Generic information message function, program is paused for delay milliseconds
    void infoMsg(std::string_view msg, int delay = 2000);

    // Exits the app after a delay
    void exitDelayed(int delay);

// Functions realted to setup sequence //

    // Starts setup sequence, called from main.cpp in normal circumstances
    bool setupSequence();

    // Immediately loads default config and generates files
    bool startDefaultMode(ConfigFile* config);

    // Guides user through the process of creating a new config
    bool startNewMode();

// Helper functions

    // capitalize string
    void capitalize(std::string& str);

    // capitalize char
    void capitalize(char& c);

    // Opens browser with the GitHub page of the project
    void openGitHubPage();
}

#endif // UI_H