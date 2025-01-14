#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

#define POWER_MODE_MINUTLY 0
#define POWER_MODE_HOURLY 1
#define POWER_MODE_LIFT 2
#define POWER_MODE_SLEEP 3

extern RTC_DATA_ATTR bool isBatteryCharging;
extern RTC_DATA_ATTR long lastVoltage;
extern RTC_DATA_ATTR long currentPowerMode;

class PowerManager {
	public:
		static void checkCharging();
		static bool isCharging();

		static void deepSleep();
		
		static float getBatteryVoltage();
		static float getBatteryPercentage();
};

#endif