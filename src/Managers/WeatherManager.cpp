#include "WeatherManager.h"

#include "NetworkManager.h"
#include <HTTPClient.h>
#include <JSON.h>

RTC_DATA_ATTR WeatherData currentWeatherData;

RTC_DATA_ATTR int weatherCheckCounter = -1;

void WeatherManager::timeTick() {
	weatherCheckCounter += 1;
}

WeatherData WeatherManager::getWeatherData() {
	return _getWeatherData( MiteOS::instance->settings.cityID
						  , MiteOS::instance->settings.lat
						  , MiteOS::instance->settings.lon
						  , MiteOS::instance->settings.weatherUnit
						  , MiteOS::instance->settings.weatherLang
						  , MiteOS::instance->settings.weatherURL
						  , MiteOS::instance->settings.weatherAPIKey
						  , MiteOS::instance->settings.weatherUpdateInterval
						  );
}

WeatherData WeatherManager::_getWeatherData(String cityID, String lat, String lon, String units, String lang,
											String url, String apiKey,
											uint8_t updateInterval) {
	
	currentWeatherData.isMetric = units == String("metric");
	
	if (weatherCheckCounter < 0) { //-1 on first run, set to updateInterval
		weatherCheckCounter = updateInterval;
	}
	
	if (weatherCheckCounter >= updateInterval) { // only update if WEATHER_UPDATE_INTERVAL has elapsed
												 // i.e. 30 minutes
		if (NetworkManager::connectWifi()) {
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
				currentWeatherData.weatherDescription = JSONVar::stringify(responseObject["weather"][0]["main"]);
				currentWeatherData.external = true;
				
				breakTime((time_t)(int)responseObject["sys"]["sunrise"], currentWeatherData.sunrise);
				breakTime((time_t)(int)responseObject["sys"]["sunset"], currentWeatherData.sunset);
				
				// sync NTP during weather API call and use timezone of lat & lon
				long gmtOffset = int(responseObject["timezone"]);
				NetworkManager::syncNTP(gmtOffset);
			} else {
				// http error
			}
			http.end();
			
			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		} else { // No WiFi, use internal temperature sensor
			uint8_t temperature = accSensor.readTemperature(); // celsius
			if (!currentWeatherData.isMetric) {
				temperature = temperature * 9. / 5. + 32.; // fahrenheit
			}
			currentWeatherData.temperature          = temperature;
			currentWeatherData.weatherConditionCode = 800;
			currentWeatherData.external             = false;
		}
		weatherCheckCounter = 0;
	} else {
		// weatherCheckCounter++;
	}
	return currentWeatherData;
}