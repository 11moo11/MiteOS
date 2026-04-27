#ifndef WEATHER_PAGE_H
#define WEATHER_PAGE_H

#include "Page.h"

class WeatherPage : public Page {
	public:
		String pageName() override { return TXT_WEATHER; };
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
	private:
		void drawWeather();
		void drawWeatherDayLine();
		void drawWeatherOverview();
		void drawMoonPhase();
};

#endif