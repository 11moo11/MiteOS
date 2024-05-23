#include "WatchfacePage.h"
#include "../MiteOS.h"

#include "Watchfaces/7SEG.h"
#include "Watchfaces/BTTF.h"

RTC_DATA_ATTR uint8_t watchFaceId = 0;

#define WATCHFACE_COUNT 2

void WatchfacePage::drawPage() {
		#ifdef DEBUG
		Serial.println("Drawing Watchface");
		#endif
		
		mDisplay.fillScreen(BACKGROUND_COLOR);
		mDisplay.setTextColor(FOREGROUND_COLOR);
		
		if(watchFaceId == 0) {
			SEG7().draw();
		}else if(watchFaceId == 1) {
			BTTF().draw();
		}
}

bool WatchfacePage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_TOGGLE_BTN) {
		if(DARKMODE) {
			DARKMODE = false;
		}else{
			watchFaceId++;
			DARKMODE = true;
			if(watchFaceId >= WATCHFACE_COUNT) watchFaceId = 0;
		}
		//DARKMODE = !DARKMODE;
		//MiteOS::refreshPage();
		return true;
	}
	
	if(buttonIndex == BTN_UP) {
		MiteOS::showPage(GLOBAL_PAGE_SETTINGS);
		return true;
	}
	return false;
}

