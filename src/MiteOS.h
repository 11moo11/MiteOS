#ifndef MITEOS_H
#define MITEOS_H

#define MITE_OS_VER "20241229"

#define BACKGROUND_COLOR (DARKMODE ? GxEPD_BLACK : GxEPD_WHITE)
#define FOREGROUND_COLOR (DARKMODE ? GxEPD_WHITE : GxEPD_BLACK)

#include <Arduino.h>
#include <bma.h>
#include <GxEPD2_BW.h>
#include <Display.h>
#include <TimeLib.h>
#include <WatchyRTC.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "UI/Page.h"
#include "Data/Configuration.h"

#include "lang/lang.h"

// Manager
#include "Managers/PageManager.h"
#include "Managers/WeatherManager.h"
#include "Managers/AlertManager.h"
#include "Managers/ActivityManager.h"
#include "Managers/BluetoothManager.h"
#include "Managers/WifiConnectionManager.h"
#include "Managers/PowerManager.h"

#include "Display/MiteDisplay.h"

#include <esp_chip_info.h>

#define ADDITONAL_BUTTON_CHECK_DURATION 1000
#define BUTTON_PRESS_REPEAT_DELAY 150

static bool displayPoweredOn = false;


#define ALARM_MODE_ONCE 0
#define ALARM_MODE_WORKDAY 1
#define ALARM_MODE_WEEKEND 2
#define ALARM_MODE_DAILY 3

typedef struct AlarmData {
	bool enableAlarm;
	bool triggered;
	uint8_t mode;
	uint8_t hour;
	uint8_t minute;
} AlarmData;

#define NOW makeTime(MiteOS::currentTime)

class MiteOS {
	public:
		static WatchyDisplay watchyDisplay;
		static MiteDisplay<WatchyDisplay, WatchyDisplay::HEIGHT> display;
		static WatchyRTC RTC;
		static tmElements_t currentTime;
		static MiteOS *instance;

	public:
		explicit MiteOS() { }; // constructor
		void init();
		void handleButtonPress();
		
		static uint8_t getBoardRevision();
		
		static void initDarkmode();
	private:
		void checkTime();  
		void _bmaConfig();
		static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
		static uint16_t _writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
		void waitForAdditionalButtons();
		void handleAdditionalButtonPress(uint8_t buttonIndex, unsigned long *holdingTime);
};

extern RTC_DATA_ATTR BMA423 accSensor;
//extern RTC_DATA_ATTR bool WIFI_CONFIGURED;
//extern RTC_DATA_ATTR bool BLE_CONFIGURED;
extern RTC_DATA_ATTR PageData pageData;

extern RTC_DATA_ATTR int gmtTimeOffset;

extern RTC_DATA_ATTR tmElements_t osBootTime;

extern RTC_DATA_ATTR bool AUTO_DARKMODE;
extern RTC_DATA_ATTR bool PREF_DARKMODE;
extern RTC_DATA_ATTR bool DARKMODE;

#define ALARM_COUNT 3

extern RTC_DATA_ATTR AlarmData timer;
extern RTC_DATA_ATTR AlarmData alarms[ALARM_COUNT];

extern RTC_DATA_ATTR bool hourVibrate;
extern RTC_DATA_ATTR bool btnFeedbackVibrate;
extern RTC_DATA_ATTR u_int8_t doubleTapBtn;

#define mDisplay MiteOS::display
#define mRTC MiteOS::RTC

#if DEBUG == true
#define printDebug(a) Serial.println(a)
#else
#define printDebug(a)
#endif

#endif