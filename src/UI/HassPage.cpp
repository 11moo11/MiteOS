#include "HassPage.h"
#include "../Managers/HassManager.h"
#include "../MiteOS.h"

#include "../Images/menu_icons.h"
#include <Fonts/FreeSans9pt7b.h>

#include "../Images/big_icons.h"

const char *haas_items[] = { 
	"Licht",
	"light.marius_right",
	"VR",
	"switch.tz3000_kdi2o9m6_ts011f_switch",
	"3D Printer",
	"switch.tz3000_kdi2o9m6_ts011f_switch_6",
	"Printer Light",
	"switch.tz3000_kdi2o9m6_ts011f_switch"
};
const int item_count = 4;

#define HASS_PAGE_COUNT 1

void HassPage::drawPage() {
	mDisplay.setFont(&FreeSans9pt7b);
	drawButtonIcon(BTN_BACK, icon_exit);
	drawButtonIcon(BTN_CONFIRM, icon_checkmark);
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
	
	displayPage();
}

bool HassPage::onButtonPressed(uint8_t buttonIndex) {
	switch(buttonIndex) {
		/*
		case BTN_BACK:
			PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
			return true;
		*/
		case BTN_CONFIRM:
			HassManager::setURL("http://hs.devforce.de:8123");
			HassManager::setToken("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiI0NzRlNjNkMDNlMTY0OTNmYWM4YmEzMjdhMDkzZjRmYiIsImlhdCI6MTcxMDMyMTg0MiwiZXhwIjoyMDI1NjgxODQyfQ.0nXo1q9Fd936L3Q6NEkMBltBNGGtIjF45xDcNEe1uZY");
			
			mDisplay.fillScreen(BACKGROUND_COLOR);
			mDisplay.setFont(&FreeSans9pt7b);
			mDisplay.setTextColor(FOREGROUND_COLOR);
			drawCentreString(TXT_WAIT, 100, 100, false);
			mDisplay.display(true);
			
			if(HassManager::toggle(haas_items[(pageData.number1 * 2) + 1 + (pageData.subPageIndex * 8)])) {
				mDisplay.fillScreen(BACKGROUND_COLOR);
				mDisplay.setFont(&FreeSans9pt7b);
				
				mDisplay.drawBitmap(80, 80, big_icon_checkmark, 40, 40, FOREGROUND_COLOR);
				
				drawCentreString(TXT_SUCCESS, 100, 150, false);
				AlertManager::vibMotor(50, 2);
			}else{
				mDisplay.fillScreen(BACKGROUND_COLOR);
				mDisplay.setFont(&FreeSans9pt7b);
				
				mDisplay.drawBitmap(80, 80, big_icon_x, 40, 40, FOREGROUND_COLOR);
				
				drawCentreString(TXT_FAILED, 100, 150, false);
				AlertManager::vibMotor(200, 2);
			}
			mDisplay.display(true);
			sleep(1);
			
			return true;
		case BTN_DOWN:
			do {
				pageData.number1++;
				if(pageData.number1 >= 4) {
					pageData.number1 = 0;
					pageData.subPageIndex++;
					if(pageData.subPageIndex >= HASS_PAGE_COUNT) {
						pageData.subPageIndex = 0;
					}
				}
			}while(pageData.number1 + (pageData.subPageIndex * 4) > item_count);

			return true;
		case BTN_UP:
			do {
				if(pageData.number1 == 0) {
					pageData.number1 = 3;
					if(pageData.subPageIndex == 0) {
						pageData.subPageIndex = HASS_PAGE_COUNT - 1;
					}else{
						pageData.subPageIndex--;
					}
				}else{
					pageData.number1--;
				}
			}while(pageData.number1 + (pageData.subPageIndex * 4) > item_count);
			return true;
	}
	
	return false;
}

void HassPage::displayPage() {
	for(uint8_t i = 0; i < min(4, item_count); i++) {
		if(i >= item_count) return;
		uint8_t x = 25 + (i >= 2 ? 80 : 0);
		uint8_t y = 25 + (i % 2 == 1 ? 70 : 0);
		
		const unsigned char* icon = big_icon_power;
		String entityId = haas_items[(i * 2) + 1 + (pageData.subPageIndex * 8)];
		if(entityId.startsWith("light.")) icon = big_icon_lightbulb;
		
		drawCentreString(haas_items[(i * 2) + (pageData.subPageIndex * 8)], x + 30, y + 68);
		
		if(pageData.number1 == i) {
			mDisplay.fillRect(x + 8, y + 8, 44, 44, FOREGROUND_COLOR);
			mDisplay.drawBitmap(x + 10, y + 10, icon, 40, 40, BACKGROUND_COLOR);
		}else{
			mDisplay.drawBitmap(x + 10, y + 10, icon, 40, 40, FOREGROUND_COLOR);
		}
	}
}