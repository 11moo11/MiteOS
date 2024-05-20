#include "MiteOS.h"

WatchyRTC MiteOS::RTC;
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> MiteOS::display(WatchyDisplay{});

RTC_DATA_ATTR BMA423 accSensor;
//RTC_DATA_ATTR bool WIFI_CONFIGURED;
//RTC_DATA_ATTR bool BLE_CONFIGURED;
//RTC_DATA_ATTR WeatherData currentWeather;
//RTC_DATA_ATTR int weatherIntervalCounter = -1;
RTC_DATA_ATTR long gmtTimeOffset = 0;

RTC_DATA_ATTR tmElements_t osBootTime;
//RTC_DATA_ATTR uint32_t lastIPAddress;
//RTC_DATA_ATTR char lastSSID[30];

RTC_DATA_ATTR PageData pageData;
RTC_DATA_ATTR bool DARKMODE;

WatchfacePage watchfacePage;
Page* pages[] = {
	&watchfacePage
};

tmElements_t MiteOS::currentTime;

void MiteOS::init() {
	#ifdef DEBUG
	Serial.begin(115200);
  	while(!Serial) delay(100);
	Serial.println("Booting up");
	#endif

	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause(); // get wake up reason
	Wire.begin(SDA, SCL);                         // init i2c
	RTC.init();

	// Init the display since is almost sure we will use it
	display.epd2.initWatchy();

	switch (wakeup_reason) {
		case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
			#ifdef DEBUG
			Serial.println("RTC Alarm");
			#endif
			//vibMotor(75, 4);
			refreshPage();
			break;
		case ESP_SLEEP_WAKEUP_EXT1: // button Press
			#ifdef DEBUG
			Serial.println("Button Press");
			#endif
			//vibMotor(75, 4);
			handleButtonPress();
			break;
		default: // reset
			#ifdef DEBUG
			Serial.println("Reset Boot");
			#endif
			// Initial configuration
			_bmaConfig();
			pageData.pageIndex = 0; // Set Page to Watchface
			gmtTimeOffset = settings.gmtOffset;
			updateTime();
			initDarkmode();

			RTC.read(osBootTime);
			
			refreshPage(false); // full update on reset
			vibMotor(75, 4);
			
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

void MiteOS::handleButtonPress() {
	uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

	if (wakeupBit & MENU_BTN_MASK) { // MENU Button (Bottom Left)
		handleButtonPress(BTN_MENU);
		return;
	}else if (wakeupBit & BACK_BTN_MASK) { // Back Button (Top Left)
		handleButtonPress(BTN_BACK);
		return;
	} else if (wakeupBit & UP_BTN_MASK) { // Up Button (Top Right)
		handleButtonPress(BTN_UP);
		return;
	} else if (wakeupBit & DOWN_BTN_MASK) { // Down Button (Bottom Right)
		handleButtonPress(BTN_DOWN);
		return;
	}
}

void MiteOS::handleButtonPress(uint8_t buttonIndex) {
	// Check if the page is available to handle the input
	if(sizeof(pages) > pageData.pageIndex) {
		if(pages[pageData.pageIndex]->onButtonPressed(buttonIndex)) { // If the page handled the input, no need for the main OS to handle it
			refreshPage();
			return;
		}
	}

	// TODO: OS Level handling
}

void MiteOS::refreshPage(bool partialRefresh) {
	updateTime();
	
	display.setFullWindow();
	// At this point it is sure we are going to update
	display.epd2.asyncPowerOn();
	
	Serial.println("Power On Display");
	
	if(sizeof(pages) > pageData.pageIndex) {
		#ifdef DEBUG
		Serial.println("Rendering Page " + String(pageData.pageIndex));
		#endif
		
		pages[pageData.pageIndex]->drawPage();
	}
	
	MiteOS::display.display(partialRefresh);
}

void MiteOS::updateTime() {
	RTC.read(currentTime);

	if (settings.vibrateOClock) {
		if (currentTime.Minute == 0) {
			// The RTC wakes us up once per minute
			vibMotor(75, 4);
		}
	}

	if(settings.enableAutoDarkMode) {
		if(MiteOS::currentTime.Hour == settings.darkmodeStartH && MiteOS::currentTime.Minute == settings.darkmodeStartM) {
			DARKMODE = !settings.inverseDarkMode;
		}else if(MiteOS::currentTime.Hour == settings.darkmodeEndH && MiteOS::currentTime.Minute == settings.darkmodeEndM) {
			DARKMODE = settings.inverseDarkMode;
		}
	}
}

void MiteOS::initDarkmode() {
	if(settings.enableAutoDarkMode) {
		if(MiteOS::currentTime.Hour > settings.darkmodeStartH || (MiteOS::currentTime.Hour == settings.darkmodeStartH && MiteOS::currentTime.Minute >= settings.darkmodeStartM)) {
			DARKMODE = !settings.inverseDarkMode;
		}else if(MiteOS::currentTime.Hour > settings.darkmodeEndH || (MiteOS::currentTime.Hour == settings.darkmodeEndH && MiteOS::currentTime.Minute >= settings.darkmodeEndM)) {
			DARKMODE = settings.inverseDarkMode;
		}
	}
	#ifdef DEBUG
	Serial.println("DARKMODE " + String(DARKMODE));
	#endif
	
}

void MiteOS::vibMotor(uint8_t intervalMs, uint8_t length) {
	pinMode(VIB_MOTOR_PIN, OUTPUT);
	bool motorOn = false;
	for (int i = 0; i < length; i++) {
		motorOn = !motorOn;
		digitalWrite(VIB_MOTOR_PIN, motorOn);
		delay(intervalMs);
	}
}

uint16_t MiteOS::_readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom((uint8_t)address, (uint8_t)len);
	uint8_t i = 0;
	while (Wire.available()) {
		data[i++] = Wire.read();
	}
	return 0;
}

uint16_t MiteOS::_writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.write(data, len);
	return (0 != Wire.endTransmission());
}

