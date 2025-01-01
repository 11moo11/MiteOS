#ifndef MITEOS_WEATHER_MANAGER_H
#define MITEOS_WEATHER_MANAGER_H

#include "TimeLib.h"
#include "../MiteOS.h"

typedef struct WeatherData {
	int8_t temperature;
	int16_t weatherConditionCode;
	bool isMetric;
	char weatherDescription[30];
	
	int8_t chip_temperature;

	tmElements_t sunrise;
	tmElements_t sunset;
	ulong timestamp;
} WeatherData;

class WeatherManager {
	public:
		static WeatherData getWeatherData(bool cached = true);
		static void timeTick();

		static bool isDataCurrent();
		
		static float getMoonPhase();
		static float getMoonPhase(uint8_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t minute = 0);
	private:
		static WeatherData _getWeatherData(String cityID, String lat, String lon, String units, String lang,
									String url, String apiKey,
									uint8_t updateInterval);
		static double _Julian(int32_t year, int32_t month, const double &day);
};

extern RTC_DATA_ATTR WeatherData currentWeatherData;
extern RTC_DATA_ATTR uint8_t weatherCheckCounter;

#define MOON_SYNODIC_PERIOD   29.530588853   // Period of moon cycle in days.
#define	MOON_SYNODIC_OFFSET   2451550.26     // Reference cycle offset in days.

#endif