#include "FileManager.h"
#include <FS.h>

bool FileManager::initialized = false;

bool FileManager::init() {
    if(initialized) return true;

    if (!LittleFS.begin(true)) {
        printDebug("Failed to mount LittleFS / Try reformatting");
        return false;
    }
    
    /*
    if(LittleFS.mkdir(PATH_DATA)) { printDebug(PATH_DATA); }
    if(LittleFS.mkdir(PATH_CALENDAR)) { printDebug(PATH_CALENDAR); }
    if(LittleFS.mkdir(PATH_NOTIFICATIONS)) { printDebug(PATH_NOTIFICATIONS); }
    if(LittleFS.mkdir(PATH_PLAYBACK)) { printDebug(PATH_PLAYBACK); }
    */
    LittleFS.mkdir(PATH_DATA);
    LittleFS.mkdir(PATH_CALENDAR);
    LittleFS.mkdir(PATH_NOTIFICATIONS);
    LittleFS.mkdir(PATH_PLAYBACK);

    initialized = true;
    return true;
}

void FileManager::printFolder(String path, uint8_t layer) {
    init();

    File root = LittleFS.open(path);
    File file = root.openNextFile();
    while(file){
        for(uint8_t i = 0; i < layer; i++) { Serial.print("- "); }
        Serial.println(file.name());
        
        if(file.isDirectory()) {
            String path = file.path();
            file.close();
            printFolder(path, layer + 1);
        }
        file = root.openNextFile();
    }
}

bool FileManager::format() {
    if(init()) {
        LittleFS.format();
    }else{
        if(!LittleFS.begin(true)) {
            printDebug("Failed to mount and format LittleFS");
            return false;
        }
        return true;
    }
    return false;
}
bool FileManager::writeFile(String path, String message) {
    init();
    return writeFile(path.c_str(), message.c_str());
}

bool FileManager::writeFile(const char* path, const char* message) {
    init();
    printDebug("Writing to file: " + String(path));

    // Open file for writing
    File file = LittleFS.open(path, FILE_WRITE, true);
    if (!file) {
        printDebug("Failed to open file for writing");
        return false;
    }

    // Write the string to the file
    if (file.print(message) > 0) {
        file.close();
        printDebug("File written successfully");
        return true;
    } else {
        file.close();
        printDebug("Write failed");
        return false;
    }
}
bool FileManager::emptyDir(String path) {
    init();
    File root = LittleFS.open(path);
    File file = root.openNextFile();

    bool success = true;
    while(file) {
        String path = file.path();
        file.close();
        
        if(!LittleFS.remove(path)) {
            printDebug("ERROR DELETE " + path);
            success = false;
        }else{
            printDebug("DELETE " + path);
        }
        file = root.openNextFile();
    }
    return success;
}
uint16_t FileManager::dirFileCount(String path) {
    init();
    File root = LittleFS.open(path);
    File file = root.openNextFile();

    uint16_t count = 0;
    while(file) {
        count++;
        file.close();
        file = root.openNextFile();
    }
    return count;
}

bool FileManager::deleteFile(String path) {
    init();
    printDebug("DELETE " + path);
    return LittleFS.remove(path);
}
bool FileManager::exists(String path) {
    init();
    return LittleFS.exists(path);
}

String FileManager::readFile(String path) {
    init();
    File file = LittleFS.open(path);
    
    String str = "";
    if(file) {
        if(file.available()) {
            str = file.readString();
            file.close();
        }
    }
    return str;
}