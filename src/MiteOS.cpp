#include "MiteOS.h"

#include "Managers/FileManager.h"
#include "Managers/AlarmManager.h"

WatchyRTC MiteOS::RTC;
WatchyDisplay MiteOS::watchyDisplay {};
MiteDisplay<WatchyDisplay, WatchyDisplay::HEIGHT> MiteOS::display(watchyDisplay);
MiteOS *MiteOS::instance;

RTC_DATA_ATTR BMA423 accSensor;
//RTC_DATA_ATTR bool BLE_CONFIGURED;
//RTC_DATA_ATTR WeatherData currentWeather;
//RTC_DATA_ATTR int weatherIntervalCounter = -1;
RTC_DATA_ATTR int gmtTimeOffset = 0;

RTC_DATA_ATTR tmElements_t osBootTime;

RTC_DATA_ATTR PageData pageData;
RTC_DATA_ATTR bool AUTO_DARKMODE;
RTC_DATA_ATTR bool PREF_DARKMODE;
RTC_DATA_ATTR bool DARKMODE;

RTC_DATA_ATTR AlarmData timer;
RTC_DATA_ATTR AlarmData alarms[ALARM_COUNT];

tmElements_t MiteOS::currentTime;

RTC_DATA_ATTR bool hourVibrate = true;
RTC_DATA_ATTR bool btnFeedbackVibrate = true;
RTC_DATA_ATTR u_int8_t doubleTapBtn = 1;

#include "esp32-hal-cpu.h"

void MiteOS::init() {
	#if DEBUG == TRUE
	// This might cause a double reset, causing the esp to fully reset steps and stuff
	// So this should only be enabled when debugging
	// But: Thats just a theory at the moment and i will test it some more
	Serial.begin(921600); // Max speed so we have about 2FPS (115200 results in about 1 FPS)
  	if(!Serial) delay(1000);
	printDebug("Booting up");
	#endif
	
	instance = this;
	
	esp_sleep_wakeup_cause_t wakeup_reason;
	wakeup_reason = esp_sleep_get_wakeup_cause(); // get wake up reason
	Wire.begin(SDA, SCL);                         // init i2c
	RTC.init();
	
	printDebug(wakeup_reason);

	// Init the display since is almost sure we will use it
	mDisplay.epd2.setDarkBorder(DARKMODE); // Its too early at first start or after changes, but it fill fix it soon after, im too lazy right now to find the correct place for it, i only know it needs to be before init
	display.epd2.initWatchy();
	mDisplay.cp437(true);
	RTC.read(currentTime);

	switch (wakeup_reason) {
		case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
			printDebug("RTC Alarm");
			checkTime();
			
			//vibMotor(75, 4);
			PageManager::refreshPage();
	
			PhoneConnectionManager::SyncNotifications();
			break;
		case ESP_SLEEP_WAKEUP_EXT1: // button Press or Accelerometer
			if (esp_sleep_get_ext1_wakeup_status() & ACC_INT_MASK) { // Woken up by accelerator
				printDebug("Accelerator");
				
				accSensor.getINT(); // Seems like this needs to be done to clear the interrupt :/
				
				/*
				if(accSensor.isTilt()) {
					printDebug("Tilt");
					// TODO: Future low power mode? (maybe only at night)
				}else if(accSensor.isActivity()) {
					printDebug("Activity");
				}else if(accSensor.isAnyNoMotion()) {
					printDebug("No Motion");
				}else if(accSensor.isStepCounter()) {
					printDebug("Step Counter");
				}
				*/
				
				if(accSensor.isDoubleClick()) {
					PageManager::handleButtonPress(BTN_DOUBLE_TAP);
				}else{
					//PageManager::handleButtonPress(BTN_SINGLE_TAP);
				}
				
				break;
			}
			printDebug("Button Press");
			
			//vibMotor(75, 4);
			handleButtonPress();
			waitForAdditionalButtons();
			break;
		
		
		// Just to figure out why random rebooting, not really used
		// Also to prevent not handled wakeup interrupts to cause the watchy to reset
		case ESP_SLEEP_WAKEUP_TIMER:
			printDebug("Wakeup caused by timer");
			break;
		case ESP_SLEEP_WAKEUP_TOUCHPAD:
			printDebug("Wakeup caused by touchpad");
			break;
		case ESP_SLEEP_WAKEUP_ULP:
			Serial.println("Wakeup caused by ULP program");
			break;
		
		case ESP_SLEEP_WAKEUP_UNDEFINED: // reset
			printDebug("Reset Boot");
			printDebug(esp_reset_reason());
			// Initial configuration
			_bmaConfig();
			pageData.pageIndex = 0; // Set Page to Watchface
			gmtTimeOffset = Configuration::getGmtOffset();
			
			RTC.read(osBootTime);
			
			printDebug("Restoring Settings File");
			Configuration::loadAll();
			ActivityManager::restoreSteps();
			
			initDarkmode();
			
			PageManager::refreshPage(false); // full update on reset
			// AlertManager::vibMotor(75, 4);
			
			setCpuFrequencyMhz(80); // I don't think it really works, but just set it to lowest frequency possible
			
			#if DEBUG
			Serial.println("==============================");
			FileManager::printFolder("/");
			Serial.println("==============================");
			#endif

			// For some reason, seems to be enabled on first boot
			esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
			break;
		
		default: break;
	}
	PowerManager::deepSleep();
}

