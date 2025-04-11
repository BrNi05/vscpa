# VS Code Project Assistant (VSCPA) BETA

## About the project

- VSCPA is a tool designed to assist C and C++ developers with debugging and building in VS Code (or OSS Code).
- It generates the `tasks` and `launch` files for the project, completely automatically or based on user preferences.
- It can be (and should be) opened in the working directory (the VS Code project folder) via the system terminal or VS Code built-in Terminal.
- After initial setup, up to "0-click" file generation.
- Latest release: v1.0.0 (BETA-2). WARNING! Linux and macOS test phase!

# Initial Setup

- Download the lastest release. (Be sure to periodically check for new releases.)
- Use the Windows installer, download and paste the executable in a folder (preferably a typical install location), or compile the code for yourself.
- For the first time only, launch the program from the install location or shortcut.
    - VSCPA will automatically add itself (and GCC if not present) to user PATH.
    - Upon success, the program will terminate, as no further setup needs to be done.
- Check the technical details section for more details.

# Basic usage

- Create folders, write code, etc. No particular preparation is needed.
- Type `vscpa` in VS Code Terminal, in system terminal opened in working directory or in the file explorer path bar, hit ENTER.
    - Working directory: where the `.vscode` folder should be
- Choose the execution mode.
    - Default mode: generate files based on your previous settings or hard-coded, yet adaptive software defaults.
        - Fallback for default mode without default config saved: C++23/C23 (depending on the files in the folder), recursive source file and header include, `-Wall`, `-Werror`, `-Wextra`, `-Wpedantic` compiler flags as args, output name: runnable, integrated console.
    - New mode: create new config (apply and/or save it as default).
        - Select language. Currently supported: C, C++
        - Select language standard (all supported by GCC or Clang).
            - VSCPA expects only an integer as input. Upon no or invalid input, the latest standard is selected.
            - Current standards:
                - C: 89, 90, 95, 99, 11, 17, 23
                - CPP: 98, 03, 11, 14, 17, 20, 23
        - Select if nested headers and .cpp/.c files should be included in compilation.
            - VSCPA ignores folders that has `vscpa_ignore` in their names during this process.
        - Enter macros that should be defined at project scope. Use `,` marks to separate them.
        - Enter compiler args separated by `,` characters. Spaces and dashes are automatically removed from the input string.
        - Enter the output executable name.
        - Select console appearance (external / VS Code Terminal).
- If new folders and files were added to the project, or other compilations settings are needed, follow the steps above. The generated files will be overwritten.
    - v1.5 will introduce editing features, until then, the above applies.
- VSCPA terminates execution and restarts in an event of any (handled) error (if so, probably IO).

# Open with arguments

- VSCPA can be customized via arguments.
    - This is not a normal startup, VSCPA will close as soon as the input is processed.
    - To prevent this, use the `stay` argument. This way, VSCPA will not close.
    - The arguments are stackable.
- Open VSCPA with the argument `fson` to enable fast setup.
    - With fast setup enabled, the default config is going to be used, without any user interaction.
- Open VSCPA with the argument `fsoff` to disable fast setup.
- Open VSCPA with the argument `help` to get redirected to the Github project page, in other words, here.
    - Any unrecognised command will trigger this command.
- Open VSCPA with the argument `factory` to perform a factory reset (should be used in case of frequent errors).
    - A factory reset does not remove PATH modifications or the generated `tasks` and `launch` files; these must be deleted manually.
- Open VSCPA with the arguments: `clang` or `gcc` to toggle between compiler families on macOS (only).
    - On Apple hardware, the default compiler is set to clang, and the debugger is set to lldb. Alternative: gcc and gdb.
    - Problem matcher is set to be gcc, as it is the official recommendation.

# Technical details and limitations

- VSCPA assumes that the necessary softwares are installed on the host machine, such as C/C++ VSC extensions, GCC, Xcode Command Line Tools on macOS.
- When using system API calls (eg. from `<shlobj.h>` on Windows platform), you must manually include the corresponding header in the task arguments.
- On the latter two platforms VSCPA modifies `~/.bashrc` or `~/.zshrc` in order to modify PATH.
- OS support: Windows 10 and 11, no compatibility limitation for Linux and macOS.
    - On Linux, VSCPA assumes that the `xdg-utils` package is installed.
- Tested on Windown 11 Pro 24H2 (build 26100.3476) and Linux Manjaro 24.2.1 KDE Plasma (kernel: 6.12.19).
    - Since Windows 10 is a discontinued OS, VSCPA has not been tested on it.

# Release notes

- 2025.03.29.: v1.0.0 BETA-1
    - First pre-release
- 2025.04.11.: v1.0.0 BETA-2
    - Second pre-release

# Future releases and roadmap

- v1.0.x: code optimizations, documentation and bugfixes
- v1.1: release/debug switch (-g flag) for terminal launch and edit mode
    - CLI args: `switch`, `rel`, `dbg` - instantly change existing VSCPA generated JSON files
    - Add debug/release switch the config generation
    - Instant release, code optimization options, etc
- v1.2: global and project-scoped default configs
    - Default config in the project folder and a global one, project scope first
- v1.3: profile system
    - startup argument driven profile loading (other means of fast setup)
    - save config as profile
    - Factory profiles: BME-Prog1, BME-Prog2, Modern C, Modern C++
- v1.4: more arguments, faster workflow
    - select profile and start immediate generation process
    - set current tasks+launch file config as local/global default
    - open vscpa into a directory from install executable
- v1.5: open generated files and edit existing data
    - dedicated startup argument as well
- v1.6: enhanced single file build task support, user configurability
- v1.7: New version available notifications
- v2.0: Makefile support
    - Makefile related error handling