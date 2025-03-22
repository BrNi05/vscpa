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

void UI::infoMsg(std::string msg)
{
    std::cout << msg << std::endl;
}


// Functions realted to setup sequence //

bool UI::setupSequence()
{
    ConfigFile* config = IO::loadConfigFile();

    if (IO::fastSetup) { return startDefaultMode(config); }

    std::cout << Setup::PROMT << "\n" << Setup::DEFAULT_MODE << "\n" << Setup::EDIT_MODE << std::endl;

    char input;
    std::cin >> input;
    switch (input)
    {
        case Setup::DEFAULT_MODE_CHAR:
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
    IO::generateVSCodeFiles(config);
    return true;
}

bool UI::startEditMode()
{
    ConfigFile config = ConfigFile();

    std::string langInput;
    std::cout << Setup::EDIT1; std::cin >> langInput;
    config.setMode(langInput == "C" ? C : CPP);

    std::string boolIn1;
    std::cout << Setup::EDIT2; std::cin >> boolIn1;
    config.setHeaderInSubDirs((boolIn1 == "Y") || (boolIn1 == "y") ? true : false);

    std::string boolIn2;
    std::cout << Setup::EDIT3; std::cin >> boolIn2;
    config.setSrcInSubDirs((boolIn2 == "Y") || (boolIn2 == "y") ? true : false);

    std::string definesIn;
    std::cout << Setup::EDIT4; std::cin >> definesIn;
    config.setDefines(definesIn);

    std::string otherArgsIn;
    std::cout << Setup::EDIT5; std::cin >> otherArgsIn;
    config.setOtherCompilerArgs(otherArgsIn);

    std::string outputPathIn;
    std::cout << Setup::EDIT6; std::cin >> outputPathIn;
    config.setOutputProgramName(outputPathIn);

    char endPromt;
    std::cout << Setup::CHOOSE_OPERATION << std::endl;
    std::cout << Setup::APPLY_TO_CURRENT_FOLDER << std::endl;
    std::cout << Setup::SAVE_AS_DEFAULT << std::endl;
    std::cout << Setup::SAVE << std::endl;
    std::cout << Setup::CANCEL << std::endl;
    std::cin >> endPromt;

    switch (endPromt)
    {
        case Setup::APPLY_TO_CURRENT_FOLDER.at(1):
            IO::generateVSCodeFiles(&config);
            return true;
        break;

        case Setup::SAVE_AS_DEFAULT.at(1):
            IO::saveConfigFile(&config);
            return true;
        break;

        case Setup::SAVE.at(1):
            IO::saveConfigFile(&config);
            IO::generateVSCodeFiles(&config);
            return true;
        break;

        case Setup::CANCEL.at(1):
            return false;
        break;

        default:
            return false;
        break;
    }
}