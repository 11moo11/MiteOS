#ifndef MITEOS_WEATHER_MANAGER_H
#define MITEOS_WEATHER_MANAGER_H

#include "TimeLib.h"
#include "../MiteOS.h"

typedef struct WeatherData {
	int8_t temperature;
	int16_t weatherConditionCode;
	bool isMetric;
	char weatherDescription[30];
	char cityName[30];
	
	int8_t chip_temperature;

	tmElements_t sunrise;
	tmElements_t sunset;
	ulong timestamp;
} WeatherData;

typedef struct WeatherDataDay {
	int8_t temperatureMin;
	int8_t temperatureMax;
	int16_t weatherConditionCode;
	bool isMetric;

	tmElements_t sunrise;
	tmElements_t sunset;
	//ulong timestamp;
} WeatherDataDay;

class WeatherManager {
	private:
		static String city;
		static String lat;
		static String lon;
		static String unit;
		static String token;
	
	public:
		static void setCity(String city)   { WeatherManager::city  = city;  }
		static void setUnit(String unit)   { WeatherManager::unit  = unit;  }
		static void setToken(String token) { WeatherManager::token = token; }
		static void setLat(String lat)     { WeatherManager::lat   = lat;   }
		static void setLon(String lon)     { WeatherManager::lon   = lon;   }
		static String getCity()  { return WeatherManager::city;  }
		static String getLat()   { return WeatherManager::lat;   }
		static String getLon()   { return WeatherManager::lon;   }
		static String getUnit()  { return WeatherManager::unit;  }
		static String getToken() { return WeatherManager::token; }
		
		static WeatherData getWeatherData(bool cached = true);
		
		static void loadOpenMeteoData(String units, String lat, String lon);
		static WeatherDataDay getWeatherDataDay(String day);
		static WeatherData getWeatherDataTime(String day, String time);

		static bool isDataCurrent();
		static bool parseISOToTm(const char* str, tmElements_t &tm);
		
		static float getMoonPhase();
		static float getMoonPhase(uint8_t year, uint8_t month, uint8_t day, uint8_t hour = 0, uint8_t minute = 0);

		static String getWeatherDescription(int code);
		static const unsigned char* getWeatherIcon(int code);
		static const unsigned char* getSmallWeatherIcon(int code);
	private:
		static WeatherData _getWeatherData(String cityID, String units, String lang, String url, String apiKey);
		static double _Julian(int32_t year, int32_t month, const double &day);
};

extern RTC_DATA_ATTR WeatherData currentWeatherData;

#define MOON_SYNODIC_PERIOD   29.530588853   // Period of moon cycle in days.
#define	MOON_SYNODIC_OFFSET   2451550.26     // Reference cycle offset in days.

#endif