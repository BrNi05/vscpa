#include "io.h"
#include "../consts/consts.h"
#include "../console/ui.h"
#include "../data/common.hpp"
#include "../data/configFile.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>

#include "../json/json.hpp"

#ifdef _WIN32
    #include <windows.h>
    #include <shlobj.h>
#else
    #include <unistd.h>
#endif
 
using JSON = nlohmann::json;
using Path = std::filesystem::path;

Path IO::ownDirPath = getAppdataPath() / IO::OWN_DIR_NAME;
Path IO::ownDirProfilesPath = ownDirPath / IO::PROFILES_DIR_NAME;
Path IO::fastSetupFilePath = ownDirPath / IO::FAST_SETUP_FILE_NAME;
Path IO::clangMarker = ownDirPath / IO::CLANG_MARKER;

bool IO::isMacOS = false;
#ifdef __MACH__
    IO::isMacOS = true;
#endif
bool IO::useClang = true;

// Config file related operations //

ConfigFile* IO::loadConfigFile()
{
    if (IO::defaultConfigPath() != "")
    {
        ConfigFile *config = new ConfigFile();

        std::ifstream file(IO::defaultConfigPath());
        if (file.is_open())
        {
            std::string line;
            int lineNum = 0;

            while (std::getline(file, line))
            {
                switch (lineNum)
                {
                    // case 0 for version check
                    case 1: config->setMode(static_cast<CMode>(std::stoi(line))); break;
                    case 2: config->setCStd(static_cast<CStd>(std::stoi(line))); break;
                    case 3: config->setCPPStd(static_cast<CPPStd>(std::stoi(line))); break;
                    case 4: config->setHeaderInSubDirs(std::stoi(line)); break;
                    case 5: config->setSrcInSubDirs(std::stoi(line)); break;
                    case 6: config->setDefines(line); break;
                    case 7: config->setOtherCompilerArgs(line); break;
                    case 8: config->setCompilerPath(line); break;
                    case 9: config->setDebuggerPath(line); break;
                    case 10: config->setOutputProgramName(line); break;
                    case 11: config->setExternalConsole(std::stoi(line)); break;
                }
                lineNum++;
            }
            file.close();
        }
        else
        {
            UI::errorMsg("loadConfigFile - open");
        }
        return config;
    }
    else
    {
        return new ConfigFile(true, findCompilerPath(), findDebuggerPath());
    }
}

void IO::saveConfigFile(ConfigFile *config)
{
    std::ofstream file(IO::defaultConfigPath(true));

    try
    {
        file << INTERNAL::PROGRAM_VERSION << std::endl;
        file << static_cast<int>(config->getMode()) << std::endl;
        file << static_cast<int>(config->getCStd()) << std::endl;
        file << static_cast<int>(config->getCPPStd()) << std::endl;
        file << config->getHeaderInSubDirs() << std::endl;
        file << config->getSrcInSubDirs() << std::endl;
        file << config->getDefines() << std::endl;
        file << config->getOtherCompilerArgs() << std::endl;
        file << config->getCompilerPath() << std::endl;
        file << config->getDebuggerPath() << std::endl;
        file << config->getOutputProgramName() << std::endl;
        file << (config->getExternalConsole() ? 1 : 0) << std::endl;

        file.close();
        UI::infoMsg(UI::SAVE_SUCCESS, 500);
    }
    catch(const std::exception& e)
    {
        UI::errorMsg("saveConfigFile - open");
    }
}

CMode IO::detectCMode()
{
    bool cppFound = false;
    bool cFound = false;

    try
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            if (entry.path().extension() == ".cpp") { cppFound = true; break; }
            else if (entry.path().extension() == ".c") { cFound = true; }
        }
    }
    catch (const std::exception& e) { UI::errorMsg("detectCMode - recursive_directory_iterator"); }

    if (cppFound || !cFound) { return CPP; }
    else { return C; }
}

