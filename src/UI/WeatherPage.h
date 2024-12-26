#ifndef WEATHER_PAGE_H
#define WEATHER_PAGE_H

#include "Page.h"

class WeatherPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
	private:
		void drawWeather();
		void drawMoonPhase();
};

#endif