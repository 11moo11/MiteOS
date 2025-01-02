#include "PowerManager.h"

#include "../MiteOS.h"

#define MIN_VOLTAGE_CHANGE_CHARGING 300 	// 0.3V
#define MIN_VOLTAGE_CHANGE_DISCHARGING 100 	// 0.1V

RTC_DATA_ATTR bool isBatteryCharging = false;
RTC_DATA_ATTR long lastVoltage = 0;

void PowerManager::deepSleep() {
	mDisplay.hibernate();
	
	BluetoothManager::powerOff();
	WifiConnectionManager::powerOff();
	
	mRTC.clearAlarm();        // resets the alarm flag in the RTC
	
	// Set GPIOs 0-39 to input to avoid power leaking out
	const uint64_t ignore = 0b11110001000000110000100111000010; // Ignore some GPIOs due to resets
	for (int i = 0; i < GPIO_NUM_MAX; i++) {
		if ((ignore >> i) & 0b1)
			continue;
		pinMode(i, INPUT);
	}
	
	// Enable RTC Wakeup as long as the battery voltage is safe
	if(getBatteryVoltage() > 3.65)
		esp_sleep_enable_ext0_wakeup((gpio_num_t) RTC_INT_PIN, 0); // enable deep sleep wake on RTC interrupt
	
	uint64_t mask = BTN_PIN_MASK | ACC_INT_MASK;
	if(doubleTapBtn == 0) mask = BTN_PIN_MASK;
	
	esp_sleep_enable_ext1_wakeup(
		mask,
		ESP_EXT1_WAKEUP_ANY_HIGH
	); // enable deep sleep wake on button press
	
	printDebug("Going to sleep...");
	
	esp_deep_sleep_start();
}


float PowerManager::getBatteryVoltage() {
	if (mRTC.rtcType == DS3231) {
		return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f; // Battery voltage goes through a 1/2 divider.
	} else {
		return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f;
	}
}

float PowerManager::getBatteryPercentage() {
	long voltage = getBatteryVoltage() * 1000;
	
	double percentage = map(voltage, 3600, 4200, 0, 100);
	
	percentage = min(100.0, percentage);
	percentage = max(  0.0, percentage);
	
	return percentage;
}

void PowerManager::checkCharging() {
	long voltage = getBatteryVoltage() * 1000;
	if(lastVoltage == 0) lastVoltage = voltage;
	
	long diff = max(voltage, lastVoltage) - min(voltage, lastVoltage);
	if(!isBatteryCharging && lastVoltage < voltage - MIN_VOLTAGE_CHANGE_CHARGING) {
		isBatteryCharging = true;
		printDebug("Watch is now charging.");
	}else if(isBatteryCharging && lastVoltage > voltage + MIN_VOLTAGE_CHANGE_DISCHARGING) {
		isBatteryCharging = false;
		printDebug("Watch started discharging.");
	}

	lastVoltage = voltage;
}

bool PowerManager::isCharging() {
	return isBatteryCharging;
}