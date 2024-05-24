#ifndef SETTINGS_H
#define SETTINGS_H

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
#define TEMP_LANG "en"
#define WEATHER_UPDATE_INTERVAL 30 //must be greater than 5, measured in minutes

//NTP Settings
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * 0 //New York is UTC -5 EST, -4 EDT, will be overwritten by weather data

// Darkmode Settings
#define ENABLE_AUTO_DARKMODE false // Makes the clock automatically switch between dark and lightmode depending on the time
#define INVERSE_DARKMODE false // Defines whether the timespan will make it darkmode or lightmode
#define DARKMODE_START_H 23
#define DARKMODE_START_M 0
#define DARKMODE_END_H 6
#define DARKMODE_END_M 30

MiteSettings settings {
	#ifdef CITY_ID
		.cityID = CITY_ID,
	#else
		.cityID = "",
		.lat = LAT,
		.lon = LON,
	#endif
	.weatherAPIKey = OPENWEATHERMAP_APIKEY,
	.weatherURL = OPENWEATHERMAP_URL,
	.weatherUnit = TEMP_UNIT,
	.weatherLang = TEMP_LANG,
	.weatherUpdateInterval = WEATHER_UPDATE_INTERVAL,
	.ntpServer = NTP_SERVER,
	.gmtOffset = GMT_OFFSET_SEC,
	//
	.vibrateOClock = true,
	//
	.enableAutoDarkMode = ENABLE_AUTO_DARKMODE,
	.inverseDarkMode = INVERSE_DARKMODE,
	.darkmodeStartH = DARKMODE_START_H,
	.darkmodeStartM = DARKMODE_START_M,
	.darkmodeEndH = DARKMODE_END_H,
	.darkmodeEndM = DARKMODE_END_M
};

#endif