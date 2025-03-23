#ifndef CONSTS_H
#define CONSTS_H

#include <string_view>

namespace UI
{
    inline constexpr std::string_view CONSOLE_DEFAULT_TITLE = "VSCPA (v1.0.0)";

    inline constexpr std::string_view GREETING_1 = "Welcome to VS Code Project Assistant!";
    inline constexpr std::string_view GREETING_2 = "Setup requires admin privileges, restarting...";

    inline constexpr std::string_view ERROR_MSG_1 = "An error occurred in component: ";
    inline constexpr std::string_view ERROR_MSG_2 = "VSCPA will now restart.";

    inline constexpr std::string_view RESET_SUCCESS = "Fast setup reset successful!";
    inline constexpr std::string_view OPENED_FROM_EXPLORER = "VSCPA was not launched from a directory, exiting...";

    inline constexpr std::string_view ADMIN_SUCCESS = "Admin privileges granted, initial setup completed.";
}

namespace Args
{
    inline constexpr std::string_view RESET = "reset"; // transform to full reset
    inline constexpr std::string_view RESET_LOC = "resetl"; // for future updates
    inline constexpr std::string_view RESET_GLOB = "resetg"; // for future updates
}

namespace IO
{
    inline constexpr std::string_view OWN_DIR_NAME = ".vsCodeProjectAssistant";
    inline constexpr std::string_view FAST_SETUP_FILE_NAME = "fastSetup.txt";
    inline constexpr std::string_view DEFAULT_CONFIG_FILE_NAME = "defaultConfig.json";
}

namespace Sysmod
{
    inline constexpr int MAX_PATH_LENGTH = 32767;
}

namespace Setup
{
    inline constexpr std::string_view PROMT = "Choose a run mode:";
    inline constexpr std::string_view DEFAULT_MODE = " D - Default mode";
    inline constexpr std::string_view EDIT_MODE = " E - Edit mode";

    inline constexpr std::string_view CHOOSE_OPERATION = "Choose an operation:";
    inline constexpr std::string_view APPLY_TO_CURRENT_FOLDER = " A - Apply to current folder";
    inline constexpr std::string_view SAVE_AS_DEFAULT = " D - Save as default";
    inline constexpr std::string_view SAVE = " S - Save as default and apply";
    inline constexpr std::string_view CANCEL = " C - Cancel and exit";

    inline constexpr std::string_view EDIT1 = "Select language ('C'/'CPP'): ";
    inline constexpr std::string_view EDIT2 = "Header files in subdirectories? (Y/N): ";
    inline constexpr std::string_view EDIT3 = "Source files in subdirectories? (Y/N): ";
    inline constexpr std::string_view EDIT4 = "Defines (separate with comas[,]):\n";
    inline constexpr std::string_view EDIT5 = "Other compiler args (separate with comas[,]):\n";
    inline constexpr std::string_view EDIT6 = "Output program (.exe) name: ";
}

#endif // CONSTS_H