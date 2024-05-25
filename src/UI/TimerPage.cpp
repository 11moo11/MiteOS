#include "TimerPage.h"
#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Images/menu_icons.h"

#define PAGE_5MINUTES 0
#define PAGE_MINUTES 1
#define PAGE_START_MENU 2
#define PAGE_TIME_SELECTION 3

void TimerPage::initPage() {
	pageData.number1 = PAGE_START_MENU;
	pageData.number3 = 5;
}

void TimerPage::drawPage() {
	if(pageData.number1 == PAGE_TIME_SELECTION) {
		const char *menuItems[] = { "10m", "15m", "20m", "30m", "45m", "1h", "2h" };
		showMenu(menuItems, 7, true);
	}else{
		drawTime();
		drawIcons();
	}
}

bool TimerPage::onButtonPressed(uint8_t buttonIndex) {
	if(pageData.number1 == PAGE_TIME_SELECTION) {
		if(handleMenuButtons(buttonIndex)) return true;
		
		if(buttonIndex == BTN_CONFIRM) {
			int data[] = { 10, 15, 20, 30, 45, 60, 120 };
			pageData.number3 = data[pageData.menuIndex];
			startTimer();
			return true;
		}
		
		if(buttonIndex == BTN_BACK) {
			pageData.number1 = PAGE_START_MENU;
			return true;
		}
		return false;
	}
	
	if(timer.enableAlarm) {
		if(timer.triggered) {
			timer.triggered = false;
			timer.enableAlarm = false;
			pageData.number3 = 5;
			pageData.number1 = PAGE_START_MENU;
			return true;
		}else{
			if(buttonIndex == BTN_UP) {
				pageData.number3 = (timer.hour - MiteOS::currentTime.Hour) * 60 + (timer.minute - MiteOS::currentTime.Minute);
				timer.enableAlarm = false;
				pageData.number1 = PAGE_START_MENU;
				return true;
			}
		}
	}else{
		if(pageData.number1 <= PAGE_MINUTES) {
			if(buttonIndex == BTN_UP) {
				pageData.number3 += (pageData.number1 == PAGE_5MINUTES ? 5 : 1);
				return true;
			}else if(buttonIndex == BTN_DOWN) {
				pageData.number3 -= (pageData.number1 == PAGE_5MINUTES ? 5 : 1);
				if(pageData.number3 < 1) pageData.number3 = 1;
				return true;
			}else if(buttonIndex == BTN_TOGGLE_BTN) {
				pageData.number1 += 1;
				if(pageData.number1 > PAGE_START_MENU) pageData.number1 = PAGE_5MINUTES;
				return true;
			}
		}else{
			if(buttonIndex == BTN_UP) {
				startTimer();
				return true;
			}else if(buttonIndex == BTN_DOWN) {
				pageData.number1 = PAGE_TIME_SELECTION;
				pageData.menuIndex = 3; // Select the center entry by default
				return true;
			}else if(buttonIndex == BTN_TOGGLE_BTN) {
				pageData.number1 += 1;
				if(pageData.number1 > PAGE_START_MENU) pageData.number1 = PAGE_5MINUTES;
				return true;
			}
		}
	}
	return false;
}

void TimerPage::drawTime() {	
	if(timer.triggered) {
		mDisplay.setFont(&DSEG7_Classic_Regular_39);
		
		drawCentreString("ALARM", 100, 110);
	}else{
		mDisplay.setFont(&DSEG7_Classic_Regular_39);
		
		uint32_t minutes = pageData.number3;
		if(timer.enableAlarm) {
			minutes = (timer.hour - MiteOS::currentTime.Hour) * 60 + (timer.minute - MiteOS::currentTime.Minute);
		}
		
		int16_t  x1, y1;
		uint16_t w, h;
		String currentTime = String(minutes);
		drawCentreString(currentTime.c_str(), 100, 110);
		
		mDisplay.setFont(&Seven_Segment10pt7b);
		if(timer.enableAlarm) {
			drawCentreString("Alarm in:", 100, 60);
		} else {
			if(pageData.number1 <= PAGE_MINUTES) {
				String right_border = (pageData.number1 == PAGE_5MINUTES ? "5" : "1");
				mDisplay.setCursor(180, 100);
				mDisplay.println(right_border);
			}
		}
		
		drawCentreString("MIN", 100, 140);
		/*
		mDisplay.setFont(&DSEG7_Classic_Bold_25);
		mDisplay.setCursor(10, 30);
		mDisplay.println(String(MiteOS::getBatteryVoltage()) + "V");
		*/
	}
}

void TimerPage::drawIcons() {
	if(timer.enableAlarm) {
		drawButtonIcon(BTN_UP, icon_stop);
	} else {
		drawButtonIcon(BTN_TOGGLE_BTN, icon_right);
		if(pageData.number1 == PAGE_MINUTES || pageData.number1 == PAGE_5MINUTES) {
			drawButtonIcon(BTN_UP, icon_plus);
			drawButtonIcon(BTN_DOWN, icon_minus);
		}else if(pageData.number1 == PAGE_START_MENU) {
			drawButtonIcon(BTN_UP, icon_play);
			drawButtonIcon(BTN_DOWN, icon_menu);
		}
	}
}

void TimerPage::startTimer() {
	int32_t hour = pageData.number3 / 60, minute = pageData.number3 % 60;
	
	timer.hour = MiteOS::currentTime.Hour + hour;
	timer.minute = MiteOS::currentTime.Minute + minute;
	while(timer.minute > 60) {
		timer.hour += 1;
		timer.minute -= 60;
	}
	
	pageData.number1 = PAGE_START_MENU;
	timer.enableAlarm = true;
}
