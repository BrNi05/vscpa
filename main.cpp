#include "./json/json.hpp"
#include "./sysmod/sysmod.h"
#include "./consts/consts.h"
#include "./console/ui.h"
#include "./io/io.h"

int main(int argc, char* argv[])
{
    bool canExit = false;
    
    UI::setConsoleTitle(UI::CONSOLE_DEFAULT_TITLE);

    /**
        * Checks for command line arguments (used for resetting fast setup)
        * If ran for first time with admin, add itself to PATH and setup
        * If ran for first time but with no admin rights, restart with elevated privileges
        * Else, start setup sequence
    **/

    if (sysmod::firstRunWithAdmin())
    {
        sysmod::addSelfToPath();
        sysmod::saveLibGen();
        UI::infoMsg(UI::ADMIN_SUCCESS.data());
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
                UI::infoMsg(UI::RESET_SUCCESS.data());
                UI::exitDelayed(2);
            }
            else if (std::string(argv[1]) == Args::FACTORY)
            {
                sysmod::factoryReset();
                UI::infoMsg(UI::FACTORY_RESET_SUCCESS.data());
                UI::exitDelayed(2);
            }
        }
        
        if (IO::startedFromFolder())
        {
            UI::infoMsg(UI::OPENED_FROM_EXPLORER.data());
            UI::exitDelayed(2);
        }
        
        while (!canExit)
        {   
            canExit = UI::setupSequence();
        }
    }

    return 0;
}