#include "ActivityPage.h"

#include "../MiteOS.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include "../Fonts/icons.h"
#include "../Images/menu_icons.h"

#define PAGE_ACTIVITY_OVERVIEW 0
#define PAGE_ACTIVITY_DAYS 1

void ActivityPage::drawPage() {
	if(pageData.subPageIndex == PAGE_ACTIVITY_OVERVIEW) {
		drawCircle();
		drawText();
	}else if(pageData.subPageIndex == PAGE_ACTIVITY_DAYS) {
		drawStepList();
	}
	drawButtonIcon(BTN_TOGGLE_BTN, icon_right);
}


bool ActivityPage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_TOGGLE_BTN) {
		pageData.subPageIndex++;
		if(pageData.subPageIndex > 1) pageData.subPageIndex = 0;
		return true;
	}
	
	return false;
}

void ActivityPage::drawCircle() {
	int lineThickness = 10;
	int diameter = 160;
	int percentage = (ActivityManager::getStepCount() / 8000.0) * 100;
	int centerX = 20;
	int centerY = 20;
	
	percentage = max(0, min(100, percentage));
	
	int radius = diameter / 2;
	double angleStep = 0.007; // Schrittweite für 1 Grad in Bogenmaß
	double endAngle = 2 * M_PI * (percentage / 100.0); // Endwinkel basierend auf dem Prozentsatz
	
	if(percentage > 0) {
		// Generierung der Kreislinie
		for (double angle = 0; angle <= endAngle; angle += angleStep) {
			for (int r = radius - lineThickness; r <= radius; ++r) {
				int x = static_cast<int>(radius + r * cos(angle));
				int y = static_cast<int>(radius + r * sin(angle));
				mDisplay.drawPixel(centerX + y, centerY + diameter - x, FOREGROUND_COLOR);
			}
		}
	}
	
	for(double angle = 0; angle <= 2 * M_PI; angle += M_PI / 10) {
		for (int r = radius - lineThickness; r <= radius; ++r) {
			int x = static_cast<int>(radius + r * cos(angle));
			int y = static_cast<int>(radius + r * sin(angle));
			mDisplay.drawPixel(centerX + y, centerY + diameter - x, FOREGROUND_COLOR);
		}
	}
}

void ActivityPage::drawText() {
	mDisplay.setFont(&Seven_Seg18pt7b);
	drawCentreString(String(ActivityManager::getStepCount()), 100, 100, false);
	drawCentreString(ActivityManager::getWalkedDistanceStr(), 100, 130, false);
	mDisplay.drawBitmap(90, 50, steps, 19, 23, FOREGROUND_COLOR);
}

void ActivityPage::drawStepList() {
	std::array<uint32_t, 7> steps = Configuration::loadSteps();
	
	mDisplay.setFont(&FreeMonoBold12pt7b);
	
	uint8_t row = 0;
	for(int i = 6; i >= 0; i--) {
		int dow = MiteOS::currentTime.Wday - i;
		if(dow < 1) dow = 7 + dow;
		
		if(i == 0) {
			mDisplay.setTextColor(BACKGROUND_COLOR);
			mDisplay.fillRect(20, 47 + (22 * (row - 1)), 160, 20, FOREGROUND_COLOR);
		}
		
		mDisplay.setCursor(30, 40 + (22 * row++));
		mDisplay.print(String(dayShortStr(dow)));
		mDisplay.print(": ");
		mDisplay.println(steps[dow - 1]);
	}
}