Path IO::findDebuggerPath()
{
    std::vector<Path> possiblePaths = {
        "C:\\msys64\\mingw64\\bin\\",
        "C:\\msys2\\mingw64\\bin\\",
        "C:\\msys64\\ucrt64\\bin\\",
        "C:\\MinGW\\bin\\",
        "C:\\Program Files\\mingw-w64\\bin\\",
        "C:\\cygwin64\\bin\\",
        "C:\\Program Files\\Git\\usr\\bin\\"
    };

    #ifndef __APPLE__
        return pathFinder(possiblePaths, "gdb");
    #elif __APPLE__
        return pathFinder(possiblePaths, "lldb");
    #endif
}

Path IO::findCompilerPath(ConfigFile *config)
{
    std::string compiler;
    if (config != nullptr) { compiler = (config->getMode() == CPP ? ((isMacOS && useClang) ? "clang++" : "g++") : ((isMacOS && useClang) ? "clang" : "gcc")); }
    else { compiler = (isMacOS && useClang) ? "clang++" : "g++"; }
    
    std::vector<Path> possiblePaths = {
        "C:\\msys64\\mingw64\\bin\\",
        "C:\\msys2\\mingw64\\bin\\",
        "C:\\msys64\\ucrt64\\bin\\",
        "C:\\MinGW\\bin\\",
        "C:\\Program Files\\mingw-w64\\bin\\",
        "C:\\cygwin64\\bin\\",
        "C:\\Program Files\\Git\\usr\\bin\\"
    };
    for (auto& path : possiblePaths) { path += compiler; }

    return pathFinder(possiblePaths, compiler);
}

Path IO::pathFinder(std::vector<Path> possiblePaths, std::string fileName)
{
    #ifdef _WIN32
        std::string newFileName = fileName + ".exe";
        for (const auto& path : possiblePaths) { if (std::filesystem::exists(path / newFileName)) { return path / newFileName; } }
    #endif

    return pathFinderFallback(fileName);
}

Path IO::pathFinderFallback(std::string compiler)
{
    char buffer[Sysmod::_MAX_PATH_] = {0};
    FILE* pipe;

    #ifdef _WIN32
        pipe = _popen(("where " + compiler).c_str(), "r");
        if (!pipe) { pipe = _popen(("command -v " + compiler).c_str(), "r"); }
        if (pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) { _pclose(pipe); }
        else { UI::errorMsg("findCompilerPath - fallbackFail"); return ""; } // compiler cannot be located
    #else
        pipe = popen(("which " + compiler).c_str(), "r");
        if (!pipe) { pipe = popen(("command -v " + compiler).c_str(), "r"); }
        if (pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) { pclose(pipe); }
        else { UI::errorMsg("findCompilerPath - fallbackFail"); return ""; } // compiler cannot be located
    #endif

    std::string result = buffer;
    if (!result.empty() && result[result.size() - 1] == '\n') { result.erase(result.size() - 1); }
    return result;
}


// JSON operations //

