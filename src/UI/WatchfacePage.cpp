#include "WatchfacePage.h"
#include "../MiteOS.h"

#include "Watchfaces/Watchface.h"
#include "Watchfaces/SEG7Watchface.h"
#include "Watchfaces/BTTFWatchface.h"
#include "Watchfaces/PokemonWatchface.h"
#include "Watchfaces/MachPaintWatchface.h"

RTC_DATA_ATTR uint8_t watchFaceId = 0;

PROGMEM SEG7Watchface seg7Watchface;
PROGMEM BTTFWatchface bttfWatchface;
PROGMEM PokemonWatchface pokemonWatchface;
PROGMEM MacPaintWatchface macPaintWatchface;
PROGMEM Watchface* watchfaces[] = {
	&seg7Watchface,
	&bttfWatchface,
	&pokemonWatchface,
	&macPaintWatchface,
};


#define WATCHFACE_COUNT 4

void WatchfacePage::drawPage() {
	printDebug("Drawing Watchface");
	
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	
	watchfaces[watchFaceId]->draw();
}

bool WatchfacePage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_TOGGLE_BTN) {
		watchFaceId++;
		if(watchFaceId >= WATCHFACE_COUNT) watchFaceId = 0;
		Configuration::saveSettings();
		return true;
	}
	
	if(buttonIndex == BTN_MENU) {
		PageManager::showPage(GLOBAL_PAGE_APPS);
		return true;
	}
	
	if(buttonIndex == BTN_UP) {
		PageManager::showPage(GLOBAL_PAGE_NOTIFICATIONS);
		return true;
	}
	return false;
}

