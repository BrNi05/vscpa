#include "configFile.h"
#include "../io/io.h"

ConfigFile::ConfigFile(bool loadDefault, std::string compilerPath)
{
    if(loadDefault)
    {
        this->mode = IO::detectCMode();
        if (this->mode == NONE) { this->mode = CPP; }
        this->cStd = C23;
        this->cppStd = CPP23;

        this->headerInSubDirs = true;
        this->srcInSubDirs = true;

        this->defines = "-Wall, -Werror";
        this->otherCompilerArgs = "";

        this->compilerPath = compilerPath;
        this->outputProgramName = "runnable";
    }
    else
    {
        ConfigFile *config = IO::loadConfigFile();
        
        this->mode = config->getMode();
        this->cStd = config->getCStd();
        this->cppStd = config->getCPPStd();
        this->headerInSubDirs = config->getHeaderInSubDirs();
        this->srcInSubDirs = config->getSrcInSubDirs();
        this->defines = config->getDefines();
        this->otherCompilerArgs = config->getOtherCompilerArgs();
        this->compilerPath = config->getCompilerPath();
        this->outputProgramName = config->getOutputProgramName();
    }
}

ConfigFile::ConfigFile() {};


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