void IO::generateVSCodeFiles(ConfigFile *config)
{
    // Make sure .vscode folder exists
    try { std::filesystem::create_directory(IO::VSC_FOLDER); }
    catch (const std::exception& e) { UI::errorMsg("generateVSCodeFiles - create_directory"); }

    // tasks.json - args //
    
    JSON args = {
        "-g",
        "-fdiagnostics-color=always",
        "-std=" + std::string(config->getMode() == CPP ? CPPStdMap.at(config->getCPPStd()).second : CStdMap.at(config->getCStd()).second),
        "-o",
        config->getOutputProgramName(),
    };

    std::stringstream ss1(config->getDefines());
    std::string temp1;
    while (std::getline(ss1, temp1, ','))
    {
        args.push_back(std::string("-D" + temp1));
    }

    std::stringstream ss2(config->getOtherCompilerArgs());
    std::string temp2;

    while (std::getline(ss2, temp2, ','))
    {
        args.push_back(("-" + temp2));
    }

    if (config->getSrcInSubDirs())
    {
        Path absolutePath = std::filesystem::absolute(std::filesystem::current_path());
        
        for (auto entry = std::filesystem::recursive_directory_iterator(std::filesystem::current_path(), std::filesystem::directory_options::skip_permission_denied); entry != std::filesystem::recursive_directory_iterator(); ++entry)
        {
            if (entry->is_directory() && entry->path().filename().string().find(IO::IGNORE_FOLDER) != std::string::npos) { entry.disable_recursion_pending(); continue; }
            
            if (entry->path().extension() == ".c" || entry->path().extension() == ".cpp")
            {
                Path relativePath = std::filesystem::relative(entry->path(), absolutePath);
                args.push_back(relativePath.generic_string());
            }
        }
    }
    else
    {
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path(), std::filesystem::directory_options::skip_permission_denied))
        {
            if (entry.path().extension() == ".c" || entry.path().extension() == ".cpp")
            {
                args.push_back(entry.path().filename());
            }
        }
    }
    
    std::unordered_set<Path> includes; // to avoid double includes
    Path basePath = std::filesystem::current_path();
    if (config->getHeaderInSubDirs())
    {
        Path relativePath;
        
        for (auto entry = std::filesystem::recursive_directory_iterator(std::filesystem::current_path(), std::filesystem::directory_options::skip_permission_denied); entry != std::filesystem::recursive_directory_iterator(); ++entry)
        {
            if (entry->is_directory() && entry->path().filename().string().find(IO::IGNORE_FOLDER) != std::string::npos) { entry.disable_recursion_pending(); continue; }
            
            if (entry->path().extension() == ".h" || entry->path().extension() == ".hpp")
            {
                relativePath = std::filesystem::relative(entry->path(), basePath);
                if (relativePath.parent_path().empty()) { continue; } // skip root dir headers
                includes.insert(relativePath.parent_path());
            }
        }

        for (const auto& include : includes) { args.push_back("-I" + include.string()); }
    }
    

    // tasks.json - tasks

    JSON singleBuildArgs = {
        "-fdiagnostics-color=always",
        "-g",
        "${file}",
        "-std=" + std::string(config->getMode() == CPP ? CPPStdMap.at(config->getCPPStd()).second : CStdMap.at(config->getCStd()).second),
        "-o",
        "${fileDirname}\\${fileBasenameNoExtension}.exe",
        "-Wall",
        "-Werror"
    };

    JSON tasks = {
        {"version", "2.0.0"},
        {"tasks", JSON::array({
            {
                {"label", "VSCPA MAIN BUILD TASK"},
                {"type", "shell"},
                {"command", (config->getMode() == CPP ? "g++" : "gcc")},
                {"args", args},
                {"group", "build"},
                {"problemMatcher", {"$gcc"}},
                {"detail", "Generated C/C++ build task"}
            },
            {
                {"label", "VSCPA Run C/C++ Executable"},
                {"type", "shell"},
                {"command", "./" + config->getOutputProgramName()},
                {"group", {{"kind", "test"}, {"isDefault", true}}},
                {"problemMatcher", JSON::array()}
            },
            {
                {"label", "VSCPA SINGLE FILE BUILD TASK"},
                {"type", "shell"},
                {"command", (config->getMode() == CPP ? ((isMacOS && useClang) ? "clang++" : "g++") : ((isMacOS && useClang) ? "clang" : "gcc"))},
                {"args", singleBuildArgs},
                {"group", {{"kind", "test"}, {"isDefault", true}}},
                {"problemMatcher", {"$gcc"}},
                {"detail", "VSCPA generated C/C++ build task"},
                {"options", { {"cwd", "${fileDirname}"}}}
            }
        })}
    };

    // launch.json

    JSON launchConfig = {
        {"version", "0.2.0"},
        {"configurations", {
            {
                {"name", "VSCPA BUILD&RUN"},
                {"type", "cppdbg"},
                {"request", "launch"},
                {"program", "${workspaceFolder}/" + config->getOutputProgramName()},
                {"args", JSON::array()},
                {"stopAtEntry", false},
                {"cwd", "${workspaceFolder}"},
                {"environment", JSON::array()},
                {"externalConsole", config->getExternalConsole()},
                {"MIMode", ((isMacOS && useClang) ? "lldb" : "gdb")},
                {"setupCommands", {
                    {
                        {"description", "Enable pretty-printing for gdb"},
                        {"text", "-enable-pretty-printing"},
                        {"ignoreFailures", true}
                    }
                }},
                {"logging", {
                    {"engineLogging", true}
                }},
                {"preLaunchTask", "VSCPA MAIN BUILD TASK"},
                {"miDebuggerPath", findDebuggerPath()}
            }
        }}
    };

    // Write to file
    Path vscFolder = std::filesystem::current_path() / IO::VSC_FOLDER;
    Path tasksFilePath = vscFolder / "tasks.json";
    Path launchFilePath = vscFolder / "launch.json";

    try
    {
        std::ofstream tasksFile(tasksFilePath, std::ios::trunc);
        std::ofstream launchFile(launchFilePath, std::ios::trunc);

        tasksFile << tasks.dump(4);
        launchFile << launchConfig.dump(4);

        tasksFile.close();
        launchFile.close();
    }
    catch (const std::exception& e) { UI::errorMsg("generateVSCodeFiles - json serialize"); }

    UI::infoMsg(UI::JSON_SUCCESS, 500);
}

