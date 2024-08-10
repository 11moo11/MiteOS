#include "WeatherPage.h"

#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/DSEG7_Classic_Regular_15.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Managers/WeatherManager.h"
#include "../Images/moon_icons.h"
#include "../Images/menu_icons.h"
#include "../Images/weather_icons.h"

void WeatherPage::drawPage() {
	//mDisplay.fillScreen(GxEPD_BLACK);
	
	drawWeather();
	drawMoonPhase();
}

void WeatherPage::drawWeather() {
	WeatherData currentWeather = WeatherManager::getWeatherData();

	if(currentWeather.weatherConditionCode <= 100) return;
	
	int8_t temperature = currentWeather.temperature;
	int16_t weatherConditionCode = currentWeather.weatherConditionCode;

	mDisplay.setFont(&DSEG7_Classic_Regular_39);
	
	mDisplay.setCursor(10, 60);
	mDisplay.println(String(temperature).c_str());
	
	mDisplay.drawBitmap(10 + String(temperature).length() * 35, 15, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, FOREGROUND_COLOR);
	
	const unsigned char* weatherIcon;
	
	mDisplay.setFont(&Seven_Segment10pt7b);
	drawCentreString(currentWeatherData.weatherDescription, 100, 90);
	
	if(currentWeather.external) {
		if(weatherConditionCode > 0) {
			//https://openweathermap.org/weather-conditions
			if(weatherConditionCode > 801){//Cloudy
				weatherIcon = cloudy;
			}else if(weatherConditionCode == 801){//Few Clouds
				weatherIcon = cloudsun;
			}else if(weatherConditionCode == 800){//Clear
				weatherIcon = sunny;
			}else if(weatherConditionCode >=700){//Atmosphere
				weatherIcon = atmosphere;
			}else if(weatherConditionCode >=600){//Snow
				weatherIcon = snow;
			}else if(weatherConditionCode >=500){//Rain
				weatherIcon = rain;
			}else if(weatherConditionCode >=300){//Drizzle
				weatherIcon = drizzle;
			}else if(weatherConditionCode >=200){//Thunderstorm
				weatherIcon = thunderstorm;
			}else{
				return;
			}
		}
	}else{
		weatherIcon = chip;
	}
	
	mDisplay.drawBitmap(130, 20, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, FOREGROUND_COLOR);
}

void WeatherPage::drawMoonPhase() {
	mDisplay.fillRect(0, 120, 200, 80, GxEPD_BLACK);
	
	float phase = WeatherManager::getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day, MiteOS::currentTime.Hour);
	
	mDisplay.drawBitmap(90, 183, icon_up, 20, 20, GxEPD_WHITE);
	
	for(int i = -1; i <= 1; i++) {
		int displayPhase = round(phase * 7) + i;
		
		const unsigned char* moonIcon;
		switch(displayPhase) {
			case 7:
				moonIcon = icon_moon_new;
				break;
			case 6:
				moonIcon = icon_moon_waning3;
				break;
			case 5:
				moonIcon = icon_moon_waning2;
				break;
			case 4:
				moonIcon = icon_moon_waning1;
				break;
			case 3:
				moonIcon = icon_moon_full;
				break;
			case 2:
				moonIcon = icon_moon_crescent1;
				break;
			case 1:
				moonIcon = icon_moon_crescent2;
				break;
			case 0:
			default:
				moonIcon = icon_moon_crescent3;
				break;
		}
		mDisplay.drawBitmap(80 - (-50 * i), 145, moonIcon, 40, 40, GxEPD_WHITE);
	}
	
	int fraction = ((1.0 - cos(2 * M_PI * phase)) * 0.5) * 100;
	
	mDisplay.setTextColor(GxEPD_WHITE);
	
	mDisplay.setFont(&DSEG7_Classic_Regular_15);
	String str = String(fraction);
	mDisplay.setCursor(80, 142);
	mDisplay.print(str.c_str());
	
	mDisplay.setFont(&Seven_Segment10pt7b);
	mDisplay.print("%");
}