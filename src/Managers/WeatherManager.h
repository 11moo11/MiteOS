#ifndef MITEOS_WEATHER_MANAGER_H
#define MITEOS_WEATHER_MANAGER_H

#include "TimeLib.h"
#include "../MiteOS.h"

typedef struct WeatherData {
	int8_t temperature;
	int16_t weatherConditionCode;
	bool isMetric;
	String weatherDescription;
	bool external;
	tmElements_t sunrise;
	tmElements_t sunset;
} WeatherData;

class WeatherManager {
	public:
		static WeatherData getWeatherData();
	private:
		static WeatherData _getWeatherData(String cityID, String lat, String lon, String units, String lang,
									String url, String apiKey,
									uint8_t updateInterval);
};

extern RTC_DATA_ATTR WeatherData currentWeatherData;
extern RTC_DATA_ATTR int weatherCheckCounter;

#endif