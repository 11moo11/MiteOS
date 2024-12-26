#include "TrainWatchface.h"
#include "../../MiteOS.h"

#include "../../Fonts/Teko_Regular40pt7b.h"
#include "../../Fonts/Teko_Regular12pt7b.h"
#include "../../Fonts/Seven_Seg18pt7b.h"
#include "Train/TrainVisuals.h"
#include "../../Fonts/icons.h"

void TrainWatchface::draw(){
	int16_t  x1, y1;
	uint16_t w, h;
	String textstring;
	int frame;
	int mn;

	// ** DRAW **

	//drawbg
	mDisplay.fillScreen(BACKGROUND_COLOR);  
	mDisplay.setFont(&Teko_Regular40pt7b);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	mDisplay.setTextWrap(false);

	//draw animated image
	mn = MiteOS::currentTime.Minute-59;
	frame = mn*12;
	mDisplay.drawBitmap(frame, 159, epd_bitmap_train_an, 908, 41, FOREGROUND_COLOR);

	//draw time
	textstring = MiteOS::currentTime.Hour;
	textstring += ":";
	if (MiteOS::currentTime.Minute < 10) {
		textstring += "0";
	} else {
		textstring += "";
	}
	textstring += MiteOS::currentTime.Minute;
	
	mDisplay.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
	mDisplay.setCursor(100-w/2, 90);
	mDisplay.print(textstring);

  	// draw battery
	mDisplay.fillRoundRect(8, 8, 34, 20, 4, FOREGROUND_COLOR);
	mDisplay.fillRoundRect(40, 12, 5, 12, 1, FOREGROUND_COLOR);
	mDisplay.fillRoundRect(10, 10, 30, 16, 3, BACKGROUND_COLOR);
	if (PowerManager::getBatteryPercentage() > 0) {
		mDisplay.fillRoundRect(11, 11, 28 * (PowerManager::getBatteryPercentage() / 100.0), 14, 3, FOREGROUND_COLOR);
	}

	mDisplay.setFont(&Teko_Regular12pt7b);

	//draw steps
	mDisplay.drawBitmap(146, 103, steps, 19, 23, FOREGROUND_COLOR);
	textstring = ActivityManager::getStepCount();
	mDisplay.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
	mDisplay.setCursor(155-w/2, 145);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	mDisplay.print(textstring);

	// draw day of week
	textstring = Lang::dayStr(MiteOS::currentTime.Wday);
	mDisplay.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
	mDisplay.setCursor(50-w/2, 120);
	mDisplay.print(textstring);


	// draw date
	textstring = Lang::monthShortStr(MiteOS::currentTime.Month);
	textstring += " ";
	textstring += MiteOS::currentTime.Day;
	textstring += ", ";
	textstring += MiteOS::currentTime.Year + 1970;
	mDisplay.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
	mDisplay.setCursor(50-w/2, 145);
	mDisplay.print(textstring);

}