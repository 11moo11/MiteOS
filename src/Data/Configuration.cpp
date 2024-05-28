#include "Configuration.h"

#include "../UI/WatchfacePage.h"

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
}

void Configuration::loadAll() {
	if(!initialized) init();
	
	loadAlarms();
	loadSettings();
}


void Configuration::saveAlarms() {
	if(!initialized) init();
	
	for(int i = 0; i < ALARM_COUNT; i++) {
		preferences.putBool(("alr" + String(i) + "en").c_str(), alarms[i].enableAlarm);
		preferences.putUInt(("alr" + String(i) + "hr").c_str(), alarms[i].hour);
		preferences.putUInt(("alr" + String(i) + "min").c_str(), alarms[i].minute);
		preferences.putUInt(("alr" + String(i) + "mode").c_str(), alarms[i].mode);
		
		#ifdef DEBUG
		Serial.print(alarms[i].enableAlarm);
		Serial.print(" ");
		Serial.print(alarms[i].hour);
		Serial.print(":");
		Serial.println(alarms[i].minute);
		#endif
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
		
		#ifdef DEBUG
		Serial.print(alarms[i].enableAlarm);
		Serial.print(" ");
		Serial.print(alarms[i].hour);
		Serial.print(":");
		Serial.println(alarms[i].minute);
		#endif
	}
	#ifdef DEBUG
	Serial.println("Loaded Alarms");
	#endif
}

void Configuration::saveSettings() {
	if(!initialized) init();
	
	preferences.putBool("darkmode", DARKMODE);
	preferences.putUInt("watchface", watchFaceId);
	#ifdef DEBUG
	Serial.println("Saved Settings");
	#endif
}
void Configuration::loadSettings() {
	if(!initialized) init();
	
	DARKMODE = preferences.getBool("darkmode", DARKMODE);
	watchFaceId = preferences.getUInt("watchface", watchFaceId);
	#ifdef DEBUG
	Serial.println("Loaded Settings");
	#endif
}