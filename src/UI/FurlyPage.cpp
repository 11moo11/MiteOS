#include "FurlyPage.h"

#include "../Images/menu_icons.h"
#include "../Images/furly_icons.h"
#include "../Fonts/FreeMonoBold7pt7b.h"
#include "../MiteOS.h"

// TODO: Sidebar
// Feed
// Give Water
// Play
// Shop

// Idea: Furly can die, but you can choose to revive it instead of getting a new one

// Evolution Steps
// Egg										- Only Warmth needed
// 15.000 steps -> very smally				- Food + Water
// 30.000 steps -> Smally					- Food + Water + Play + Learn
// 50.000 steps -> Grown Up (unlock store)	- Food + Water + Play + Work

// Stats tracking: Date of each step
// Days lived

// To keep Track of:
// Happiness
// Hungryness
// Thirstiness
// Education?

#define FURLY_PAGE pageData.subPageIndex
#define MAIN_SCROLL_OFFSET pageData.number1
#define START_STEP_COUNTER pageData.largenumber1

#define FURLY_PAGE_MAIN_PAGE 0

#define MAIN_SELECTION_ICON_COUNT(level) (level == 3 ? 5 : (level == 2 ? 4 : (level == 1 ? 2 : 1)))
const unsigned char* main_selection_icons[4] = {
	icon_furly_info,
	icon_furly_sun,
	icon_furly_bottle,
	icon_furly_food,
};


#define LEVEL_1_STEPS 15000
#define LEVEL_2_STEPS LEVEL_1_STEPS + 30000
#define LEVEL_3_STEPS LEVEL_2_STEPS + 50000

void FurlyPage::drawPage() {
	if(START_STEP_COUNTER == 0) {
		Configuration::init();
		START_STEP_COUNTER = Configuration::preferences.getUInt("furly_init", 0);
		if(START_STEP_COUNTER == 0) {
			Configuration::preferences.putUInt("furly_init", ActivityManager::getTotalStepCount());
			START_STEP_COUNTER = 1;
		}
	}
	
	
	switch(FURLY_PAGE) {
		case FURLY_PAGE_MAIN_PAGE:
			drawMainPage();
			break;
		default:
			drawButtonIcon(BTN_BACK, icon_exit);
			break;
	}
}

bool FurlyPage::onButtonPressed(uint8_t buttonIndex) {
	uint8_t level = getLevel();
	switch(FURLY_PAGE) {
		case FURLY_PAGE_MAIN_PAGE:
			switch(buttonIndex) {
				case BTN_UP:
					MAIN_SCROLL_OFFSET--;
					if(MAIN_SCROLL_OFFSET > 250) MAIN_SCROLL_OFFSET = MAIN_SELECTION_ICON_COUNT(level) - 1;
					return true;
				case BTN_DOWN:
					MAIN_SCROLL_OFFSET++;
					if(MAIN_SCROLL_OFFSET >= MAIN_SELECTION_ICON_COUNT(level)) MAIN_SCROLL_OFFSET = 0;
					return true;
				case BTN_CONFIRM:
					
					return true;
			}
			break;
		default:
			break;
	}
	
	return false;
}

void FurlyPage::drawMainPage() {
	drawButtonIcon(BTN_BACK, icon_exit);
	drawButtonIcon(BTN_CONFIRM, icon_checkmark);
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
	
	uint8_t level = getLevel();
	if(level < 1) {
		mDisplay.drawRect(160, 80, 40, 40, FOREGROUND_COLOR);
	}else{
		mDisplay.drawRect(160, 35, 40, 130, FOREGROUND_COLOR);
	}
	
	mDisplay.fillRect(150, 90, 20, 20, BACKGROUND_COLOR);
	mDisplay.drawBitmap(150, 90, icon_right, 20, 20, FOREGROUND_COLOR);
	
	mDisplay.drawBitmap(165, 85, main_selection_icons[MAIN_SCROLL_OFFSET], 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 85, 30, 30, FOREGROUND_COLOR);
	
	if(level >= 1) {
		uint8_t prev = MAIN_SCROLL_OFFSET ==                                    0 ? MAIN_SELECTION_ICON_COUNT(level) - 1 : MAIN_SCROLL_OFFSET - 1;
		mDisplay.drawBitmap(165, 40, main_selection_icons[prev], 30, 30, FOREGROUND_COLOR);
		mDisplay.drawRect(165, 40, 30, 30, FOREGROUND_COLOR);
		
		uint8_t next = MAIN_SCROLL_OFFSET == MAIN_SELECTION_ICON_COUNT(level) - 1 ?                                    0 : MAIN_SCROLL_OFFSET + 1;
		mDisplay.drawBitmap(165, 130, main_selection_icons[next], 30, 30, FOREGROUND_COLOR);
		mDisplay.drawRect(165, 130, 30, 30, FOREGROUND_COLOR);
	}
	
	drawFurly(80, 100);
	drawProgressBar();
}

