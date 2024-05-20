#include "Page.h"

#include "../MiteOS.h"
#include <GxEPD2_EPD.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void Page::showMenu(char *menuItems[], bool partialRefresh) {
	MiteOS::display.setFullWindow();
	MiteOS::display.fillScreen(GxEPD_BLACK);
	MiteOS::display.setFont(&FreeMonoBold9pt7b);

	int16_t x1, y1;
	uint16_t w, h;
	int16_t yPos;

	for (int i = 0; i < sizeof(menuItems); i++) {
		yPos = MENU_HEIGHT + (MENU_HEIGHT * i);
		MiteOS::display.setCursor(0, yPos);
		if (i == pageData.menuIndex) {
			MiteOS::display.getTextBounds(menuItems[i], 0, yPos, &x1, &y1, &w, &h);
			MiteOS::display.fillRect(x1 - 1, y1 - 10, 200, h + 15, GxEPD_WHITE);
			MiteOS::display.setTextColor(GxEPD_BLACK);
			MiteOS::display.println(menuItems[i]);
		} else {
			MiteOS::display.setTextColor(GxEPD_WHITE);
			MiteOS::display.println(menuItems[i]);
		}
	}

	MiteOS::display.display(partialRefresh);

	//guiState = MAIN_MENU_STATE;
	//alreadyInMenu = false;
}