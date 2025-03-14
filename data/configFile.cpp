#include "configFile.h"

ConfigFile::ConfigFile(bool loadDefault)
{
    // look for default config file and load it wiht IO
}


// Getters and setters

void ConfigFile::setMode(CMode mode)
{
    this->mode = mode;
}

void ConfigFile::setStd(CStd cStd)
{
    if (this->mode == C)
    {
        this->cStd = cStd;
    }
}

void ConfigFile::setStd(CPPStd cppStd)
{
    if (this->mode == CPP)
    {
        this->cppStd = cppStd;
    }
}

void ConfigFile::setHeaderInSubDirs(bool headerInSubDirs)
{
    this->headerInSubDirs = headerInSubDirs;
}

void ConfigFile::setSrcInSubDirs(bool srcInSubDirs)
{
    this->srcInSubDirs = srcInSubDirs;
}

void ConfigFile::setDefines(std::string defines)
{
    this->defines = defines;
}

void ConfigFile::setOtherCompilerArgs(std::string otherCompilerArgs)
{
    this->otherCompilerArgs = otherCompilerArgs;
}

void ConfigFile::setCompilerPath(std::string compilerPath)
{
    this->compilerPath = compilerPath;
}

void ConfigFile::setOutputProgramName(std::string outputProgramName)
{
    this->outputProgramName = outputProgramName;
}

void ConfigFile::setTaskName(std::string taskName)
{
    this->taskName = taskName;
}

void ConfigFile::setFastSetup(bool fastSetup)
{
    this->fastSetup = fastSetup;
}

CMode ConfigFile::getMode()
{
    return this->mode;
}

CStd ConfigFile::getCStd()
{
    if (this->mode == C)
    {
        return this->cStd;
    }
    else { return NOT_C; }
}

CPPStd ConfigFile::getCPPStd()
{
    if (this->mode == CPP)
    {
        return this->cppStd;
    }
    else { return NOT_CPP; }
}

bool ConfigFile::getHeaderInSubDirs()
{
    return this->headerInSubDirs;
}

bool ConfigFile::getSrcInSubDirs()
{
    return this->srcInSubDirs;
}

std::string ConfigFile::getDefines()
{
    return this->defines;
}

std::string ConfigFile::getOtherCompilerArgs()
{
    return this->otherCompilerArgs;
}

std::string ConfigFile::getCompilerPath()
{
    return this->compilerPath;
}

std::string ConfigFile::getOutputProgramName()
{
    return this->outputProgramName;
}

std::string ConfigFile::getTaskName()
{
    return this->taskName;
}

bool ConfigFile::getFastSetup()
{
    return this->fastSetup;
}