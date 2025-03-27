#include "./json/json.hpp"
#include "./sysmod/sysmod.h"
#include "./consts/consts.h"
#include "./console/ui.h"
#include "./io/io.h"

int main(int argc, char* argv[])
{
    bool canExit = false;
    
    UI::setConsoleTitle(UI::CONSOLE_DEFAULT_TITLE);

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
        UI::infoMsg(UI::ADMIN_SUCCESS);
        exit(0);
    }
    else if (sysmod::firstRun())
    {
        UI::warnFirstRun();
    }
    else
    {
        sysmod::saveLibGen(); // make sure own folders do exist
        
        if (argc == 2)
        {
            if (std::string(argv[1]) == Args::RESET)
            {
                IO::resetFastSetup();
                UI::infoMsg(UI::RESET_SUCCESS);
                UI::exitDelayed(2);
            }
            else if (std::string(argv[1]) == Args::FACTORY)
            {
                sysmod::factoryReset();
                UI::infoMsg(UI::FACTORY_RESET_SUCCESS);
                UI::exitDelayed(2);
            }
        }
        
        // Disable direct launch from explorer
        if (IO::startedFromFolder())
        {
            UI::infoMsg(UI::OPENED_FROM_EXPLORER);
            UI::exitDelayed(2);
        }
        
        while (!canExit)
        {   
            canExit = UI::setupSequence();
        }
    }

    return 0;
}