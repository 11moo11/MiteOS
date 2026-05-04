#include "MiteoWatchface.h"

#include "MiteOS.h"
#include "../../Fonts/DSEG7_Classic_Regular_39.h"
#include "../../Fonts/FreeMonoBold10pt7b.h"
#include "../../Fonts/FreeMonoBold7pt7b.h"
#include "../../Fonts/LCDM2B__10pt7b.h"

#define WINDOW_SIZE_X 140
#define WINDOW_SIZE_Y 100
#define WINDOW_START_X (DISPLAY_WIDTH - WINDOW_SIZE_X) / 2
#define WINDOW_START_Y (DISPLAY_HEIGHT - WINDOW_SIZE_Y) / 2

void MiteoWatchface::draw() {
	drawBackground();
	drawForeground();
}

void MiteoWatchface::drawBackground() {
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.setTextColor(FOREGROUND_COLOR);

	mDisplay.drawRoundRect(5, 5, 190, 190, 20, FOREGROUND_COLOR);
	mDisplay.drawRoundRect(6, 6, 188, 188, 20, FOREGROUND_COLOR);
	mDisplay.drawRoundRect(10, 10, 180, 180, 20, FOREGROUND_COLOR);
	mDisplay.drawRoundRect(WINDOW_START_X - 5, WINDOW_START_Y - 5, WINDOW_SIZE_X + 10, WINDOW_SIZE_Y + 10, 5, FOREGROUND_COLOR);

	mDisplay.setFont(&FreeMonoBold10pt7b);
	mDisplay.setCursor(20, 30);
	mDisplay.println("MITEO");

	mDisplay.setFont(&FreeMonoBold7pt7b);
	mDisplay.setCursor(130, 28);
	mDisplay.println("CHRONO");

	mDisplay.drawBitmap(40, DISPLAY_HEIGHT - 30, icon_left, 20, 20, BACKGROUND_COLOR);
	mDisplay.drawBitmap(DISPLAY_WIDTH - 60, DISPLAY_HEIGHT - 30, icon_right, 20, 20, BACKGROUND_COLOR);
	Page::drawCentreString("ILLUMINATOR", 100, DISPLAY_HEIGHT - 30);
}

void MiteoWatchface::drawForeground() {
	mDisplay.setTextColor(BACKGROUND_COLOR);

	mDisplay.fillRoundRect(WINDOW_START_X, WINDOW_START_Y, WINDOW_SIZE_X, WINDOW_SIZE_Y, 5, FOREGROUND_COLOR);

	mDisplay.setFont(&DSEG7_Classic_Regular_39);
	mDisplay.setCursor(WINDOW_START_X, WINDOW_START_Y + WINDOW_SIZE_Y - 10);
	mDisplay.println(getTimeStr());

	mDisplay.setFont(&LCDM2B__10pt7b);
	mDisplay.setCursor(WINDOW_START_X + 10, WINDOW_START_Y + 28);
	mDisplay.print(dayShortStr(MiteOS::currentTime.Wday));

	mDisplay.print(" ");

	if(MiteOS::currentTime.Day < 10) mDisplay.print("0");
	mDisplay.print(MiteOS::currentTime.Day);
	mDisplay.print(" ");
	mDisplay.println(monthShortStr(MiteOS::currentTime.Month));
}