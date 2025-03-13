#include "ui.h"
#include "../sysmod/sysmod.h"

#include <iostream>
#include <thread>

namespace UI
{
    void setConsoleTitle(std::string title)
    {
        #ifdef _WIN32
            SetConsoleTitleA(title.c_str());
        #else
            std::cout << "\033]0;" << title << "\007";
        #endif
    }

    void warnFirstRun()
    {
        std::cout << "Welcome!" << std::endl << "Setup requires admin priviliges, restarting..." << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(3));
        sysmod::restartWithAdmin();
    }

    void setupSequence()
    {
        
    }
}