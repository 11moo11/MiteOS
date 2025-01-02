#include "AlarmPage.h"
#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Bold_25.h"
#include <DSEG7_Classic_Bold_53.h>
#include "../Images/menu_icons.h"
#include "../Images/app_icons.h"
#include "../Data/DayMask.h"

#define ALARM_PAGE_OVERVIEW 0
#define ALARM_PAGE_CONFIGURATION 1
#define ALARM_PAGE_SET_TIME 2
#define ALARM_PAGE_SET_DAYS 3

void AlarmPage::initPage() {
	pageData.subPageIndex = ALARM_PAGE_OVERVIEW;
}

void AlarmPage::drawPage() {
	for(uint8_t i = 0; i < ALARM_COUNT; i++) {
		if(alarms[i].triggered) {
			String item = "";
			if(alarms[i].hour < 10) item += "0";
			item += String(alarms[i].hour) + ":";
			
			if(alarms[i].minute < 10) item += "0";
			item += String(alarms[i].minute);
			
			AlertManager::showPermanentAlert(TXT_ALARM " " + String(i + 1), app_icon_alarm, item);
			alarms[i].triggered = false;
			
			PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
			PageManager::refreshPage();
			return;
		}
	}
	
	if(pageData.subPageIndex == ALARM_PAGE_OVERVIEW) {
		drawOverview();
	}else if(pageData.subPageIndex == ALARM_PAGE_CONFIGURATION) {
		const char *menuItems[] = {
			(alarms[pageData.number2].enableAlarm ? TXT_ENABLE_ON : TXT_ENABLE_OFF),
			TXT_SET_TIME,
			TXT_DAYS
		};
		showMenu(menuItems, 3, true, TXT_ALARM " " + String(pageData.number2 + 1));
	}else if(pageData.subPageIndex == ALARM_PAGE_SET_DAYS) {
		DayMask dm = unpackDayMask(alarms[pageData.number2].mode);

		if(dm.once) {
			const char *menuItems[] = {
				TXT_CHECKBOX_ON" "TXT_ONCE,
			};

			showMenu(menuItems, 1, true, TXT_DAYS);
		}else{
			const char *menuItems[] = {
				TXT_CHECKBOX_OFF" "TXT_ONCE,
				(dm.monday ? TXT_CHECKBOX_ON" "TXT_MONDAY : TXT_CHECKBOX_OFF" "TXT_MONDAY),
				(dm.tuesday ? TXT_CHECKBOX_ON" "TXT_TUESDAY : TXT_CHECKBOX_OFF" "TXT_TUESDAY),
				(dm.wednesday ? TXT_CHECKBOX_ON" "TXT_WEDNESDAY : TXT_CHECKBOX_OFF" "TXT_WEDNESDAY),
				(dm.thursday ? TXT_CHECKBOX_ON" "TXT_THURSDAY : TXT_CHECKBOX_OFF" "TXT_THURSDAY),
				(dm.friday ? TXT_CHECKBOX_ON" "TXT_FRIDAY : TXT_CHECKBOX_OFF" "TXT_FRIDAY),
				(dm.saturday ? TXT_CHECKBOX_ON" "TXT_SATURDAY : TXT_CHECKBOX_OFF" "TXT_SATURDAY),
				(dm.sunday ? TXT_CHECKBOX_ON" "TXT_SUNDAY : TXT_CHECKBOX_OFF" "TXT_SUNDAY),
			};

			showMenu(menuItems, 8, true, TXT_DAYS);
		}
	}
	drawIcons();
}

bool AlarmPage::onButtonPressed(uint8_t buttonIndex) {
	switch(pageData.subPageIndex){
		case ALARM_PAGE_OVERVIEW:
			if(buttonIndex == BTN_DOWN) {
				pageData.number2++;
				if(pageData.number2 >= ALARM_COUNT) pageData.number2 = 0;
				return true;
			}else if(buttonIndex == BTN_UP) {
				pageData.number2--;
				if(pageData.number2 > 250) pageData.number2 = ALARM_COUNT - 1;
				return true;
			}else if(buttonIndex == BTN_MENU) {
				pageData.subPageIndex = ALARM_PAGE_CONFIGURATION;
				return true;
			}
			break;
		case ALARM_PAGE_CONFIGURATION:
			if(handleMenuButtons(buttonIndex)) {
				return true;
			}else if(buttonIndex == BTN_BACK) {
				Configuration::saveAlarms();
				pageData.subPageIndex = ALARM_PAGE_OVERVIEW;
				return true;
			}else if(buttonIndex == BTN_CONFIRM) {
				if(pageData.menuIndex == 0) { // Enable / Disable
					alarms[pageData.number2].enableAlarm = !alarms[pageData.number2].enableAlarm;
					return true;
				}else if(pageData.menuIndex == 1) { // Set Time
					setTime();
					return true;
				}else if(pageData.menuIndex == 2) { // Type Selection
					pageData.subPageIndex = ALARM_PAGE_SET_DAYS;
					return true;
				}
			}
			break;
		
		case ALARM_PAGE_SET_TIME:
			setTime();
			break;
		
		case ALARM_PAGE_SET_DAYS:
			if(handleMenuButtons(buttonIndex)) {
				return true;
			}else if(buttonIndex == BTN_BACK) {
				pageData.subPageIndex = ALARM_PAGE_CONFIGURATION;
				return true;
			}else if(buttonIndex == BTN_CONFIRM) {
				DayMask dm = unpackDayMask(alarms[pageData.number2].mode);
				switch(pageData.menuIndex) {
					case 0: dm.once      = !dm.once;      break;
					case 1: dm.monday    = !dm.monday;    break;
					case 2: dm.tuesday   = !dm.tuesday;   break;
					case 3: dm.wednesday = !dm.wednesday; break;
					case 4: dm.thursday  = !dm.thursday;  break;
					case 5: dm.friday    = !dm.friday;    break;
					case 6: dm.saturday  = !dm.saturday;  break;
					case 7: dm.sunday    = !dm.sunday;    break;
					default: break;
				}
				alarms[pageData.number2].mode = packDayMask(dm);
				return true;
			}
			break;
		
		default: break;
	}
	
	return false;
}

