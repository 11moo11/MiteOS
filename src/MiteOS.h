#ifndef MITEOS_H
#define MITEOS_H

#define MITE_OS_VER "0.0.1"

#define DEBUG

#include <Arduino.h>
#include <bma.h>
#include <GxEPD2_BW.h>
#include <Display.h>
#include <TimeLib.h>
#include <WatchyRTC.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "UI/Page.h"

// Manager
#include "Managers/PageManager.h"
#include "Managers/WeatherManager.h"

#define ADDITONAL_BUTTON_CHECK_DURATION 500
#define BUTTON_PRESS_REPEAT_DELAY 150

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

typedef struct AlarmData {
	bool enableAlarm;
	bool triggered;
	int8_t hour;
	int8_t minute;
} AlarmData;

class MiteOS {
	public:
		MiteSettings settings;
		static WatchyDisplay watchyDisplay;
		static GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display;
		static WatchyRTC RTC;
		static tmElements_t currentTime;
		static MiteOS *instance;

	public:
		explicit MiteOS(const MiteSettings &s) : settings(s) {} // constructor
		void init();
		void deepSleep();
		void handleButtonPress();
		
  		static void vibMotor(uint8_t intervalMs = 100, uint8_t length = 20);
  		static float getBatteryVoltage();
		static float getBatteryPercentage();
		
		static uint8_t getBoardRevision();
		
	private:
		void checkTime();  
		void _bmaConfig();
		static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
		static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
		void initDarkmode();
		void waitForAdditionalButtons();
		void handleAdditionalButtonPress(uint8_t buttonIndex, unsigned long *holdingTime);
};

extern RTC_DATA_ATTR BMA423 accSensor;
//extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
//extern RTC_DATA_ATTR bool BLE_CONFIGURED;
extern RTC_DATA_ATTR PageData pageData;
extern RTC_DATA_ATTR tmElements_t osBootTime;

extern RTC_DATA_ATTR bool DARKMODE;

extern RTC_DATA_ATTR AlarmData timer;
extern RTC_DATA_ATTR AlarmData alarms[2];

#define BACKGROUND_COLOR (DARKMODE ? GxEPD_BLACK : GxEPD_WHITE)
#define FOREGROUND_COLOR (DARKMODE ? GxEPD_WHITE : GxEPD_BLACK)

#define mDisplay MiteOS::display
#define mRTC MiteOS::RTC

#endif