#include "WeatherPage.h"

#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/DSEG7_Classic_Regular_15.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Fonts/FreeSans6pt7b.h"
#include "../Managers/WeatherManager.h"
#include "../Images/moon_icons.h"
#include "../Images/menu_icons.h"
#include "../Images/big_icons.h"
#include "../Images/weather_icons.h"

#define WEATHER_PAGE_SITES 2

void WeatherPage::drawPage() {
	//mDisplay.fillScreen(GxEPD_BLACK);
	
	drawButtonIcon(BTN_HOME, icon_home);
	if(pageData.subPageIndex == 0) {
		drawButtonIcon(BTN_UP, icon_refresh);
	}
	drawButtonIcon(BTN_CONFIRM, icon_right);

	if(pageData.subPageIndex == 0)
		drawWeather();
	else
		drawMoonPhase();
}

bool WeatherPage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_UP) {
		WeatherManager::getWeatherData(false);
		return true;
	}else if(buttonIndex == BTN_CONFIRM) {
		pageData.subPageIndex++;
		if(pageData.subPageIndex >= WEATHER_PAGE_SITES)
			pageData.subPageIndex = 0;
		return true;
	}
	return false;
}

void WeatherPage::drawWeather() {
	WeatherData currentWeather = WeatherManager::getWeatherData();

	if(currentWeather.weatherConditionCode <= 100) return;
	
	// Set the values to last weather data
	int8_t temperature = currentWeather.temperature;
	int16_t weatherConditionCode = currentWeather.weatherConditionCode;
	String weatherDescription = currentWeatherData.weatherDescription;

	// Check if the weather data is older than 3 hours and stop showing it
	if(hour(NOW - currentWeatherData.timestamp) >= 3) {
		temperature = currentWeather.chip_temperature;
		String(TXT_CHIP).toCharArray(currentWeatherData.weatherDescription, 20);
	}

	mDisplay.setFont(&DSEG7_Classic_Regular_39);
	
	mDisplay.setCursor(25, 70);
	mDisplay.println(String(temperature).c_str());
	
	mDisplay.drawBitmap(25 + String(temperature).length() * 35, 25, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, FOREGROUND_COLOR);
	
	const unsigned char* weatherIcon;
	
	mDisplay.setFont(&Seven_Segment10pt7b);
	drawCentreString(currentWeatherData.weatherDescription, 100, 90);
	
	if(hour(NOW - currentWeatherData.timestamp) < 3) {
		if(weatherConditionCode > 0) {
			//https://openweathermap.org/weather-conditions
			if(weatherConditionCode > 801){ //Cloudy
				weatherIcon = cloudy;
			}else if(weatherConditionCode == 801){ //Few Clouds
				weatherIcon = cloudsun;
			}else if(weatherConditionCode == 800){ //Clear
				weatherIcon = sunny;
			}else if(weatherConditionCode >= 700){ //Atmosphere
				weatherIcon = atmosphere;
			}else if(weatherConditionCode >= 600){ //Snow
				weatherIcon = snow;
			}else if(weatherConditionCode >= 500){ //Rain
				weatherIcon = rain;
			}else if(weatherConditionCode >= 300){ //Drizzle
				weatherIcon = drizzle;
			}else if(weatherConditionCode >= 200){ //Thunderstorm
				weatherIcon = thunderstorm;
			}else{
				return;
			}
		}

		mDisplay.drawBitmap(40, 110, big_icon_sunrise, 40, 40, FOREGROUND_COLOR);
		drawCentreString(String(currentWeather.sunrise.Hour) + ":" + (currentWeather.sunrise.Minute < 10 ? "0" : "") + String(currentWeather.sunrise.Minute), 60, 170, false);

		mDisplay.drawBitmap(120, 110, big_icon_sunset, 40, 40, FOREGROUND_COLOR);
		drawCentreString(String(currentWeather.sunset.Hour) + ":" + (currentWeather.sunset.Minute < 10 ? "0" : "") + String(currentWeather.sunset.Minute), 140, 170, false);

		mDisplay.setFont(&FreeSans6pt7b);
		drawCentreString(String(hour(currentWeather.timestamp)) + ":" + (minute(currentWeather.timestamp) < 10 ? "0" : "") + String(minute(currentWeather.timestamp)), 100, 190);
	}else{
		weatherIcon = chip;
	}
	
	mDisplay.drawBitmap(120, 30, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, FOREGROUND_COLOR);
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
	//mDisplay.fillRect(0, 120, 200, 80, GxEPD_BLACK);
	
	mDisplay.drawBitmap(90, 83, icon_up_l, 20, 20, FOREGROUND_COLOR);
	
	for(int i = -2; i <= 2; i++) {
		// Get current moon phase
		// Phase from 0 - 0.5 = Increasing Moon
		// Phase from 0.5 - 1 = Decreasing Moon
		float phase = WeatherManager::getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day + i, MiteOS::currentTime.Hour);
		
		// Calculate what icon needs to be on the left and right, also make non current moons smaller
		uint8_t size = (i == 0 ? 40 : 30);
		const uint8_t* icon_left = (phase > 0.5 ? (i == 0 ? icon_moon_full : icon_moon_full_small) : (i == 0 ? icon_moon_new : icon_moon_new_small));
		const uint8_t* icon_right = (phase > 0.5 ? (i == 0 ? icon_moon_new : icon_moon_new_small) : (i == 0 ? icon_moon_full : icon_moon_full_small));
		
		// Flip Images when Darkmode to render a full moon always white
		if(DARKMODE) {
			const uint8_t* buf = icon_left;
			icon_left = icon_right;
			icon_right = buf;
		}

		// Make sure phase is between 0 and 1 to calculate size
		uint8_t value = (phase > 0.5 ? size * ((phase - 0.5) * 2) : size * phase * 2);
		
		// Draw the moons
		drawPartialBitmap(80 - (-35 * i + (i > 0 ? -10 : 0)), 45 - (i != 0 ? -10 + abs(i) * 5 : 0), icon_left, size, size,   0, size - value, FOREGROUND_COLOR);
		drawPartialBitmap(80 - (-35 * i + (i > 0 ? -10 : 0)), 45 - (i != 0 ? -10 + abs(i) * 5 : 0), icon_right, size, size, size - value, size, FOREGROUND_COLOR);
		
		// If its the current day, draw the percentages and indicator
		if(i == 0) {
			int fraction = ((1.0 - cos(2 * M_PI * phase)) * 0.5) * 100;
			
			mDisplay.setTextColor(FOREGROUND_COLOR);
			
			mDisplay.setFont(&DSEG7_Classic_Regular_15);
			String str = String(fraction);
			mDisplay.setCursor(80, 42);
			mDisplay.print(str.c_str());
			
			mDisplay.setFont(&Seven_Segment10pt7b);
			mDisplay.print("%");
			
			float last_val = phase;
			uint8_t offset_y = 110;
			for(uint8_t d = 0; d < 30; d++) {
				float p = WeatherManager::getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day + d, 23, 59);
				if(p >= 0.5 && last_val < 0.5) {
  					time_t base = makeTime(MiteOS::currentTime);
					struct tm* tm = localtime(&base);
					tm->tm_mday += d;
					time_t next = mktime(tm);

					mDisplay.drawBitmap(30, offset_y, (DARKMODE ? icon_moon_new_small : icon_moon_full_small), 30, 30, FOREGROUND_COLOR);
					
					mDisplay.setCursor(70, offset_y + 20);
					mDisplay.print(day(next));
					mDisplay.print(".");
					mDisplay.print(month(next));
					mDisplay.print(".");

					mDisplay.setCursor(120, offset_y + 20);
					mDisplay.print(String(d) + " " + TXT_DAYS);

					offset_y += 40;
				}if(p <= 0.5 && last_val > 0.5) {
  					time_t base = makeTime(MiteOS::currentTime);
					struct tm* tm = localtime(&base);
					tm->tm_mday += d;
					time_t next = mktime(tm);
					
					mDisplay.drawBitmap(30, offset_y, (DARKMODE ? icon_moon_full_small : icon_moon_new_small), 30, 30, FOREGROUND_COLOR);

					mDisplay.setCursor(70, offset_y + 20);
					mDisplay.print(day(next));
					mDisplay.print(".");
					mDisplay.print(month(next));
					mDisplay.print(".");

					mDisplay.setCursor(120, offset_y + 20);
					mDisplay.print(String(d) + " " + TXT_DAYS);

					offset_y += 40;
				}

				last_val = p;
			}
		}
	}

}
