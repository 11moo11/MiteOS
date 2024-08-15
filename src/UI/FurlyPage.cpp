#include "FurlyPage.h"

#include "../Images/menu_icons.h"
#include "../Images/furly_icons.h"
#include "../Fonts/FreeMonoBold7pt7b.h"
#include "../Fonts/FreeMonoBold10pt7b.h"
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
#define MAIN_SCROLL_OFFSET pageData.number0
#define START_STEP_COUNTER pageData.largenumber0

#define CREDITS pageData.number1

#define LAST_WARMTH pageData.largenumber1
#define LAST_FOOD pageData.largenumber2
#define LAST_WATER pageData.largenumber3
#define LAST_FUN pageData.largenumber4
#define EDU_START pageData.largenumber5
#define WORK_START pageData.largenumber6

#define FURLY_PAGE_MAIN 0
#define FURLY_PAGE_INFO 1
#define FURLY_PAGE_SETTINGS 2

#define WARMTH_REQUIRE 43200 // 12 hours
#define FOOD_REQUIRE   43200 // 12 hours
#define WATER_REQUIRE  43200 // 12 hours
#define FUN_REQUIRE    86400 //  1 day

#define MAIN_SELECTION_ICON_COUNT(level) (level == 3 ? 8 : (level == 2 ? 7 : (level == 1 ? 5 : 3)))
const unsigned char* main_selection_icons[8] = {
	furly_icon_gear,
	icon_furly_info,
	icon_furly_sun,
	icon_furly_food,
	icon_furly_bottle,
	icon_empty, // Play
	icon_empty, // Education
	icon_empty, // Work
};


#define LEVEL_1_STEPS 15000
#define LEVEL_2_STEPS LEVEL_1_STEPS + 30000
#define LEVEL_3_STEPS LEVEL_2_STEPS + 50000

void FurlyPage::initPage() {
	MAIN_SCROLL_OFFSET = 2; // Default to the sun
	
	
	Configuration::init();
	START_STEP_COUNTER = Configuration::preferences.getUInt("furly_init", 0);
	if(START_STEP_COUNTER == 0) {
		Configuration::preferences.putUInt("furly_init", ActivityManager::getTotalStepCount());
		START_STEP_COUNTER = ActivityManager::getTotalStepCount();
	}
	
	LAST_WARMTH = Configuration::preferences.getUInt("furly_warmth", 0);
	if(LAST_WARMTH == 0) { Configuration::preferences.putUInt("furly_warmth", NOW); LAST_WARMTH = NOW; }
	LAST_FOOD   = Configuration::preferences.getUInt("furly_food", 0);
	if(LAST_FOOD == 0) { Configuration::preferences.putUInt("furly_food", NOW); LAST_FOOD = NOW; }
	LAST_WATER  = Configuration::preferences.getUInt("furly_water", 0);
	if(LAST_WATER == 0) { Configuration::preferences.putUInt("furly_water", NOW); LAST_WATER = NOW; }
	LAST_FUN    = Configuration::preferences.getUInt("furly_fun", 0);
	if(LAST_FUN == 0) { Configuration::preferences.putUInt("furly_fun", NOW); LAST_FUN = NOW; }
	
	EDU_START   = Configuration::preferences.getUInt("furly_edu", 0);
	WORK_START  = Configuration::preferences.getUInt("furly_work", 0);
	
	printDebug("Warmth: " + String(LAST_WARMTH));
	printDebug("Hungry: " + String(LAST_FOOD));
	printDebug("Thirst: " + String(LAST_WATER));
	printDebug("Fun " + String(LAST_FUN));
}

void FurlyPage::drawPage() {
	
	
	const char *menuItems[] = {
		TXT_TEST
	};
	switch(FURLY_PAGE) {
		case FURLY_PAGE_MAIN:
			drawMainPage();
			break;
		case FURLY_PAGE_INFO:
			drawInfoPage();
			break;
		case FURLY_PAGE_SETTINGS:
			showMenu(menuItems, 1, true, TXT_SETTINGS);
			break;
		default:
			drawButtonIcon(BTN_BACK, icon_exit);
			break;
	}
}

