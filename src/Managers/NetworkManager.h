#ifndef MITEOS_NETWORK_MANAGER_H
#define MITEOS_NETWORK_MANAGER_H

#include <WiFiManager.h>

extern RTC_DATA_ATTR bool WifiConfigured;
extern RTC_DATA_ATTR uint32_t lastWifiIPAddress;
extern RTC_DATA_ATTR char lastWifiSSID[30];

class NetworkManager {
    public:
        static bool WIFI_CONNECTED;
    public:
        static void registerNewWifiConnection();
        static bool connectWifi();
        
        static void showSyncNTP();
        static bool syncNTP();
        static bool syncNTP(long gmt);
        static bool syncNTP(long gmt, String ntpServer);
        
        static void powerOff();
    private:
		static void _configModeCallback(WiFiManager *myWiFiManager);
};

#endif