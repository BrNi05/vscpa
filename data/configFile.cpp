#include "configFile.h"
#include "../io/io.h"

ConfigFile::ConfigFile(bool loadFallback, Path compilerPath, Path debuggerPath)
{
    if(loadFallback)
    {
        this->mode = IO::detectCMode();
        this->cStd = C23;
        this->cppStd = CPP23;

        this->headerInSubDirs = true;
        this->srcInSubDirs = true;

        this->defines = "";
        this->otherCompilerArgs = "Wall, Werror, Wextra, pedantic";

        this->compilerPath = compilerPath;
        this->debuggerPath = debuggerPath;
        this->outputProgramName = "runnable";
        this->externalConsole = false;
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
        this->debuggerPath = config->getDebuggerPath();
        this->outputProgramName = config->getOutputProgramName();
        this->externalConsole = config->getExternalConsole();

        delete config;
    }
}

ConfigFile::ConfigFile() {};

// Getters and setters

void ConfigFile::setMode(CMode mode)
{
    this->mode = mode;
}

void ConfigFile::setCStd(CStd cStd)
{
    if (this->mode == C)
    {
        this->cStd = cStd;
    }
}

void ConfigFile::setCPPStd(CPPStd cppStd)
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

void ConfigFile::setDebuggerPath(std::string debuggerPath)
{
    this->debuggerPath = debuggerPath;
}

void ConfigFile::setOutputProgramName(std::string outputProgramName)
{
    this->outputProgramName = outputProgramName;
}

void ConfigFile::setExternalConsole(bool externalConsole)
{
    this->externalConsole = externalConsole;
}

CMode ConfigFile::getMode()
{
    return this->mode;
}

CStd ConfigFile::getCStd()
{
    return this->cStd; // Returned but not always used (placeholder value)
}

CPPStd ConfigFile::getCPPStd()
{
    return this->cppStd; // Returned but not always used (placeholder value)
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

Path ConfigFile::getCompilerPath()
{
    return this->compilerPath;
}

Path ConfigFile::getDebuggerPath()
{
    return this->debuggerPath;
}

std::string ConfigFile::getOutputProgramName()
{
    return this->outputProgramName;
}

bool ConfigFile::getExternalConsole()
{
    return this->externalConsole;
}