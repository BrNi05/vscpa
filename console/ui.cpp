#include "ui.h"
#include "../sysmod/sysmod.h"
#include "../data/configFile.h"
#include "../io/io.h"

#include <iostream>
#include <thread>
#include <stdlib.h>
#include <algorithm>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#endif

// Functions for console management an error messages //  

void UI::setConsoleSize(int width, int height)
{
    #ifdef _WIN32
        // Windows terminal cannot be resized from code
    #else
        std::cout << "\033[8;" << height << ";" << width << "t" << std::flush;
    #endif
}

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
    infoMsg(UI::GREETING_1, 0); infoMsg(UI::GREETING_2);
    sysmod::restartWithAdmin();
}

void UI::errorMsg(std::string where)
{
    std::cout << UI::ERROR_MSG_1 << where << std::endl;
    infoMsg(UI::ERROR_MSG_2);
    sysmod::restartApp();
}

void UI::infoMsg(std::string_view msg, int delay)
{
    std::cout << msg << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(delay));
}

void UI::exitDelayed(int delay)
{
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    exit(0);
}

// Functions realted to setup sequence //

bool UI::setupSequence()
{
    ConfigFile* config = IO::loadConfigFile();

    if (IO::fastSetupExists()) { return startDefaultMode(config); }

    std::cout << Setup::PROMT << "\n" << Setup::DEFAULT_MODE << "\n" << Setup::EDIT_MODE << std::endl;

    char input;
    std::cin >> input;
    input = toupper(input);

    switch (input)
    {
        case Setup::EDIT_MODE.at(1):
            delete config;
            return startEditMode();
        break;

        case Setup::DEFAULT_MODE.at(1):
        default:
            return startDefaultMode(config);
        break;
    }
}

bool UI::startDefaultMode(ConfigFile* config)
{
    IO::generateVSCodeFiles(config);
    delete config;
    return true;
}

bool UI::startEditMode()
{
    ConfigFile config = ConfigFile();
    std::string inputBuffer;

    std::cout << Setup::EDIT1; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setMode(inputBuffer == "C" ? C : CPP);

    std::cout << Setup::EDIT2; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setHeaderInSubDirs((inputBuffer == "Y") || (inputBuffer == "y") ? true : false);

    std::cout << Setup::EDIT3; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setSrcInSubDirs((inputBuffer == "Y") || (inputBuffer == "y") ? true : false);

    std::cin.ignore();
    std::cout << Setup::EDIT4; std::getline(std::cin, inputBuffer);
    inputBuffer.erase(std::remove_if(inputBuffer.begin(), inputBuffer.end(), ::isspace), inputBuffer.end());
    config.setDefines(inputBuffer);

    std::cout << Setup::EDIT5; std::getline(std::cin, inputBuffer);
    inputBuffer.erase(std::remove_if(inputBuffer.begin(), inputBuffer.end(), ::isspace), inputBuffer.end());
    config.setOtherCompilerArgs(inputBuffer);

    std::cout << Setup::EDIT6; std::cin >> inputBuffer;
    config.setOutputProgramName(inputBuffer);

    std::cout << Setup::EDIT7; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setExternalConsole((inputBuffer == "Y") || (inputBuffer == "y") ? true : false);

    char endPromt;
    std::cout << Setup::CHOOSE_OPERATION << std::endl;
    std::cout << Setup::APPLY_TO_CURRENT_FOLDER << std::endl;
    std::cout << Setup::SAVE_AS_DEFAULT << std::endl;
    std::cout << Setup::SAVE << std::endl;
    std::cout << Setup::CANCEL << std::endl;
    std::cin >> endPromt; capitalize(endPromt);

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
        default:
            return false;
        break;
    }
}

// Helper functions

void UI::capitalize(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void UI::capitalize(char& c)
{
    c = toupper(c);
}

void UI::openGitHubPage()
{
    std::string url = "https://github.com/BrNi05/proj-assist";

    #ifdef _WIN32
        std::string command = "start " + url;
    #elif __APPLE__
        std::string command = "open " + url;
    #elif __linux__
        std::string command = "xdg-open " + url;
    #endif

    system(command.c_str());
}