#include "SettingsPage.h"
#include "../MiteOS.h"
#include "../Managers/WifiConnectionManager.h"
#include "../Managers/FileManager.h"
#include <DSEG7_Classic_Bold_53.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../Data/Configuration.h"
#include "WatchfacePage.h"

#include "../Images/menu_icons.h"

#define SETTINGS_PAGE_OVERVIEW 0
#define SETTINGS_PAGE_WATCHFACE 1
#define SETTINGS_PAGE_TIME 2
#define SETTINGS_PAGE_NETWORK 3
#define SETTINGS_PAGE_DISPLAY 4
#define SETTINGS_PAGE_INTERACT 5
#define SETTINGS_PAGE_STORAGE 6
#define SETTINGS_PAGE_TEST 7

#define SETTINGS_PAGE_DARKMODE 11
#define SETTINGS_PAGE_DBL_TAP 12
#define SETTINGS_PAGE_WATCHFACE_SELECT 13
#define SETTINGS_PAGE_WATCHFACE_TOP_LEFT 14
#define SETTINGS_PAGE_WATCHFACE_TOP_RIGHT 15
#define SETTINGS_PAGE_WAKEUP_BEHAVIOUR 16

void SettingsPage::drawPage() {
	if(pageData.subPageIndex == SETTINGS_PAGE_OVERVIEW) {
		const char *menuItems[] = {
			TXT_ABOUT " " TXT_OS_NAME, TXT_WATCHFACE, TXT_TIME, TXT_NETWORK, TXT_DISPLAY, TXT_INTERACTION, TXT_STORAGE, TXT_DEBUG
		};
		
		showMenu(menuItems, 8, true, TXT_SETTINGS);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE) {
		const char *menuItems[] = {
			TXT_WATCHFACE, TXT_WATCHFACE_TOP_LEFT, TXT_WATCHFACE_TOP_RIGHT
		};

		showMenu(menuItems, 3, true, TXT_WATCHFACE);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_TIME) {
		const char *menuItems[] = {
			TXT_SET_TIME, TXT_SYNC_NTP, 
			(hourVibrate ? TXT_CHECKBOX_ON " " TXT_HOUR_VIB : TXT_CHECKBOX_OFF " " TXT_HOUR_VIB)
		};
		
		showMenu(menuItems, 3, true, TXT_TIME);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_NETWORK) {
		const char *menuItems[] = {
			TXT_SETUP_WIFI
		};
		
		showMenu(menuItems, 1, true, TXT_NETWORK);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_DISPLAY) {
		const char *menuItems[] = {
			TXT_COLOR_SCHEME, TXT_WAKEUP_BEHAVIOUR
		};
		
		showMenu(menuItems, 2, true, TXT_DISPLAY);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_INTERACT) {
		const char *menuItems[] = {
			(btnFeedbackVibrate ? TXT_CHECKBOX_ON " " TXT_INTERACT_VIBRATE_ON_BTN : TXT_CHECKBOX_OFF " " TXT_INTERACT_VIBRATE_ON_BTN),
			TXT_INTERACT_DBL_TAP
		};
		
		showMenu(menuItems, 2, true, TXT_INTERACTION);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_STORAGE) {
		float sizeSpace = Configuration::getSize() * 32 / 1000.0;
		float usedSpace = Configuration::usedSpace() * 32 / 1000.0;
		
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString("Settings:", 100, 40);
		drawCentreString(String(usedSpace, 1) + "KB / " + String(sizeSpace, 1) + "KB", 100, 60);
		
		drawProgressBar(30, 65, 140, 10, usedSpace / (float) sizeSpace);
		drawCentreString(String((usedSpace / (float) sizeSpace) * 100, 0) + " %", 100, 90);
		

		drawButtonIcon(BTN_BACK, icon_left);

		// Initialize LittleFS
		if (FileManager::init()) {
			sizeSpace = LittleFS.totalBytes() / 1000.0;
			usedSpace = LittleFS.usedBytes() / 1000.0;

			drawCentreString("Files:", 100, 120);
			drawCentreString(String(usedSpace, 1) + "KB / " + String(sizeSpace, 1) + "KB", 100, 140);
			
			drawProgressBar(30, 145, 140, 10, usedSpace / (float) sizeSpace);
			drawCentreString(String((usedSpace / (float) sizeSpace) * 100, 0) + " %", 100, 170);
		}
	}else if(pageData.subPageIndex == SETTINGS_PAGE_TEST) {
		const char *menuItems[] = {
			TXT_VIB_MOTOR, TXT_SHOW_ACC, TXT_REBOOT
		};
		
		showMenu(menuItems, 3, true, TXT_DEBUG);
	}
	
	
	
	
	else if(pageData.subPageIndex == SETTINGS_PAGE_DARKMODE) {
		const char *menuItems[] = {
			(AUTO_DARKMODE ? TXT_OPTION_LEFT " " TXT_TIMED " " TXT_OPTION_RIGHT : TXT_OPTION_LEFT " " TXT_STATIC " " TXT_OPTION_RIGHT),
			(PREF_DARKMODE ? TXT_OPTION_LEFT " " TXT_DARKMODE " " TXT_OPTION_RIGHT : TXT_OPTION_LEFT " " TXT_LIGHTMODE " " TXT_OPTION_RIGHT)
		};
		
		showMenu(menuItems, AUTO_DARKMODE ? 1 : 2, true, TXT_COLOR_SCHEME);
	} else if(pageData.subPageIndex == SETTINGS_PAGE_DBL_TAP) {
		const char *menuItems[] = {
			(doubleTapBtn == 0 ? TXT_CHECKBOX_ON " " TXT_DISABLE : TXT_CHECKBOX_OFF " " TXT_DISABLE),
			(doubleTapBtn == 1 ? TXT_CHECKBOX_ON " " TXT_BTN_1   : TXT_CHECKBOX_OFF " " TXT_BTN_1),
			(doubleTapBtn == 2 ? TXT_CHECKBOX_ON " " TXT_BTN_2   : TXT_CHECKBOX_OFF " " TXT_BTN_2),
			(doubleTapBtn == 3 ? TXT_CHECKBOX_ON " " TXT_BTN_3   : TXT_CHECKBOX_OFF " " TXT_BTN_3),
			(doubleTapBtn == 4 ? TXT_CHECKBOX_ON " " TXT_BTN_4   : TXT_CHECKBOX_OFF " " TXT_BTN_4),
		};
		
		showMenu(menuItems, 5, true, TXT_INTERACT_DBL_TAP);
	} else if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_SELECT) {
		const char* menuItems[WATCHFACE_COUNT];
		String strings[WATCHFACE_COUNT];

		for(uint8_t i = 0; i < WATCHFACE_COUNT; i++) {
			strings[i] = ((i == watchFaceId ? TXT_CHECKBOX_ON : TXT_CHECKBOX_OFF) + ((WatchfacePage*) PageManager::getPage(GLOBAL_PAGE_WATCHFACE))->getWatchface(i)->watchfaceName());
			menuItems[i] = strings[i].c_str();
		}

		showMenu(menuItems, WATCHFACE_COUNT, true, TXT_WATCHFACE);
	} else if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_LEFT || pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_RIGHT) {
		const char* menuItems[PAGE_COUNT];
		String strings[PAGE_COUNT];

		uint8_t index = 0;
		for(uint8_t i = 0; i < PAGE_COUNT; i++) {
			if(!PageManager::getPage(i)->isPageable()) {
				strings[index] = (i == (pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_LEFT ? actionTopLeft : actionTopRight) ? TXT_CHECKBOX_ON : TXT_CHECKBOX_OFF) + PageManager::getPage(i)->pageName();
				menuItems[index] = strings[index].c_str();
				index++;
			}
		}

		showMenu(menuItems, index, true, (pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_LEFT ? TXT_WATCHFACE_TOP_LEFT : TXT_WATCHFACE_TOP_RIGHT));
	}else if(pageData.subPageIndex == SETTINGS_PAGE_WAKEUP_BEHAVIOUR) {
		const char *menuItems[] = {
			(currentPowerMode == POWER_MODE_MINUTLY ? TXT_CHECKBOX_ON " " TXT_WAKEUP_MINUTE : TXT_CHECKBOX_OFF " " TXT_WAKEUP_MINUTE),
			(currentPowerMode == POWER_MODE_HOURLY  ? TXT_CHECKBOX_ON " " TXT_WAKEUP_HOUR   : TXT_CHECKBOX_OFF " " TXT_WAKEUP_HOUR  ),
			(currentPowerMode == POWER_MODE_LIFT    ? TXT_CHECKBOX_ON " " TXT_WAKEUP_TILT   : TXT_CHECKBOX_OFF " " TXT_WAKEUP_TILT  ),
			(currentPowerMode == POWER_MODE_SLEEP   ? TXT_CHECKBOX_ON " " TXT_WAKEUP_NEVER  : TXT_CHECKBOX_OFF " " TXT_WAKEUP_NEVER ),
		};
		
		showMenu(menuItems, 4, true, TXT_WAKEUP_BEHAVIOUR);
	}
}

