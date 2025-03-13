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

namespace UI
{
    // Sets the console title
    void setConsoleTitle(std::string title = UI::consoleDefaultTitle);

    // Warns the user about first run and lack of admin privileges
    void warnFirstRun();

    // Starts setup sequence, called from main.cpp in normal circumstances
    void setupSequence();
}

#endif // UI_H