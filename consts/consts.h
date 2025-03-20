#ifndef CONSTS_H
#define CONSTS_H

#include <string_view>

namespace UI
{
    inline constexpr std::string_view CONSOLE_DEFAULT_TITLE = "VS Code Project Assistant";

    inline constexpr std::string_view GREETING_1 = "Welcome to the VS Code Project Assistant!";
    inline constexpr std::string_view GREETING_2 = "Setup requires admin privileges, restarting...";

    inline constexpr std::string_view ERROR_MSG_1 = "An error occurred in component: ";
    inline constexpr std::string_view ERROR_MSG_2 = "The program will now restart.";
}

namespace Args
{
    inline constexpr std::string_view RESET = "reset";
}

namespace IO
{
    inline constexpr std::string_view OWN_DIR_NAME = ".VSCodeProjectAssistant";
    inline constexpr std::string_view FAST_SETUP_FILE_NAME = "fastSetup.txt";
}

namespace Sysmod
{
    inline constexpr int MAX_PATH_LENGTH = 32767;  // No need for extern, just constexpr
}

namespace Setup
{
    inline constexpr std::string_view PROMT = "Choose a run mode:";
    inline constexpr std::string_view DEFAULT_MODE = "D - Default mode";
    inline constexpr std::string_view EDIT_MODE = "E - Edit mode";

    inline constexpr char dDEFAULT_MODE_CHAR = 'D';
    inline constexpr char EDIT_MODE_CHAR = 'E';
}

#endif // CONSTS_H