bool SettingsPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex))
		return true;
	
	if(buttonIndex == BTN_BACK) {
		if(pageData.subPageIndex == SETTINGS_PAGE_DARKMODE) { // Darkmode
			pageData.subPageIndex = SETTINGS_PAGE_DISPLAY;
		} else if(pageData.subPageIndex == SETTINGS_PAGE_DBL_TAP) { // Darkmode
			pageData.subPageIndex = SETTINGS_PAGE_INTERACT;
		} else if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_SELECT) { // Watchface
			pageData.subPageIndex = SETTINGS_PAGE_WATCHFACE;
		} else if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_LEFT) { // Watchface
			pageData.subPageIndex = SETTINGS_PAGE_WATCHFACE;
		} else if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_RIGHT) { // Watchface
			pageData.subPageIndex = SETTINGS_PAGE_WATCHFACE;
		} else if(pageData.subPageIndex == SETTINGS_PAGE_WAKEUP_BEHAVIOUR) { // Watchface
			pageData.subPageIndex = SETTINGS_PAGE_DISPLAY;
		} else if(pageData.subPageIndex > 0) {
			pageData.subPageIndex = 0;
		}else{
			return false;
		}
		return true;
	}else if(buttonIndex == BTN_CONFIRM) {
		uint8_t index = 0;
		switch(pageData.subPageIndex) {
			case SETTINGS_PAGE_OVERVIEW:
				if(pageData.menuIndex == 0) {
					PageManager::showPage(GLOBAL_PAGE_ABOUT);
				}else{
					pageData.subPageIndex = pageData.menuIndex;
				}
				return true;

			case SETTINGS_PAGE_WATCHFACE:
				switch(pageData.menuIndex) {
					case 0:
						pageData.subPageIndex = SETTINGS_PAGE_WATCHFACE_SELECT;
						return true;
					case 1:
						pageData.subPageIndex = SETTINGS_PAGE_WATCHFACE_TOP_LEFT;
						return true;
					case 2:
						pageData.subPageIndex = SETTINGS_PAGE_WATCHFACE_TOP_RIGHT;
						return true;
					
					default: break;
				}
				break;

			case SETTINGS_PAGE_TIME:
				switch(pageData.menuIndex) {
					case 0:
						setTime();
						return true;
					
					case 1:
						WifiConnectionManager::showSyncNTP();
						return true;

					case 2:
						hourVibrate = !hourVibrate;
						Configuration::saveSettings();
						return true;
					
					default: break;
				}
				break;
			
			case SETTINGS_PAGE_NETWORK:
				switch(pageData.menuIndex) {
					case 0:
						WifiConnectionManager::registerNewWifiConnection();
						return true;
					
					case 1:
						WifiConnectionManager::registerNewWifiConnection();
						return true;
					
					default: break;
				}
				break;
			
			case SETTINGS_PAGE_DISPLAY:
				switch(pageData.menuIndex) {
					case 0:
						pageData.subPageIndex = SETTINGS_PAGE_DARKMODE;
						return true;
					
					case 1:
						pageData.subPageIndex = SETTINGS_PAGE_WAKEUP_BEHAVIOUR;
						return true;
					
					default: break;
				}
				break;
			
			case SETTINGS_PAGE_INTERACT:
				switch(pageData.menuIndex) {
					case 0:
						btnFeedbackVibrate = !btnFeedbackVibrate;
						Configuration::saveSettings();
						return true;
					
					case 1:
						pageData.subPageIndex = SETTINGS_PAGE_DBL_TAP;
						return true;
					
					default: break;
				}
				break;
			
			
			
			
			case SETTINGS_PAGE_DARKMODE:
				switch(pageData.menuIndex) {
					case 0:
						AUTO_DARKMODE = !AUTO_DARKMODE;
						MiteOS::initDarkmode();
						Configuration::saveSettings();
						return true;
					case 1:
						PREF_DARKMODE = !PREF_DARKMODE;
						MiteOS::initDarkmode();
						Configuration::saveSettings();
						return true;
					
					default: break;
				}
				break;
			
			case SETTINGS_PAGE_DBL_TAP:
				doubleTapBtn = pageData.menuIndex;
				Configuration::saveSettings();
				return true;
			
			case SETTINGS_PAGE_TEST:
				switch(pageData.menuIndex) {
					case 0:
						AlertManager::vibMotor(50, 50);
						return true;
					
					case 1:
						showAccelerometer();
						return true;

					case 2:
						mDisplay.setFullWindow();
						mDisplay.fillScreen(BACKGROUND_COLOR);
						drawCentreString("bye", 100, 100, false);
						mDisplay.display(true); // partial refresh

						delay(10);
						
						ESP.restart();
						return true;
					
					default: break;
				}
				break;
			
			case SETTINGS_PAGE_WATCHFACE_SELECT:
				watchFaceId = pageData.menuIndex;
				Configuration::saveSettings();
				return true;

			case SETTINGS_PAGE_WATCHFACE_TOP_LEFT:
			case SETTINGS_PAGE_WATCHFACE_TOP_RIGHT:

				index = 0;
				for(uint8_t i = 0; i < PAGE_COUNT; i++) {
					if(!PageManager::getPage(i)->isPageable()) {
						if(index == pageData.menuIndex) {
							index = i;
							break;
						}
						index++;
					}
				}

				if(pageData.subPageIndex == SETTINGS_PAGE_WATCHFACE_TOP_LEFT){
					actionTopLeft = index;
				}else{
					actionTopRight = index;
				}
				
				Configuration::saveSettings();
				return true;

			case SETTINGS_PAGE_WAKEUP_BEHAVIOUR:
				PowerManager::setPowerMode(pageData.menuIndex);
				return true;
			
			default: break;
		}
	}
	
	return false;
}


