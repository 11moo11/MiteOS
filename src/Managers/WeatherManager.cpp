#include "WeatherManager.h"

#include "../MiteOS.h"
#include "WifiConnectionManager.h"
#include <HTTPClient.h>
#include <JSON.h>

RTC_DATA_ATTR WeatherData currentWeatherData;

RTC_DATA_ATTR uint8_t weatherCheckCounter = 250;

void WeatherManager::timeTick() {
	weatherCheckCounter += 1;
}

WeatherData WeatherManager::getWeatherData(bool cached) {
	if(cached) return currentWeatherData;
	return _getWeatherData( Configuration::getCityID()
						  , Configuration::getLat()
						  , Configuration::getLon()
						  , Configuration::getWeatherUnit()
						  , Configuration::getWeatherLang()
						  , Configuration::getWeatherURL()
						  , Configuration::getWeatherAPIKey()
						  , Configuration::getWeatherUpdateInterval()
						  );
}

WeatherData WeatherManager::_getWeatherData(String cityID, String lat, String lon, String units, String lang,
											String url, String apiKey,
											uint8_t updateInterval) {
	
	currentWeatherData.isMetric = units == String("metric");
	
	if (weatherCheckCounter >= updateInterval) { // only update if WEATHER_UPDATE_INTERVAL has elapsed
												 // i.e. 30 minutes
		if (WifiConnectionManager::connectWifi()) {
			HTTPClient http; // Use Weather API for live data if WiFi is connected
			http.setConnectTimeout(3000); // 3 second max timeout
			String weatherQueryURL = url;
			
			if(cityID != ""){
				weatherQueryURL.replace("{cityID}", cityID);
			}else{
				weatherQueryURL.replace("{lat}", lat);
				weatherQueryURL.replace("{lon}", lon);
			}
			
			weatherQueryURL.replace("{units}", units);
			weatherQueryURL.replace("{lang}", lang);
			weatherQueryURL.replace("{apiKey}", apiKey);
			
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
				
				currentWeatherData.timestamp = NOW;

				// sync NTP during weather API call and use timezone of lat & lon
				WifiConnectionManager::syncNTP(gmtTimeOffset);
			} else {
				// http error
			}
			http.end();
			
			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		}
		
		uint8_t chip_temperature = accSensor.readTemperature(); // celsius
		if (!currentWeatherData.isMetric) {
			chip_temperature = chip_temperature * 9. / 5. + 32.; // fahrenheit
		}
		currentWeatherData.chip_temperature = chip_temperature;
		//currentWeatherData.weatherConditionCode = 800;
		//currentWeatherData.external             = false;
		//String(TXT_CHIP).toCharArray(currentWeatherData.weatherDescription, 20);
		
		WifiConnectionManager::powerOff();
		weatherCheckCounter = 0;
	} else {
		// weatherCheckCounter++;
	}
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