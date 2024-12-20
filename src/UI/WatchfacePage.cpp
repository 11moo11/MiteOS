#include "WatchfacePage.h"
#include "../MiteOS.h"

#include "Watchfaces/Watchface.h"
#include "Watchfaces/SEG7Watchface.h"
#include "Watchfaces/BTTFWatchface.h"
#include "Watchfaces/PokemonWatchface.h"
#include "Watchfaces/MachPaintWatchface.h"
#include "Watchfaces/AnalogWatchface.h"
#include "Watchfaces/HobbitTimeWatchface.h"
#include "Watchfaces/CalendarWatchface.h"
#include "Watchfaces/TrainWatchface.h"

RTC_DATA_ATTR uint8_t watchFaceId = 0;

PROGMEM SEG7Watchface seg7Watchface;
PROGMEM BTTFWatchface bttfWatchface;
PROGMEM PokemonWatchface pokemonWatchface;
PROGMEM MacPaintWatchface macPaintWatchface;
PROGMEM AnalogWatchface analogWatchface;
PROGMEM HobbitTimeWatchface hobbitTimeWatchface;
PROGMEM CalendarWatchface calendarWatchface;
PROGMEM TrainWatchface trainWatchface;
PROGMEM Watchface* watchfaces[] = {
	&seg7Watchface,
	&bttfWatchface,
	&pokemonWatchface,
	&macPaintWatchface,
	&analogWatchface,
	&hobbitTimeWatchface,
	&calendarWatchface,
	&trainWatchface,
};

#define WATCHFACE_COUNT 8

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

