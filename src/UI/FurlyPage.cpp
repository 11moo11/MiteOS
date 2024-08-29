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

// Random Name: Patata, Kevin, 

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
#define SUBPAGE_INDEX pageData.number1

#define LAST_LEVEL pageData.number2

#define START_STEP_COUNTER pageData.largenumber0

#define LAST_WARMTH pageData.largenumber1
#define LAST_FOOD pageData.largenumber2
#define LAST_WATER pageData.largenumber3
#define LAST_FUN pageData.largenumber4
#define EDU_START pageData.largenumber5
#define WORK_START pageData.largenumber6

#define CREDITS pageData.number7

#define FURLY_BORN pageData.largenumber9

#define FURLY_PAGE_MAIN 0
#define FURLY_PAGE_INFO 1
#define FURLY_PAGE_SETTINGS 2

#define WARMTH_REQUIRE 43200 // 12 hours
#define FOOD_REQUIRE   43200 // 12 hours
#define WATER_REQUIRE  43200 // 12 hours
#define FUN_REQUIRE    86400 //  1 day

#define FURLY_WARMTH_ID 2
#define FURLY_HUNGER_ID 3
#define FURLY_THIRST_ID 4
#define FURLY_PLAY_ID 5
#define FURLY_EDU_ID 6
#define FURLY_WORK_ID 7


#define MAIN_SELECTION_ICON_COUNT(level) (level == 3 ? 8 : (level == 2 ? 7 : (level == 1 ? 5 : 3)))
#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

const unsigned char* main_selection_icons[8] = {
	icon_furly_gear,
	icon_furly_info,
	icon_furly_sun,
	icon_furly_food,
	icon_furly_bottle,
	icon_furly_play,
	icon_furly_edu,
	icon_furly_work,
};

const char active_selection[4] = {
	0b00000111,
	0b00011011,
	0b01111011,
	0b10111011
};

#if DEBUG == true
#define LEVEL_1_STEPS 25
#define LEVEL_2_STEPS LEVEL_1_STEPS + 25
#define LEVEL_3_STEPS LEVEL_2_STEPS + 25
#else
#define LEVEL_1_STEPS 15000
#define LEVEL_2_STEPS LEVEL_1_STEPS + 30000
#define LEVEL_3_STEPS LEVEL_2_STEPS + 50000
#endif

void FurlyPage::initPage() {
	MAIN_SCROLL_OFFSET = 1; // Default to the info
	
	Configuration::init();
	START_STEP_COUNTER = Configuration::preferences.getUInt("furly_init", 0);
	LAST_LEVEL = Configuration::preferences.getUChar("last_level", (uint8_t) 0);
	if(START_STEP_COUNTER == 0) {
		Configuration::preferences.putUInt("furly_init", ActivityManager::getTotalStepCount());
		START_STEP_COUNTER = ActivityManager::getTotalStepCount();
	}
	
	LAST_WARMTH = Configuration::preferences.getLong64("furly_warmth", 0);
	if(LAST_WARMTH == 0) { Configuration::preferences.putLong64("furly_warmth", NOW); LAST_WARMTH = NOW; }
	
	LAST_FOOD   = Configuration::preferences.getLong64("furly_food", 0);
	LAST_WATER  = Configuration::preferences.getLong64("furly_water", 0);
	LAST_FUN    = Configuration::preferences.getLong64("furly_fun", 0);
	
	EDU_START   = Configuration::preferences.getLong64("furly_edu", 0);
	WORK_START  = Configuration::preferences.getLong64("furly_work", 0);
	
	FURLY_BORN = Configuration::preferences.getLong64("furly_born", 0);
	if(FURLY_BORN == 0) { Configuration::preferences.putLong64("furly_born", NOW); FURLY_BORN = NOW; }
	
	printDebug("Warmth: " + String(LAST_WARMTH));
	printDebug("Hungry: " + String(LAST_FOOD));
	printDebug("Thirst: " + String(LAST_WATER));
	printDebug("Fun " + String(LAST_FUN));
}

void FurlyPage::levelChanged() {
	uint8_t level = getLevel();
	if(LAST_LEVEL != level) {
		
		for(uint8_t i = 2; i < 8; i++) {
			if(CHECK_BIT(active_selection[level], i)) {
				refillState(i);
			}
		}
		
		LAST_LEVEL = level;
		Configuration::preferences.putUChar("last_level", level);
	}
}

