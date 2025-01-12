#include "WifiConnectionManager.h"
#include "../MiteOS.h"
#include <NTPClient.h>
#include "WeatherManager.h"
#include "../Images/menu_icons.h"

RTC_DATA_ATTR bool WifiConfigured;
RTC_DATA_ATTR uint32_t lastWifiIPAddress;
RTC_DATA_ATTR char lastWifiSSID[30];

bool WifiConnectionManager::connectWifi() {
	BluetoothManager::powerOff();
	
	PageManager::showConnectionIcon(icon_wifi);

	WifiConfigured = false;

	WiFi.mode(WIFI_MODE_STA);

	if (WL_CONNECT_FAILED != WiFi.begin()) { // WiFi not setup, you can also use hard coded credentials
											 // with WiFi.begin(SSID,PASS);
		WifiConfigured = false;
		uint8_t wait = 50;
		while (wait-- > 0) {
			if(WiFi.status() == WL_CONNECTED) {
				lastWifiIPAddress = WiFi.localIP();
				WiFi.SSID().toCharArray(lastWifiSSID, 30);
				WifiConfigured = true;
				break;
			}
			delay(100);
		}
	}

	if(!WifiConfigured) {
		delay(50);

		// turn off radios
		powerOff();
	}

	return WifiConfigured;
}



void WifiConnectionManager::registerNewWifiConnection() {
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

		lastWifiIPAddress = WiFi.localIP();
		WiFi.SSID().toCharArray(lastWifiSSID, 30);
		
		WeatherManager::getWeatherData();
	}
	mDisplay.display(false); // full refresh
	
	// turn off radios
	WiFi.mode(WIFI_OFF);
	btStop();
	
	// enable lightsleep on busy
	mDisplay.epd2.setBusyCallback(WatchyDisplay::busyCallback);
}

void WifiConnectionManager::_configModeCallback(WiFiManager *myWiFiManager) {
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

void WifiConnectionManager::showSyncNTP() {
	mDisplay.setFullWindow();
	mDisplay.fillScreen(GxEPD_BLACK);
	mDisplay.setFont(&FreeMonoBold9pt7b);
	mDisplay.setTextColor(GxEPD_WHITE);
	mDisplay.setCursor(0, 30);
	mDisplay.println("Syncing NTP... ");
	mDisplay.print("GMT offset: ");
	mDisplay.println(gmtTimeOffset);
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

	powerOff();

	mDisplay.display(true); // full refresh
	delay(3000);
}

bool WifiConnectionManager::syncNTP() { // NTP sync - call after connecting to WiFi and 
								 // remember to turn it back off
	return syncNTP(gmtTimeOffset, Configuration::getNtpServer().c_str());
}

bool WifiConnectionManager::syncNTP(long gmt) {
  return syncNTP(gmt, Configuration::getNtpServer().c_str());
}

bool WifiConnectionManager::syncNTP(long gmt, String ntpServer) {
	// NTP sync - call after connecting to
	// WiFi and remember to turn it back off
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP, ntpServer.c_str(), gmt);
	timeClient.begin();
	if (!timeClient.forceUpdate()) {
		return false; // NTP sync failed
	}
	tmElements_t tm;
	time_t t = timeClient.getEpochTime();
	mRTC.doBreakTime(t, tm);
	mRTC.set(tm);
	
	return true;
}

void WifiConnectionManager::powerOff() {
	WiFi.disconnect();
	WiFi.mode(WIFI_OFF);
	
	WifiConfigured = false;
}