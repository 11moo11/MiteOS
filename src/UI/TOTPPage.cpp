#include "TOTPPage.h"

#include "../MiteOS.h"

#include "../Images/menu_icons.h"
#include "../Fonts/FreeSans6pt7b.h"
#include "../Fonts/Teko_Regular12pt7b.h"
#include "../Fonts/Teko_Regular20pt7b.h"
#include <Fonts/FreeSans9pt7b.h>
#include "../Managers/TOTPManager.h"

#include "../Data/CSV.h"

#define TOTP_PAGE_COUNT pageData.number1

void TOTPPage::drawPage() {
	Configuration::init();
	pageData.string0 = Configuration::preferences.getString("totp");
	TOTP_PAGE_COUNT = (uint8_t) ceil(CSV::getLength(pageData.string0) / 6.0);

	drawButtonIcon(BTN_BACK, icon_exit);
	drawButtonIcon(BTN_CONFIRM, icon_refresh);
	if(TOTP_PAGE_COUNT > 1) {
		drawButtonIcon(BTN_UP, icon_up);
		drawButtonIcon(BTN_DOWN, icon_down);
	}
	
	uint8_t index = pageData.subPageIndex * 6;
	
	for(uint8_t i = 0; i < 3; i++) {
		if(index + i * 2 >= CSV::getLength(pageData.string0)) break;

		mDisplay.setFont(&Teko_Regular12pt7b);
		drawCentreString(CSV::getElement(pageData.string0, index + (i * 2)), 100, 37 + (55 * i), false);
		
		mDisplay.setFont(&Teko_Regular20pt7b);
		String s = TOTPHandler::GenerateTotp(CSV::getElement(pageData.string0, index + (i * 2) + 1), NOW - gmtTimeOffset);
		s = s.substring(0, 3) + " " + s.substring(3, 6);
		drawCentreString(s, 100, 65 + (55 * i), false);
	}
	
	mDisplay.setFont(&FreeSans9pt7b);
	drawCentreString(String(pageData.subPageIndex + 1) + " / " + TOTP_PAGE_COUNT, 100, 15, false);
	
	mDisplay.setFont(&FreeSans6pt7b);
	uint8_t seconds = 30 - (second(NOW) % 30);
	drawCentreString(String(seconds) + "s", 100, 196, false);
	mDisplay.drawFastHLine(40, 198, 120 * (seconds / 30.0), FOREGROUND_COLOR);
	mDisplay.drawFastHLine(40, 199, 120 * (seconds / 30.0), FOREGROUND_COLOR);
	drawDitherBox(40, 198, 120, 2, 2, 2);
}

bool TOTPPage::onButtonPressed(uint8_t buttonIndex) {
	switch(buttonIndex) {
		case BTN_DOWN:
			if(TOTP_PAGE_COUNT > 1) {
				do {
					pageData.subPageIndex++;
					if(pageData.subPageIndex >= TOTP_PAGE_COUNT) {
						pageData.subPageIndex = 0;
					}
				}while(pageData.subPageIndex >= TOTP_PAGE_COUNT);
			}
			return true;
		
		case BTN_UP:
			if(TOTP_PAGE_COUNT > 1) {
				do {
					if(pageData.subPageIndex == 0) {
						pageData.subPageIndex = TOTP_PAGE_COUNT - 1;
					}else{
						pageData.subPageIndex--;
					}
				}while(pageData.subPageIndex >= TOTP_PAGE_COUNT);
			}
			return true;
		
		case BTN_CONFIRM:
			return true;
	}

	return false;
}