// TODO: Make it custom and not just copy paste
void SettingsPage::setTime() {
	mRTC.read(MiteOS::currentTime);
	
	int8_t minute = MiteOS::currentTime.Minute;
	int8_t hour   = MiteOS::currentTime.Hour;
	int8_t day    = MiteOS::currentTime.Day;
	int8_t month  = MiteOS::currentTime.Month;
	int8_t year   = tmYearToY2k(MiteOS::currentTime.Year);
	
	int8_t setIndex = SET_HOUR;
	
	int8_t blink = 0;
	
	pinMode(DOWN_BTN_PIN, INPUT);
	pinMode(UP_BTN_PIN, INPUT);
	pinMode(MENU_BTN_PIN, INPUT);
	pinMode(BACK_BTN_PIN, INPUT);
	
	mDisplay.setFullWindow();

	delay(500);
	
	while (1) {
		if (digitalRead(MENU_BTN_PIN) == 1) {
			setIndex++;
			if (setIndex > SET_DAY) {
				break;
			}
		}
		if (digitalRead(BACK_BTN_PIN) == 1) {
			if (setIndex != SET_HOUR) {
				setIndex--;
			}
		}
		
		blink = 1 - blink;

		if (digitalRead(DOWN_BTN_PIN) == 1) {
			blink = 1;
			switch (setIndex) {
				case SET_HOUR:
					hour == 23 ? (hour = 0) : hour++;
					break;
				case SET_MINUTE:
					minute == 59 ? (minute = 0) : minute++;
					break;
				case SET_YEAR:
					year == 99 ? (year = 0) : year++;
					break;
				case SET_MONTH:
					month == 12 ? (month = 1) : month++;
					break;
				case SET_DAY:
					day == 31 ? (day = 1) : day++;
					break;
				default:
					break;
			}
		}

		if (digitalRead(UP_BTN_PIN) == 1) {
			blink = 1;
			switch (setIndex) {
				case SET_HOUR:
					hour == 0 ? (hour = 23) : hour--;
					break;
				case SET_MINUTE:
					minute == 0 ? (minute = 59) : minute--;
					break;
				case SET_YEAR:
					year == 0 ? (year = 99) : year--;
					break;
				case SET_MONTH:
					month == 1 ? (month = 12) : month--;
					break;
				case SET_DAY:
					day == 1 ? (day = 31) : day--;
					break;
				default:
					break;
			}
		}
		
		mDisplay.fillScreen(GxEPD_BLACK);
		mDisplay.setTextColor(GxEPD_WHITE);
		mDisplay.setFont(&DSEG7_Classic_Bold_53);
		
		mDisplay.setCursor(5, 80);
		if (setIndex == SET_HOUR) { // blink hour digits
			mDisplay.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
		}
		if (hour < 10) {
			mDisplay.print("0");
		}
		mDisplay.print(hour);

		mDisplay.setTextColor(GxEPD_WHITE);
		mDisplay.print(":");

		mDisplay.setCursor(108, 80);
		if (setIndex == SET_MINUTE) { // blink minute digits
			mDisplay.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
		}
		if (minute < 10) {
			mDisplay.print("0");
		}
		mDisplay.print(minute);

		mDisplay.setTextColor(GxEPD_WHITE);

		mDisplay.setFont(&FreeMonoBold9pt7b);
		mDisplay.setCursor(45, 150);
		if (setIndex == SET_YEAR) { // blink minute digits
			mDisplay.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
		}
		mDisplay.print(2000 + year);

		mDisplay.setTextColor(GxEPD_WHITE);
		mDisplay.print("/");

		if (setIndex == SET_MONTH) { // blink minute digits
			mDisplay.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
		}
		if (month < 10) {
			mDisplay.print("0");
		}
		mDisplay.print(month);

		mDisplay.setTextColor(GxEPD_WHITE);
		mDisplay.print("/");

		if (setIndex == SET_DAY) { // blink minute digits
			mDisplay.setTextColor(blink ? GxEPD_WHITE : GxEPD_BLACK);
		}
		if (day < 10) {
			mDisplay.print("0");
		}
		mDisplay.print(day);
		mDisplay.display(true); // partial refresh
	}

	tmElements_t tm;
	tm.Month  = month;
	tm.Day    = day;
	tm.Year   = y2kYearToTm(year);
	tm.Hour   = hour;
	tm.Minute = minute;
	tm.Second = 0;

	mRTC.set(tm);
	
	PageManager::refreshPage();
}

