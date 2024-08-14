#ifndef MITEOS_PAGE_MANAGER_H
#define MITEOS_PAGE_MANAGER_H

#include "../MiteOS.h"

#define GLOBAL_PAGE_WATCHFACE 0
#define GLOBAL_PAGE_TIMER 1
#define GLOBAL_PAGE_ALARM 2
#define GLOBAL_PAGE_SETTINGS 3
#define GLOBAL_PAGE_ABOUT 4
#define GLOBAL_PAGE_WEATHER 5
#define GLOBAL_PAGE_APPS 6
#define GLOBAL_PAGE_ACTIVITY 7
#define GLOBAL_PAGE_BLUETOOTH 8
#define GLOBAL_PAGE_NOTIFICATIONS 9
#define GLOBAL_PAGE_PLAYBACK 10
#define GLOBAL_PAGE_HASS 11
#define GLOBAL_PAGE_FURLY 12

#define PAGE_COUNT 13

class PageManager {
	public:
		static void nextPage();
		static void previousPage();
		static void showPage(uint8_t pageIndex);
		
		static void refreshPage(bool partialRefresh = true);
		static void handleButtonPress(uint8_t buttonIndex);
		
		static void showConnectionIcon(const unsigned char* icon);
};

#endif