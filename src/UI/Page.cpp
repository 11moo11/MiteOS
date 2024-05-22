#include "Page.h"

#include "../MiteOS.h"
#include <GxEPD2_EPD.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void Page::showMenu(const char *menuItems[], uint8_t itemCount, bool partialRefresh) {
	MiteOS::display.setFullWindow();
	MiteOS::display.fillScreen(BACKGROUND_COLOR);
	MiteOS::display.setFont(&FreeMonoBold9pt7b);
	
	int16_t x1, y1;
	uint16_t w, h;
	int16_t yPos;

	if(pageData.menuIndex >= itemCount) pageData.menuIndex = 0;
	else if(pageData.menuIndex < 0 || pageData.menuIndex > 250) pageData.menuIndex = itemCount - 1;

	for (int i = 0; i < itemCount; i++) {
		yPos = MENU_HEIGHT + (MENU_HEIGHT * i);
		MiteOS::display.setCursor(0, yPos);
		if (i == pageData.menuIndex) {
			MiteOS::display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
			MiteOS::display.fillRect(x1 - 1, y1 - 10, 200, h + 15, FOREGROUND_COLOR);
			MiteOS::display.setTextColor(BACKGROUND_COLOR);
			MiteOS::display.println(menuItems[i]);
		} else {
			MiteOS::display.setTextColor(FOREGROUND_COLOR);
			MiteOS::display.println(menuItems[i]);
		}
	}

	MiteOS::display.display(partialRefresh);

	//guiState = MAIN_MENU_STATE;
	//alreadyInMenu = false;
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