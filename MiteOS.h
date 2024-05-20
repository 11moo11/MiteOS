#ifndef MITEOS_H
#define MITEOS_H

#include <Arduino.h>
#include <bma.h>
#include <GxEPD2_BW.h>
#include <Display.h>
#include "UI/Page.h"
#include <TimeLib.h>
#include <WatchyRTC.h>

// Pages
#include "UI/WatchfacePage.h"

typedef struct MiteSettings {
	// Weather Settings
	String cityID;
	String lat;
	String lon;
	String weatherAPIKey;
	String weatherURL;
	String weatherUnit;
	String weatherLang;
	int8_t weatherUpdateInterval;
	// NTP Settings
	String ntpServer;
	int gmtOffset;
	//
	bool vibrateOClock;
} MiteSettings;

typedef struct WeatherData {
	int8_t temperature;
	int16_t weatherConditionCode;
	bool isMetric;
	String weatherDescription;
	bool external;
	tmElements_t sunrise;
	tmElements_t sunset;
} WeatherData;

class MiteOS {
	public:
		MiteSettings settings;
		static GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;
		static WatchyRTC RTC;
		tmElements_t currentTime;

	public:
		explicit MiteOS(const MiteSettings &s) : settings(s) {} // constructor
		void init();
		void deepSleep();
};

//extern RTC_DATA_ATTR BMA423 sensor;
//extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
//extern RTC_DATA_ATTR bool BLE_CONFIGURED;
#endif