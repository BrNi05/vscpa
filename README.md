# VS Code Project Assistant (VSCPA) BETA

## About the project

- VSCPA is a tool designed to assist C and C++ developers with debugging and building in VS Code.
- It generates the `tasks` and `launch` files for the project, completely automatically or based on user preferences.
- It can be (and should be) opened in the working directory (the VS Code project folder) via the system or VS Code Terminal.
- After initial setup, up to "0-click" file generation.
- Latest release: v1.0.0 (BETA) - during the BETA, Linux and MacOS support is disabled.

# Initial Setup

- Download the lastest release. (Be sure to periodically check for new releases.)
- Use the installer or manually paste the executable in a folder (preferably a typical install location).
- For the first time only, launch the program from the install location or shortcut.
    - Execute VSCPA with admin priviliges (`sudo` on linux), or else the program will restart requesting elevation.
    - VSCPA will automatically add itself (and GCC if not present) to environment variables.
    - Check the technical details section for more details.
    - Upon success, the program will terminate, as no further setup needs to be done. 

# Usage

- Create some folders and files for your C/C++ project or just start coding.
- If you and your code is ready for debugging or release, use VSCPA to and get to work in seconds.
- If not using VS Code Terminal, locate your project's root folder (working directory), where the `.vscode` folder should be.
- Type `vscpa` in VS Code Terminal, in system terminal opened in working directory or in the file explorer path bar, hit ENTER.
- Choose the execution mode.
    - Default mode: generate files based on your previous settings or hard-coded, yet adaptive software defaults.
        - Fallback for default mode without default config saved: C++23/C23 (depending on the files in the folder), recursive source file and header include, `-Wall`, `-Werror`, `-Wextra`, `-Wpedantic` compiler flags as args, output name: runnable, integrated console.
    - Edit mode: create new config (apply and/or save it as default).
        - Select language. Currently supported: C, C++
        - Select language standard (all supported by GCC).
        - Select if nested headers and .cpp/.c files should be included in compilation.
            - VSCPA ignores folders that has `vscpa_ignore` in their names during this process.
        - Enter macros that should be defined at project scope. Use `,` marks to separate them.
        - Enter compiler args separated by `,` marks. (Such as `Wall` or `Werror`, without `-` or spaces.).
            - Spaces and dashes are automatically removed from the input string, so technically they won't cause errors.
        - Enter the output executable name.
        - Select console appearance (external / VS Code Terminal).
- If new folders and files were added to the project, or other compilations settings are needed, follow the steps above. The generated files will be overwritten.
- VSCPA terminates execution and restarts in an event of any (handled) error (if so, probably IO).

# Open with arguments

- VSCPA can be customized via arguments. This is not a normal startup, VSCPA will close as soon as the input is processed.
- Open VSCPA with the argument `fson` to enable fast setup.
    - With fast setup enabled, the default config is going to be used, without any user interaction.
- Open VSCPA with the argument `fsoff` to disable fast setup.
    - Eg. in VS Code terminal: `vscpa fsoff`.
- Open VSCPA with the argument `help` to get redirected to the Github project page, in other words, here.
    - Any unrecognised command will trigger this reaction.
- Open VSCPA with the argument `factory` to perform a factory reset (should be used in case of frequent errors).
    - A factory reset does not remove environment path modifications or the generated `tasks` and `launch` files; these must be deleted manually.


# Technical details and limitations
- When using system API calls (eg. from `<shlobj.h>` on Windows platform), you must manually include the corresponding header in the task arguments.
- Due to the lack of documentation of the Windows API, notifying the system about the change in environment variables is done by killing `explorer.exe` and then restarting it. This takes quite some time and can be dangerous. Manual restarting is advised.
- OS support: Windows 11 23H2 and newer, no compatibility limitations for Linux and macOS.
- Tested on Windown 11 Pro 24H2 (build 26100.3476) and Linux Manjaro 24.2.1 KDE Plasma (kernel: 6.12.19)

# Future releases and roadmap
- v1.0.1: code optimization and bugfixes
- v1.1: release/debug switch (-g flag) for terminal launch and edit mode
- v1.2: better UI, enhanced edit mode
- v1.3: global and project-scoped default configs
    - Default config in the project folder and a global one, project scope first
- v1.4: profile system
    - startup argument driven profile loading (other mean of fast setup)
    - save config as profile
    - Factory profiles: BME-Prog1, BME-Prog2, Modern C, Modern C++
- v1.4: open generated files and edit existing data
- v1.5: enhanced single file build task
- v2.0: Makefile support
    - Options to further customize the generation process.