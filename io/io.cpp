#include "io.h"


bool IO::fastSetup = false;

bool IO::defaultConfigExists()
{

}

ConfigFile* IO::loadConfigFile()
{
    // loads the config file if there is one created
    // returns an empty object if not

    fastSetup = true; // set
}

int IO::saveConfigFile()
{

}

int IO::generateVSCodeFiles()
{
    // and .vscode folder as well if not present
    // delete existing content (rmdir forced)
}

bool IO::deleteFastSetup()
{
    
}