void FurlyPage::drawFurly(uint8_t x, uint8_t y, uint8_t gender) {
	uint8_t level = getLevel();
	
	switch(level) {
		case 0:
			mDisplay.drawBitmap(x - (FURLY_EGG_WIDTH / 2), y - (FURLY_EGG_HEIGHT / 2), furly_state_egg, FURLY_EGG_WIDTH, FURLY_EGG_HEIGHT, FOREGROUND_COLOR);
			break;
		
		case 1:
		case 2:
		case 3:
			mDisplay.drawBitmap(x - (FURLY_WIDTH / 2), y - (FURLY_HEIGHT / 2), furly_part_body, FURLY_WIDTH, FURLY_HEIGHT, FOREGROUND_COLOR);
			
			mDisplay.drawBitmap(x -  4, y +  6, furly_part_nose, FURLY_NOSE_WIDTH, FURLY_NOSE_HEIGHT, FOREGROUND_COLOR); // NOSE
			
			mDisplay.drawBitmap(x - 20, y + 11, furly_part_mouth_happy, FURLY_MOUTH_WIDTH, FURLY_MOUTH_HEIGHT, FOREGROUND_COLOR); // MOUTH
			
			if(gender == GENDER_MALE) {
				mDisplay.drawBitmap(x - 22, y - 13, furly_part_eye_m, FURLY_EYE_WIDTH, FURLY_EYE_HEIGHT, FOREGROUND_COLOR); // LEFT EYE
				mDisplay.drawBitmapFlippedH(x +  5, y - 13, furly_part_eye_m, FURLY_EYE_WIDTH, FURLY_EYE_HEIGHT, FOREGROUND_COLOR); // RIGHT EYE
				
				mDisplay.drawBitmap(x + 21, y + 4, furly_part_tail_m, FURLY_TAIL_WIDTH, FURLY_TAIL_HEIGHT, FOREGROUND_COLOR); // TAIL
			}else if(gender == GENDER_FEMALE) {
				mDisplay.drawBitmap(x - 22, y - 13, furly_part_eye_f, FURLY_EYE_WIDTH, FURLY_EYE_HEIGHT, FOREGROUND_COLOR); // LEFT EYE
				mDisplay.drawBitmapFlippedH(x +  5, y - 13, furly_part_eye_f, FURLY_EYE_WIDTH, FURLY_EYE_HEIGHT, FOREGROUND_COLOR); // RIGHT EYE
				
				mDisplay.drawBitmap(x + 21, y + 4, furly_part_tail_f, FURLY_TAIL_WIDTH, FURLY_TAIL_HEIGHT, FOREGROUND_COLOR); // TAIL
			}
			break;
	}
}

uint32_t FurlyPage::getLevelSteps() {
	return ActivityManager::getTotalStepCount() - START_STEP_COUNTER;
}
uint8_t FurlyPage::getLevel() {
	uint32_t counter = getLevelSteps();
	if(counter > LEVEL_3_STEPS) return 3;
	if(counter > LEVEL_2_STEPS) return 2;
	if(counter > LEVEL_1_STEPS) return 1;
	return 0;
}

void FurlyPage::drawProgressBar() {
	mDisplay.drawRect(50, 10, 100, 10, FOREGROUND_COLOR);
	
	uint8_t level = getLevel();
	uint32_t steps = getLevelSteps() - (level == 3 ? LEVEL_3_STEPS : (level == 2 ? LEVEL_2_STEPS : (level == 1 ? LEVEL_3_STEPS : 0)));
	float progress = steps * 1.0 / (level == 0 ? LEVEL_1_STEPS : (level == 1 ? LEVEL_2_STEPS : (level == 2 ? LEVEL_3_STEPS : 9999999)));
	mDisplay.fillRect(51, 11, 98 * progress, 8, FOREGROUND_COLOR);
	
	mDisplay.setFont(&FreeMonoBold7pt7b);
	Page::drawCentreString(String(steps) + " / " + String((level == 0 ? LEVEL_1_STEPS : (level == 1 ? LEVEL_2_STEPS : (level == 2 ? LEVEL_3_STEPS : 9999999)))), 100, 30);
}