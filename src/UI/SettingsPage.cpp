#include "SettingsPage.h"
#include "../MiteOS.h"
#include "../Managers/NetworkManager.h"
#include <DSEG7_Classic_Bold_53.h>

void SettingsPage::drawPage() {
	
	const char *menuItems[] = {
		"About MiteOS", "Vibrate Motor", "Accelerometer",
		"Set Time",     "Setup WiFi",    "Sync NTP"
	};
	
	showMenu(menuItems, 6, true);
}

bool SettingsPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex))
		return true;
	
	if(buttonIndex == BTN_BACK) {
		PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
		return true;
	}else if(buttonIndex == BTN_CONFIRM) {
		switch(pageData.menuIndex) {
			case 0:
				PageManager::showPage(GLOBAL_PAGE_ABOUT);
				return true;
			
			case 1:
				MiteOS::vibMotor(50, 50);
				return true;
			
			case 2:
				showAccelerometer();
				return true;
			
			case 3:
				setTime();
				return true;
			
			case 4:
				NetworkManager::registerNewWifiConnection();
				return true;
				
			case 5:
				NetworkManager::showSyncNTP();
				return true;
			
			default:
				break;
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