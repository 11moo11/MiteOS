#include "WeatherManager.h"

#include "../MiteOS.h"
#include "WifiConnectionManager.h"
#include <HTTPClient.h>
#include <JSON.h>

RTC_DATA_ATTR WeatherData currentWeatherData;

RTC_DATA_ATTR long lastWeatherCheck = 0;

String WeatherManager::city;
String WeatherManager::unit;
String WeatherManager::token;

bool WeatherManager::isDataCurrent() {
	return currentWeatherData.timestamp > 0 && hour(NOW - currentWeatherData.timestamp) <= WEATHER_DATA_MAX_AGE;
}



WeatherData WeatherManager::getWeatherData(bool cached) {
	if(!cached) lastWeatherCheck = 0;

	// Check if weather data is current enough to be cached or just force an update
	if( WeatherManager::isDataCurrent()
	&& (NOW - lastWeatherCheck) / 60 <= WEATHER_UPDATE_INTERVAL
	)
		return currentWeatherData;

	Configuration::loadOwmConfig();

	return _getWeatherData( WeatherManager::getCity()
						  , WeatherManager::getUnit()
						  , Configuration::getWeatherLang()
						  , Configuration::getWeatherURL()
						  , WeatherManager::getToken()
						  );
}

WeatherData WeatherManager::_getWeatherData(String cityID, String units, String lang, String url, String apiKey) {
	
	currentWeatherData.isMetric = units == String("metric");

	if((NOW - lastWeatherCheck) / 60 > WEATHER_UPDATE_INTERVAL && apiKey.length() > 0 && cityID.length() > 0 && units.length() > 0) {
		lastWeatherCheck = NOW;

		if (WifiConnectionManager::connectWifi()) {
			HTTPClient http; // Use Weather API for live data if WiFi is connected
			http.setConnectTimeout(3000); // 3 second max timeout
			String weatherQueryURL = url;
			
			weatherQueryURL.replace("{cityID}", cityID);
			weatherQueryURL.replace("{units}", units);
			weatherQueryURL.replace("{lang}", lang);
			weatherQueryURL.replace("{apiKey}", apiKey);
			
			printDebug(weatherQueryURL);

			http.begin(weatherQueryURL.c_str());
			
			int httpResponseCode = http.GET();
			if (httpResponseCode == 200) {
				String payload             = http.getString();
				
				JSONVar responseObject     = JSON.parse(payload);
				currentWeatherData.temperature = int(responseObject["main"]["temp"]);
				currentWeatherData.weatherConditionCode = int(responseObject["weather"][0]["id"]);
				String desc = JSONVar::stringify(responseObject["weather"][0]["description"]);
				desc.substring(1, desc.length() - 1).toCharArray(currentWeatherData.weatherDescription, 30);
				//currentWeatherData.external = true;
				
				gmtTimeOffset = int(responseObject["timezone"]);

				breakTime((time_t)(int)responseObject["sys"]["sunrise"] + gmtTimeOffset, currentWeatherData.sunrise);
				breakTime((time_t)(int)responseObject["sys"]["sunset"] + gmtTimeOffset, currentWeatherData.sunset);
				
				// sync NTP during weather API call and use timezone of lat & lon
				WifiConnectionManager::syncNTP(gmtTimeOffset);

				currentWeatherData.timestamp = NOW;
			} else {
				// http error
			}
			http.end();
			
			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		}
	
		WifiConnectionManager::powerOff();
	}

	uint8_t chip_temperature = accSensor.readTemperature(); // celsius
	if (!currentWeatherData.isMetric) {
		chip_temperature = chip_temperature * 9. / 5. + 32.; // fahrenheit
	}
	currentWeatherData.chip_temperature = chip_temperature;

	return currentWeatherData;
}

double WeatherManager::_Julian(int32_t year, int32_t month, const double &day) {
	int32_t b, c, e;
	b = 0;
	if (month < 3) {
		year--;
		month += 12;
	}
	
	if (year > 1582 || (year == 1582 && month > 10) ||
		(year == 1582 && month == 10 && day > 15)) {
		int32_t a;
		a = year / 100;
		b = 2 - a + a / 4;
	}
	c = 365.25 * year;
	e = 30.6001 * (month + 1);

	return b + c + e + day + 1720994.5;
}

float WeatherManager::getMoonPhase() {
	return getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day, MiteOS::currentTime.Hour, MiteOS::currentTime.Minute);
}


// Phase from 0 - 0.5 = Increasing Moon
// Phase from 0.5 - 1 = Decreasing Moon
float WeatherManager::getMoonPhase(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute) {
	double j = _Julian(((int32_t) year) + 1970, month, (double) day + (hour / 24.0) + (minute / 1440.0));
	
	// Calculate illumination (synodic) phase.
	// From number of days since new moon on Julian date MOON_SYNODIC_OFFSET
	// (1815UTC January 6, 2000), determine remainder of incomplete cycle.
	float phase = (j - MOON_SYNODIC_OFFSET) / MOON_SYNODIC_PERIOD;
	phase -= floor(phase);
	
	return phase;
}