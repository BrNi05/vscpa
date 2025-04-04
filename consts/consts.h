#ifndef CONSTS_H
#define CONSTS_H

#include <string_view>

namespace INTERNAL
{
    // Used for compatibility checks for future versions
    inline constexpr std::string_view PROGRAM_VERSION = "1.0.0";
}

namespace UI
{
    inline constexpr std::string_view CONSOLE_DEFAULT_TITLE = "VSCPA (v1.0.0)";

    inline constexpr std::string_view WINSYS_NOT_SUPPORTED = "VSCPA only supports Windows 11, yet! Exiting...";

    inline constexpr std::string_view GREETING_1 = "Welcome to VS Code Project Assistant!";
    inline constexpr std::string_view GREETING_2 = "\nSetup requires admin privileges, restarting...";

    inline constexpr std::string_view ERROR_MSG_1 = "An error occurred in component: ";
    inline constexpr std::string_view ERROR_MSG_2 = "VSCPA will now restart.";

    inline constexpr std::string_view HELP_MENU = "Help is on the way! Redirecting to user manual...";
    inline constexpr std::string_view UNREC_CMD = "Unrecognized command. Redirecting to user manual...";

    inline constexpr std::string_view RESET_SUCCESS = "Fast setup reset successful! Exiting...";
    inline constexpr std::string_view ENABLE_SUCCESS = "Fast setup enabled! Exiting...";
    inline constexpr std::string_view FACTORY_RESET_SUCCESS = "Factory reset successful! Exiting...";
    inline constexpr std::string_view FACTORY_RESET_EXTRA = "Environment variables were not cleared. In case of a conflict, please clear them manually.\n";
    inline constexpr std::string_view OPENED_FROM_EXPLORER = "VSCPA was launched from install location. Exiting...";

    inline constexpr std::string_view ADMIN_SUCCESS = "Admin privileges granted, initial setup completed.";
    inline constexpr std::string_view ADMIN_WARNING_2 = "\nWARNING! SAVE YOUR WORK! WINDOWS WILL BE PARTIALLY UNRESPONSIVE IF YOU SELECT [Y]!";
    inline constexpr std::string_view ADMIN_WARNING_2_EXTRA = "Recommended: [N] and follow later instructions.";
    inline constexpr std::string_view ADMIN_WARNING = "\nexplorer.exe has to be restarted to apply changes. (Y/N)";
    inline constexpr std::string_view ADMIN_SUCCESS_2N = "Please restart your machine or manually restart explorer.exe to work with VSCPA. Exiting...";
    inline constexpr std::string_view ADMIN_SUCCESS_2Y_1 = "\nExplorer will restart in about 30 seconds. Please wait.";
    inline constexpr std::string_view ADMIN_SUCCESS_2Y = "Now, you can use VSCPA from the VS Code Terminal. Exiting...";

    inline constexpr std::string_view SAVE_SUCCESS = "Config file saved successfully.";
    inline constexpr std::string_view JSON_SUCCESS = "Tasks and launch files generated successfully.";
}

namespace Args
{
    inline constexpr std::string_view HELP = "help";

    inline constexpr std::string_view FAST_SETUP_ON = "fson";
    inline constexpr std::string_view FAST_SETUP_OFF = "fsoff";

    inline constexpr std::string_view FACTORY = "factory";
}

namespace IO
{
    inline constexpr std::string_view OWN_DIR_NAME = ".vsCodeProjectAssistant";
    inline constexpr std::string_view FAST_SETUP_FILE_NAME = "fastSetup.txt";
    inline constexpr std::string_view DEFAULT_CONFIG_FILE_NAME = "defaultConfig.txt";
    inline constexpr std::string_view PROFILES_DIR_NAME = "profiles";
    inline constexpr std::string_view VSC_FOLDER = ".vscode";

    inline constexpr std::string_view IGNORE_FOLDER = "vscpa_ignore";
}

namespace Sysmod
{
    inline constexpr int MAX_PATH_LENGTH = 32767; // for PATH environment variable on Windows
    inline constexpr int _MAX_PATH_ = 257; // Windows limit (+1 for null termination), but used on all platforms
}

namespace Setup
{
    inline constexpr std::string_view PROMT = "Choose mode:";
    inline constexpr std::string_view DEFAULT_MODE = " D - Default mode";
    inline constexpr std::string_view NEW_MODE = " N - New mode";
    // inline constexpr std::string_view EDIT_MODE = " E - Edit mode";

    inline constexpr std::string_view CHOOSE_OPERATION = "\nChoose an operation:\n";
    inline constexpr std::string_view APPLY_TO_CURRENT_FOLDER = " A - Apply to current folder";
    inline constexpr std::string_view SAVE_AS_DEFAULT = " D - Save as default";
    inline constexpr std::string_view SAVE = " S - Save as default and apply to current folder";
    inline constexpr std::string_view CANCEL = " C - Cancel and go back";

    inline constexpr std::string_view EDIT1 = "\nSelect language ('C'/'CPP'): ";
    inline constexpr std::string_view EDIT1_2 = "\nSelect C/CPP standard (ENTER: C23/CPP23): ";
    inline constexpr std::string_view EDIT2 = "\nHeader files in subdirectories? (Y/N): ";
    inline constexpr std::string_view EDIT3 = "\nSource files in subdirectories? (Y/N): ";
    inline constexpr std::string_view EDIT4 = "\nDefines (separate ONLY with comas[,]):\n";
    inline constexpr std::string_view EDIT5 = "\nOther compiler args (separate ONLY with comas[,]):\n";
    inline constexpr std::string_view EDIT6 = "\nOutput program (.exe) name: ";
    inline constexpr std::string_view EDIT7 = "\nExternal console? (Y/N): ";
}

#endif // CONSTS_H