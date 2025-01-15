#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

#define POWER_MODE_MINUTLY 0
#define POWER_MODE_HOURLY 1
#define POWER_MODE_LIFT 2
#define POWER_MODE_SLEEP 3

extern RTC_DATA_ATTR bool isBatteryCharging;
extern RTC_DATA_ATTR long lastVoltage;
extern RTC_DATA_ATTR uint8_t currentPowerMode;
extern RTC_DATA_ATTR uint8_t powerMap[24];

class PowerManager {
	public:
		static void checkCharging();
		static bool isCharging();

		/// @brief Puts the watch to deep sleep and sets all nessisary data
		static void deepSleep();
		
		/// @brief Returns the current battery voltage
		/// @return Battery voltage in V (e.g. 3.7)
		static float getBatteryVoltage();
		/// @brief Returns the current battery percentage
		/// @return Battery percentage (0.0 - 100.0)
		static float getBatteryPercentage();

		/// @brief Sets the current power mode
		/// @param powerMode POWER_MODE_MINUTLY 0 | POWER_MODE_HOURLY 1 | POWER_MODE_LIFT 2 | POWER_MODE_SLEEP 3
		static void setPowerMode(uint8_t powerMode);
};

#endif