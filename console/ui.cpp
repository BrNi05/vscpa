#include "ui.h"
#include "../sysmod/sysmod.h"
#include "../data/configFile.h"
#include "../io/io.h"

#include <iostream>
#include <thread>

#ifdef _WIN32
    #include <windows.h>
#endif

void UI::setConsoleTitle(std::string title)
{
    #ifdef _WIN32
        SetConsoleTitleA(title.c_str());
    #else
        std::cout << "\033]0;" << title << "\007";
    #endif
}

void clearConsole()
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD count;
        DWORD cellCount;
        COORD homeCoords = {0, 0};

        if (hConsole == INVALID_HANDLE_VALUE) { } //! MESSAGE AND TERMINATE

        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) { } //! MESSAGE AND TERMINATE
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
    std::cout << UI::greeting1 << std::endl << UI::greeting2 << std::endl;
        
    std::this_thread::sleep_for(std::chrono::seconds(3));
    sysmod::restartWithAdmin();
}

bool UI::setupSequence()
{
    // User inputs run mode

    ConfigFile* config = IO::loadConfigFile();
    if (config->getFastSetup())
    {
        return startDefaultMode(config);
    }

    std::cout << "Choose a run mode:" << std::endl;
    std::cout << "D - Default mode" << std::endl;
    std::cout << "E - Edit mode" << std::endl;

    char input;
    std::cin >> input;
    switch (input)
    {
        case 'D':
            return startDefaultMode(config);
        break;

        case 'E':
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

bool UI::resetFastSetup()
{
    bool success = true;

    // delete the fast setup flag from the default config file if existing
    // modify the config file

    return success;
}