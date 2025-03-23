#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "common.hpp"
#include <string>
#include <filesystem>

using Path = std::filesystem::path;

class ConfigFile
{
    public:
        
        // Load system default if no save is found
        ConfigFile(bool loadDefault, Path compilerPath, Path debuggerPath);
        ConfigFile();

        void setMode(CMode mode);
        void setCStd(CStd cStd);
        void setCPPStd(CPPStd cppStd);
        void setHeaderInSubDirs(bool headerInSubDirs);
        void setSrcInSubDirs(bool srcInSubDirs);
        void setDefines(std::string defines);
        void setOtherCompilerArgs(std::string otherCompilerArgs);
        void setCompilerPath(std::string compilerPath);
        void setDebuggerPath(std::string debuggerPath);
        void setOutputProgramName(std::string outputProgramName);

        CMode getMode();
        CStd getCStd();
        CPPStd getCPPStd();
        bool getHeaderInSubDirs();
        bool getSrcInSubDirs();
        std::string getDefines();
        std::string getOtherCompilerArgs();
        Path getCompilerPath();
        Path getDebuggerPath();
        std::string getOutputProgramName();

    private:
        CMode mode; // fallback to CPP
        CStd cStd;
        CPPStd cppStd; // fallback to C++23

        bool headerInSubDirs;
        bool srcInSubDirs;

        std::string defines;
        std::string otherCompilerArgs;

        Path compilerPath;
        Path debuggerPath;

        std::string outputProgramName;
};

#endif // CONFIGFILE_H