#include "SettingsPage.h"
#include "../MiteOS.h"
#include "../Managers/NetworkManager.h"
#include <DSEG7_Classic_Bold_53.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../Data/Configuration.h"

#include "../Images/menu_icons.h"

#define SETTINGS_PAGE_OVERVIEW 0
#define SETTINGS_PAGE_TIME 1
#define SETTINGS_PAGE_NETWORK 2
#define SETTINGS_PAGE_DISPLAY 3
#define SETTINGS_PAGE_STORAGE 4
#define SETTINGS_PAGE_TEST 5

#define SETTINGS_PAGE_DARKMODE 11

void SettingsPage::drawPage() {
	if(pageData.subPageIndex == SETTINGS_PAGE_OVERVIEW) {
		const char *menuItems[] = {
			TXT_ABOUT " " TXT_OS_NAME, TXT_TIME, TXT_NETWORK, TXT_DISPLAY, TXT_STORAGE, TXT_TEST
		};
		
		showMenu(menuItems, 6, true, TXT_SETTINGS);
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
			TXT_COLOR_SCHEME
		};
		
		showMenu(menuItems, 1, true, TXT_DISPLAY);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_STORAGE) {
		int sizeSpace = Configuration::getSize();
		int usedSpace = Configuration::usedSpace();
		
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString("Entries:", 100, 50);
		drawCentreString(String(usedSpace) + " / " + String(sizeSpace), 100, 70);
		
		mDisplay.drawRect(30, 90, 140, 20, FOREGROUND_COLOR);
		mDisplay.fillRect(30, 90, 140 * (usedSpace / (float) sizeSpace), 20, FOREGROUND_COLOR);
		drawCentreString(String((usedSpace / (float) sizeSpace) * 100) + " %", 100, 130);
		
		drawButtonIcon(BTN_BACK, icon_left);
	}else if(pageData.subPageIndex == SETTINGS_PAGE_TEST) {
		const char *menuItems[] = {
			TXT_VIB_MOTOR, TXT_SHOW_ACC
		};
		
		showMenu(menuItems, 2, true, TXT_TEST);
	}
	
	
	
	
	else if(pageData.subPageIndex == SETTINGS_PAGE_DARKMODE) {
		const char *menuItems[] = {
			(AUTO_DARKMODE ? TXT_OPTION_LEFT " " TXT_TIMED " " TXT_OPTION_RIGHT : TXT_OPTION_LEFT " " TXT_STATIC " " TXT_OPTION_RIGHT),
			(PREF_DARKMODE ? TXT_OPTION_LEFT " " TXT_DARKMODE " " TXT_OPTION_RIGHT : TXT_OPTION_LEFT " " TXT_LIGHTMODE " " TXT_OPTION_RIGHT)
		};
		
		showMenu(menuItems, AUTO_DARKMODE ? 1 : 2, true, TXT_COLOR_SCHEME);
	}
}

bool SettingsPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex))
		return true;
	
	if(buttonIndex == BTN_BACK) {
		if(pageData.subPageIndex == 11) { // Darkmode
			pageData.subPageIndex = SETTINGS_PAGE_DISPLAY;
		} else if(pageData.subPageIndex > 0) {
			pageData.subPageIndex = 0;
		}else{
			PageManager::showPage(GLOBAL_PAGE_APPS);
		}
		return true;
	}else if(buttonIndex == BTN_CONFIRM) {
		switch(pageData.subPageIndex) {
			case SETTINGS_PAGE_OVERVIEW:
				if(pageData.menuIndex == 0) {
					PageManager::showPage(GLOBAL_PAGE_ABOUT);
				}else{
					pageData.subPageIndex = pageData.menuIndex;
				}
				return true;
			
			case SETTINGS_PAGE_TIME:
				switch(pageData.menuIndex) {
					case 0:
						setTime();
						return true;
					
					case 1:
						NetworkManager::showSyncNTP();
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
						NetworkManager::registerNewWifiConnection();
						return true;
					
					case 1:
						NetworkManager::registerNewWifiConnection();
						return true;
					
					default: break;
				}
				break;
			
			case SETTINGS_PAGE_DISPLAY:
				switch(pageData.menuIndex) {
					case 0:
						pageData.subPageIndex = SETTINGS_PAGE_DARKMODE;
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
			
			case SETTINGS_PAGE_TEST:
				switch(pageData.menuIndex) {
					case 0:
						AlertManager::vibMotor(50, 50);
						return true;
					
					case 1:
						showAccelerometer();
						return true;
					
					default: break;
				}
				break;
			
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