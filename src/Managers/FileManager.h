#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>

#include "../MiteOS.h"
#include <LittleFS.h>

#define PATH_DATA "/dat/"
#define PATH_CALENDAR PATH_DATA"cal/"
#define PATH_NOTIFICATIONS PATH_DATA"not/"
#define PATH_PLAYBACK PATH_DATA"pb/"
#define PATH_WEATHER PATH_DATA"wd/"

class FileManager {
	public:
		static bool initialized;
	public:
		static bool init();
		static bool format();
		static bool writeFile(String path, String message);
		static bool writeFile(const char* path, const char* message);
		static bool emptyDir(String path);
		static uint16_t dirFileCount(String path);
		static bool deleteFile(String path);
		static bool exists(String path);
		static String readFile(String path);
		static void printFolder(String path, uint8_t layer = 0);
		static void clearCache();
};

#endif