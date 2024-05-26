#include "MoonPage.h"

#include "../MiteOS.h"
#include "../Managers/WeatherManager.h"
#include "../Images/moon_icons.h"
#include "../Images/menu_icons.h"

void MoonPage::drawPage() {
	mDisplay.fillScreen(GxEPD_BLACK);
	
	float phase = WeatherManager::getMoonPhase(MiteOS::currentTime.Year, MiteOS::currentTime.Month, MiteOS::currentTime.Day);
	
	mDisplay.drawBitmap(90, 118, icon_up, 20, 20, GxEPD_WHITE);
	
	for(int i = -1; i <= 1; i++) {
		int displayPhase = (phase * 7) + i;
		
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
		mDisplay.drawBitmap(80 - (-50 * i), 80, moonIcon, 40, 40, GxEPD_WHITE);
	}
	
	int fraction = ((1.0 - cos(2 * M_PI * phase)) * 0.5) * 100;
	
	mDisplay.setTextColor(GxEPD_WHITE);
	
	String str = String(fraction) + "%";
	drawCentreString(str.c_str(), 100, 70);
}