#include "MiteOS.h"

WatchyRTC MiteOS::RTC;
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> MiteOS::display(WatchyDisplay{});

//RTC_DATA_ATTR int pageIndex;
//RTC_DATA_ATTR int menuIndex;

RTC_DATA_ATTR BMA423 accSensor;
//RTC_DATA_ATTR bool WIFI_CONFIGURED;
//RTC_DATA_ATTR bool BLE_CONFIGURED;
//RTC_DATA_ATTR WeatherData currentWeather;
//RTC_DATA_ATTR int weatherIntervalCounter = -1;
RTC_DATA_ATTR long gmtTimeOffset = 0;

//RTC_DATA_ATTR tmElements_t bootTime;
//RTC_DATA_ATTR uint32_t lastIPAddress;
//RTC_DATA_ATTR char lastSSID[30];

const Page pages[] = {
	WatchfacePage(),
	WatchfacePage()
};

void MiteOS::init() {
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause(); // get wake up reason
	Wire.begin(SDA, SCL);                         // init i2c
	RTC.init();

	// Init the display since is almost sure we will use it
	display.epd2.initWatchy();

	switch (wakeup_reason) {
		case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
			RTC.read(currentTime);
		case ESP_SLEEP_WAKEUP_EXT1: // button Press
			//handleButtonPress();
			break;
		default: // reset
			//RTC.config(datetime);
			//_bmaConfig();
			gmtTimeOffset = settings.gmtOffset;
			RTC.read(currentTime);
			//RTC.read(bootTime);
			//showWatchFace(false); // full update on reset
			//vibMotor(75, 4);
			// For some reason, seems to be enabled on first boot
			esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
			break;
	}
	deepSleep();
}

void MiteOS::deepSleep() {
	display.hibernate();
	RTC.clearAlarm();        // resets the alarm flag in the RTC

	// Set GPIOs 0-39 to input to avoid power leaking out
	const uint64_t ignore = 0b11110001000000110000100111000010; // Ignore some GPIOs due to resets
	for (int i = 0; i < GPIO_NUM_MAX; i++) {
		if ((ignore >> i) & 0b1)
			continue;
		pinMode(i, INPUT);
	}
	esp_sleep_enable_ext0_wakeup((gpio_num_t) RTC_INT_PIN, 0); // enable deep sleep wake on RTC interrupt
	esp_sleep_enable_ext1_wakeup(
		BTN_PIN_MASK,
		ESP_EXT1_WAKEUP_ANY_HIGH
	); // enable deep sleep wake on button press

	esp_deep_sleep_start();
}
