#include "Page.h"


RTC_DATA_ATTR PageData pageData;

void Page::showMenu(char *menuItems[], byte menuIndex, bool partialRefresh) {
	GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> display = MiteOS.display;
	
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);

	int16_t x1, y1;
	uint16_t w, h;
	int16_t yPos;

	for (int i = 0; i < sizeof(menuItems); i++) {
		yPos = MENU_HEIGHT + (MENU_HEIGHT * i);
		display.setCursor(0, yPos);
		if (i == menuIndex) {
			display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
			display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
			display.setTextColor(GxEPD_BLACK);
			display.println(menuItems[i]);
		} else {
			display.setTextColor(GxEPD_WHITE);
			display.println(menuItems[i]);
		}
	}

	display.display(partialRefresh);

	//guiState = MAIN_MENU_STATE;
	//alreadyInMenu = false;
}