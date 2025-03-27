# VS Code Project Assistant (VSCPA)

## About the project

- VSCPA is a tool for C and C++ developers using VS Code.
- It generates the `tasks` and `launch` files for the project, totally automatically or based on user preferences.
- It can be (and should be) opened from the working directory.
- After initial setup, up to 0-click file generation.

# Initial Setup

- Download the lastest release. (Be sure to periodically check for new releases.)
- Use the installer or manually paste the executable to a folder (preferably a typical install location).
- For the first time only, launch the program from the install location or shortcut.
    - Run VSCPA with admin priviliges (`sudo` on linux), else the program will restart requesting elevation.
    - VSCPA will automatically add itself (and GCC if not present) to environment variables.
    - Upon success, the program will terminate, as no further job needs to be done. 

# Usage

- Create your basic folders and files structure for your C/C++ project.
- Locate your projects root folder, where the `.vscode` folder should be.
- Type `vscpa` in VS Code Terminal or in the path bar, hit ENTER.
- Choose the execution mode.
    - Default mode: generate files based on your previous settings or hard-coded, yet adaptive software defaults.
        - Fallback for default mode without default config saved: C++23/C23 (depending on the files in the folder), recursive source file and header include, -Wall -Werror args.
    - Edit mode: create new config (apply and/or save it as default).
        - Select language. Currently supported: C, C++
        - Select language standard (all supported by GCC).
        - Select if nested headers and .cpp/.c files should be included in compilation.
            - VSCPA ignores folders that has `vscpa_ignore` in their names during this process.
        - Enter macros that should be defined at project scope. Use `,` marks to separate them.
        - Enter compiler args separated by `,` marks. (Such as `Wall` or `Werror`, without `-`).
        - Enter output program's name.
        - Select console appearance (external / VS Code Terminal).
- Open VSCPA with the argument `reset` to disable fast setup.
    - In VS Code terminal: `vscpa reset`.
- Open VSCPA with the argument `factory` to perform a factory reset (should be used in case of frequent errors).
- VSCPA terimates execution and restarts in an event of any error (if so, probably IO).

# Technical details
- When using system API calls (eg. from `<shlobj.h>` on Windows platform), you must manually include the corresponding header in the task arguments.
- OS support: Windows 11 23H2 and newer, no compatibility limitations for Linux and macOS.
- Tested on Windown 11 Pro 24H2 (build 26100.3476) and Linux Manjaro 24.2.1 KDE Plasma (kernel: 6.12.19)

# Future releases and roadmap
- v1.0.1: code optimization and bugfixes
- v1.1: release/debug switch (-g flag) with terminal launch
- v1.2: better UI, enhanced edit mode
- v1.3: global and project-scoped default configs
    - Default config in the project folder and a global one, project scope first
    - Multiple reset flags accordingly
- v1.4: profile system
    - startup argument driven profile loading (other mean of fast setup)
    - save config as profile
    - Factory profiles: BME-Prog1, BME-Prog2, Modern C, Modern C++
- v1.4: open generated files and edit existing data
- v1.5: enhanced single file build task
- v2.0: Makefile support
    - Options to further customize the generation process.