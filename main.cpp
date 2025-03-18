#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include "./json/json.hpp"
#include "./sysmod/sysmod.h"
#include "./consts/consts.h"
#include "./console/ui.h"
#include "./io/io.h"

#include "./devtools/memtrace.h"

int main(int argc, char* argv[])
{
    bool canExit = false;
    
    UI::setConsoleTitle(UI::consoleDefaultTitle);
    
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
    }
    else if (sysmod::firstRun())
    {
        UI::warnFirstRun();
    }
    else
    {
        if (argc == 1)
        {
            if (std::string(argv[1]) == Args::reset)
            {
                UI::resetFastSetup();
            }
        }
        
        while (!canExit)
        {
            canExit = UI::setupSequence();
        }
    }

    return 0;
}