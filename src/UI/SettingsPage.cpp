#include "SettingsPage.h"
#include "../MiteOS.h"

void SettingsPage::drawPage() {
	
	const char *menuItems[] = {
		"About Watchy", "Vibrate Motor", "Show Accelerometer",
		"Set Time",     "Setup WiFi",    "Update Firmware",
		"Sync NTP"};
	
	showMenu(menuItems, 7, true);
}

bool SettingsPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex))
		return true;
	
	if(buttonIndex == BTN_BACK) {
		MiteOS::showPage(GLOBAL_PAGE_WATCHFACE);
		return true;
	}else if(buttonIndex == BTN_CONFIRM) {
		if(pageData.menuIndex == 0) {
			MiteOS::showPage(GLOBAL_PAGE_ABOUT);
			return true;
		}
	}
	
	return false;
}