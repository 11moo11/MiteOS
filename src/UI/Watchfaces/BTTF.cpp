#include "BTTF.h"
#include "../../MiteOS.h"

#include "../../Fonts/DSEG7_Classic_Bold_25.h"
#include "../../Fonts/Seven_Seg18pt7b.h"

// For more fonts look here:
// https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
//#define FONT_LARGE       Bohemian_Typewriter22pt7b
//#define FONT_MEDUM       Bohemian_Typewriter18pt7b
#define FONT_7_SEG_LARGE    DSEG7_Classic_Bold_25
#define FONT_7_SEG_MEDIUM   Seven_Seg18pt7b

#define YEAR_OFFSET 10

void BTTF::draw(){
	mDisplay.drawBitmap(0, 0, bttf, 200, 200, FOREGROUND_COLOR);
	drawTime();
	drawDate();
	drawSteps();
	drawBattery();
}


void BTTF::printCentered(uint16_t x, uint16_t y, String text){
	int16_t  x1, y1;
	uint16_t w, h;

	mDisplay.getTextBounds(text, 40, 100, &x1, &y1, &w, &h);
	mDisplay.setCursor(x-w/2, y+h/2);
	mDisplay.println(text);
}


void BTTF::drawTime(){
	mDisplay.setFont(&FONT_7_SEG_LARGE);
	mDisplay.setTextColor(BACKGROUND_COLOR);

	String hourStr = String(MiteOS::currentTime.Hour);
	hourStr = MiteOS::currentTime.Hour < 10 ? "0" + hourStr : hourStr;
	printCentered(101, 42, hourStr);

	String minStr = String(MiteOS::currentTime.Minute);
	minStr = MiteOS::currentTime.Minute < 10 ? "0" + minStr : minStr;
	printCentered(165, 42, minStr);
}


void BTTF::drawDate(){
	mDisplay.setFont(&FONT_7_SEG_MEDIUM);
	mDisplay.setTextColor(BACKGROUND_COLOR);

	String month = monthShortStr(MiteOS::currentTime.Month);
	printCentered(35, 103, month);

	String dayStr = String(MiteOS::currentTime.Day);
	dayStr = MiteOS::currentTime.Day < 10 ? "0" + dayStr : dayStr;
	printCentered(94, 103, dayStr);

	//if(alarm_timer < 0){
		String yearStr = String(1970 + MiteOS::currentTime.Year + YEAR_OFFSET);
		printCentered(157, 103, yearStr);
	//} else {
	//    String alarm_str = "T-" + String(alarm_timer);
	//    printCentered(157, 103, alarm_str);
	//}
}


void BTTF::drawBattery(){
	mDisplay.setFont(&FONT_7_SEG_MEDIUM);
	mDisplay.setTextColor(BACKGROUND_COLOR);

	
	int8_t bat = MiteOS::getBatteryPercentage();
	String batStr = String(bat);
	batStr = bat < 10 ? "0" + batStr : batStr;
	batStr = bat < 100 ? "0" + batStr : batStr;
	printCentered(151, 169, batStr);
}


void BTTF::drawSteps(){
	mDisplay.setFont(&FONT_7_SEG_MEDIUM);
	mDisplay.setTextColor(BACKGROUND_COLOR);

	uint32_t steps = accSensor.getCounter();
	String stepStr = String(steps);
	for(int i=1; i<5; i++){
		stepStr = steps < pow(10, i) ? "0" + stepStr : stepStr;
	}

	printCentered(62, 169, stepStr);
}