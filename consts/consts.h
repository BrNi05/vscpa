#ifndef CONSTS_H
#define CONSTS_H

#include <string_view>

namespace INTERNAL
{
    // Used for compatibility checks for future versions
    inline constexpr std::string_view PROGRAM_VERSION = "1.0.0";
    inline constexpr std::string_view README_LINK = "https://github.com/BrNi05/vscpa";
}

namespace UI
{
    inline constexpr std::string_view CONSOLE_DEFAULT_TITLE = "VSCPA (v1.0.0)";

    inline constexpr std::string_view WINSYS_NOT_SUPPORTED = "VSCPA only supports Windows 10 and 11! Exiting...";

    inline constexpr std::string_view GREETING_1 = "Welcome to VS Code Project Assistant!";
    inline constexpr std::string_view GREETING_2 = "\nInitial setup started...";

    inline constexpr std::string_view ERROR_MSG_1 = "An error occurred in component: ";
    inline constexpr std::string_view ERROR_MSG_2 = "VSCPA will now restart.";

    inline constexpr std::string_view HELP_MENU = "Help is on the way! Redirecting to user manual...";
    inline constexpr std::string_view UNREC_CMD = "Unrecognized command. Redirecting to user manual...";

    inline constexpr std::string_view RESET_SUCCESS = "Fast setup reset successful!";
    inline constexpr std::string_view ENABLE_SUCCESS = "Fast setup enabled!";
    inline constexpr std::string_view FACTORY_RESET_SUCCESS = "Factory reset successful!";
    inline constexpr std::string_view FACTORY_RESET_EXTRA = "Environment variables were not cleared. In case of a conflict, please clear them manually.";
    inline constexpr std::string_view OPENED_FROM_EXPLORER = "VSCPA was launched from install location.";
    inline constexpr std::string_view COMPILER = "Compiler toggled!";

    inline constexpr std::string_view ADMIN_SUCCESS = "\nInitial setup completed. \nNow, you can use VSCPA from the VS Code Terminal. Exiting...\n";

    inline constexpr std::string_view SAVE_SUCCESS = "Config file saved successfully.";
    inline constexpr std::string_view JSON_SUCCESS = "Tasks and launch files generated successfully.\n";

    inline constexpr std::string_view NL = "";
}

namespace Args
{
    inline constexpr std::string_view HELP = "help";

    inline constexpr std::string_view FAST_SETUP_ON = "fson";
    inline constexpr std::string_view FAST_SETUP_OFF = "fsoff";

    inline constexpr std::string_view FACTORY = "factory";

    inline constexpr std::string_view GCC = "gcc";
    inline constexpr std::string_view CLANG = "clang";

    inline constexpr std::string_view STAY = "stay";
}

namespace IO
{
    inline constexpr std::string_view OWN_DIR_NAME = ".vsCodeProjectAssistant";
    inline constexpr std::string_view FAST_SETUP_FILE_NAME = "fastSetup.txt";
    inline constexpr std::string_view DEFAULT_CONFIG_FILE_NAME = "defaultConfig.txt";
    inline constexpr std::string_view PROFILES_DIR_NAME = "profiles";
    inline constexpr std::string_view VSC_FOLDER = ".vscode";

    inline constexpr std::string_view IGNORE_FOLDER = "vscpa_ignore";

    inline constexpr std::string_view CLANG_MARKER = "clang.txt";
}

namespace Sysmod
{
    inline constexpr int MAX_PATH_LENGTH = 32767; // for PATH environment variable on Windows
    inline constexpr int _MAX_PATH_ = 1025; // Reasonable limit (+1 for null termination), enforced on all platforms
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

    inline constexpr std::string_view EDIT1 = "Select language ('C'/'CPP'): ";
    inline constexpr std::string_view EDIT1_2 = "\nSelect C/CPP standard (ENTER: C23/CPP23): ";
    inline constexpr std::string_view EDIT2 = "\nHeader files in subdirectories? (Y/N): ";
    inline constexpr std::string_view EDIT3 = "\nSource files in subdirectories? (Y/N): ";
    inline constexpr std::string_view EDIT4 = "\nDefines (separate ONLY with comas[,]):\n";
    inline constexpr std::string_view EDIT5 = "\nOther compiler args (separate ONLY with comas[,]):\n";
    inline constexpr std::string_view EDIT6 = "\nOutput program (.exe) name: ";
    inline constexpr std::string_view EDIT7 = "\nExternal console? (Y/N): ";
}

#endif // CONSTS_H