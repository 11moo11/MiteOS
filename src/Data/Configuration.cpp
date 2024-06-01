#include "Configuration.h"

#include "../UI/WatchfacePage.h"
#include "../Managers/BluetoothManager.h"

bool Configuration::initialized = false;
Preferences Configuration::preferences;


void Configuration::init() {
	preferences.begin("miteos", false); 
	initialized = true;
	
	#ifdef DEBUG
	Serial.println("Initialized Configuration");
	#endif
}

void Configuration::saveAll() {
	if(!initialized) init();
	
	saveAlarms();
	saveSettings();
	saveBluetooth();
}

void Configuration::loadAll() {
	if(!initialized) init();
	
	loadAlarms();
	loadSettings();
	loadBluetooth();
}


void Configuration::saveAlarms() {
	if(!initialized) init();
	
	for(int i = 0; i < ALARM_COUNT; i++) {
		preferences.putBool(("alr" + String(i) + "en").c_str(), alarms[i].enableAlarm);
		preferences.putUInt(("alr" + String(i) + "hr").c_str(), alarms[i].hour);
		preferences.putUInt(("alr" + String(i) + "min").c_str(), alarms[i].minute);
		preferences.putUInt(("alr" + String(i) + "mode").c_str(), alarms[i].mode);
	}
	
	#ifdef DEBUG
	Serial.println("Saved Alarms");
	#endif
}
void Configuration::loadAlarms() {
	if(!initialized) init();
	
	for(int i = 0; i < ALARM_COUNT; i++) {
		alarms[i].enableAlarm = preferences.getBool(("alr" + String(i) + "en").c_str(), alarms[i].enableAlarm);
		alarms[i].hour		  = preferences.getUInt(("alr" + String(i) + "hr").c_str(), alarms[i].hour);
		alarms[i].minute 	  = preferences.getUInt(("alr" + String(i) + "min").c_str(), alarms[i].minute);
		alarms[i].mode 		  = preferences.getUInt(("alr" + String(i) + "mode").c_str(), alarms[i].mode);
	}
	#ifdef DEBUG
	Serial.println("Loaded Alarms");
	#endif
}


void Configuration::saveSettings() {
	if(!initialized) init();

	preferences.putBool("autodark", AUTO_DARKMODE);
	preferences.putBool("darkmode", PREF_DARKMODE);
	preferences.putBool("hourvib", hourVibrate);
	preferences.putUInt("watchface", watchFaceId);
	#ifdef DEBUG
	Serial.println("Saved Settings");
	#endif
}
void Configuration::loadSettings() {
	if(!initialized) init();
	
	AUTO_DARKMODE = preferences.getBool("autodark", AUTO_DARKMODE);
	PREF_DARKMODE = preferences.getBool("darkmode", PREF_DARKMODE);
	hourVibrate   = preferences.getBool("hourvib", hourVibrate);
	watchFaceId   = preferences.getUInt("watchface", watchFaceId);
	#ifdef DEBUG
	Serial.println("Loaded Settings");
	#endif
}

void Configuration::saveBluetooth() {
	if(!initialized) init();
	
	preferences.putString("btLastDevice", btLastDevice.toString().c_str());
	
	#ifdef DEBUG
	Serial.println("Saved Bluetooth");
	#endif
}

void Configuration::loadBluetooth() {
	if(!initialized) init();
	
	String deviceId = preferences.getString("btLastDevice", "0");
	if(deviceId.length() > 5) {
		btDeviceRegistered = true;
		btLastDevice = BLEAddress(deviceId.c_str());
	}
	
	Serial.println(btDeviceRegistered);
	Serial.println(btLastDevice.toString().c_str());
	
	#ifdef DEBUG
	Serial.println("Loaded Bluetooth");
	#endif
}


void Configuration::saveSteps() {
	if(!initialized) init();
	
	uint8_t dow = MiteOS::currentTime.Wday;
	if(MiteOS::currentTime.Hour == 0 && MiteOS::currentTime.Minute == 0) {
		dow--;
		if(dow == dowInvalid) {
			dow = dowSaturday;
		}
	}
	preferences.putUInt(("steps" + String(dow)).c_str(), ActivityManager::getStepCount());
}
std::array<uint32_t, 7> Configuration::loadSteps() {
	if(!initialized) init();
	
	std::array<uint32_t, 7> steps;
	
	for(uint8_t dow = 1; dow <= 7; dow++) {
		steps[dow - 1] = preferences.getUInt(("steps" + String(dow)).c_str(), 0);
	}
	steps[MiteOS::currentTime.Wday - 1] = ActivityManager::getStepCount();
	
	return steps;
}