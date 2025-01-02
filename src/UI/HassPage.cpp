#include "HassPage.h"
#include "../Managers/HassManager.h"
#include "../MiteOS.h"

#include "../Images/menu_icons.h"
#include <Fonts/FreeSans9pt7b.h>

#include "../Images/big_icons.h"

uint8_t item_count = 1;
uint8_t page_count = 1;
String ent;

void HassPage::drawPage() {
	loadData();

	mDisplay.setFont(&FreeSans9pt7b);
	drawButtonIcon(BTN_BACK, icon_exit);
	drawButtonIcon(BTN_CONFIRM, icon_checkmark);
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
	
	displayPage();
}

void HassPage::loadData() {
	Configuration::loadHassConfig();

	ent = HassManager::getEntities();
	item_count = 1;
	for (int i = 0; i < ent.length(); i++) {
		if (ent.charAt(i) == ',') {
			item_count++;
		}
	}
	item_count /= 2;
	page_count = (uint8_t) max((uint8_t) 1, (uint8_t) ceil(item_count / 4.0));
}

String HassPage::getElement(uint8_t index) {
	uint16_t tindex = 0;
	for(uint8_t i = index; i > 0; i--) {
		tindex = ent.indexOf(',', tindex) + 1;
	}
	return ent.substring(tindex, ent.indexOf(',', tindex));
}

bool HassPage::onButtonPressed(uint8_t buttonIndex) {
	loadData();

	switch(buttonIndex) {
		/*
		case BTN_BACK:
			PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
			return true;
		*/
		case BTN_CONFIRM:
			mDisplay.fillScreen(BACKGROUND_COLOR);
			mDisplay.setFont(&FreeSans9pt7b);
			mDisplay.setTextColor(FOREGROUND_COLOR);
			drawCentreString(TXT_WAIT, 100, 100, false);
			mDisplay.display(true);
			
			if(HassManager::toggle(getElement((pageData.number1 * 2) + 1 + (pageData.subPageIndex * 8)))) {
				drawSuccess(true);
			}else{
				drawSuccess(false);
			}
			
			return true;
		case BTN_DOWN:
			do {
				pageData.number1++;
				if(pageData.number1 >= 4) {
					pageData.number1 = 0;
					pageData.subPageIndex++;
					if(pageData.subPageIndex >= page_count) {
						pageData.subPageIndex = 0;
					}
				}
			}while(pageData.number1 + (pageData.subPageIndex * 4) >= item_count);
			printDebug(pageData.number1);
			return true;
		case BTN_UP:
			do {
				if(pageData.number1 == 0) {
					pageData.number1 = 3;
					if(pageData.subPageIndex == 0) {
						pageData.subPageIndex = page_count - 1;
					}else{
						pageData.subPageIndex--;
					}
				}else{
					pageData.number1--;
				}
			}while(pageData.number1 + (pageData.subPageIndex * 4) >= item_count);
			printDebug(pageData.number1);
			return true;
	}
	
	return false;
}

void HassPage::displayPage() {
	for(uint8_t i = 0; i < 4; i++) {
		if(i >= (pageData.subPageIndex * 4) + item_count) break;

		uint8_t x = 25 + (i >= 2 ? 80 : 0);
		uint8_t y = 25 + (i % 2 == 1 ? 70 : 0);
		
		const unsigned char* icon = big_icon_power;
		String entityId = getElement((i * 2) + 1 + (pageData.subPageIndex * 8));
		if(entityId.startsWith("light.")) icon = big_icon_lightbulb;
		
		drawCentreString(getElement((i * 2) + (pageData.subPageIndex * 8)), x + 30, y + 68);
		
		if(pageData.number1 == i) {
			mDisplay.fillRect(x + 8, y + 8, 44, 44, FOREGROUND_COLOR);
			mDisplay.drawBitmap(x + 10, y + 10, icon, 40, 40, BACKGROUND_COLOR);
		}else{
			mDisplay.drawBitmap(x + 10, y + 10, icon, 40, 40, FOREGROUND_COLOR);
		}
	}

	drawCentreString(String(pageData.subPageIndex + 1) + " / " + String(page_count), 100, 15, false);
}