#include "SettingsPage.h"
#include "../MiteOS.h"

void SettingsPage::drawPage() {
	
	const char *menuItems[] = {
		"About MiteOS", "Vibrate Motor", "Accelerometer",
		"Set Time",     "Setup WiFi",    "Sync NTP"
	};
	
	showMenu(menuItems, 6, true);
}

bool SettingsPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex))
		return true;
	
	if(buttonIndex == BTN_BACK) {
		MiteOS::showPage(GLOBAL_PAGE_WATCHFACE);
		return true;
	}else if(buttonIndex == BTN_CONFIRM) {
		switch(pageData.menuIndex) {
			case 0:
				MiteOS::showPage(GLOBAL_PAGE_ABOUT);
				return true;
			
			case 1:
				MiteOS::vibMotor(50, 50);
				return true;
			
			case 4:
				setupWifi();
				return true;
			
			default:
				break;
		}
	}
	
	return false;
}

void SettingsPage::setupWifi() {
	mDisplay.epd2.setBusyCallback(0); // temporarily disable lightsleep on busy
	
	WiFiManager wifiManager;
	wifiManager.resetSettings();
	wifiManager.setTimeout(WIFI_AP_TIMEOUT);
	wifiManager.setAPCallback(_configModeCallback);
	
	mDisplay.setFullWindow();
	mDisplay.fillScreen(GxEPD_BLACK);
	mDisplay.setFont(&FreeMonoBold9pt7b);
	mDisplay.setTextColor(GxEPD_WHITE);
	if (!wifiManager.autoConnect(WIFI_AP_SSID)) { // WiFi setup failed
		mDisplay.println("Setup failed &");
		mDisplay.println("timed out!");
	} else {
		mDisplay.println("Connected to:");
		mDisplay.println(WiFi.SSID());
		mDisplay.println("Local IP:");
		mDisplay.println(WiFi.localIP());
		//weatherIntervalCounter = -1; // Reset to force weather to be read again
		//lastIPAddress = WiFi.localIP();
		//WiFi.SSID().toCharArray(lastSSID, 30);
	}
	mDisplay.display(false); // full refresh
	
	// turn off radios
	WiFi.mode(WIFI_OFF);
	btStop();
	
	// enable lightsleep on busy
	mDisplay.epd2.setBusyCallback(WatchyDisplay::busyCallback);
}


void SettingsPage::_configModeCallback(WiFiManager *myWiFiManager) {
	mDisplay.setFullWindow();
	mDisplay.setFont(&FreeMonoBold9pt7b);
	
	mDisplay.setCursor(0, 30);
	mDisplay.println("Connect to");
	mDisplay.print("SSID: ");
	mDisplay.println(WIFI_AP_SSID);
	mDisplay.print("IP: ");
	mDisplay.println(WiFi.softAPIP());
	mDisplay.println("MAC address:");
	mDisplay.println(WiFi.softAPmacAddress().c_str());
	
	mDisplay.display(false); // full refresh
}