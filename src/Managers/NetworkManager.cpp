#include "NetworkManager.h"
#include "../MiteOS.h"
#include "settings.h"
#include <NTPClient.h>
#include "WeatherManager.h"
#include "../Images/menu_icons.h"

RTC_DATA_ATTR bool WifiConfigured;
RTC_DATA_ATTR uint32_t lastWifiIPAddress;
RTC_DATA_ATTR char lastWifiSSID[30];

bool NetworkManager::connectWifi() {
	BluetoothManager::powerOff();
	
	PageManager::showConnectionIcon(icon_wifi);
	
	if (WL_CONNECT_FAILED == WiFi.begin()) { // WiFi not setup, you can also use hard coded credentials
											 // with WiFi.begin(SSID,PASS);
		WifiConfigured = false;
	} else {
		if (WL_CONNECTED == WiFi.waitForConnectResult()) { // attempt to connect for 10s
			lastWifiIPAddress = WiFi.localIP();
			WiFi.SSID().toCharArray(lastWifiSSID, 30);
			WifiConfigured = true;
		} else { // connection failed, time out
			WifiConfigured = false;
			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		}
	}
	return WifiConfigured;
}



void NetworkManager::registerNewWifiConnection() {
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
		
		weatherCheckCounter = -1; // Reset to force weather to be read again
		
		lastWifiIPAddress = WiFi.localIP();
		WiFi.SSID().toCharArray(lastWifiSSID, 30);
	}
	mDisplay.display(false); // full refresh
	
	// turn off radios
	WiFi.mode(WIFI_OFF);
	btStop();
	
	// enable lightsleep on busy
	mDisplay.epd2.setBusyCallback(WatchyDisplay::busyCallback);
}

void NetworkManager::_configModeCallback(WiFiManager *myWiFiManager) {
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

void NetworkManager::showSyncNTP() {
	mDisplay.setFullWindow();
	mDisplay.fillScreen(GxEPD_BLACK);
	mDisplay.setFont(&FreeMonoBold9pt7b);
	mDisplay.setTextColor(GxEPD_WHITE);
	mDisplay.setCursor(0, 30);
	mDisplay.println("Syncing NTP... ");
	mDisplay.print("GMT offset: ");
	mDisplay.println(MiteOS::instance->settings.gmtOffset);
	mDisplay.display(false); // full refresh
	
	
	if (connectWifi()) {
		if (syncNTP()) {
			mDisplay.println("NTP Sync Success\n");
			mDisplay.println("Current Time Is:");

			mRTC.read(MiteOS::currentTime);

			mDisplay.print(tmYearToCalendar(MiteOS::currentTime.Year));
			mDisplay.print("/");
			mDisplay.print(MiteOS::currentTime.Month);
			mDisplay.print("/");
			mDisplay.print(MiteOS::currentTime.Day);
			mDisplay.print(" - ");

			if (MiteOS::currentTime.Hour < 10) {
				mDisplay.print("0");
			}
			mDisplay.print(MiteOS::currentTime.Hour);
			mDisplay.print(":");
			if (MiteOS::currentTime.Minute < 10) {
				mDisplay.print("0");
			}
			mDisplay.println(MiteOS::currentTime.Minute);
		} else {
			mDisplay.println("NTP Sync Failed");
		}
	} else {
		mDisplay.println("WiFi Not Configured");
	}
	
	mDisplay.display(true); // full refresh
	delay(3000);
}

bool NetworkManager::syncNTP() { // NTP sync - call after connecting to WiFi and 
								 // remember to turn it back off
	return syncNTP(MiteOS::instance->settings.gmtOffset, MiteOS::instance->settings.ntpServer.c_str());
}

bool NetworkManager::syncNTP(long gmt) {
  return syncNTP(gmt, MiteOS::instance->settings.ntpServer.c_str());
}

bool NetworkManager::syncNTP(long gmt, String ntpServer) {
	// NTP sync - call after connecting to
	// WiFi and remember to turn it back off
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP, ntpServer.c_str(), gmt);
	timeClient.begin();
	if (!timeClient.forceUpdate()) {
		return false; // NTP sync failed
	}
	tmElements_t tm;
	breakTime((time_t)timeClient.getEpochTime(), tm);
	mRTC.set(tm);
	return true;
}

void NetworkManager::powerOff() {
	WiFi.mode(WIFI_OFF);
	
	WifiConfigured = false;
}