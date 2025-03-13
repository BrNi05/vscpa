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
    UI::setConsoleTitle();
    
    /**
        * If ran for first time with admin, add itself to PATH
        * If ran for first time but with no admin rights, restart with elevated privileges
        * Else, start setup sequence
    */
    if (sysmod::firstRunWithAdmin())
    {
        sysmod::addSelfToPath();
    }
    else if (sysmod::firstRun())
    {
        UI::warnFirstRun();
    }
    else
    {
        UI::setupSequence();
    }

    return 0;
}