bool FurlyPage::onButtonPressed(uint8_t buttonIndex) {
	uint8_t level = getLevel();
	switch(FURLY_PAGE) {
		case FURLY_PAGE_MAIN:
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
					switch(MAIN_SCROLL_OFFSET) {
						case 0: // Gear
							FURLY_PAGE = FURLY_PAGE_SETTINGS;
							break;
						case 1: // Info
							FURLY_PAGE = FURLY_PAGE_INFO;
							break;
						case 2: // Warmth
							LAST_WARMTH = NOW;
							Configuration::preferences.putUInt("furly_warmth", LAST_WARMTH);
							break;
						case 3: // Food
							LAST_FOOD = NOW;
							Configuration::preferences.putUInt("furly_food", LAST_FOOD);
							break;
						case 4: // Water
							LAST_WATER = NOW;
							Configuration::preferences.putUInt("furly_water", LAST_WATER);
							break;
						default: break;
					}
					return true;
			}
			break;
		
		case FURLY_PAGE_INFO:
			switch(buttonIndex){
				case BTN_BACK:
					FURLY_PAGE = FURLY_PAGE_MAIN;
					break;
				default: break;
			}
			return true;
		
		case FURLY_PAGE_SETTINGS:
			if(handleMenuButtons(buttonIndex)) return true;
			
			switch(buttonIndex){
				case BTN_BACK:
					FURLY_PAGE = FURLY_PAGE_MAIN;
					break;
				default: break;
			}
			return true;
		
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
	
	mDisplay.drawRect(160, 35, 40, 130, FOREGROUND_COLOR);
	
	mDisplay.fillRect(150, 90, 20, 20, BACKGROUND_COLOR);
	mDisplay.drawBitmap(150, 90, icon_right, 20, 20, FOREGROUND_COLOR);
	
	mDisplay.drawBitmap(165, 85, main_selection_icons[MAIN_SCROLL_OFFSET], 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 85, 30, 30, FOREGROUND_COLOR);
	
	uint8_t level = getLevel();
	uint8_t prev = MAIN_SCROLL_OFFSET ==                                    0 ? MAIN_SELECTION_ICON_COUNT(level) - 1 : MAIN_SCROLL_OFFSET - 1;
	mDisplay.drawBitmap(165, 40, main_selection_icons[prev], 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 40, 30, 30, FOREGROUND_COLOR);
	
	Page::drawDitherBox(160, 35, 40, 5, 2, 1);
	Page::drawDitherBox(160, 40, 40, 5, 1, 1);
	Page::drawDitherBox(160, 45, 40, 5, 1, 2);
	Page::drawDitherBox(160, 50, 40, 5, 1, 3);
	
	
	
	uint8_t next = MAIN_SCROLL_OFFSET == MAIN_SELECTION_ICON_COUNT(level) - 1 ?                                    0 : MAIN_SCROLL_OFFSET + 1;
	mDisplay.drawBitmap(165, 130, main_selection_icons[next], 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 130, 30, 30, FOREGROUND_COLOR);
	
	Page::drawDitherBox(160, 160, 40, 5, 2, 1);
	Page::drawDitherBox(160, 155, 40, 5, 1, 1);
	Page::drawDitherBox(160, 150, 40, 5, 1, 2);
	Page::drawDitherBox(160, 145, 40, 5, 1, 3);
	
	
	
	drawFurly(80, 100);
	drawStepProgressBar();
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

void FurlyPage::drawStepProgressBar() {
	uint8_t level = getLevel();
	uint32_t steps = getLevelSteps() - (level == 3 ? LEVEL_3_STEPS : (level == 2 ? LEVEL_2_STEPS : (level == 1 ? LEVEL_3_STEPS : 0)));
	float progress = steps * 1.0 / (level == 0 ? LEVEL_1_STEPS : (level == 1 ? LEVEL_2_STEPS : (level == 2 ? LEVEL_3_STEPS : 9999999)));
	
	Page::drawProgressBar(50, 10, 100, 10, progress, FOREGROUND_COLOR);
	
	mDisplay.setFont(&FreeMonoBold7pt7b);
	Page::drawCentreString(String(steps) + " / " + String((level == 0 ? LEVEL_1_STEPS : (level == 1 ? LEVEL_2_STEPS : (level == 2 ? LEVEL_3_STEPS : 9999999)))), 100, 30);
	
	if(MAIN_SCROLL_OFFSET >= 2 && MAIN_SCROLL_OFFSET <= 5) {
		float progress = 0;
		String txt;
		if(MAIN_SCROLL_OFFSET == 2)      { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WARMTH) / (float) WARMTH_REQUIRE)); txt = TXT_WARMTH; }
		else if(MAIN_SCROLL_OFFSET == 3) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FOOD  ) / (float) FOOD_REQUIRE  )); txt = TXT_HUNGER; }
		else if(MAIN_SCROLL_OFFSET == 4) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WATER ) / (float) WATER_REQUIRE )); txt = TXT_THIRST; }
		else if(MAIN_SCROLL_OFFSET == 5) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FUN   ) / (float) FUN_REQUIRE   )); txt = TXT_FUN;    }
		
		Page::drawCentreString(txt, 100, 175);
		Page::drawProgressBar(50, 180, 100, 10, progress, FOREGROUND_COLOR);
	}
}


void FurlyPage::drawInfoPage() {
	drawButtonIcon(BTN_BACK, icon_left);
	
	mDisplay.drawRect(30, 15, 140, 170, FOREGROUND_COLOR);
	
	uint8_t level = getLevel();
	
	mDisplay.setFont(&FreeMonoBold10pt7b);
	
	float progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WARMTH) / (float) WARMTH_REQUIRE));
	Page::drawProgressBar(50, 40, 100, 10, progress);
	mDisplay.setCursor(50, 35);
	mDisplay.print(TXT_WARMTH);
	
	if(level < 1) return;
	
	progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FOOD) / (float) FOOD_REQUIRE));
	Page::drawProgressBar(50, 70, 100, 10, progress);
	mDisplay.setCursor(50, 65);
	mDisplay.print(TXT_HUNGER);
	
	progress = min(1.0f, max(0.0f, 1 - (NOW - WATER_REQUIRE) / (float) LAST_WATER));
	Page::drawProgressBar(50, 100, 100, 10, progress);
	mDisplay.setCursor(50, 95);
	mDisplay.print(TXT_THIRST);
	
	if(level < 2) return;
	progress = min(1.0f, max(0.0f, 1 - (NOW - FUN_REQUIRE) / (float) LAST_FUN));
	Page::drawProgressBar(50, 130, 100, 10, progress);
	mDisplay.setCursor(50, 125);
	mDisplay.print(TXT_FUN);
	
	Page::drawProgressBar(50, 160, 100, 10, 1);
	mDisplay.setCursor(50, 155);
	mDisplay.print(TXT_EDUCATION);
}