void FurlyPage::refillState(uint8_t state) {
	uint8_t level = getLevel();
	
	switch (state) {
		case FURLY_WARMTH_ID:
			printDebug("Refuel Warmth");
			LAST_WARMTH = NOW;
			Configuration::preferences.putULong64("furly_warmth", LAST_WARMTH);
			break;
		
		case FURLY_HUNGER_ID:
			printDebug("Refuel Food");
			LAST_FOOD = NOW;
			Configuration::preferences.putULong64("furly_food", LAST_FOOD);
			break;
		
		case FURLY_THIRST_ID:
			printDebug("Refuel Water");
			LAST_WATER = NOW;
			Configuration::preferences.putULong64("furly_water", LAST_WATER);
			break;
			
		case FURLY_PLAY_ID:
			printDebug("Refuel Fun");
			LAST_FUN = NOW;
			Configuration::preferences.putULong64("furly_fun", LAST_FUN);
			break;
		
		default: break;
	}
}

void FurlyPage::drawPage() {
	if(LAST_LEVEL != getLevel()) {
		levelChanged();
	}
	
	const char *menuItems[] = {
		TXT_TEST,
		TXT_RESTART
	};
	switch(FURLY_PAGE) {
		case FURLY_PAGE_MAIN:
			SUBPAGE_INDEX = 0;
			drawMainPage();
			break;
		case FURLY_PAGE_INFO:
			drawInfoPage();
			break;
		case FURLY_PAGE_SETTINGS:
			showMenu(menuItems, 2, true, TXT_SETTINGS);
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
					do{ MAIN_SCROLL_OFFSET--; if(MAIN_SCROLL_OFFSET > 8) MAIN_SCROLL_OFFSET = 7; }while(!CHECK_BIT(active_selection[level], MAIN_SCROLL_OFFSET));
					return true;
				case BTN_DOWN:
					do{ MAIN_SCROLL_OFFSET++; if(MAIN_SCROLL_OFFSET > 7) MAIN_SCROLL_OFFSET = 0; }while(!CHECK_BIT(active_selection[level], MAIN_SCROLL_OFFSET));
					return true;
				case BTN_CONFIRM:
					switch(MAIN_SCROLL_OFFSET) {
						case 0: // Gear
							FURLY_PAGE = FURLY_PAGE_SETTINGS;
							break;
						
						case 1: // Info
							FURLY_PAGE = FURLY_PAGE_INFO;
							break;
						
						case FURLY_WARMTH_ID: // Warmth
							refillState(FURLY_WARMTH_ID);
							break;
						
						case FURLY_HUNGER_ID: // Food
							refillState(FURLY_HUNGER_ID);
							break;
						
						case FURLY_THIRST_ID: // Water
							refillState(FURLY_THIRST_ID);
							break;
							
						case FURLY_PLAY_ID: // Fun
							refillState(FURLY_PLAY_ID);
							break;
						
						default: break;
					}
					return true;
			}
			break;
		
		case FURLY_PAGE_INFO:
			switch(buttonIndex){
				case BTN_UP:
					SUBPAGE_INDEX--;
					if(SUBPAGE_INDEX > 250) SUBPAGE_INDEX = 1;
					break;
				case BTN_DOWN:
					SUBPAGE_INDEX++;
					if(SUBPAGE_INDEX > 1) SUBPAGE_INDEX = 0;
					break;
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
				
				case BTN_CONFIRM:
					if(pageData.menuIndex == 1) {
						resetFurly();
					}
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
	uint8_t prev = MAIN_SCROLL_OFFSET; do{ prev--; if(prev > 8) prev = 7; }while(!CHECK_BIT(active_selection[level], prev));
	mDisplay.drawBitmap(165, 40, main_selection_icons[prev], 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 40, 30, 30, FOREGROUND_COLOR);
	
	Page::drawDitherBox(161, 35, 38, 5, 2, 1);
	Page::drawDitherBox(161, 40, 38, 5, 1, 1);
	Page::drawDitherBox(161, 45, 38, 5, 1, 2);
	Page::drawDitherBox(161, 50, 38, 5, 1, 3);
	
	
	
	uint8_t next = MAIN_SCROLL_OFFSET; do{ next++; if(next > 7) next = 0; }while(!CHECK_BIT(active_selection[level], next));
	mDisplay.drawBitmap(165, 130, main_selection_icons[next], 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 130, 30, 30, FOREGROUND_COLOR);
	
	Page::drawDitherBox(161, 160, 38, 5, 2, 1);
	Page::drawDitherBox(161, 155, 38, 5, 1, 1);
	Page::drawDitherBox(161, 150, 38, 5, 1, 2);
	Page::drawDitherBox(161, 145, 38, 5, 1, 3);
	
	
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
			mDisplay.drawBitmap(x - (FURLY_SMOL_WIDTH / 2), y - (FURLY_SMOL_HEIGHT / 2), furly_state_smol, FURLY_SMOL_WIDTH, FURLY_SMOL_HEIGHT, FOREGROUND_COLOR);
			break;
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
	if(level >= 3) return;
	uint32_t steps = getLevelSteps() - (level == 3 ? LEVEL_3_STEPS : (level == 2 ? LEVEL_2_STEPS : (level == 1 ? LEVEL_1_STEPS : 0)));
	float progress = min(1.0, max(0.0, steps * 1.0 / (level == 0 ? LEVEL_1_STEPS : (level == 1 ? LEVEL_2_STEPS - LEVEL_1_STEPS : (level == 2 ? LEVEL_3_STEPS - LEVEL_2_STEPS : 9999999)))));
	
	Page::drawProgressBar(50, 10, 100, 10, progress, FOREGROUND_COLOR);
	
	mDisplay.setFont(&FreeMonoBold7pt7b);
	Page::drawCentreString(String(steps) + " / " + String((level == 0 ? LEVEL_1_STEPS : (level == 1 ? LEVEL_2_STEPS - LEVEL_1_STEPS : (level == 2 ? LEVEL_3_STEPS - LEVEL_2_STEPS : 9999999)))), 100, 30);
	
	if(MAIN_SCROLL_OFFSET >= 2 && MAIN_SCROLL_OFFSET <= 5) {
		float progress = 0;
		String txt;
		     if(MAIN_SCROLL_OFFSET == FURLY_WARMTH_ID) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WARMTH) / (float) WARMTH_REQUIRE)); txt = TXT_WARMTH;    }
		else if(MAIN_SCROLL_OFFSET == FURLY_HUNGER_ID) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FOOD  ) / (float) FOOD_REQUIRE  )); txt = TXT_HUNGER;    }
		else if(MAIN_SCROLL_OFFSET == FURLY_THIRST_ID) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WATER ) / (float) WATER_REQUIRE )); txt = TXT_THIRST;    }
		else if(MAIN_SCROLL_OFFSET == FURLY_PLAY_ID  ) { progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FUN   ) / (float) FUN_REQUIRE   )); txt = TXT_FUN;       }
		else if(MAIN_SCROLL_OFFSET == FURLY_EDU_ID   ) { progress = 0.5f;                                                                   txt = TXT_EDUCATION; }
		
		Page::drawCentreString(txt, 100, 175);
		Page::drawProgressBar(50, 180, 100, 10, progress, FOREGROUND_COLOR);
	}
}


