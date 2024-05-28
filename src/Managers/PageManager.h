#ifndef MITEOS_PAGE_MANAGER_H
#define MITEOS_PAGE_MANAGER_H

#include "../MiteOS.h"

#define GLOBAL_PAGE_WATCHFACE 0
#define GLOBAL_PAGE_TIMER 1
#define GLOBAL_PAGE_ALARM 2
#define GLOBAL_PAGE_SETTINGS 3
#define GLOBAL_PAGE_ABOUT 4
#define GLOBAL_PAGE_WEATHER 5

#define PAGE_COUNT 5

class PageManager {
	public:
		static void nextPage();
		static void previousPage();
		static void showPage(uint8_t pageIndex);
		
		static void refreshPage(bool partialRefresh = true);
		static void handleButtonPress(uint8_t buttonIndex);
};

#endif