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

void drawPartialBitmap(int16_t x, int16_t y, const uint8_t* bitmap, int16_t w, int16_t h, int16_t start_w, int16_t end_w, uint16_t color) {
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t b = 0;
	
	if(start_w < 0) start_w = 0;
	if(end_w > w) end_w = w;
	
	mDisplay.startWrite();
	for (int16_t j = 0; j < h; j++, y++) {
		for (int16_t i = 0; i < end_w; i++) {
			if (i & 7)
				b <<= 1;
			else
				b = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
			
			if(i < start_w) continue;
			
			if (b & 0x80)
				mDisplay.writePixel(x + i, y, color);
		}
	}
	mDisplay.endWrite();
}

void WeatherPage::drawMoonPhase() {
	mDisplay.fillRect(0, 120, 200, 80, GxEPD_BLACK);
	
	mDisplay.drawBitmap(90, 183, icon_up, 20, 20, GxEPD_WHITE);
	
	for(int i = -2; i <= 2; i++) {
		// Get current moon phase
		// Phase from 0 - 0.5 = Increasing Moon
		// Phase from 0.5 - 1 = Decreasing Moon
		float phase = WeatherManager::getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day + i, MiteOS::currentTime.Hour);
		
		// Calculate what icon needs to be on the left and right, also make non current moons smaller
		uint8_t size = (i == 0 ? 40 : 30);
		const uint8_t* icon_left = (phase > 0.5 ? (i == 0 ? icon_moon_full : icon_moon_full_small) : (i == 0 ? icon_moon_new : icon_moon_new_small));
		const uint8_t* icon_right = (phase > 0.5 ? (i == 0 ? icon_moon_new : icon_moon_new_small) : (i == 0 ? icon_moon_full : icon_moon_full_small));
		
		// Make sure phase is between 0 and 1 to calculate size
		uint8_t value = (phase > 0.5 ? size * ((phase - 0.5) * 2) : size * phase * 2);
		
		// Draw the moons
		drawPartialBitmap(80 - (-35 * i + (i > 0 ? -10 : 0)), 145 - (i != 0 ? -10 + abs(i) * 5 : 0), icon_left, size, size,   0, size - value, GxEPD_WHITE);
		drawPartialBitmap(80 - (-35 * i + (i > 0 ? -10 : 0)), 145 - (i != 0 ? -10 + abs(i) * 5 : 0), icon_right, size, size, size - value, size, GxEPD_WHITE);
		
		// If its the current day, draw the percentages and indicator
		if(i == 0) {
			int fraction = ((1.0 - cos(2 * M_PI * phase)) * 0.5) * 100;
			
			mDisplay.setTextColor(GxEPD_WHITE);
			
			mDisplay.setFont(&DSEG7_Classic_Regular_15);
			String str = String(fraction);
			mDisplay.setCursor(80, 142);
			mDisplay.print(str.c_str());
			
			mDisplay.setFont(&Seven_Segment10pt7b);
			mDisplay.print("%");
		}
	}
}
