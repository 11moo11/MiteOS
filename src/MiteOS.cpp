#include "MiteOS.h"

WatchyRTC MiteOS::RTC;
WatchyDisplay MiteOS::watchyDisplay {};
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> MiteOS::display(watchyDisplay);
MiteOS *MiteOS::instance;

RTC_DATA_ATTR BMA423 accSensor;
//RTC_DATA_ATTR bool BLE_CONFIGURED;
//RTC_DATA_ATTR WeatherData currentWeather;
//RTC_DATA_ATTR int weatherIntervalCounter = -1;
RTC_DATA_ATTR long gmtTimeOffset = 0;

RTC_DATA_ATTR tmElements_t osBootTime;

RTC_DATA_ATTR PageData pageData;
RTC_DATA_ATTR bool AUTO_DARKMODE;
RTC_DATA_ATTR bool PREF_DARKMODE;
RTC_DATA_ATTR bool DARKMODE;

RTC_DATA_ATTR AlarmData timer;
RTC_DATA_ATTR AlarmData alarms[ALARM_COUNT];

tmElements_t MiteOS::currentTime;

RTC_DATA_ATTR bool hourVibrate = true;

void MiteOS::init() {
	#ifdef DEBUG
	Serial.begin(115200);
  	if(!Serial) delay(1000);
	Serial.println("Booting up");
	#endif

	instance = this;

	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause(); // get wake up reason
	Wire.begin(SDA, SCL);                         // init i2c
	RTC.init();

	// Init the display since is almost sure we will use it
	display.epd2.initWatchy();
	RTC.read(currentTime);

	switch (wakeup_reason) {
		case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
			#ifdef DEBUG
			Serial.println("RTC Alarm");
			#endif
			checkTime();
			
			WeatherManager::timeTick();
			
			//vibMotor(75, 4);
			PageManager::refreshPage();
			break;
		case ESP_SLEEP_WAKEUP_EXT1: // button Press			
			if (esp_sleep_get_ext1_wakeup_status() & ACC_INT_MASK) { // Woken up by accelerator
				#ifdef DEBUG
				Serial.println("Accelerator");
				#endif
				if(accSensor.isTilt()) {
					#ifdef DEBUG
					Serial.println("Tilt");
					#endif
					// TODO: Future low power mode (maybe only at night)
				}
				
				if(accSensor.isDoubleClick()) {
					PageManager::handleButtonPress(BTN_DOUBLE_TAP);
				}else{
					PageManager::handleButtonPress(BTN_SINGLE_TAP);
				}
				break;
			}
			#ifdef DEBUG
			Serial.println("Button Press");
			#endif
			
			//vibMotor(75, 4);
			handleButtonPress();
			waitForAdditionalButtons();
			break;
		default: // reset
			#ifdef DEBUG
			Serial.println("Reset Boot");
			#endif
			// Initial configuration
			_bmaConfig();
			pageData.pageIndex = 0; // Set Page to Watchface
			gmtTimeOffset = settings.gmtOffset;

			RTC.read(osBootTime);
			
			#ifdef DEBUG
			Serial.println("Restoring Settings File");
			#endif
			Configuration::loadAll();
			
			initDarkmode();
			
			PageManager::refreshPage(false); // full update on reset
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
		BTN_PIN_MASK/* | ACC_INT_MASK */,
		ESP_EXT1_WAKEUP_ANY_HIGH
	); // enable deep sleep wake on button press

	esp_deep_sleep_start();
}

void MiteOS::waitForAdditionalButtons() {
	bool timeout     = false;
	long lastTimeout = millis();
	pinMode(MENU_BTN_PIN, INPUT);
	pinMode(BACK_BTN_PIN, INPUT);
	pinMode(UP_BTN_PIN, INPUT);
	pinMode(DOWN_BTN_PIN, INPUT);

	// Initialize with one so they dont get registered again and need to wait the 500ms
	unsigned long MenuBtnPressedSince = 1;
	unsigned long BackBtnPressedFor = 1;
	unsigned long UpBtnPressedFor = 1;
	unsigned long DownBtnPressedFor = 1;

	while (!timeout) {
		if (millis() - lastTimeout > ADDITONAL_BUTTON_CHECK_DURATION) {
      		timeout = true;
    	} else {
			if (digitalRead(MENU_BTN_PIN) == 1) {
				lastTimeout = millis();
				handleAdditionalButtonPress(BTN_MENU, &MenuBtnPressedSince);
			} else {
				MenuBtnPressedSince = 0;
			}
			
			if (digitalRead(BACK_BTN_PIN) == 1) {
				lastTimeout = millis();
				handleAdditionalButtonPress(BTN_BACK, &BackBtnPressedFor);
			} else {
				BackBtnPressedFor = 0;
			}
			
			if (digitalRead(UP_BTN_PIN) == 1) {
				lastTimeout = millis();
				handleAdditionalButtonPress(BTN_UP, &UpBtnPressedFor);
			} else {
				UpBtnPressedFor = 0;
			}
			
			if (digitalRead(DOWN_BTN_PIN) == 1) {
				lastTimeout = millis();
				handleAdditionalButtonPress(BTN_DOWN, &DownBtnPressedFor);
			} else {
				DownBtnPressedFor = 0;
			}
		}
		delay(50);
	}
}

