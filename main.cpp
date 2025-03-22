#include "./json/json.hpp"
#include "./sysmod/sysmod.h"
#include "./consts/consts.h"
#include "./console/ui.h"
#include "./io/io.h"
#include "./devtools/memtrace.h"

#include <thread>


int main(int argc, char* argv[])
{
    bool canExit = false;
    
    UI::setConsoleTitle(UI::CONSOLE_DEFAULT_TITLE);
    
    IO::fastSetup = IO::fastSetupExists(); // init

    /**
        * Checks for command line arguments (used for resetting fast setup)
        * If ran for first time with admin, add itself to PATH and setup
        * If ran for first time but with no admin rights, restart with elevated privileges
        * Else, start setup sequence
    */
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
        if (argc == 1)
        {
            if (std::string(argv[1]) == Args::RESET)
            {
                IO::resetFastSetup();
                UI::infoMsg(UI::RESET_SUCCESS.data());
                std::this_thread::sleep_for(std::chrono::seconds(1));
                exit(0);
            }
        }
        
        if (IO::startedFromFolder())
        {
            UI::infoMsg(UI::OPENED_FROM_EXPLORER.data());
            std::this_thread::sleep_for(std::chrono::seconds(2));
            exit(0);
        }

        while (!canExit)
        {   
            canExit = UI::setupSequence();
        }
    }

    return 0;
}