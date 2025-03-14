#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <string>

enum CMode
{
    C,
    CPP
};

enum CStd
{
    NOT_C,
    C89,
    C90,
    C95,
    C99,
    C11,
    C17,
    C23
};

enum CPPStd
{
    NOT_CPP,
    CPP98,
    CPP03,
    CPP11,
    CPP14,
    CPP17,
    CPP20,
    CPP23
};

class ConfigFile
{
    public:
        ConfigFile(bool loadDefault = true);

        void setMode(CMode mode);
        void setStd(CStd cStd);
        void setStd(CPPStd cppStd);
        void setHeaderInSubDirs(bool headerInSubDirs);
        void setSrcInSubDirs(bool srcInSubDirs);
        void setDefines(std::string defines);
        void setOtherCompilerArgs(std::string otherCompilerArgs);
        void setCompilerPath(std::string compilerPath);
        void setOutputProgramName(std::string outputProgramName);
        void setTaskName(std::string taskName);
        void setFastSetup(bool fastSetup);

        CMode getMode();
        CStd getCStd();
        CPPStd getCPPStd();
        bool getHeaderInSubDirs();
        bool getSrcInSubDirs();
        std::string getDefines();
        std::string getOtherCompilerArgs();
        std::string getCompilerPath();
        std::string getOutputProgramName();
        std::string getTaskName();
        bool getFastSetup();

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
        std::string taskName;
        bool fastSetup;
};

#endif // CONFIGFILE_H