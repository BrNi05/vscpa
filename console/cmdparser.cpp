#include "cmdparser.hpp"

#include "ui.h"
#include "../consts/consts.h"
#include "../io/io.h"
#include "../sysmod/sysmod.h"

void Parser::processCmd(int argc, char* argv[])
{
    bool helpCalled = false;
    bool stayOpen = false;

    for (int i = 1; i < argc; ++i) { for (int j = 0; argv[i][j] != '\0'; ++j) { argv[i][j] = std::tolower(argv[i][j]); } }
    
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == Args::FACTORY)
        {
            sysmod::factoryReset();
            UI::infoMsg(UI::FACTORY_RESET_SUCCESS, 0);
            UI::exitDelayed(1); // ignore remaining commands, exit
        }
        
        else if (std::string(argv[i]) == Args::FAST_SETUP_OFF)
        {
            IO::resetFastSetup();
            UI::infoMsg(UI::RESET_SUCCESS, 0);
        }

        else if (std::string(argv[i]) == Args::FAST_SETUP_ON)
        {
            IO::enableFastSetup();
            UI::infoMsg(UI::ENABLE_SUCCESS, 0);
        }

        else if (std::string(argv[i]) == Args::HELP && !helpCalled)
        {
            helpCalled = true;
            UI::infoMsg(UI::HELP_MENU, 0);
            UI::openGitHubPage();
        }
            
        else if ((std::string(argv[i]) == Args::GCC || std::string(argv[i]) == Args::CLANG))
        {
            if (IO::isMacOS)
            {
                IO::useClangCompiler(std::string_view(argv[1]) == Args::CLANG, true);
                UI::infoMsg(UI::COMPILER, 0);
            }
        }

        else if (std::string(argv[i]) == Args::STAY) { stayOpen = true; }

        else if (!helpCalled) // unrecognised command
        {
            helpCalled = true;
            UI::infoMsg(UI::UNREC_CMD, 0);
            UI::openGitHubPage();
        }
    }

    if(!stayOpen && argc >= 2) { UI::exitDelayed(1); }
}