#include "WeatherManager.h"

#include "../MiteOS.h"
#include "WifiConnectionManager.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "../Managers/FileManager.h"

RTC_DATA_ATTR long lastWeatherCheck = 0;
RTC_DATA_ATTR long lastWeatherDownload = 0;

RTC_DATA_ATTR WeatherData currentWeatherData;

String WeatherManager::city;
String WeatherManager::unit;
String WeatherManager::token;
String WeatherManager::lat;
String WeatherManager::lon;

bool WeatherManager::isDataCurrent() {
	return currentWeatherData.timestamp > 0 && day(NOW - currentWeatherData.timestamp) <= WEATHER_DATA_MAX_AGE;
}



WeatherData WeatherManager::getWeatherData(bool cached) {
	if(!cached) {
		lastWeatherCheck = 0;
		lastWeatherDownload = 0;
	}

	// Check if weather data is current enough to be cached or just force an update
	if( WeatherManager::isDataCurrent()
	&& (NOW - lastWeatherCheck) / 60 <= WEATHER_UPDATE_INTERVAL
	)
		return currentWeatherData;

	Configuration::loadOwmConfig();


	loadOpenMeteoData(WeatherManager::getUnit(), WeatherManager::getLat(), WeatherManager::getLon());

	
	// TODO: Dynamic Check
	char buffer[11];
	snprintf(buffer, 11, "%04d-%02d-%02d", tmYearToCalendar(MiteOS::currentTime.Year), MiteOS::currentTime.Month, MiteOS::currentTime.Day);
	String day = String(buffer);

	snprintf(buffer, 6, "%02d:%02d", MiteOS::currentTime.Hour, 0);
	String time = String(buffer);


	WeatherDataDay dayData = getWeatherDataDay(day);
	WeatherData timeData = getWeatherDataTime(day, time);

	currentWeatherData.temperature = timeData.temperature;
	currentWeatherData.weatherConditionCode = timeData.weatherConditionCode;
	currentWeatherData.isMetric = timeData.isMetric;
	String("").toCharArray(currentWeatherData.weatherDescription, 30);

	currentWeatherData.sunrise = dayData.sunrise;
	currentWeatherData.sunset = dayData.sunset;
	currentWeatherData.timestamp = NOW;
	lastWeatherCheck = NOW;
	return currentWeatherData;
	/*return _getWeatherData( WeatherManager::getCity()
						  , WeatherManager::getUnit()
						  , Configuration::getWeatherLang()
						  , Configuration::getWeatherURL()
						  , WeatherManager::getToken()
						  );
	*/
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

				JsonDocument json;
  				deserializeJson(json, payload);
				currentWeatherData.temperature = int(json["main"]["temp"]);
				currentWeatherData.weatherConditionCode = int(json["weather"][0]["id"]);
				String desc = json["weather"][0]["description"];
				desc.toCharArray(currentWeatherData.weatherDescription, 30);
				//currentWeatherData.external = true;
				
				gmtTimeOffset = int(json["timezone"]);

				breakTime((time_t)(int)json["sys"]["sunrise"] + gmtTimeOffset, currentWeatherData.sunrise);
				breakTime((time_t)(int)json["sys"]["sunset"] + gmtTimeOffset, currentWeatherData.sunset);
				
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

void WeatherManager::loadOpenMeteoData(String units, String lat, String lon) {

	String unit = units == String("metric") ? "celsius" : "fahrenheit";
	currentWeatherData.isMetric = units == String("metric");

	if((NOW - lastWeatherDownload) / 60 > WEATHER_DOWNLOAD_INTERVAL && lat.length() > 0 && lon.length() > 0) {
		lastWeatherDownload = NOW;

		if (WifiConnectionManager::connectWifi()) {
			HTTPClient http; // Use Weather API for live data if WiFi is connected
			http.setConnectTimeout(3000); // 3 second max timeout
			String weatherQueryURL = OPENMETEO_URL;
			
			weatherQueryURL.replace("{lat}", lat);
			weatherQueryURL.replace("{long}", lon);
			weatherQueryURL.replace("{units}", unit);
			
			printDebug(weatherQueryURL);

			http.begin(weatherQueryURL.c_str());


			int httpResponseCode = http.GET();
			if (httpResponseCode == 200) {
				String payload = http.getString();

				//JsonDocument json;
  				//deserializeJson(json, payload);

				FileManager::writeFile(PATH_WEATHER"data", payload);

				currentWeatherData.timestamp = NOW;
			} else {
				// http error
			}
			http.end();
			
			// turn off radios
			btStop();
		}
		
		WifiConnectionManager::powerOff();
	}
}
bool WeatherManager::parseISOToTm(const char* str, tmElements_t &tm) {
  int year, month, day, hour, minute;
  
  // sscanf gibt die Anzahl der erfolgreich gematchten Werte zurück
  // 't' im Format-String fängt das Trennzeichen ab
  if (sscanf(str, "%d-%d-%dT%d:%d", &year, &month, &day, &hour, &minute) == 5) {
    tm.Year = CalendarYrToTm(year); // Konvertiert z.B. 2026 in den Offset seit 1970
    tm.Month = month;
    tm.Day = day;
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = 0; // Nicht im String enthalten, daher auf 0 setzen
    return true;
  }
  return false;
}

WeatherDataDay WeatherManager::getWeatherDataDay(String day) {
	WeatherDataDay weatherData;

	if(FileManager::exists(PATH_WEATHER"data")) {
		String file = FileManager::readFile(PATH_WEATHER"data");
		
		JsonDocument json;
		deserializeJson(json, file);

		if(json.containsKey("daily")) {
			if(json["daily"].containsKey("time")) {
				JsonArray arr = json["daily"]["time"].as<JsonArray>();
				int index = -1;
				for(int i = 0;i < arr.size(); i++) {
					if(day.equalsIgnoreCase(arr[i].as<String>())) {
						index = i;
						break;
					}
				}
				if(index >= 0) {
					WeatherManager::parseISOToTm(json["daily"]["sunrise"][index], weatherData.sunrise);
					WeatherManager::parseISOToTm(json["daily"]["sunset"][index], weatherData.sunset);
					weatherData.weatherConditionCode = json["daily"]["weather_code"][index];
					weatherData.temperatureMin = json["daily"]["temperature_2m_min"][index];
					weatherData.temperatureMax = json["daily"]["temperature_2m_max"][index];
				}else{
					printDebug(day + " not found in weather data");
				}
			}
		}
	}
	return weatherData;
}
WeatherData WeatherManager::getWeatherDataTime(String day, String time) {
	WeatherData weatherData;
	if(FileManager::exists(PATH_WEATHER"data")) {
		String file = FileManager::readFile(PATH_WEATHER"data");
		
		JsonDocument json;
		deserializeJson(json, file);

		String key = day + "T" + time;

		if(json.containsKey("hourly")) {
			if(json["hourly"].containsKey("time")) {
				JsonArray arr = json["hourly"]["time"].as<JsonArray>();
				int index = -1;
				for(int i = 0;i < arr.size(); i++) {
					if(key.equalsIgnoreCase(arr[i].as<String>())) {
						index = i;
						break;
					}
				}
				if(index >= 0) {
					weatherData.weatherConditionCode = json["hourly"]["weather_code"][index];
					weatherData.temperature = json["hourly"]["temperature_2m"][index];
					weatherData.isMetric = String("°C").equalsIgnoreCase(json["hourly_units"]["temperature_2m"]);
				}else{
					printDebug(key + " not found in weather data");
				}
			}
		}
	}
	return weatherData;
}