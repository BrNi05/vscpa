#include "./json/json.hpp"
#include "./sysmod/sysmod.h"
#include "./consts/consts.h"
#include "./console/ui.h"
#include "./io/io.h"
#include "./console/cmdparser.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
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
        UI::infoMsg(UI::GREETING_1, 0); UI::infoMsg(UI::GREETING_2, 1000);
        sysmod::addSelfToPath();
        sysmod::saveLibGen();
        UI::infoMsg(UI::ADMIN_SUCCESS, 0);
        UI::exitDelayed(2);
    }
    else
    {
        sysmod::saveLibGen(); // make sure own folders do exist
        IO::useClangCompiler();
        
        Parser::processCmd(argc, argv);
        
        // Disable direct launch from explorer
        if (IO::startedFromFolder())
        {
            UI::infoMsg(UI::OPENED_FROM_EXPLORER, 1500);
            UI::exitDelayed(0);
        }
        
        if (argc >= 2) { UI::infoMsg(UI::NL, 1000); }
        while (!canExit)
        {   
            UI::clearConsole();
            canExit = UI::setupSequence();
        }
    }

    return 0;
}