bool IO::fastSetupExists()
{
    return std::filesystem::exists(fastSetupFilePath);
}

void IO::enableFastSetup()
{
    if (!IO::fastSetupExists())
    {
        try { std::ofstream file(fastSetupFilePath); }
        catch (const std::exception& e) { UI::errorMsg("enableFastSetup - ofstream"); }
    }
}

void IO::resetFastSetup()
{
    try
    {
        if (fastSetupExists()) { std::filesystem::remove(fastSetupFilePath); }
    }
    catch(const std::exception& e) { UI::errorMsg("resetFastSetup - remove"); }
}


// File and dir path operations //

Path IO::getAppdataPath()
{
    #ifdef _WIN32
        char path[MAX_PATH];
        if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
        {
            return std::string(path);
        }
        else { UI::errorMsg("getAppdataPath - SHGetKnownFolderPath"); return ""; }
    #else
        const char* path = std::getenv("HOME");
        if (path != nullptr)
        {
            return Path(path) / ".local/share";
        }
        else { UI::errorMsg("getAppdataPath - getenv-HOME"); return ""; }
    #endif
}

bool IO::ownDirExists()
{
    return (std::filesystem::exists(ownDirPath) && std::filesystem::is_directory(ownDirPath));
}

bool IO::useClangCompiler(bool state, bool set)
{
    if (set)
    {
        useClang = state;
        if (useClang && !std::filesystem::exists(clangMarker)) { std::ofstream file(clangMarker); }
        else if (std::filesystem::exists(clangMarker)) { std::filesystem::remove(clangMarker); }
    }
    
    return useClang;
}

Path IO::defaultConfigPath(bool creation)
{
    Path defConfPath = ownDirPath / IO::DEFAULT_CONFIG_FILE_NAME;

    if (std::filesystem::exists(defConfPath) && std::filesystem::file_size(defConfPath) != 0)
    {
        return defConfPath;
    }
    else if (creation) { return defConfPath; } // returns (currently non-existant) target path for config file generation
    return "";
}

bool IO::startedFromFolder()
{
    char exePath[Sysmod::_MAX_PATH_] = {0};

    #ifdef _WIN32
        GetModuleFileNameA(NULL, exePath, sizeof(exePath));
    #elif __linux__
        ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
        if (len == -1) { return true; } // fallback to true, making the program exit at later state
    #elif __APPLE__
        uint32_t size = sizeof(exePath);
        if (_NSGetExecutablePath(exePath, &size) != 0) { return true; } // fallback to true, making the program exit at later state
    #endif

    Path exeDir = std::filesystem::canonical(exePath).parent_path();
    Path currentDir = std::filesystem::current_path();

    return (exeDir == currentDir);
}