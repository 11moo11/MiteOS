#ifndef ACTIVITY_MANAGER_H
#define ACTIVITY_MANAGER_H

#include <Arduino.h>

class ActivityManager {
	public:
		static void restoreSteps();
		static void resetSteps(bool save = true);
		static void saveSteps();
		static uint32_t getStepCount();
		static float getWalkedDistance();
		static String getWalkedDistanceStr();
};

#endif