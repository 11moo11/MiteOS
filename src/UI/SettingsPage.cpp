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
		MiteOS::showPage(0);
		return true;
	}

	return false;
}