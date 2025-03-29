#include "./json/json.hpp"
#include "./sysmod/sysmod.h"
#include "./consts/consts.h"
#include "./console/ui.h"
#include "./io/io.h"

#include <iostream>

int main(int argc, char* argv[])
{
    // BETA release - disable execution on not tested platforms
    #if defined(__linux__) || defined(__APPLE__)
        UI:infoMsg("BETA release! Only Win11 is supported, yet. Exiting...");
        UI::exitDelayed(0);
    #endif

    bool canExit = false;
    
    // Resize for initial setup, but not for normal use
    if (IO::startedFromFolder())
    {
        UI::setConsoleSize(90, 40);
        UI::setConsoleTitle(UI::CONSOLE_DEFAULT_TITLE);
    }
    
    // Check for Win support (Win11 only)
    if (!sysmod::winSysSupported())
    {
        UI::infoMsg(UI::WINSYS_NOT_SUPPORTED);
        UI::exitDelayed(0);
    }

    // Runs after install
    if (sysmod::firstRunWithAdmin())
    {
        sysmod::addSelfToPath();
        sysmod::saveLibGen();
        UI::infoMsg(UI::ADMIN_SUCCESS, 0);

        UI::infoMsg(UI::ADMIN_WARNING_2, 0);
        UI::infoMsg(UI::ADMIN_WARNING_2_EXTRA, 0);
        UI::infoMsg(UI::ADMIN_WARNING, 0);
        char input; std::cin >> input;
        if (input == 'Y' || input == 'y')
        {
            sysmod::addSelfToPathHelper();
            UI::infoMsg(UI::ADMIN_SUCCESS_2Y_1, 0);
            UI::infoMsg(UI::ADMIN_SUCCESS_2Y, 1);
        }
        else { UI::infoMsg(UI::ADMIN_SUCCESS_2N, 1); }

        UI::exitDelayed(2);
    }
    else if (sysmod::firstRun())
    {
        UI::warnFirstRun();
    }
    else
    {
        sysmod::saveLibGen(); // make sure own folders do exist
        
        if (argc >= 2)
        {
            if (std::string(argv[1]) == Args::FAST_SETUP_OFF)
            {
                IO::resetFastSetup();
                UI::infoMsg(UI::RESET_SUCCESS, 0);
                UI::exitDelayed(1);
            }
            else if (std::string(argv[1]) == Args::FAST_SETUP_ON)
            {
                IO::enableFastSetup();
                UI::infoMsg(UI::ENABLE_SUCCESS, 0);
                UI::exitDelayed(1);
            }
            else if (std::string(argv[1]) == Args::FACTORY)
            {
                sysmod::factoryReset();
                UI::infoMsg(UI::FACTORY_RESET_SUCCESS, 0);
                UI::exitDelayed(1);
            }
            else if (std::string(argv[1]) == Args::HELP)
            {
                UI::infoMsg(UI::HELP_MENU, 1);
                UI::openGitHubPage();
                UI::exitDelayed(1);
            }
            else
            {
                UI::infoMsg(UI::UNREC_CMD, 1);
                UI::openGitHubPage();
                UI::exitDelayed(1);
            }
        }
        
        // Disable direct launch from explorer
        if (IO::startedFromFolder())
        {
            UI::infoMsg(UI::OPENED_FROM_EXPLORER, 2);
            UI::exitDelayed(0);
        }
        
        while (!canExit)
        {   
            UI::clearConsole();
            canExit = UI::setupSequence();
        }
    }

    return 0;
}