void SettingsPage::showAccelerometer() {
	mDisplay.setFullWindow();
	mDisplay.fillScreen(GxEPD_BLACK);
	mDisplay.setFont(&FreeMonoBold9pt7b);
	mDisplay.setTextColor(GxEPD_WHITE);

	Accel acc;

	long previousMillis = 0;
	long interval       = 200;
	
	pinMode(BACK_BTN_PIN, INPUT);

	while (1) {
		unsigned long currentMillis = millis();
		
		if (digitalRead(BACK_BTN_PIN) == 1) {
			break;
		}
		
		if (currentMillis - previousMillis > interval) {
			previousMillis = currentMillis;
			// Get acceleration data
			bool res          = accSensor.getAccel(acc);
			uint8_t direction = accSensor.getDirection();
			mDisplay.fillScreen(GxEPD_BLACK);
			mDisplay.setCursor(0, 30);
			if (res == false) {
				mDisplay.println("getAccel FAIL");
			} else {
				mDisplay.print("  X:");
				mDisplay.println(acc.x);
				mDisplay.print("  Y:");
				mDisplay.println(acc.y);
				mDisplay.print("  Z:");
				mDisplay.println(acc.z);
				
				mDisplay.setCursor(30, 130);
				switch (direction) {
					case DIRECTION_DISP_DOWN:
						mDisplay.println("FACE DOWN");
						break;
					case DIRECTION_DISP_UP:
						mDisplay.println("FACE UP");
						break;
					case DIRECTION_BOTTOM_EDGE:
						mDisplay.println("BOTTOM EDGE");
						break;
					case DIRECTION_TOP_EDGE:
						mDisplay.println("TOP EDGE");
						break;
					case DIRECTION_RIGHT_EDGE:
						mDisplay.println("RIGHT EDGE");
						break;
					case DIRECTION_LEFT_EDGE:
						mDisplay.println("LEFT EDGE");
						break;
					default:
						mDisplay.println("ERROR!!!");
						break;
				}
			}
			mDisplay.display(true); // full refresh
		}
	}
	
	PageManager::refreshPage();
}