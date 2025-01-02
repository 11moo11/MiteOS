#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

extern RTC_DATA_ATTR bool isBatteryCharging;
extern RTC_DATA_ATTR long lastVoltage;

class PowerManager {
	public:
		static void checkCharging();
		static bool isCharging();

		static void deepSleep();
		
		static float getBatteryVoltage();
		static float getBatteryPercentage();
};

#endif