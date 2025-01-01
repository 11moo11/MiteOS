//Weather Settings
#define CITY_ID "2948825" //New York City https://openweathermap.org/current#cityid

//You can also use LAT,LON for your location instead of CITY_ID, but not both
//#define LAT "40.7127" //New York City, Looked up on https://www.latlong.net/
//#define LON "-74.0059"

#ifdef CITY_ID
	#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/weather?id={cityID}&lang={lang}&units={units}&appid={apiKey}" //open weather api using city ID
#else
	#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/weather?lat={lat}&lon={lon}&lang={lang}&units={units}&appid={apiKey}" //open weather api using lat lon
#endif

#define OPENWEATHERMAP_APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb" //use your own API key :)
#define TEMP_UNIT "metric" //metric = Celsius , imperial = Fahrenheit
#define WEATHER_UPDATE_INTERVAL 30 //must be greater than 5, smaller than 255, measured in minutes

//NTP Settings
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * 0 //New York is UTC -5 EST, -4 EDT, will be overwritten by weather data

// Darkmode Settings // TODO Put as in System Settings
#define INVERSE_DARKMODE false // Defines whether the timespan will make it darkmode or lightmode
#define DARKMODE_START_H 23
#define DARKMODE_START_M 0
#define DARKMODE_END_H 6
#define DARKMODE_END_M 30

// Possible Values LANG_EN, LANG_DE, LANG_FR
#define LANG_EN


#define STEPS_PER_KM 1400
#define KM_TO_MILES 0.621371

// This will dramatically reduce battery life and responsiveness of the watch
// Should only be used for making screenshot and never for daily driving
#define ENABLE_SCREENSHOTS true
#define DEBUG true




#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <Preferences.h>
#include "../MiteOS.h"
#include "../lang/lang.h"

#include "../Managers/PhoneConnectionManager.h"

class Configuration : Preferences {
    public:
        static Preferences preferences;
    public:
        static void init();
        
        static void loadAll();
        static void saveAll();
        
        static void saveSettings();
        static void loadSettings();
        
        static void saveAlarms();
        static void loadAlarms();
        
        static void saveBluetooth();
        static void loadBluetooth();
        
        static void saveHassConfig();
        static void loadHassConfig();
        
        static void saveNotification(uint8_t index, Notification n);
        static Notification loadNotification(uint8_t index);
        
        static void saveSteps();
        static std::array<uint32_t, 7> loadSteps(bool useCurrentData = true);
        static void saveTotalSteps();
        static uint32_t loadTotalSteps();
        
        // Settings functions - TODO: Replace with App Settings
        #ifdef CITY_ID
        static String getCityID() { return CITY_ID; };
        static String getLat() { return ""; };
        static String getLon() { return ""; };
        #else
        static String getCityID() { return ""; };
        static String getLat() { return LAT; };
        static String getLon() { return LON; };
        #endif
        static String getWeatherAPIKey() { return OPENWEATHERMAP_APIKEY; };
        static String getWeatherURL() { return OPENWEATHERMAP_URL; };
        static String getWeatherUnit() { return TEMP_UNIT; };
        static String getWeatherLang() { return TEMP_LANG; };
        static uint8_t getWeatherUpdateInterval() { return WEATHER_UPDATE_INTERVAL; };
        
        static String getNtpServer() { return NTP_SERVER; };
        static int getGmtOffset() { return GMT_OFFSET_SEC; };

        static bool getInverseDarkMode() { return INVERSE_DARKMODE; };
        static uint8_t getDarkmodeStartH() { return DARKMODE_START_H; };
        static uint8_t getDarkmodeStartM() { return DARKMODE_START_M; };
        static uint8_t getDarkmodeEndH() { return DARKMODE_END_H; };
        static uint8_t getDarkmodeEndM() { return DARKMODE_END_M; };


        static int getSize();
        static int usedSpace();
        static int freeSpace();
    private:
        static bool initialized;
};
#endif