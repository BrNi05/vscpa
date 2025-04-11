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
        UI::infoMsg("BETA release! Only Win11 is supported, yet. Exiting...");
        UI::exitDelayed(0);
    #endif

    bool canExit = false;
    
    // Set console title - only seen when initial setup is done
    if (IO::startedFromFolder())
    {
        UI::setConsoleTitle(UI::CONSOLE_DEFAULT_TITLE);
    }
    
    // Check for Win support (Win11 only)
    if (!sysmod::winSysSupported())
    {
        UI::infoMsg(UI::WINSYS_NOT_SUPPORTED);
        UI::exitDelayed(0);
    }

    if (sysmod::firstRun())
    {
        UI::infoMsg(UI::GREETING_1, 0); UI::infoMsg(UI::GREETING_2);
        sysmod::addSelfToPath();
        sysmod::saveLibGen();
        UI::infoMsg(UI::ADMIN_SUCCESS, 0);
        UI::exitDelayed(1);
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