#include "SEG7Watchface.h"

#include "../../MiteOS.h"
#include "../../Fonts/DSEG7_Classic_Regular_15.h"
#include "../../Fonts/DSEG7_Classic_Bold_25.h"
#include "../../Fonts/DSEG7_Classic_Regular_39.h"
#include "DSEG7_Classic_Bold_53.h"
#include "../../Fonts/Seven_Segment10pt7b.h"
#include "../../Fonts/icons.h"
#include "../../Managers/WifiConnectionManager.h"
#include "../../Managers/WeatherManager.h"
#include "../../Images/weather_icons.h"

#define USE_SEGMENTS false
const uint8_t BATTERY_SEGMENT_COUNT = 5;
const uint8_t BATTERY_SEGMENT_SPACING = 2;
const uint8_t BATTERY_SEGMENTS_HEIGHT = 11;
const uint8_t BATTERY_SEGMENTS_WIDTH = 26; // Total available width

void SEG7Watchface::draw() {
	drawTime();
	drawDate();
	drawSteps();
	drawWeather();
	drawBattery();
	  
	mDisplay.drawBitmap(120, 77, WifiConfigured ? wifi : wifioff, 26, 18, FOREGROUND_COLOR);
	//if(BLE_CONFIGURED){
	//    mDisplay.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	//}
}

void SEG7Watchface::drawTime() {
	mDisplay.setFont(&DSEG7_Classic_Bold_53);
	mDisplay.setCursor(5, 53+5);
	int mDisplayHour;
	if(HOUR_12_24==12){
		mDisplayHour = ((MiteOS::currentTime.Hour+11)%12)+1;
	} else {
		mDisplayHour = MiteOS::currentTime.Hour;
	}
	if(mDisplayHour < 10){
		mDisplay.print("0");
	}
	mDisplay.print(mDisplayHour);
	mDisplay.print(":");
	if(MiteOS::currentTime.Minute < 10){
		mDisplay.print("0");
	}
	mDisplay.println(MiteOS::currentTime.Minute);
}

void SEG7Watchface::drawDate() {
	mDisplay.setFont(&Seven_Segment10pt7b);
	
	int16_t  x1, y1;
	uint16_t w, h;
	
	String dayOfWeek = Lang::dayStr(MiteOS::currentTime.Wday);
	mDisplay.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
	if(MiteOS::currentTime.Wday == 4){
		w = w - 5;
	}
	mDisplay.setCursor(85 - w, 85);
	mDisplay.println(dayOfWeek);
	
	String month = Lang::monthShortStr(MiteOS::currentTime.Month);
	mDisplay.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
	mDisplay.setCursor(85 - w, 110);
	mDisplay.println(month);
	
	mDisplay.setFont(&DSEG7_Classic_Bold_25);
	mDisplay.setCursor(5, 120);
	if(MiteOS::currentTime.Day < 10){
	  mDisplay.print("0");
	}
	mDisplay.println(MiteOS::currentTime.Day);
	mDisplay.setCursor(5, 150);
	mDisplay.println(tmYearToCalendar(MiteOS::currentTime.Year));// offset from 1970, since year is stored in uint8_t
}
void SEG7Watchface::drawSteps() {
	uint32_t stepCount = ActivityManager::getStepCount();
	mDisplay.drawBitmap(10, 165, steps, 19, 23, FOREGROUND_COLOR);
	mDisplay.setCursor(35, 190);
	mDisplay.println(stepCount);
}
void SEG7Watchface::drawBattery() {
	mDisplay.drawBitmap(154, 73, battery, 37, 21, FOREGROUND_COLOR);
	mDisplay.fillRect(159, 78, 28, BATTERY_SEGMENTS_HEIGHT, BACKGROUND_COLOR);//clear battery segments
	
	float bat_percentage = PowerManager::getBatteryPercentage();
	
	if(USE_SEGMENTS) {
		if(bat_percentage > 98) {
			mDisplay.fillRect(159, 78, BATTERY_SEGMENTS_WIDTH, BATTERY_SEGMENTS_HEIGHT, FOREGROUND_COLOR);
		}else{
			uint8_t segments = bat_percentage / (100 / BATTERY_SEGMENT_COUNT);
			if(segments < BATTERY_SEGMENT_COUNT && ((int) bat_percentage) % (100 / BATTERY_SEGMENT_COUNT) > (100 / BATTERY_SEGMENT_COUNT) / 2) segments++;
			
			uint8_t width = (BATTERY_SEGMENTS_WIDTH - (BATTERY_SEGMENT_SPACING * (BATTERY_SEGMENT_COUNT - 1))) / BATTERY_SEGMENT_COUNT;
			
			for(uint8_t i = 0; i < segments; i++){
				mDisplay.fillRect(159 + (i * (width + BATTERY_SEGMENT_SPACING)), 78, width, BATTERY_SEGMENTS_HEIGHT, FOREGROUND_COLOR);
			}
		}
	}else{
		mDisplay.fillRect(159, 78, BATTERY_SEGMENTS_WIDTH * (bat_percentage / 100), BATTERY_SEGMENTS_HEIGHT, FOREGROUND_COLOR);
		mDisplay.drawRect(159, 78, BATTERY_SEGMENTS_WIDTH, BATTERY_SEGMENTS_HEIGHT, FOREGROUND_COLOR);
	}
	/*
	for(float i = 0; i <= 100; i++) {
		Serial.print((uint8_t) i);
		Serial.print(" - ");
		Serial.println((int16_t) (BATTERY_SEGMENTS_WIDTH * (i / 100)));
	}
	*/
}

void SEG7Watchface::drawWeather(){
	WeatherData currentWeather = WeatherManager::getWeatherData();
	
	int8_t temperature = WeatherManager::isDataCurrent() ? currentWeather.temperature : currentWeather.chip_temperature;
	int16_t weatherConditionCode = currentWeather.weatherConditionCode;
	
	mDisplay.setFont(&DSEG7_Classic_Regular_39);
	int16_t  x1, y1;
	uint16_t w, h;
	mDisplay.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
	if(159 - w - x1 > 87){
		mDisplay.setCursor(159 - w - x1, 150);
	}else{
		mDisplay.setFont(&DSEG7_Classic_Bold_25);
		mDisplay.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
		mDisplay.setCursor(159 - w - x1, 136);
	}
	mDisplay.println(temperature);
	mDisplay.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	const unsigned char* weatherIcon;
	
	if(WeatherManager::isDataCurrent()){
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
	}else{
		weatherIcon = chip;
	}
	
	mDisplay.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, FOREGROUND_COLOR);
}
