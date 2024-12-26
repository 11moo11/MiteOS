#include "AppPage.h"

#include "../Images/menu_icons.h"
#include "../Images/app_icons.h"
#include "../Managers/PageManager.h"
#include <Fonts/FreeSans9pt7b.h>

#define APP_PAGE_COUNT 2

RTC_DATA_ATTR uint8_t currentAppPage;
RTC_DATA_ATTR uint8_t currentAppIndex;

static const unsigned char* const pageIcons[] = {
	app_icon_gear,
	app_icon_alarm,
	app_icon_stopwatch,
	app_icon_bluetooth,
	
	app_icon_music,
	app_icon_messages,
	app_icon_hass,
	app_icon_paw,
};

static const String pageTitles[] = {
	TXT_SETTINGS,
	TXT_ALARM,
	TXT_TIMER,
	TXT_BLUETOOTH,
	
	TXT_MEDIA,
	TXT_NOTIFICATION,
	TXT_HASS,
	TXT_FURLY,
};
static const uint8_t pageTarget[] = {
	GLOBAL_PAGE_SETTINGS,
	GLOBAL_PAGE_ALARM,
	GLOBAL_PAGE_TIMER,
	GLOBAL_PAGE_BLUETOOTH,
	
	GLOBAL_PAGE_PLAYBACK,
	GLOBAL_PAGE_NOTIFICATIONS,
	GLOBAL_PAGE_HASS,
	GLOBAL_PAGE_FURLY,
};

void AppPage::drawPage() {
	mDisplay.setFont(&FreeSans9pt7b);
	drawButtonIcon(BTN_BACK, icon_home);
	drawButtonIcon(BTN_CONFIRM, icon_checkmark);
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);

	if(APP_PAGE_COUNT > 1) {
		drawCentreString(String(currentAppPage + 1) + " / " + String(APP_PAGE_COUNT), 100, 15, false);
	}
	displayPage();
}

bool AppPage::onButtonPressed(uint8_t buttonIndex) {
	switch(buttonIndex) {
		case BTN_BACK:
			PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
			return true;
		case BTN_CONFIRM:
			if(pageTarget[currentAppIndex + (currentAppPage * 4)] != 255) {
				PageManager::showPage(pageTarget[currentAppIndex + (currentAppPage * 4)]);
			}
			return true;
		case BTN_DOWN:
			do {
				currentAppIndex++;
				if(currentAppIndex >= 4) {
					currentAppIndex = 0;
					currentAppPage++;
					if(currentAppPage >= APP_PAGE_COUNT) {
						currentAppPage = 0;
					}
				}
			}while(pageTarget[currentAppIndex + (currentAppPage * 4)] == 255);

			return true;
		case BTN_UP:
			do {
				if(currentAppIndex == 0) {
					currentAppIndex = 3;
					if(currentAppPage == 0) {
						currentAppPage = APP_PAGE_COUNT - 1;
					}else{
						currentAppPage--;
					}
				}else{
					currentAppIndex--;
				}
			}while(pageTarget[currentAppIndex + (currentAppPage * 4)] == 255);
			return true;
	}
}

void AppPage::displayPage() {
	for(uint8_t i = 0; i < 4; i++) {
		uint8_t x = 25 + (i >= 2 ? 90 : 0);
		uint8_t y = 25 + (i % 2 == 1 ? 70 : 0);
		drawCentreString(pageTitles[i + (currentAppPage * 4)], x + 30, y + 68);
		
		if(currentAppIndex == i) {
			mDisplay.fillRect(x + 8, y + 8, 44, 44, FOREGROUND_COLOR);
			mDisplay.drawBitmap(x + 10, y + 10, pageIcons[i + (currentAppPage * 4)], 40, 40, BACKGROUND_COLOR);
		}else{
			mDisplay.drawBitmap(x + 10, y + 10, pageIcons[i + (currentAppPage * 4)], 40, 40, FOREGROUND_COLOR);
		}
	}
}