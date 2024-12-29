#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>

#include "../MiteOS.h"
#include "SPIFFS.h"

class FileManager {
	public:
		static bool init();
		static bool format();
};

#endif