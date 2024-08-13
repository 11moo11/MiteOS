#include "WoolyPage.h"

#include "../Images/menu_icons.h"
#include "../MiteOS.h"

// TODO: Sidebar
// Feed
// Give Water
// Play
// Shop

// Idea: Wooly an die, but you can choose to revive it instead of getting a new one

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

void WoolyPage::drawPage() {
	drawMainPage();
}

bool WoolyPage::onButtonPressed(uint8_t buttonIndex) {
	return false;
}

void WoolyPage::drawMainPage() {
	drawButtonIcon(BTN_BACK, icon_exit);
	drawButtonIcon(BTN_CONFIRM, icon_checkmark);
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
	
	mDisplay.drawRect(160, 30, 41, 140, FOREGROUND_COLOR);
	
	mDisplay.drawRect(165, 40, 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 85, 30, 30, FOREGROUND_COLOR);
	mDisplay.drawRect(165, 130, 30, 30, FOREGROUND_COLOR);
}

void WoolyPage::drawWooly() {
	
}