void FurlyPage::drawInfoPage() {
	drawButtonIcon(BTN_BACK, icon_left);
	
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
	
	mDisplay.drawRect(25, 15, 150, 170, FOREGROUND_COLOR);
	uint8_t offset_y = 40;
	
	drawScrollBar(SUBPAGE_INDEX, 2);
	
	uint8_t level = getLevel();
	if(SUBPAGE_INDEX == 0) {
		offset_y -= 5;
		mDisplay.setFont(&FreeMonoBold10pt7b);
		
		mDisplay.setCursor(35, offset_y);
		mDisplay.print(TXT_LEVEL);
		mDisplay.print(": ");
		mDisplay.print(level);
		
		offset_y += 18;
		
		mDisplay.setCursor(35, offset_y);
		mDisplay.print(TXT_MOOD);
		mDisplay.print(": ");
		mDisplay.print("-");
		
		offset_y += 30;
		
		
		
		mDisplay.setCursor(35, offset_y);
		mDisplay.print(TXT_BIRTHDATE);
		mDisplay.print(": ");
		
		offset_y += 18;
		
		mDisplay.setCursor(35, offset_y);
		long l = FURLY_BORN;
		mDisplay.print(day(FURLY_BORN));
		mDisplay.print(".");
		mDisplay.print(month(FURLY_BORN));
		mDisplay.print(".");
		mDisplay.print(year(FURLY_BORN));
		
		offset_y += 25;
		
		mDisplay.setCursor(35, offset_y);
		mDisplay.print(TXT_AGE);
		mDisplay.print(": ");
		
		
		offset_y += 18;
		
		mDisplay.setCursor(35, offset_y);
		time_t b = FURLY_BORN;
		time_t c = makeTime(MiteOS::currentTime);
		int totalSeconds = c-b;
		//int seconds = (totalSeconds % 60);
		int minutes = (totalSeconds % 3600) / 60;
		int hours = (totalSeconds % 86400) / 3600;
		int days = ((totalSeconds % (86400 * 30)) / 86400); 
		int months = (totalSeconds % (86400 * 30 * 12)) / (86400 * 30); 
		int years = totalSeconds / (86400 * 30 * 12); 
		if(years > 0) {
			mDisplay.print(years);
			mDisplay.print("y");
		}
		if(months > 0) {
			mDisplay.print(months);
			mDisplay.print("m");
		}
		if(days > 0) {
			mDisplay.print(days);
			mDisplay.print("d");
		}
		if(months <= 0) { // Only show hours and minutes when less than a month
			mDisplay.print(hours);
			mDisplay.print("h");
			mDisplay.print(minutes);
			mDisplay.println("m");
		}
		
	} else if(SUBPAGE_INDEX == 1) {
		
		float progress;
		uint8_t hours;
		if(CHECK_BIT(active_selection[level], FURLY_WARMTH_ID)) {
			progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WARMTH) / (float) WARMTH_REQUIRE));
			Page::drawProgressBar(35, offset_y, 100, 10, progress);
			mDisplay.setFont(&FreeMonoBold10pt7b);
			mDisplay.setCursor(35, offset_y - 5);
			mDisplay.print(TXT_WARMTH);
			
			hours = round(progress * WARMTH_REQUIRE / 3600);
			mDisplay.setFont(&FreeMonoBold7pt7b);
			mDisplay.setCursor(140, offset_y + 8);
			mDisplay.print(hours);
			mDisplay.print("h");
		
			offset_y += 35;
		}
		
		if(CHECK_BIT(active_selection[level], FURLY_HUNGER_ID)) {
			progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FOOD) / (float) FOOD_REQUIRE));
			Page::drawProgressBar(35, offset_y, 100, 10, progress);
			mDisplay.setFont(&FreeMonoBold10pt7b);
			mDisplay.setCursor(35, offset_y - 5);
			mDisplay.print(TXT_HUNGER);
			
			hours = round(progress * FOOD_REQUIRE / 3600);
			mDisplay.setFont(&FreeMonoBold7pt7b);
			mDisplay.setCursor(140, offset_y + 8);
			mDisplay.print(hours);
			mDisplay.print("h");
			
			offset_y += 35;
		}
		
		if(CHECK_BIT(active_selection[level], FURLY_THIRST_ID)) {
			progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_WATER) / (float) WATER_REQUIRE));
			Page::drawProgressBar(35, offset_y, 100, 10, progress);
			mDisplay.setFont(&FreeMonoBold10pt7b);
			mDisplay.setCursor(35, offset_y - 5);
			mDisplay.print(TXT_THIRST);
			
			hours = round(progress * WATER_REQUIRE / 3600);
			mDisplay.setFont(&FreeMonoBold7pt7b);
			mDisplay.setCursor(140, offset_y + 8);
			mDisplay.print(hours);
			mDisplay.print("h");
			
			offset_y += 35;
		}
		
		if(CHECK_BIT(active_selection[level], FURLY_PLAY_ID)) {
			progress = min(1.0f, max(0.0f, 1 - (NOW - LAST_FUN) / (float) FUN_REQUIRE));
			Page::drawProgressBar(35, offset_y, 100, 10, progress);
			mDisplay.setFont(&FreeMonoBold10pt7b);
			mDisplay.setCursor(35, offset_y - 5);
			mDisplay.print(TXT_FUN);
			
			hours = round(progress * FUN_REQUIRE / 3600);
			mDisplay.setFont(&FreeMonoBold7pt7b);
			mDisplay.setCursor(140, offset_y + 8);
			mDisplay.print(hours);
			mDisplay.print("h");
		
			offset_y += 35;
		}
		
		if(CHECK_BIT(active_selection[level], FURLY_EDU_ID)) {
			Page::drawProgressBar(35, offset_y, 100, 10, 1);
			mDisplay.setFont(&FreeMonoBold10pt7b);
			mDisplay.setCursor(35, offset_y - 5);
			mDisplay.print(TXT_EDUCATION);
		}
	}
}

void FurlyPage::resetFurly() {
	
	Configuration::init();
	
	Configuration::preferences.putUInt("furly_init", 0);
	Configuration::preferences.putUChar("last_level", (uint8_t) 0);
	Configuration::preferences.getLong64("furly_warmth", NOW);
	Configuration::preferences.putLong64("furly_born", 0);
	
	FURLY_PAGE = FURLY_PAGE_MAIN;
	
	initPage();
}