#include "PowerManager.h"

#include "../MiteOS.h"
#include "../func/TimeFunc.h"

#define MIN_VOLTAGE_CHANGE_CHARGING 300 	// 0.3V
#define MIN_VOLTAGE_CHANGE_DISCHARGING 100 	// 0.1V

RTC_DATA_ATTR bool isBatteryCharging = false;
RTC_DATA_ATTR long lastVoltage = 0;
RTC_DATA_ATTR uint8_t currentPowerMode = POWER_MODE_MINUTLY;
RTC_DATA_ATTR uint8_t powerMap[24];

void PowerManager::deepSleep() {
	mDisplay.hibernate();
	
	BluetoothManager::powerOff();
	WifiConnectionManager::powerOff();
	
	// Set the RTC Wakeup behaviour, if its not waking up every minute, we need to at least let it wake up on the next alarm or timer
	mRTC.clearAlarm();        // resets the alarm flag in the RTC
	if(currentPowerMode == POWER_MODE_MINUTLY) {
		mRTC.nextMinuteWake(true);
	}else if(currentPowerMode == POWER_MODE_HOURLY) {
		AlarmData nextAlarm = AlarmManager::getNextAlarm();
		if(nextAlarm.enableAlarm) {
			long diff = TimeFunc::timeDiff(nextAlarm);
			if(diff < 60) {
				mRTC.atMinuteWake(nextAlarm.minute);
			}else{
				mRTC.atMinuteWake(0);
			}
		}else{
			mRTC.atMinuteWake(0);
		}
	}else if(currentPowerMode == POWER_MODE_SLEEP || currentPowerMode == POWER_MODE_LIFT) {
		AlarmData nextAlarm = AlarmManager::getNextAlarm();
		if(nextAlarm.enableAlarm) {
			mRTC.atTimeWake(nextAlarm.hour, nextAlarm.minute);
		}else{
			mRTC.atTimeWake(0, 0);
		}
	}
	
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
	if(doubleTapBtn == 0 && currentPowerMode != POWER_MODE_LIFT) mask = BTN_PIN_MASK;
	
	esp_sleep_enable_ext1_wakeup(
		mask,
		ESP_EXT1_WAKEUP_ANY_HIGH
	); // enable deep sleep wake on button press
	
	printDebug("Going to sleep...");
	
	esp_deep_sleep_start();
}


float PowerManager::getBatteryVoltage() {
	if (mRTC.getType() == 1) { // DS3231
		return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f; // Battery voltage goes through a 1/2 divider.
	} else {
		return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f;
	}
}

float PowerManager::getBatteryPercentage() {
	short voltage = getBatteryVoltage() * 1000;
	
	// Values from https://intofpv.com/t-lipo-voltage-quick-chart (Modified the 5% and 0% voltages to fit within the 3.6V cutoff voltage)
	short voltages[] = { 4200, 4150, 4110, 4080, 4020, 3980, 3950, 3910, 3870, 3850, 3840, 3820, 3800, 3790, 3770, 3750, 3730, 3710, 3690, 3640, 3600 };

	double percentage;
	if(voltage > voltages[0]) {
		percentage = 100;
	}else if(voltage < voltages[20]) {
		percentage = 0;
	}else {
		for(uint8_t i = 0; i <= 20; i++) {
			if(voltages[i] >= voltage && voltages[i + 1] <= voltage) {
				percentage = map(voltage, voltages[i + 1], voltages[i], (95.0 - i * 5.0), (100.0 - i * 5.0));
				break;
			}
		}
	}

	//double percentage = map(voltage, 3600, 4200, 0, 100);
	
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

void PowerManager::setPowerMode(uint8_t powerMode) {
	if(currentPowerMode != powerMode) {
		currentPowerMode = powerMode;
		
		MiteOS::updateBmaConfig();
	}
}