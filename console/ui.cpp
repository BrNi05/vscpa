#include "ui.h"
#include "../sysmod/sysmod.h"
#include "../data/configFile.h"
#include "../io/io.h"

#include <iostream>
#include <thread>
#include <algorithm>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// Functions for console management an error messages //  

void UI::setConsoleTitle(std::string_view title)
{
    #ifdef _WIN32
        SetConsoleTitleA(title.data());
    #else
    if (isatty(fileno(stdout))) { std::cout << "\033]0;" << title << "\007" << std::flush; }
    #endif
}

void UI::clearConsole()
{
    #ifdef _WIN32
        try
        {
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
        }
        catch(const std::exception& e) { } // if opened in VSCode Terminal, this might throw an error   
    #else
        if (isatty(fileno(stdout))) { std::cout << "\033[2J\033[H" << std::flush; }
    #endif
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

    std::cout << Setup::PROMT << "\n" << Setup::DEFAULT_MODE << "\n" << Setup::NEW_MODE << std::endl;

    char input;
    std::cin >> input;
    input = toupper(input);
    clearConsole();

    switch (input)
    {
        case Setup::NEW_MODE.at(1):
            delete config;
            return startNewMode();
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

bool UI::startNewMode()
{
    ConfigFile config = ConfigFile();
    std::string inputBuffer;

    std::cout << Setup::EDIT1; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setMode(inputBuffer == "C" ? C : CPP);

    bool CStdSet = false;
    std::cout << Setup::EDIT1_2; std::cin >> inputBuffer; capitalize(inputBuffer);
    if (config.getMode() == CPP)
    {
        for (const auto& pair : CPPStdMap)
        {
            if (pair.second.find(inputBuffer) != std::string::npos) { config.setCPPStd(pair.first); CStdSet = true; break; }
        }
        if (!CStdSet) { config.setCPPStd(CPP23); } // default to CPP23 if no match found
        config.setCStd(C23); // in case the user switches to C mode later
    }
    else
    {
        for (const auto& pair : CStdMap)
        {
            if (pair.second.find(inputBuffer) != std::string::npos) { config.setCStd(pair.first); CStdSet = true; break; }
        }
        if (!CStdSet) { config.setCStd(C23); } // default to C23 if no match found
        config.setCPPStd(CPP23); // in case the user switches to CPP mode later
    }

    std::cout << Setup::EDIT2; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setHeaderInSubDirs((inputBuffer == "Y") || (inputBuffer == "y") ? true : false);

    std::cout << Setup::EDIT3; std::cin >> inputBuffer; capitalize(inputBuffer);
    config.setSrcInSubDirs((inputBuffer == "Y") || (inputBuffer == "y") ? true : false);

    std::cin.ignore();
    std::cout << Setup::EDIT4; std::getline(std::cin, inputBuffer);
    inputBuffer.erase(std::remove_if(inputBuffer.begin(), inputBuffer.end(), ::isspace), inputBuffer.end());
    inputBuffer.erase(std::remove(inputBuffer.begin(), inputBuffer.end(), '-'), inputBuffer.end());
    config.setDefines(inputBuffer);

    std::cout << Setup::EDIT5; std::getline(std::cin, inputBuffer);
    inputBuffer.erase(std::remove_if(inputBuffer.begin(), inputBuffer.end(), ::isspace), inputBuffer.end());
    inputBuffer.erase(std::remove(inputBuffer.begin(), inputBuffer.end(), '-'), inputBuffer.end());
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

    clearConsole();

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

// Helper functions //

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
    std::string url(INTERNAL::README_LINK);

    #ifdef _WIN32
        system(("start " + url).c_str());
    #elif __APPLE__
        system(("open " + url).c_str());
    #elif __linux__
        system(("xdg-open " + url).c_str());
    #endif
}