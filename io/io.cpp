#include "io.h"
#include "../consts/consts.h"
#include "../console/ui.h"

#include <iostream>
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
    #include <shlobj.h>
    //#include <shlobj_core.h>
    #include <vector>
#else
    #include <unistd.h>
    #include <cstdlib.h>
#endif


bool IO::fastSetup = false;


// Config file related operations //

ConfigFile* IO::loadConfigFile()
{
    if (IO::defaultConfigPath() != "")
    {
        ConfigFile *config = new ConfigFile();

        //! load file and fill the object

        return config;
    }
    else
    {
        return new ConfigFile(true, findCompilerPath());
    }
}

CMode IO::detectCMode()
{
    bool cppFound = false;
    bool cFound = false;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
    {
        if (entry.path().extension() == ".cpp") { cppFound = true; }
        else if (entry.path().extension() == ".c") { cFound = true; }
    }

    if (cppFound || !cFound) { return CPP; }
    else { return C; }
}

std::string IO::findCompilerPath()
{
    #ifdef _WIN32
        std::vector<std::string> possiblePaths;

        possiblePaths =
        { "C:\\msys64\\ucrt64\\bin\\gdb.exe",
          "C:\\MinGW\\bin\\gdb.exe",
          "C:\\Program Files\\mingw-w64\\bin\\gdb.exe",
          "C:\\msys2\\mingw64\\bin\\gdb.exe",
          "C:\\cygwin64\\bin\\gdb.exe",
          "C:\\Program Files\\Git\\usr\\bin\\gdb.exe"
        };

        for (const auto& path : possiblePaths)
        {
            if (std::filesystem::exists(path)) { return path; }
        }

        // Fallback, assuming gdb is in PATH
        char buffer[PATH_MAX];
        FILE* pipe = _popen("where gdb", "r");
        if (!pipe) { pipe = popen("command -v gdb", "r"); }
        if (pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) { pclose(pipe); return std::string(buffer); }
        else { UI::errorMsg("findCompilerPath - fallbackFail"); return ""; }
    #else
        char buffer[PATH_MAX];
        FILE* pipe = popen("which gdb", "r");
        if (!pipe) { pipe = popen("command -v gdb", "r"); }
        if (pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) { pclose(pipe); return std::string(buffer); }
        else { UI::errorMsg("findCompilerPath - fallbackFail"); return ""; }
    #endif
}


// JSON operations //

void IO::saveConfigFile(ConfigFile *config)
{
    
}

void IO::generateVSCodeFiles(ConfigFile *config)
{
    // Safe delete existing .vscode folder and content
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(".vscode"))
        {
            // check if filename is tasks.json or launch.json
            if (entry.path().filename() == "tasks.json" || entry.path().filename() == "launch.json")
            {
                std::filesystem::remove(entry.path());
            }
        }
    }
    catch (const std::exception& e) { UI::errorMsg("generateVSCodeFiles - remove_all"); }

    //! Create tasks.json and launch.json files

}

bool IO::fastSetupExists()
{
    return std::filesystem::exists(getAppdataPath() / IO::OWN_DIR_NAME / IO::FAST_SETUP_FILE_NAME);
}

void IO::resetFastSetup()
{
    if (fastSetupExists()) { std::filesystem::remove(getAppdataPath() / IO::OWN_DIR_NAME / IO::FAST_SETUP_FILE_NAME); }
}


// File and dir path operations //

std::filesystem::path IO::getAppdataPath()
{
    #ifdef _WIN32
        PWSTR path = nullptr;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path)))
        {
            std::filesystem::path appDataPath = path;
            CoTaskMemFree(path);
            return appDataPath;
        }
        else { UI::errorMsg("getAppdataPath - SHGetKnownFolderPath"); return ""; }
    #else
        const char* path = std::getenv("HOME");
        if (path != nullptr)
        {
            return std::filesystem::path(path) / ".local/share";
        }
        else { UI::errorMsg("getAppdataPath - getenv-HOME"); }

    #endif
}

bool IO::ownDirExists()
{
    return std::filesystem::exists(getAppdataPath() / IO::OWN_DIR_NAME) && std::filesystem::is_directory(getAppdataPath() / IO::OWN_DIR_NAME);
}

std::filesystem::path IO::defaultConfigPath()
{
    std::filesystem::path defaultConfigPath = getAppdataPath() / IO::OWN_DIR_NAME / IO::DEFAULT_CONFIG_FILE_NAME;

    if (std::filesystem::exists(defaultConfigPath) && std::filesystem::file_size(defaultConfigPath) != 0) { return defaultConfigPath; }
    else { return ""; }
}

bool IO::startedFromFolder()
{
    char exePath[1024] = {0};

    #ifdef _WIN32
        GetModuleFileName(NULL, exePath, sizeof(exePath));
    #elif __linux__
        size_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
        if (len != -1) { exePath[len] = '\0'; }
    #elif __APPLE__
        uint32_t size = sizeof(exePath);
        if (_NSGetExecutablePath(exePath, &size) == 0) { exePath[size] = '\0'; }
    #endif

    std::filesystem::path exeDir = std::filesystem::canonical(exePath).parent_path();
    std::filesystem::path currentDir = std::filesystem::current_path();

    return !(exeDir == currentDir);
}