#include "Page.h"

#include "../MiteOS.h"
#include <GxEPD2_EPD.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../Images/menu_icons.h"

void Page::showMenu(const char *menuItems[], uint8_t itemCount, bool partialRefresh) {
	mDisplay.setFullWindow();
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.setFont(&FreeMonoBold9pt7b);
	
	int16_t x1, y1;
	uint16_t w, h;
	int16_t yPos;
	
	if(pageData.menuIndex < 0 || pageData.menuIndex > 250) pageData.menuIndex = itemCount - 1;
	else if(pageData.menuIndex >= itemCount) pageData.menuIndex = 0;
	
	for (int i = 0; i < itemCount; i++) {
		yPos = MENU_HEIGHT + (MENU_HEIGHT * i);
		mDisplay.setCursor(25, yPos);
		if (i == pageData.menuIndex) {
			mDisplay.getTextBounds(menuItems[i], 25, yPos, &x1, &y1, &w, &h);
			mDisplay.fillRect(x1 - 1, y1 - 10, 150, h + 15, FOREGROUND_COLOR);
			mDisplay.setTextColor(BACKGROUND_COLOR);
			mDisplay.println(menuItems[i]);
		} else {
			mDisplay.setTextColor(FOREGROUND_COLOR);
			mDisplay.println(menuItems[i]);
		}
	}
	
	MiteOS::drawButtonIcon(BTN_BACK, icon_left);
	MiteOS::drawButtonIcon(BTN_MENU, icon_checkmark);
	MiteOS::drawButtonIcon(BTN_UP,   icon_up);
	MiteOS::drawButtonIcon(BTN_DOWN, icon_down);
}

bool Page::handleMenuButtons(uint8_t buttonIndex) {
	if(buttonIndex == BTN_UP) {
		pageData.menuIndex -= 1;
		return true;
	}else if(buttonIndex == BTN_DOWN) {
		pageData.menuIndex += 1;
		return true;
	}
	return false;
}