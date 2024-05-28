#include "AlarmPage.h"
#include "../MiteOS.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include "../Images/menu_icons.h"

#define ALARM_PAGE_OVERVIEW 0

void AlarmPage::initPage() {
	pageData.subPageIndex = ALARM_PAGE_OVERVIEW;
}

void AlarmPage::drawPage() {
	switch(pageData.subPageIndex) {
		case ALARM_PAGE_OVERVIEW:
			drawOverview();
			break;
		default: break;
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
			}
			break;
		default: break;
	}
	
	return false;
}

void AlarmPage::drawOverview() {
	int16_t x1, y1;
	uint16_t w, h;
	
	mDisplay.setFont(&Seven_Seg18pt7b);
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
		
		mDisplay.setCursor(60, yPos + 35);
		mDisplay.println(item);
		
		if(!alarms[i].enableAlarm) mDisplay.fillRect(45, yPos + 24, 110, 3, i == pageData.number2 ? BACKGROUND_COLOR : FOREGROUND_COLOR);
		
	}
}

void AlarmPage::drawIcons() {
	switch(pageData.subPageIndex){
		case ALARM_PAGE_OVERVIEW:
			drawButtonIcon(BTN_UP, icon_up);
			drawButtonIcon(BTN_DOWN, icon_down);
			drawButtonIcon(BTN_CONFIRM, icon_gear);
			break;
		default: break;
	}
}