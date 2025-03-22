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
- Locate your projects root folder, where the `.vscode` folder shall be.
- Type `vscpa` in the path bar, hit ENTER.
- Choose the execution mode.
    - Default mode: generate files based on your previous settings or hard-coded, yet adaptive software defaults.
    - Edit mode: create new config (apply and/or save it as default).
        - Select language. Currently supported: C, C++
        - Select language standard (all supported by GCC).
        - Select if headers and .cpp/.c files should be included in compilation.
        - Enter macros that should be defined at project scope. Use `,` marks to separate them.
        - Enter compiler args separated by `,` marks. (Such as `Wall` or `Werror`).
        - Enter output program's name.
- Open VSCPA with the argument `reset` to disable fast setup.
- VSCPA terimates execution and restarts in an event of any error (if so, probably IO).

# Future releases and roadmap
- v1.1: support for auto-add G++, bugfixes
- v1.2: better UI, enhanced edit mode