void MiteOS::handleAdditionalButtonPress(uint8_t buttonIndex, unsigned long *lastTime) {
	unsigned long currentTime = millis();

	unsigned long holdingTime = 0;
	
	if(*lastTime > 0)
		holdingTime = currentTime - *lastTime;
	else
		holdingTime = BUTTON_PRESS_REPEAT_DELAY + 1;

	// Only accept additional buttons once its been held for longer time or pressed again
	if(*lastTime == 0 || holdingTime > BUTTON_PRESS_REPEAT_DELAY) {
		if(holdingTime > (unsigned long) BUTTON_PRESS_REPEAT_DELAY) {
			//holdingTime -= (unsigned long) BUTTON_PRESS_REPEAT_DELAY;
			PageManager::handleButtonPress(buttonIndex);
		}
		*lastTime = currentTime;
	}
}

void MiteOS::handleButtonPress() {
	uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

	if (wakeupBit & MENU_BTN_MASK) { // MENU Button (Bottom Left)
		PageManager::handleButtonPress(BTN_MENU);
		return;
	}else if (wakeupBit & BACK_BTN_MASK) { // Back Button (Top Left)
		PageManager::handleButtonPress(BTN_BACK);
		return;
	} else if (wakeupBit & UP_BTN_MASK) { // Up Button (Top Right)
		PageManager::handleButtonPress(BTN_UP);
		return;
	} else if (wakeupBit & DOWN_BTN_MASK) { // Down Button (Bottom Right)
		PageManager::handleButtonPress(BTN_DOWN);
		return;
	}
}

void MiteOS::checkTime() {
	if (hourVibrate) {
		if (currentTime.Minute == 0) {
			// The RTC wakes us up once per minute
			vibMotor(75, 4);
		}
	}

	if(AUTO_DARKMODE) {
		if(MiteOS::currentTime.Hour == settings.darkmodeStartH && MiteOS::currentTime.Minute == settings.darkmodeStartM) {
			DARKMODE = !settings.inverseDarkMode;
		}else if(MiteOS::currentTime.Hour == settings.darkmodeEndH && MiteOS::currentTime.Minute == settings.darkmodeEndM) {
			DARKMODE = settings.inverseDarkMode;
		}
	}

	if(timer.triggered
	|| (timer.enableAlarm && currentTime.Hour == timer.hour && currentTime.Minute == timer.minute)) {
		timer.triggered = true;
		pageData.pageIndex = 1;
		vibMotor(100, 20);
	}
}

void MiteOS::initDarkmode() {
	if(AUTO_DARKMODE) {
		if(MiteOS::currentTime.Hour > instance->settings.darkmodeStartH || (MiteOS::currentTime.Hour == instance->settings.darkmodeStartH && MiteOS::currentTime.Minute >= instance->settings.darkmodeStartM)) {
			DARKMODE = !instance->settings.inverseDarkMode;
		}else if(MiteOS::currentTime.Hour < instance->settings.darkmodeEndH || (MiteOS::currentTime.Hour == instance->settings.darkmodeEndH && MiteOS::currentTime.Minute <= instance->settings.darkmodeEndM)) {
			DARKMODE = !instance->settings.inverseDarkMode;
		}else{
			DARKMODE = instance->settings.inverseDarkMode;
		}
	}else{
		DARKMODE = PREF_DARKMODE;
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
	
	/*
	struct bma423_axes_remap remap_data;
	remap_data.x_axis      = 1;
	remap_data.x_axis_sign = 0xFF;
	remap_data.y_axis      = 0;
	remap_data.y_axis_sign = 0xFF;
	remap_data.z_axis      = 2;
	remap_data.z_axis_sign = 0xFF;
	// Need to raise the wrist function, need to set the correct axis
	accSensor.setRemapAxes(&remap_data);
	*/
	struct bma423_axes_remap remap_data = {
			.x_axis      = 1,
			.x_axis_sign = 0,
			.y_axis      = 0,
			.y_axis_sign = 0,
			.z_axis      = 2,
			.z_axis_sign = 1,
	};

	accSensor.setRemapAxes(&remap_data);
	
	// Enable BMA423 isStepCounter feature
	accSensor.enableFeature(BMA423_STEP_CNTR, true);
	// Enable BMA423 isTilt feature
	accSensor.enableFeature(BMA423_TILT, false);
	// Enable BMA423 isDoubleClick feature
	accSensor.enableFeature(BMA423_WAKEUP, true);
	
	// Reset steps
	//accSensor.resetStepCounter();
	
	// Turn on feature interrupt
	//accSensor.enableStepCountInterrupt();
	//accSensor.enableTiltInterrupt();
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

float MiteOS::getBatteryPercentage() {
    float voltage = getBatteryVoltage();

    uint8_t percentage = 2808.3808 * pow(voltage, 4)
                        - 43560.9157 * pow(voltage, 3)
                        + 252848.5888 * pow(voltage, 2)
                        - 650767.4615 * voltage
                        + 626532.5703;
    percentage = min((uint8_t) 100, percentage);
    percentage = max((uint8_t) 0, percentage);
    return percentage;
}

uint8_t MiteOS::getBoardRevision() {
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	
	if(chip_info.model == CHIP_ESP32){ //Revision 1.0 - 2.0
		Wire.beginTransmission(0x68); //v1.0 has DS3231
		if (Wire.endTransmission() == 0){
			return 10;
		}
		delay(1);
		Wire.beginTransmission(0x51); //v1.5 and v2.0 have PCF8563
		if (Wire.endTransmission() == 0){
			pinMode(35, INPUT);
			if(digitalRead(35) == 0){
				return 20; //in rev 2.0, pin 35 is BTN 3 and has a pulldown
			}else{
				return 15; //in rev 1.5, pin 35 is the battery ADC
			}
		}
	}
	if(chip_info.model == CHIP_ESP32S3){ //Revision 3.0
		return 30;
	}
	return -1;
}