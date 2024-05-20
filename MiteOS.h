#ifndef MITEOS_H
#define MITEOS_H

#define DEBUG

#include <Arduino.h>
#include <bma.h>
#include <GxEPD2_BW.h>
#include <Display.h>
#include <TimeLib.h>
#include <WatchyRTC.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// Pages
#include "Page.h"
#include "WatchfacePage.h"

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
	//
	bool enableAutoDarkMode;
	bool inverseDarkMode;
	uint8_t darkmodeStartH;
	uint8_t darkmodeStartM;
	uint8_t darkmodeEndH;
	uint8_t darkmodeEndM;
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
		static tmElements_t currentTime;

	public:
		explicit MiteOS(const MiteSettings &s) : settings(s) {} // constructor
		void init();
		void deepSleep();
  		void handleButtonPress();
  		void handleButtonPress(uint8_t buttonIndex);
		void refreshPage(bool partialRefresh = true);
  		void vibMotor(uint8_t intervalMs = 100, uint8_t length = 20);
  		static float getBatteryVoltage();
	
	private:
		void updateTime();  
		void _bmaConfig();
		static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
		static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
		void initDarkmode();
};

extern RTC_DATA_ATTR BMA423 accSensor;
//extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
//extern RTC_DATA_ATTR bool BLE_CONFIGURED;
extern RTC_DATA_ATTR PageData pageData;

extern RTC_DATA_ATTR bool DARKMODE;

#endif