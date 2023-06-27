#ifndef spiffs_module_h
#define spiffs_module_h

#include "SPIFFS.h"
#include <FS.h>

class IFile
{
    char msgs[4096];

public:
    void setupFile();
    void write(fs::FS &fs, const char *path, String message);
};

#endif