#ifndef MOON_PAGE_H
#define MOON_PAGE_H
#include "Page.h"
#include "../Managers/WeatherManager.h"
#include "../Images/moon_icons.h"

class MoonPage : public Page {
	public:
		void drawPage() override {
			float phase = WeatherManager::getMoonPhase();
			
			if(phase <= 0.1 || phase >= 0.9) {
				mDisplay.drawBitmap(40, 40, icon_moon_full, 80, 80, FOREGROUND_COLOR);
			} else if(phase >= 0.8) {
				mDisplay.drawBitmap(40, 40, icon_moon_waning3, 80, 80, FOREGROUND_COLOR);
			} else if(phase >= 0.7) {
				mDisplay.drawBitmap(40, 40, icon_moon_waning2, 80, 80, FOREGROUND_COLOR);
			} else if(phase >= 0.6) {
				mDisplay.drawBitmap(40, 40, icon_moon_waning1, 80, 80, FOREGROUND_COLOR);
			} else if(phase >= 0.4) {
				mDisplay.drawBitmap(40, 40, icon_moon_crescent1, 80, 80, FOREGROUND_COLOR);
			} else if(phase >= 0.3) {
				mDisplay.drawBitmap(40, 40, icon_moon_crescent2, 80, 80, FOREGROUND_COLOR);
			} else if(phase >= 0.2) {
				mDisplay.drawBitmap(40, 40, icon_moon_crescent3, 80, 80, FOREGROUND_COLOR);
			}
		};
};

#endif