#include "PageManager.h"

// Pages
#include "../UI/WatchfacePage.h"
#include "../UI/TimerPage.h"
#include "../UI/AlarmPage.h"
#include "../UI/SettingsPage.h"
#include "../UI/AboutPage.h"
#include "../UI/WeatherPage.h"
#include "../UI/AppPage.h"
#include "../UI/ActivityPage.h"
#include "../UI/BluetoothPage.h"
#include "../UI/NotificationPage.h"
#include "../UI/PlaybackPage.h"
#include "../UI/HassPage.h"
#include "../UI/WoolyPage.h"
#include "../UI/TOTPPage.h"
#include "../UI/CalendarPage.h"

PROGMEM WatchfacePage watchfacePage;
PROGMEM TimerPage timerPage;
PROGMEM SettingsPage settingsPage;
PROGMEM AboutPage aboutPage;
PROGMEM WeatherPage weatherPage;
PROGMEM AlarmPage alarmPage;
PROGMEM AppPage appPage;
PROGMEM ActivityPage activityPage;
PROGMEM BluetoothPage bluetoothPage;
PROGMEM NotificationPage notificationPage;
PROGMEM PlaybackPage playbackPage;
PROGMEM HassPage hassPage;
PROGMEM WoolyPage woolyPage;
PROGMEM TOTPPage totpPage;
PROGMEM CalendarPage calendarPage;
PROGMEM Page* pages[] = {
	&watchfacePage,
	&timerPage,
	&alarmPage,
	&settingsPage,
	&aboutPage,
	&weatherPage,
	&appPage,
	&activityPage,
	&bluetoothPage,
	&notificationPage,
	&playbackPage,
	&hassPage,
	&woolyPage,
	&totpPage,
	&calendarPage,
};

RTC_DATA_ATTR bool returnToAppPage = false;

void PageManager::handleButtonPress(uint8_t buttonIndex) {
	if(buttonIndex == BTN_DOUBLE_TAP) {
		if(doubleTapBtn == 0) return;
		buttonIndex = doubleTapBtn;
	}else if(buttonIndex == BTN_SINGLE_TAP) {
		return; // TODO: Implement in the future
	}
	
	if(btnFeedbackVibrate) AlertManager::vibMotor(30, 2);
	
	// Check if the page is available to handle the input
	if(sizeof(pages) > pageData.pageIndex) {
		if(pages[pageData.pageIndex]->onButtonPressed(buttonIndex)) { // If the page handled the input, no need for the main OS to handle it
			refreshPage();
			return;
		}
	}

	if(buttonIndex == BTN_NEXT_PAGE) {
		nextPage();
		
		refreshPage();
		return;
	}else if(buttonIndex == BTN_HOME) {
		/*
		if(pageData.pageIndex != GLOBAL_PAGE_APPS && !pages[pageData.pageIndex]->isPageable())
			showPage(GLOBAL_PAGE_APPS);
		else
			showPage(GLOBAL_PAGE_WATCHFACE);
		*/
		goBack();

		refreshPage();
		return;
	}
}


void PageManager::refreshPage(bool partialRefresh) {
	// At this point it is sure we are going to update
	mDisplay.epd2.asyncPowerOn();
	
	printDebug("Power On Display");
	
	displayPoweredOn = true;
	
	if(sizeof(pages) > pageData.pageIndex) {
		printDebug("Rendering Page " + String(pageData.pageIndex));
		
		mDisplay.fillScreen(BACKGROUND_COLOR);
		
		mDisplay.setTextColor(FOREGROUND_COLOR);
		
		pages[pageData.pageIndex]->drawPage();
	}
	
	mDisplay.display(partialRefresh);
	mDisplay.screenshot();
}

Page* PageManager::getPage(uint8_t pageIndex) {
	if(pageIndex < 0 || pageIndex >= PAGE_COUNT) return pages[0];

	return pages[pageIndex];
}

void PageManager::showPage(uint8_t pageIndex) {
	if(pageIndex < 0 || pageIndex >= PAGE_COUNT) return;
	
	if(pageData.pageIndex != pageIndex) { // Reset the page data when its really changed
		if(pageData.pageIndex == GLOBAL_PAGE_APPS && pageIndex != GLOBAL_PAGE_WATCHFACE)
			returnToAppPage = true;

		pageData = PageData();
		pageData.subPageIndex = 0;
		pageData.menuIndex = 0;
		pageData.menuOffset = 0;
		pages[pageIndex]->initPage();
	}
	
	pageData.pageIndex = pageIndex;
	//osInstance->refreshPage();
}

void PageManager::goBack() {
	if(returnToAppPage) {
		returnToAppPage = false;
		PageManager::showPage(GLOBAL_PAGE_APPS);
	}else{
		PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
	}
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

void PageManager::showConnectionIcon(const unsigned char* icon) {
	if(!displayPoweredOn) {
		mDisplay.epd2.asyncPowerOn();
		mDisplay.fillScreen(BACKGROUND_COLOR);
		mDisplay.setTextColor(FOREGROUND_COLOR);
		displayPoweredOn = true;
	}
	
	mDisplay.drawBitmap(90, 0, icon, 20, 20, FOREGROUND_COLOR);
	mDisplay.display(true);
}