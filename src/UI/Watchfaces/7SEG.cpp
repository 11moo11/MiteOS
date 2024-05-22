#include "7SEG.h"

#include "../../MiteOS.h"
#include "../../Fonts/DSEG7_Classic_Regular_15.h"
#include "../../Fonts/DSEG7_Classic_Bold_25.h"
#include "../../Fonts/DSEG7_Classic_Regular_39.h"
#include "DSEG7_Classic_Bold_53.h"
#include "../../Fonts/Seven_Segment10pt7b.h"
#include "../../Fonts/icons.h"

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

void SEG7::draw() {
    drawTime();
    drawDate();
    drawSteps();
    drawWeather();
    drawBattery();
      
    //MiteOS::display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    //if(BLE_CONFIGURED){
    //    MiteOS::display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    //}
}

void SEG7::drawTime() {
    MiteOS::display.setFont(&DSEG7_Classic_Bold_53);
    MiteOS::display.setCursor(5, 53+5);
    int displayHour;
    if(HOUR_12_24==12){
      displayHour = ((MiteOS::currentTime.Hour+11)%12)+1;
    } else {
      displayHour = MiteOS::currentTime.Hour;
    }
    if(displayHour < 10){
        MiteOS::display.print("0");
    }
    MiteOS::display.print(displayHour);
    MiteOS::display.print(":");
    if(MiteOS::currentTime.Minute < 10){
        MiteOS::display.print("0");
    }
    MiteOS::display.println(MiteOS::currentTime.Minute);
}

void SEG7::drawDate(){
    MiteOS::display.setFont(&Seven_Segment10pt7b);

    int16_t  x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayStr(MiteOS::currentTime.Wday);
    MiteOS::display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
    if(MiteOS::currentTime.Wday == 4){
        w = w - 5;
    }
    MiteOS::display.setCursor(85 - w, 85);
    MiteOS::display.println(dayOfWeek);

    String month = monthShortStr(MiteOS::currentTime.Month);
    MiteOS::display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    MiteOS::display.setCursor(85 - w, 110);
    MiteOS::display.println(month);

    MiteOS::display.setFont(&DSEG7_Classic_Bold_25);
    MiteOS::display.setCursor(5, 120);
    if(MiteOS::currentTime.Day < 10){
    	MiteOS::display.print("0");
    }
    MiteOS::display.println(MiteOS::currentTime.Day);
    MiteOS::display.setCursor(5, 150);
    MiteOS::display.println(tmYearToCalendar(MiteOS::currentTime.Year));// offset from 1970, since year is stored in uint8_t
}
void SEG7::drawSteps(){
    // reset step counter at midnight
    if (MiteOS::currentTime.Hour == 0 && MiteOS::currentTime.Minute == 0){
      accSensor.resetStepCounter();
    }
    uint32_t stepCount = accSensor.getCounter();
    MiteOS::display.drawBitmap(10, 165, steps, 19, 23, FOREGROUND_COLOR);
    MiteOS::display.setCursor(35, 190);
    MiteOS::display.println(stepCount);
}
void SEG7::drawBattery(){
    MiteOS::display.drawBitmap(154, 73, battery, 37, 21, FOREGROUND_COLOR);
    MiteOS::display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, BACKGROUND_COLOR);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = MiteOS::getBatteryVoltage();
    if(VBAT > 4.1){
        batteryLevel = 3;
    }
    else if(VBAT > 3.95 && VBAT <= 4.1){
        batteryLevel = 2;
    }
    else if(VBAT > 3.80 && VBAT <= 3.95){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.80){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        MiteOS::display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, FOREGROUND_COLOR);
    }
}

void SEG7::drawWeather(){
	/*
    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    MiteOS::display.setFont(&DSEG7_Classic_Regular_39);
    int16_t  x1, y1;
    uint16_t w, h;
    MiteOS::display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
    if(159 - w - x1 > 87){
        MiteOS::display.setCursor(159 - w - x1, 150);
    }else{
        MiteOS::display.setFont(&DSEG7_Classic_Bold_25);
        MiteOS::display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
        MiteOS::display.setCursor(159 - w - x1, 136);
    }
    MiteOS::display.println(temperature);
    MiteOS::display.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    const unsigned char* weatherIcon;

    if(WIFI_CONFIGURED){
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
      }else 
      return;
    }else{
      weatherIcon = chip;
    }
    
    MiteOS::display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
		*/
}