void AlarmPage::drawOverview() {
	int16_t x1, y1;
	uint16_t w, h;
	
	mDisplay.setFont(&DSEG7_Classic_Bold_25);
	for(uint8_t i = 0; i < 3; i++) {
		uint8_t yPos = 18 + (55 * i);
		
		String item = "";
		if(alarms[i].hour < 10) item += "0";
		item += String(alarms[i].hour) + ":";
		
		if(alarms[i].minute < 10) item += "0";
		item += String(alarms[i].minute);
		
		if (i == pageData.number2) {
			mDisplay.fillRect(25, yPos, 150, 50, FOREGROUND_COLOR);
			mDisplay.setTextColor(BACKGROUND_COLOR);
		} else {
			mDisplay.setTextColor(FOREGROUND_COLOR);
		}
		
		mDisplay.setCursor(56, yPos + 38);
		mDisplay.println(item);
		
		if(alarms[i].enableAlarm) mDisplay.drawBitmap(30, yPos + 14, icon_alarm, 20, 20, i == pageData.number2 ? BACKGROUND_COLOR : FOREGROUND_COLOR);
		else mDisplay.fillRect(45, yPos + 26, 110, 2, i == pageData.number2 ? BACKGROUND_COLOR : FOREGROUND_COLOR);
		
	}
}

void AlarmPage::drawIcons() {
	switch(pageData.subPageIndex){
		case ALARM_PAGE_OVERVIEW:
			drawButtonIcon(BTN_UP, icon_up);
			drawButtonIcon(BTN_DOWN, icon_down);
			drawButtonIcon(BTN_MENU, icon_gear);
			drawButtonIcon(BTN_BACK, icon_exit);
			break;
		case ALARM_PAGE_SET_DAYS:
			drawButtonIcon(BTN_UP, icon_up);
			drawButtonIcon(BTN_DOWN, icon_down);
			drawButtonIcon(BTN_CONFIRM, icon_checkmark);
			drawButtonIcon(BTN_BACK, icon_left);
			break;
		default: break;
	}
}

void AlarmPage::setTime() {
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
			if (setIndex > SET_MINUTE) {
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
					alarms[pageData.number2].hour == 0 ? (alarms[pageData.number2].hour = 23) : alarms[pageData.number2].hour--;
					break;
				case SET_MINUTE:
					alarms[pageData.number2].minute == 0 ? (alarms[pageData.number2].minute = 59) : alarms[pageData.number2].minute--;
					break;
				default:
					break;
			}
		}

		if (digitalRead(UP_BTN_PIN) == 1) {
			blink = 1;
			switch (setIndex) {
				case SET_HOUR:
					alarms[pageData.number2].hour == 23 ? (alarms[pageData.number2].hour = 0) : alarms[pageData.number2].hour++;
					break;
				case SET_MINUTE:
					alarms[pageData.number2].minute == 59 ? (alarms[pageData.number2].minute = 0) : alarms[pageData.number2].minute++;
					break;
				default:
					break;
			}
		}
		
		mDisplay.fillScreen(BACKGROUND_COLOR);
		mDisplay.setTextColor(FOREGROUND_COLOR);
		mDisplay.setFont(&DSEG7_Classic_Bold_53);
		
		mDisplay.setCursor(5, 120);
		if (setIndex == SET_HOUR) { // blink hour digits
			mDisplay.setTextColor(blink ? FOREGROUND_COLOR : BACKGROUND_COLOR);
		}
		if (alarms[pageData.number2].hour < 10) {
			mDisplay.print("0");
		}
		mDisplay.print(alarms[pageData.number2].hour);

		mDisplay.setTextColor(FOREGROUND_COLOR);
		mDisplay.print(":");

		mDisplay.setCursor(108, 120);
		if (setIndex == SET_MINUTE) { // blink minute digits
			mDisplay.setTextColor(blink ? FOREGROUND_COLOR : BACKGROUND_COLOR);
		}
		if (alarms[pageData.number2].minute < 10) {
			mDisplay.print("0");
		}
		mDisplay.print(alarms[pageData.number2].minute);
		
		
		mDisplay.display(true); // partial refresh
	}
	
	pageData.subPageIndex = ALARM_PAGE_CONFIGURATION;
}