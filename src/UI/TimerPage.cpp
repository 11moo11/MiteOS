#include "TimerPage.h"
#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Fonts/icons.h"

#define TIMER_SLIM_MODE true

void TimerPage::drawPage() {
	drawTime();
	drawIcons();
}

bool TimerPage::onButtonPressed(uint8_t buttonIndex) {
	if(timer.enableAlarm) {
		if(timer.triggered) {
			timer.triggered = false;
			timer.enableAlarm = false;
			pageData.number3 = 1;
			return true;
		}else{
			if(buttonIndex == BTN_UP) {
				pageData.number3 = (timer.hour - MiteOS::currentTime.Hour) * 60 + (timer.minute - MiteOS::currentTime.Minute);
				timer.enableAlarm = false;
				return true;
			}
		}
	}else{
		#if TIMER_SLIM_MODE
		if(buttonIndex == BTN_UP) {
			pageData.number3 += 1;
			return true;
		}else if(buttonIndex == BTN_DOWN) {
			pageData.number3 -= 1;
			return true;
		}else if(buttonIndex == BTN_TOGGLE_BTN) {
			startTimer();
			return true;
		}
		#else
		if(pageData.number1 < 2) {
			if(buttonIndex == BTN_UP) {
				pageData.number3 += (pageData.number1 == 0 ? 1 : 5);
				return true;
			}else if(buttonIndex == BTN_DOWN) {
				pageData.number3 -= (pageData.number1 == 0 ? 1 : 5);
				return true;
			}else if(buttonIndex == BTN_TOGGLE_BTN) {
				pageData.number1 += 1;
				if(pageData.number1 >= 3) pageData.number1 = 0;
				return true;
			}
		}else{
			if(buttonIndex == BTN_UP) {
				startTimer();
				return true;
			}else if(buttonIndex == BTN_TOGGLE_BTN) {
				pageData.number1 += 1;
				if(pageData.number1 >= 3) pageData.number1 = 0;
				return true;
			}
		}
		#endif
	}
	return false;
}

void TimerPage::drawTime() {
	MiteOS::display.setFont(&DSEG7_Classic_Regular_39);

	if(timer.triggered) {
		MiteOS::drawCentreString("ALARM", 100, 110);
	}else{
		uint32_t minutes = pageData.number3;
		if(timer.enableAlarm) {
			minutes = (timer.hour - MiteOS::currentTime.Hour) * 60 + (timer.minute - MiteOS::currentTime.Minute);
		}

		int16_t  x1, y1;
		uint16_t w, h;
		String currentTime = String(minutes);
		MiteOS::drawCentreString(currentTime.c_str(), 100, 110);

		#if TIMER_SLIM_MODE
		pageData.number1 = 0;
		#endif

		MiteOS::display.setFont(&Seven_Segment10pt7b);
		if(timer.enableAlarm) {
			MiteOS::drawCentreString("Alarm in:", 100, 60);
		} else {
			if(pageData.number1 < 2) {
				String right_border = (pageData.number1 == 0 ? "1" : "5");
				MiteOS::display.setCursor(180, 100);
				MiteOS::display.println(right_border);
			}
		}

		MiteOS::drawCentreString("MIN", 100, 140);
		/*
		MiteOS::display.setFont(&DSEG7_Classic_Bold_25);
		MiteOS::display.setCursor(10, 30);
		MiteOS::display.println(String(MiteOS::getBatteryVoltage()) + "V");
		*/
	}
}

void TimerPage::drawIcons() {
	if(timer.enableAlarm) {
		MiteOS::drawButtonIcon(BTN_UP, icon_stop);
	} else {
		#if TIMER_SLIM_MODE
		MiteOS::drawButtonIcon(BTN_TOGGLE_BTN, icon_play);
		#else
		MiteOS::drawButtonIcon(BTN_TOGGLE_BTN, icon_right);
		#endif
		if(pageData.number1 == 0) {
			MiteOS::drawButtonIcon(BTN_UP, icon_plus);
			MiteOS::drawButtonIcon(BTN_DOWN, icon_minus);
		}else if(pageData.number1 == 1) {
			MiteOS::drawButtonIcon(BTN_UP, icon_plus);
			MiteOS::drawButtonIcon(BTN_DOWN, icon_minus);
		}else if(pageData.number1 == 2) {
			MiteOS::drawButtonIcon(BTN_UP, icon_play);
		}
	}
}

void TimerPage::startTimer() {
	int32_t hour = pageData.number3 / 60, minute = pageData.number3 % 60;

	timer.hour = MiteOS::currentTime.Hour + hour;
	timer.minute = MiteOS::currentTime.Minute + minute;
	while(timer.minute > 60) {
		hour += 1;
		timer.minute -= 60;
	}

	timer.enableAlarm = true;
}