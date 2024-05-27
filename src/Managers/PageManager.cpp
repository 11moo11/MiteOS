#include "PageManager.h"

// Pages
#include "../UI/WatchfacePage.h"
#include "../UI/TimerPage.h"
#include "../UI/SettingsPage.h"
#include "../UI/AboutPage.h"
#include "../UI/WeatherPage.h"

WatchfacePage watchfacePage;
TimerPage timerPage;
SettingsPage settingsPage;
AboutPage aboutPage;
WeatherPage weatherPage;
Page* pages[] = {
	&watchfacePage,
	&timerPage,
	&settingsPage,
	&aboutPage,
	&weatherPage
};


void PageManager::handleButtonPress(uint8_t buttonIndex) {
	// Check if the page is available to handle the input
	if(sizeof(pages) > pageData.pageIndex) {
		if(pages[pageData.pageIndex]->onButtonPressed(buttonIndex)) { // If the page handled the input, no need for the main OS to handle it
			refreshPage();
			return;
		}
	}

	if(buttonIndex == BTN_MENU) {
		nextPage();
		showPage(pageData.pageIndex);
		
		refreshPage();
		return;
	}
}


void PageManager::refreshPage(bool partialRefresh) {
	mDisplay.setFullWindow();
	// At this point it is sure we are going to update
	mDisplay.epd2.asyncPowerOn();
	
	#ifdef DEBUG
	Serial.println("Power On Display");
	#endif
	
	if(sizeof(pages) > pageData.pageIndex) {
		#ifdef DEBUG
		Serial.println("Rendering Page " + String(pageData.pageIndex));
		#endif
		mDisplay.setFullWindow();
		mDisplay.fillScreen(BACKGROUND_COLOR);
		//watchyDisplay.setDarkBorder(DARKMODE);
		
		mDisplay.setTextColor(FOREGROUND_COLOR);
		
		pages[pageData.pageIndex]->drawPage();
	}
	
	mDisplay.display(partialRefresh);
}

void PageManager::showPage(uint8_t pageIndex) {
	if(pageIndex < 0 || pageIndex >= PAGE_COUNT) return;
	
	if(pageData.pageIndex != pageIndex) { // Reset the page data when its really changed
		pageData = PageData();
		pages[pageIndex]->initPage();
	}
	
	pageData.pageIndex = pageIndex;
	//osInstance->refreshPage();
}

void PageManager::nextPage() {
	uint8_t newIndex = pageData.pageIndex;
	
	do {
		newIndex++;
		if(newIndex >= PAGE_COUNT) newIndex = 0;
	} while(! pages[newIndex]->isPageable());
	
	showPage(newIndex);
}

void PageManager::previousPage() {
	uint8_t newIndex = pageData.pageIndex;
	
	do {
		newIndex--;
		if(newIndex < 0) newIndex = PAGE_COUNT - 1;
	} while(! pages[newIndex]->isPageable());
	
	showPage(newIndex);
}