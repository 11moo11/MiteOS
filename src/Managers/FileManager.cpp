#include "FileManager.h"

bool FileManager::init() {
    if (!SPIFFS.begin(false)) {
        printDebug("Failed to mount SPIFFS / Try reformatting");
        return false;
    }
    return true;
}

bool FileManager::format() {
    if(init()) {
        SPIFFS.format();
    }else{
        if (!SPIFFS.begin(true)) {
            printDebug("Failed to mount SPIFFS / Try reformatting");
            return false;
        }
        return true;
    }
}