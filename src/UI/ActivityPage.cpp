#include "ActivityPage.h"

#include "../MiteOS.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include <Fonts/FreeMono9pt7b.h>
#include "../Fonts/icons.h"
#include "../Images/menu_icons.h"

#define PAGE_ACTIVITY_OVERVIEW 0
#define PAGE_ACTIVITY_DAYS 1
#define PAGE_ACTIVITY_TOTAL 2

#define TARGET_STEP_COUNT 8000

void ActivityPage::drawPage() {
	if(pageData.subPageIndex == PAGE_ACTIVITY_OVERVIEW) {
		int lineThickness = 10;
		int diameter = 160;
		int percentage = (ActivityManager::getStepCount() / (float) TARGET_STEP_COUNT) * 100;
		int centerX = 20;
		int centerY = 20;
		
		drawCircle(centerX, centerY, diameter, lineThickness, percentage);
		
		drawText();
	}else if(pageData.subPageIndex == PAGE_ACTIVITY_DAYS) {
		drawStepList();
	}else if(pageData.subPageIndex == PAGE_ACTIVITY_TOTAL) {
		mDisplay.setFont(&Seven_Seg18pt7b);
		drawCentreString(String("Total:"), 100, 50, false);
		drawCentreString(String(ActivityManager::getTotalStepCount()), 100, 120, false);
		drawCentreString(ActivityManager::getTotalWalkedDistanceStr(), 100, 150, false);
		mDisplay.drawBitmap(90, 70, steps, 19, 23, FOREGROUND_COLOR);
	}
	drawButtonIcon(BTN_CONFIRM, icon_right);
	drawButtonIcon(BTN_HOME, icon_home);
}


bool ActivityPage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_CONFIRM) {
		pageData.subPageIndex++;
		if(pageData.subPageIndex > 2) pageData.subPageIndex = 0;
		return true;
	}
	
	return false;
}

void ActivityPage::drawText() {
	mDisplay.setFont(&Seven_Seg18pt7b);
	drawCentreString(String(ActivityManager::getStepCount()), 100, 100, false);
	drawCentreString(ActivityManager::getWalkedDistanceStr(), 100, 130, false);
	mDisplay.drawBitmap(90, 50, steps, 19, 23, FOREGROUND_COLOR);
}

void ActivityPage::drawStepList() {
	std::array<uint32_t, 7> steps = Configuration::loadSteps();
	
	mDisplay.setFont(&FreeMono9pt7b);
	
	uint8_t row = 0;
	for(int i = 6; i >= 0; i--) {
		uint8_t row = i / 3;
		uint8_t col = i % 3;
		if(row == 2) col++;
		
		int dow = MiteOS::currentTime.Wday - i;
		if(dow < 1) dow = 7 + dow;
		
		int stepCount = steps[dow - 1];
		int percentage = (stepCount / (float) TARGET_STEP_COUNT) * 100;
		
		if(i == 0) {
			mDisplay.setTextColor(BACKGROUND_COLOR);
			mDisplay.fillRect(0, 0, 66, 69, FOREGROUND_COLOR);
			drawCircle(16 + (66 * col), 4 + (66 * row), 35, 4, percentage, BACKGROUND_COLOR);
		}else{
			drawCircle(16 + (66 * col), 4 + (66 * row), 35, 4, percentage);
		}
		
		drawCentreString(String(Lang::dayShortStr(dow)), 33 + (66 * col), 33 + (66 * row) + 18, false);
		drawCentreString(String(steps[dow - 1]), 33 + (66 * col), 33 + (66 * row) + 32, false);
		
		/*
		if(i == 0) {
			mDisplay.setTextColor(BACKGROUND_COLOR);
			mDisplay.fillRect(20, 47 + (22 * (row - 1)), 160, 20, FOREGROUND_COLOR);
		}
		
		mDisplay.setCursor(30, 40 + (22 * row++));
		mDisplay.print(String(Lang::dayShortStr(dow)));
		mDisplay.print(": ");
		mDisplay.println(steps[dow - 1]);
		*/
	}
}