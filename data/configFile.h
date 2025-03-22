#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include "common.hpp"
#include <string>

class ConfigFile
{
    public:
        
        // Load system default if no save is found
        ConfigFile(bool loadDefault, std::string compilerPath);
        ConfigFile();

        void setMode(CMode mode);
        void setStd(CStd cStd);
        void setStd(CPPStd cppStd);
        void setHeaderInSubDirs(bool headerInSubDirs);
        void setSrcInSubDirs(bool srcInSubDirs);
        void setDefines(std::string defines);
        void setOtherCompilerArgs(std::string otherCompilerArgs);
        void setCompilerPath(std::string compilerPath);
        void setOutputProgramName(std::string outputProgramName);

        CMode getMode();
        CStd getCStd();
        CPPStd getCPPStd();
        bool getHeaderInSubDirs();
        bool getSrcInSubDirs();
        std::string getDefines();
        std::string getOtherCompilerArgs();
        std::string getCompilerPath();
        std::string getOutputProgramName();

    private:
        CMode mode;
        CStd cStd; // if C is used
        CPPStd cppStd; // if C++ is used

        bool headerInSubDirs;
        bool srcInSubDirs;

        std::string defines;
        std::string otherCompilerArgs;

        std::string compilerPath;

        std::string outputProgramName;
};

#endif // CONFIGFILE_H