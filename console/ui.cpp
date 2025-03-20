#include "ui.h"
#include "../sysmod/sysmod.h"
#include "../data/configFile.h"
#include "../io/io.h"

#include <iostream>
#include <thread>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#endif

// Functions for console management an error messages //  

void UI::setConsoleTitle(std::string_view title)
{
    #ifdef _WIN32
        SetConsoleTitleA(title.data());
    #else
        std::cout << "\033]0;" << title.data() << "\007";
    #endif
}

void UI::clearConsole()
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = {0, 0};

        if (hConsole == INVALID_HANDLE_VALUE) { UI::errorMsg("console clearing - handle"); }

        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) { UI::errorMsg("console clearing - buffer info"); }
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count);
        FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);

        SetConsoleCursorPosition(hConsole, homeCoords);
    #else
        std::cout << "\033[2J\033[H" << std::flush;
    #endif
}

void UI::warnFirstRun()
{
    std::cout << UI::GREETING_1 << std::endl << UI::GREETING_2 << std::endl;
        
    std::this_thread::sleep_for(std::chrono::seconds(3));
    sysmod::restartWithAdmin();
}

void UI::errorMsg(std::string where)
{
    std::cout << UI::ERROR_MSG_1 << where << std::endl;
    std::cout << UI::ERROR_MSG_2 << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    sysmod::restartApp();
}


// Functions realted to setup sequence //

bool UI::setupSequence()
{
    // User inputs run mode

    ConfigFile* config = IO::loadConfigFile();
    if (config->getFastSetup())
    {
        return startDefaultMode(config);
    }

    std::cout << Setup::PROMT << "\n" << Setup::DEFAULT_MODE << "\n" << Setup::EDIT_MODE << std::endl;

    char input;
    std::cin >> input;
    switch (input)
    {
        case Setup::dDEFAULT_MODE_CHAR:
            return startDefaultMode(config);
        break;

        case Setup::EDIT_MODE_CHAR:
            return startEditMode();
        break;

        default:
            return startDefaultMode(config);
        break;
    }
}

bool UI::startDefaultMode(ConfigFile* config)
{
    // load
    // if no config file, create one based on extensions in the folder, cpp fallback if norting is known

    return false;
}

bool UI::startEditMode()
{
    ConfigFile config = ConfigFile(false);

    // user input and promting

    char endPromt;
    std::cout << "Choose operation:" << std::endl;
    std::cout << "  A - Apply to current folder" << std::endl;
    std::cout << "  D - Save as default" << std::endl;
    std::cout << "  S - Save as default and apply" << std::endl;
    std::cout << "  C - Cancel and exit" << std::endl;
    std::cin >> endPromt;

    switch (endPromt)
    {
        case 'A':
            IO::generateVSCodeFiles();
            return true;
        break;

        case 'D':
            IO::saveConfigFile();
            return true;
        break;

        case 'S':
            IO::saveConfigFile();
            IO::generateVSCodeFiles();
            return true;
        break;

        case 'C':
            return false;
        break;

        default:
            return false;
        break;
    }
}


// Functions realted to modifications // 

void UI::resetFastSetup()
{
    IO::deleteFastSetup();
    // modify the config file

}