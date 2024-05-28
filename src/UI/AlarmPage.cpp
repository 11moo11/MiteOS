#include "AlarmPage.h"
#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Images/menu_icons.h"


#define ALARM_PAGE_OVERVIEW 0

void AlarmPage::initPage() {
	pageData.number1 = ALARM_PAGE_OVERVIEW;
}

void AlarmPage::drawPage() {
	switch(pageData.number1) {
		case ALARM_PAGE_OVERVIEW:
			drawOverview();
			break;
		default: break;
	}
}

bool AlarmPage::onButtonPressed(uint8_t buttonIndex) {
	
	return false;
}

void AlarmPage::drawOverview() {
	int16_t x1, y1;
	uint16_t w, h;
	
	for(uint8_t i = 0; i < 2; i++) {
		uint8_t yPos = MENU_HEIGHT + (MENU_HEIGHT * i);
		mDisplay.setCursor(25, yPos);
		String item = "Alarm " + String(1);
		if (i == pageData.number2) {
			mDisplay.getTextBounds(item, 25, yPos, &x1, &y1, &w, &h);
			mDisplay.fillRect(x1 - 1, y1 - 10, 150, h + 15, FOREGROUND_COLOR);
			mDisplay.setTextColor(BACKGROUND_COLOR);
			mDisplay.println(item);
		} else {
			mDisplay.setTextColor(FOREGROUND_COLOR);
			mDisplay.println(item);
		}
	}
}