void MiteOS::_bmaConfig() {
	if (accSensor.begin(_readRegister, _writeRegister, delay) == false) {
		// fail to init BMA
		return;
	}

	// Accel parameter structure
	Acfg cfg;
	/*!
		Output data rate in Hz, Optional parameters:
			- BMA4_OUTPUT_DATA_RATE_0_78HZ
			- BMA4_OUTPUT_DATA_RATE_1_56HZ
			- BMA4_OUTPUT_DATA_RATE_3_12HZ
			- BMA4_OUTPUT_DATA_RATE_6_25HZ
			- BMA4_OUTPUT_DATA_RATE_12_5HZ
			- BMA4_OUTPUT_DATA_RATE_25HZ
			- BMA4_OUTPUT_DATA_RATE_50HZ
			- BMA4_OUTPUT_DATA_RATE_100HZ
			- BMA4_OUTPUT_DATA_RATE_200HZ
			- BMA4_OUTPUT_DATA_RATE_400HZ
			- BMA4_OUTPUT_DATA_RATE_800HZ
			- BMA4_OUTPUT_DATA_RATE_1600HZ
	*/
	cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
	/*!
		G-range, Optional parameters:
			- BMA4_ACCEL_RANGE_2G
			- BMA4_ACCEL_RANGE_4G
			- BMA4_ACCEL_RANGE_8G
			- BMA4_ACCEL_RANGE_16G
	*/
	cfg.range = BMA4_ACCEL_RANGE_2G;
	/*!
		Bandwidth parameter, determines filter configuration, Optional parameters:
			- BMA4_ACCEL_OSR4_AVG1
			- BMA4_ACCEL_OSR2_AVG2
			- BMA4_ACCEL_NORMAL_AVG4
			- BMA4_ACCEL_CIC_AVG8
			- BMA4_ACCEL_RES_AVG16
			- BMA4_ACCEL_RES_AVG32
			- BMA4_ACCEL_RES_AVG64
			- BMA4_ACCEL_RES_AVG128
	*/
	cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

	/*! Filter performance mode , Optional parameters:
		- BMA4_CIC_AVG_MODE
		- BMA4_CONTINUOUS_MODE
	*/
	cfg.perf_mode = BMA4_CONTINUOUS_MODE;

	// Configure the BMA423 accelerometer
	accSensor.setAccelConfig(cfg);

	// Enable BMA423 accelerometer
	// Warning : Need to use feature, you must first enable the accelerometer
	// Warning : Need to use feature, you must first enable the accelerometer
	accSensor.enableAccel();

	struct bma4_int_pin_config config;
	config.edge_ctrl = BMA4_LEVEL_TRIGGER;
	config.lvl       = BMA4_ACTIVE_HIGH;
	config.od        = BMA4_PUSH_PULL;
	config.output_en = BMA4_OUTPUT_ENABLE;
	config.input_en  = BMA4_INPUT_DISABLE;
	// The correct trigger interrupt needs to be configured as needed
	accSensor.setINTPinConfig(config, BMA4_INTR1_MAP);

	struct bma423_axes_remap remap_data;
	remap_data.x_axis      = 1;
	remap_data.x_axis_sign = 0xFF;
	remap_data.y_axis      = 0;
	remap_data.y_axis_sign = 0xFF;
	remap_data.z_axis      = 2;
	remap_data.z_axis_sign = 0xFF;
	// Need to raise the wrist function, need to set the correct axis
	accSensor.setRemapAxes(&remap_data);

	// Enable BMA423 isStepCounter feature
	accSensor.enableFeature(BMA423_STEP_CNTR, true);
	// Enable BMA423 isTilt feature
	accSensor.enableFeature(BMA423_TILT, true);
	// Enable BMA423 isDoubleClick feature
	accSensor.enableFeature(BMA423_WAKEUP, true);

	// Reset steps
	//accSensor.resetStepCounter();

	// Turn on feature interrupt
	accSensor.enableStepCountInterrupt();
	accSensor.enableTiltInterrupt();
	// It corresponds to isDoubleClick interrupt
	accSensor.enableWakeupInterrupt();
}

float MiteOS::getBatteryVoltage() {
	if (RTC.rtcType == DS3231) {
		return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f; // Battery voltage goes through a 1/2 divider.
	} else {
		return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f;
	}
}