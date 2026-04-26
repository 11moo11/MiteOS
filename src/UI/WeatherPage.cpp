#include "WeatherPage.h"

#include "../MiteOS.h"
#include "../Fonts/DSEG7_Classic_Regular_39.h"
#include "../Fonts/DSEG7_Classic_Regular_15.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include "../Fonts/Seven_Segment10pt7b.h"
#include "../Fonts/FreeSans6pt7b.h"
#include "../Fonts/FreeMonoBold10pt7b.h"
#include "../Managers/WeatherManager.h"
#include "../Images/moon_icons.h"
#include "../Images/menu_icons.h"
#include "../Images/big_icons.h"
#include "../Images/weather_icons.h"

#define FORECAST_DAY_COUNT 7

#define WEATHER_PAGE_SITES 3

#define PAGE_CURRENT 0
#define PAGE_OVERVIEW 1
#define PAGE_MOON 2

void WeatherPage::drawPage() {
	//mDisplay.fillScreen(GxEPD_BLACK);
	
	drawButtonIcon(BTN_HOME, icon_home);
	if(pageData.subPageIndex != PAGE_MOON) {
		drawButtonIcon(BTN_UP, icon_refresh);
	}
	drawButtonIcon(BTN_CONFIRM, icon_right);

	if(pageData.subPageIndex == PAGE_CURRENT)
		drawWeather();
	else if(pageData.subPageIndex == PAGE_OVERVIEW)
		drawWeatherOverview();
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

	// Set the values to last weather data
	int8_t temperature = currentWeather.temperature;
	int16_t weatherConditionCode = currentWeather.weatherConditionCode;
	String weatherDescription = currentWeatherData.weatherDescription;

	// Check if the weather data is older than 3 hours and stop showing it
	if(NOW - currentWeatherData.timestamp >= 7 * 24 * 60 * 60) {
		temperature = currentWeather.chip_temperature;
		String(TXT_CHIP).toCharArray(currentWeatherData.weatherDescription, 20);
	}

	mDisplay.setFont(&DSEG7_Classic_Regular_39);
	
	mDisplay.setCursor(25, 70);
	mDisplay.println(String(temperature).c_str());
	
	mDisplay.drawBitmap(25 + String(temperature).length() * 35, 25, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, FOREGROUND_COLOR);
	
	const unsigned char* weatherIcon = sunny;
	
	mDisplay.setFont(&Seven_Segment10pt7b);
	drawCentreString(currentWeatherData.weatherDescription, 100, 90);
	
	if(NOW - currentWeatherData.timestamp < 7 * 24 * 60 * 60) {
		if(weatherConditionCode > 0) {
			//https://github.com/open-meteo/open-meteo/blob/743ee9b3e3b3791a13a7547dd49b4bf46d7f3530/Sources/App/Helper/WeatherCode.swift#L4
			if(weatherConditionCode >= 90)
				weatherIcon = thunderstorm;
			else if(weatherConditionCode >= 80)
				weatherIcon = rain;
			else if(weatherConditionCode >= 70)
				weatherIcon = snow;
			else if(weatherConditionCode >= 60)
				weatherIcon = rain;
			else if(weatherConditionCode >= 50)
				weatherIcon = drizzle;
			else if(weatherConditionCode >= 40)
				weatherIcon = atmosphere;
			else if(weatherConditionCode >= 3)
				weatherIcon = cloudy;
			else if(weatherConditionCode >= 1)
				weatherIcon = cloudsun;
			else
				weatherIcon = sunny;

			//https://openweathermap.org/weather-conditions
			/*
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
			*/
		}

		mDisplay.setFont(&FreeSans6pt7b);
		drawCentreString(String(hour(currentWeather.timestamp)) + ":" + (minute(currentWeather.timestamp) < 10 ? "0" : "") + String(minute(currentWeather.timestamp)), 100, 190);
	}else{
		weatherIcon = chip;
	}
	
	if(NOW - currentWeatherData.timestamp < 24 * 60 * 60) {
		mDisplay.drawBitmap(40, 110, big_icon_sunrise, 40, 40, FOREGROUND_COLOR);
		drawCentreString(String(currentWeather.sunrise.Hour) + ":" + (currentWeather.sunrise.Minute < 10 ? "0" : "") + String(currentWeather.sunrise.Minute), 60, 170, false);

		mDisplay.drawBitmap(120, 110, big_icon_sunset, 40, 40, FOREGROUND_COLOR);
		drawCentreString(String(currentWeather.sunset.Hour) + ":" + (currentWeather.sunset.Minute < 10 ? "0" : "") + String(currentWeather.sunset.Minute), 140, 170, false);
	}
	
	mDisplay.drawBitmap(120, 30, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, FOREGROUND_COLOR);
}

void WeatherPage::drawWeatherOverview() {
	int16_t table_start_y = DISPLAY_WIDTH / 2 - DISPLAY_WIDTH / 8;
	int16_t col_width = DISPLAY_WIDTH / FORECAST_DAY_COUNT;
	int16_t col_height = DISPLAY_HEIGHT / 2;

	mDisplay.setFont(&FreeMonoBold10pt7b);

	mDisplay.drawLine(0, table_start_y + 30, DISPLAY_WIDTH, table_start_y + 30, FOREGROUND_COLOR);

	for (int i = 0; i < FORECAST_DAY_COUNT; i++) {
		// 2. Add 'i' days in seconds (SECS_PER_DAY is a constant in TimeLib)
		time_t futureTime = NOW + (i * SECS_PER_DAY);

		// 3. Create a new tmElements_t and populate it from the timestamp
		tmElements_t tm;
		breakTime(futureTime, tm);

		// TODO: Dynamic Check
		char buffer[11];
		snprintf(buffer, 11, "%04d-%02d-%02d", tmYearToCalendar(tm.Year), tm.Month, tm.Day);
		String day = String(buffer);

		WeatherDataDay data = WeatherManager::getWeatherDataDay(day);

		if(i == 0)
			drawDitherBox(col_width * i, table_start_y, col_width, col_height);
		else
			mDisplay.drawRect(col_width * i, table_start_y, col_width, col_height, FOREGROUND_COLOR);
		
		drawCentreString(String(Lang::dayShortStr(tm.Wday)).substring(0, 2), col_width * i + (col_width / 2), table_start_y + 13, false);
		drawCentreString(String(tm.Day), col_width * i + (col_width / 2), table_start_y + 28, false);

		drawCentreString(String(data.temperatureMin), col_width * i + (col_width / 2), table_start_y + 70, false);
		drawCentreString(String(data.temperatureMax), col_width * i + (col_width / 2), table_start_y + 90, false);
	}

	/*
	mDisplay.drawBitmap(40, 110, big_icon_sunrise, 40, 40, FOREGROUND_COLOR);
	drawCentreString(String(data.sunrise.Hour) + ":" + (data.sunrise.Minute < 10 ? "0" : "") + String(data.sunrise.Minute), 60, 170, false);

	mDisplay.drawBitmap(120, 110, big_icon_sunset, 40, 40, FOREGROUND_COLOR);
	drawCentreString(String(data.sunset.Hour) + ":" + (data.sunset.Minute < 10 ? "0" : "") + String(data.sunset.Minute), 140, 170, false);
	*/
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
