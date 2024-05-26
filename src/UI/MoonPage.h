#ifndef MOON_PAGE_H
#define MOON_PAGE_H

#include "Page.h"
#include "../MiteOS.h"
#include "../Managers/WeatherManager.h"
#include "../Images/moon_icons.h"
#include "../Images/menu_icons.h"

class MoonPage : public Page {
	public:
		void drawPage() {
			mDisplay.fillScreen(GxEPD_BLACK);
			
			for(int i = -1; i <= 1; i++) {
				float phase = WeatherManager::getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day + (i * 4));
				
				int displayPhase = phase * 7;
				
				switch(displayPhase) {
					case 7:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_new, 40, 40, GxEPD_WHITE);
						break;
					case 6:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_waning3, 40, 40, GxEPD_WHITE);
						break;
					case 5:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_waning2, 40, 40, GxEPD_WHITE);
						break;
					case 4:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_waning1, 40, 40, GxEPD_WHITE);
						break;
					case 3:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_full, 40, 40, GxEPD_WHITE);
						break;
					case 2:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_crescent1, 40, 40, GxEPD_WHITE);
						break;
					case 1:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_crescent2, 40, 40, GxEPD_WHITE);
						break;
					case 0:
					default:
						mDisplay.drawBitmap(80 - (-50 * i), 80, icon_moon_crescent3, 40, 40, GxEPD_WHITE);
						break;
				}
			}
			mDisplay.drawBitmap(90, 125, icon_up, 20, 20, GxEPD_WHITE);
		};
};

#endif