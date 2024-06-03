#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

class PowerManager {
	public:
		static void deepSleep();
		
		static float getBatteryVoltage();
		static float getBatteryPercentage();
};

#endif