#include "TimerPage.h"
#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Images/menu_icons.h"
#include "../Images/app_icons.h"
#include <Fonts/FreeMonoBold24pt7b.h>

#define PAGE_5MINUTES 0
#define PAGE_MINUTES 1
#define PAGE_START_MENU 2
#define PAGE_TIME_SELECTION 3

void TimerPage::initPage() {
	pageData.subPageIndex = PAGE_START_MENU;
	pageData.largenumber1 = 5;
}

void TimerPage::drawPage() {
	if(pageData.subPageIndex == PAGE_TIME_SELECTION) {
		const char *menuItems[] = { "10m", "15m", "20m", "30m", "45m", "1h", "2h" };
		showMenu(menuItems, 7, true, TXT_PRESET);
	}else{
		drawTime();
		drawIcons();
	}
}

bool TimerPage::onButtonPressed(uint8_t buttonIndex) {
	if(pageData.subPageIndex == PAGE_TIME_SELECTION) {
		if(handleMenuButtons(buttonIndex)) return true;
		
		if(buttonIndex == BTN_CONFIRM) {
			int data[] = { 10, 15, 20, 30, 45, 60, 120 };
			pageData.largenumber1 = data[pageData.menuIndex];
			startTimer();
			return true;
		}
		
		if(buttonIndex == BTN_BACK) {
			pageData.subPageIndex = PAGE_START_MENU;
			return true;
		}
		return false;
	}
	
	if(timer.enableAlarm) {
		if(buttonIndex == BTN_UP) {
			pageData.largenumber1 = (timer.hour - MiteOS::currentTime.Hour) * 60 + (timer.minute - MiteOS::currentTime.Minute);
			timer.enableAlarm = false;
			pageData.subPageIndex = PAGE_START_MENU;
			return true;
		}
	}else{
		if(pageData.subPageIndex <= PAGE_MINUTES) {
			if(buttonIndex == BTN_UP) {
				pageData.largenumber1 += (pageData.subPageIndex == PAGE_5MINUTES ? 5 : 1);
				return true;
			}else if(buttonIndex == BTN_DOWN) {
				pageData.largenumber1 -= (pageData.subPageIndex == PAGE_5MINUTES ? 5 : 1);
				if(pageData.largenumber1 < 1) pageData.largenumber1 = 1;
				return true;
			}else if(buttonIndex == BTN_MENU) {
				pageData.subPageIndex += 1;
				if(pageData.subPageIndex > PAGE_START_MENU) pageData.subPageIndex = PAGE_5MINUTES;
				return true;
			}
		}else{
			if(buttonIndex == BTN_UP) {
				startTimer();
				return true;
			}else if(buttonIndex == BTN_DOWN) {
				pageData.subPageIndex = PAGE_TIME_SELECTION;
				pageData.menuIndex = 3; // Select the center entry by default
				return true;
			}else if(buttonIndex == BTN_MENU) {
				pageData.subPageIndex += 1;
				if(pageData.subPageIndex > PAGE_START_MENU) pageData.subPageIndex = PAGE_5MINUTES;
				return true;
			}
		}
	}
	return false;
}

void TimerPage::drawTime() {	
	if(timer.triggered) {
		AlertManager::showPermanentAlert(TXT_TIMER_CAPS, app_icon_stopwatch);
		
		timer.triggered = false;
		timer.enableAlarm = false;
		pageData.largenumber1 = 5;
		pageData.subPageIndex = PAGE_START_MENU;
	}
	
	mDisplay.setFont(&DSEG7_Classic_Regular_39);
	
	uint32_t minutes = pageData.largenumber1;
	if(timer.enableAlarm) {
		minutes = (timer.hour - MiteOS::currentTime.Hour) * 60 + (timer.minute - MiteOS::currentTime.Minute);
	}
	
	int16_t  x1, y1;
	uint16_t w, h;
	String currentTime = String(minutes);
	drawCentreString(currentTime.c_str(), 100, 110);
	
	mDisplay.setFont(&Seven_Segment10pt7b);
	if(timer.enableAlarm) {
		drawCentreString(TXT_ALARM_IN, 100, 60);
	} else {
		if(pageData.subPageIndex <= PAGE_MINUTES) {
			String right_border = (pageData.subPageIndex == PAGE_5MINUTES ? "5" : "1");
			mDisplay.setCursor(180, 100);
			mDisplay.println(right_border);
		}
	}
	
	drawCentreString(TXT_MIN_CAPS, 100, 140);
	/*
	mDisplay.setFont(&DSEG7_Classic_Bold_25);
	mDisplay.setCursor(10, 30);
	mDisplay.println(String(PowerManager::getBatteryVoltage()) + "V");
	*/
}

void TimerPage::drawIcons() {
	drawButtonIcon(BTN_BACK, icon_exit);
	if(timer.enableAlarm) {
		drawButtonIcon(BTN_UP, icon_stop);
	} else {
		drawButtonIcon(BTN_MENU, icon_right);
		if(pageData.subPageIndex == PAGE_MINUTES || pageData.subPageIndex == PAGE_5MINUTES) {
			drawButtonIcon(BTN_UP, icon_plus);
			drawButtonIcon(BTN_DOWN, icon_minus);
		}else if(pageData.subPageIndex == PAGE_START_MENU) {
			drawButtonIcon(BTN_UP, icon_play);
			drawButtonIcon(BTN_DOWN, icon_menu);
		}
	}
}

void TimerPage::startTimer() {
	int32_t hour = pageData.largenumber1 / 60, minute = pageData.largenumber1 % 60;
	
	timer.hour = MiteOS::currentTime.Hour + hour;
	timer.minute = MiteOS::currentTime.Minute + minute;
	while(timer.minute > 60) {
		timer.hour += 1;
		timer.minute -= 60;
	}
	
	pageData.subPageIndex = PAGE_START_MENU;
	timer.enableAlarm = true;
}
