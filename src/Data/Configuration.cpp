#include "Configuration.h"

#include "../UI/WatchfacePage.h"
#include "../Managers/BluetoothManager.h"

#include <nvs.h>

bool Configuration::initialized = false;
Preferences Configuration::preferences;


void Configuration::init() {
	if(initialized) return;
	
	preferences.begin("miteos", false); 
	initialized = true;
	
	printDebug("Initialized Configuration");
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
	
	printDebug("Saved Alarms");
}
void Configuration::loadAlarms() {
	if(!initialized) init();
	
	for(int i = 0; i < ALARM_COUNT; i++) {
		alarms[i].enableAlarm = preferences.getBool(("alr" + String(i) + "en").c_str(), alarms[i].enableAlarm);
		alarms[i].hour		  = preferences.getUInt(("alr" + String(i) + "hr").c_str(), alarms[i].hour);
		alarms[i].minute 	  = preferences.getUInt(("alr" + String(i) + "min").c_str(), alarms[i].minute);
		alarms[i].mode 		  = preferences.getUInt(("alr" + String(i) + "mode").c_str(), alarms[i].mode);
	}
	printDebug("Loaded Alarms");
}


void Configuration::saveSettings() {
	if(!initialized) init();

	preferences.putBool("autodark",  AUTO_DARKMODE);
	preferences.putBool("darkmode",  PREF_DARKMODE);
	preferences.putBool("hourvib",   hourVibrate);
	preferences.putBool("btnvib",    btnFeedbackVibrate);
	preferences.putUInt("dblTapBtn", doubleTapBtn);
	preferences.putUInt("watchface", watchFaceId);
	
	printDebug("Saved Settings");
}
void Configuration::loadSettings() {
	if(!initialized) init();
	
	AUTO_DARKMODE = preferences.getBool("autodark", AUTO_DARKMODE);
	PREF_DARKMODE = preferences.getBool("darkmode", PREF_DARKMODE);
	hourVibrate   = preferences.getBool("hourvib", hourVibrate);
	btnFeedbackVibrate = preferences.getBool("btnvib", btnFeedbackVibrate);
	doubleTapBtn  = preferences.getUInt("dblTapBtn", doubleTapBtn);
	watchFaceId   = preferences.getUInt("watchface", watchFaceId);
	
	printDebug("Loaded Settings");
}

void Configuration::saveBluetooth() {
	if(!initialized) init();
	
	if(btDeviceRegistered) {
		preferences.putString("btLastDevice", btLastDevice.toString().c_str());
	}else{
		preferences.remove("btLastDevice");
	}
	
	printDebug("Saved Bluetooth");
}

void Configuration::loadBluetooth() {
	if(!initialized) init();
	
	String deviceId = preferences.getString("btLastDevice", "0");
	if(deviceId.length() > 5) {
		btDeviceRegistered = true;
		btLastDevice = BLEAddress(deviceId.c_str());
	}
	
	printDebug(btDeviceRegistered);
	printDebug(btLastDevice.toString().c_str());
	
	printDebug("Loaded Bluetooth");
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


void Configuration::saveNotification(uint8_t index, Notification n) {
	String prefix = "n" + String(index);
	Configuration::preferences.putString((prefix + "app").c_str(), n.app_name);
	Configuration::preferences.putString((prefix + "title").c_str(), n.title);
	Configuration::preferences.putString((prefix + "text").c_str(), n.message);
}

Notification Configuration::loadNotification(uint8_t index) {
	String prefix = "n" + String(index);
	
	Notification n;
	
	Configuration::preferences.getString((prefix + "app").c_str(), "---").toCharArray(n.app_name, NOTIFICATION_APP_NAME_LENGTH);
	Configuration::preferences.getString((prefix + "title").c_str(), "---").toCharArray(n.title, NOTIFICATION_TITLE_LENGTH);
	Configuration::preferences.getString((prefix + "text").c_str(), "---").toCharArray(n.message, NOTIFICATION_MESSAGE_LENGTH);
	
	return n;
}


int Configuration::getSize() {
	nvs_stats_t nvs_stats;
	esp_err_t err = nvs_get_stats(NULL, &nvs_stats);
	
	
    if(err){
        return 0;
    }
    return nvs_stats.total_entries;
}
int Configuration::usedSpace() {
	nvs_stats_t nvs_stats;
	esp_err_t err = nvs_get_stats(NULL, &nvs_stats);
	
	if(err){
		return 0;
	}
	return nvs_stats.used_entries;
}
int Configuration::freeSpace() {
	return getSize() - usedSpace();
}