void MiteOS::waitForAdditionalButtons() {
	bool timeout     = false;
	long lastTimeout = millis();
	pinMode(MENU_BTN_PIN, INPUT);
	pinMode(BACK_BTN_PIN, INPUT);
	pinMode(  UP_BTN_PIN, INPUT);
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
			if(accSensor.getINT()) {
				if(accSensor.isDoubleClick()) {
					lastTimeout = millis();
					PageManager::handleButtonPress(BTN_DOUBLE_TAP);
				}else{
					//lastTimeout = millis();
					//PageManager::handleButtonPress(BTN_SINGLE_TAP);
				}
			}
			
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
			AlertManager::vibMotor(75, 4);
		}
	}
	
	if(currentTime.Hour == 0 && currentTime.Minute == 0) {
		ActivityManager::resetSteps();
	}else if(currentTime.Minute % 10 == 0) { // Saving steps each 10 minutes to make sure when the clock crashes its not loosing all progress
		ActivityManager::saveSteps();
	}

	if(currentTime.Hour == 2 && currentTime.Minute == 1) {
		if(WifiConnectionManager::connectWifi()) {
			WifiConnectionManager::syncNTP();
		}
	}
	
	if(AUTO_DARKMODE) {
		if(MiteOS::currentTime.Hour == Configuration::getDarkmodeStartH() && MiteOS::currentTime.Minute == Configuration::getDarkmodeStartM()) {
			DARKMODE = !Configuration::getInverseDarkMode();
		}else if(MiteOS::currentTime.Hour == Configuration::getDarkmodeEndH() && MiteOS::currentTime.Minute == Configuration::getDarkmodeEndM()) {
			DARKMODE = Configuration::getInverseDarkMode();
		}
	}

	AlarmManager::checkTimers();
	AlarmManager::checkAlarms();
	
	PowerManager::checkCharging();
	WeatherManager::timeTick();
	WeatherManager::getWeatherData();
}

void MiteOS::initDarkmode() {
	if(AUTO_DARKMODE) {
		if(MiteOS::currentTime.Hour > Configuration::getDarkmodeStartH() || (MiteOS::currentTime.Hour == Configuration::getDarkmodeStartH() && MiteOS::currentTime.Minute >= Configuration::getDarkmodeStartM())) {
			DARKMODE = !Configuration::getInverseDarkMode();
		}else if(MiteOS::currentTime.Hour < Configuration::getDarkmodeEndH() || (MiteOS::currentTime.Hour == Configuration::getDarkmodeEndH() && MiteOS::currentTime.Minute <= Configuration::getDarkmodeEndM())) {
			DARKMODE = !Configuration::getInverseDarkMode();
		}else{
			DARKMODE = Configuration::getInverseDarkMode();
		}
	}else{
		DARKMODE = PREF_DARKMODE;
	}
	printDebug("DARKMODE " + String(DARKMODE));
	
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
	
	
	// Quick and Dirty fix to access the sensitivity
	bma4_dev __devFptr;
	__devFptr.dev_addr       = BMA4_I2C_ADDR_PRIMARY;
	__devFptr.interface      = BMA4_I2C_INTERFACE;
	__devFptr.bus_read       = _readRegister;
	__devFptr.bus_write      = _writeRegister;
	__devFptr.delay          = delay;
	__devFptr.read_write_len = 8;
	__devFptr.resolution     = 12;
	__devFptr.feature_len    = BMA423_FEATURE_SIZE;
	bma423_wakeup_set_sensitivity(7, &__devFptr);
	bma423_tap_selection(0x01, &__devFptr);
	
	// Enable BMA423 isStepCounter feature
	accSensor.enableFeature(BMA423_STEP_CNTR, true);
	
	// Enable BMA423 isDoubleClick feature
	accSensor.enableFeature(BMA423_WAKEUP, true);
	
	// Enable BMA423 isTilt feature
	accSensor.enableFeature(BMA423_TILT, false);
	accSensor.enableFeature(BMA423_ACTIVITY, false);
	accSensor.enableFeature(BMA423_ANY_MOTION, false);
	accSensor.enableFeature(BMA423_NO_MOTION, false);
	
	// Reset steps
	//accSensor.resetStepCounter();
	
	// Turn on feature interrupt
	//accSensor.enableStepCountInterrupt();
	//accSensor.enableTiltInterrupt();
	// It corresponds to isDoubleClick interrupt
	accSensor.enableWakeupInterrupt();
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