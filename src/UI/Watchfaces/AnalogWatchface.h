#ifndef WATCHY_ANALOG_H
#define WATCHY_ANALOG_H

// All credits to https://github.com/BenjaminGabel/AnalogWatchFace/tree/main/v2

#include "Watchface.h"

#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include <Fonts/FreeSerifBold12pt7b.h>
#include "Analog/FreeSerifBoldItalic4pt7b.h"
#include "Analog/clockFace_square2.h"

class AnalogWatchface : public Watchface {
	public:
		void draw() {
			//background
			mDisplay.drawBitmap(0, 0, clockFace_square2, 200, 200, FOREGROUND_COLOR);
			mDisplay.fillCircle(100, 100, 3, FOREGROUND_COLOR);

			//Logo
			mDisplay.setCursor(80, 40);
			mDisplay.setFont(&FreeSerifBoldItalic9pt7b);
			mDisplay.print("Gabel ");
			mDisplay.setCursor(54, 60);
			mDisplay.setFont(&FreeSerifItalic9pt7b);
			mDisplay.print("Chronometrie");

			//date
			String dateDay = "";
			if(MiteOS::currentTime.Day < 10){
				dateDay += "0";
			}
			dateDay += MiteOS::currentTime.Day;
			mDisplay.fillRect(128, 88, 27, 24, FOREGROUND_COLOR);
			mDisplay.setFont(&FreeSerifBold12pt7b);
			mDisplay.setTextColor(BACKGROUND_COLOR);
			mDisplay.setCursor(129, 107);
			mDisplay.print(dateDay);
			
			//weekday
			String wDay = Lang::dayShortStr(MiteOS::currentTime.Wday);
			wDay = wDay.substring(0,wDay.length() - 1);
			mDisplay.fillRect(79, 140, 42, 22, FOREGROUND_COLOR);
			mDisplay.setFont(&FreeSerifBold12pt7b);
			mDisplay.setTextColor(BACKGROUND_COLOR);
			Page::drawCentreString(wDay, 100, 158);
			
			// draw battery
			mDisplay.drawCircleHelper(45, 100, 20, 2, FOREGROUND_COLOR);
			mDisplay.drawCircleHelper(45, 100, 20, 4, FOREGROUND_COLOR);
			mDisplay.drawPixel(65, 100, FOREGROUND_COLOR);
			mDisplay.drawFastVLine(45, 79, 4, FOREGROUND_COLOR);
			mDisplay.drawFastHLine(63, 100, 4, FOREGROUND_COLOR);
			mDisplay.drawFastVLine(45, 118, 4, FOREGROUND_COLOR);
			mDisplay.setFont(&FreeSerifBoldItalic4pt7b);
			mDisplay.setTextColor(FOREGROUND_COLOR);
			mDisplay.setCursor(44, 76);
			mDisplay.print("1");
			mDisplay.setCursor(70, 97);
			mDisplay.print("1");
			mDisplay.setCursor(69, 101);
			mDisplay.print("--");
			mDisplay.drawPixel(71, 100, FOREGROUND_COLOR);
			mDisplay.setCursor(69, 107);
			mDisplay.print("2");
			mDisplay.setCursor(43, 127);
			mDisplay.print("0");
			mDisplay.fillCircle(45, 100, 2, FOREGROUND_COLOR);
			double batteryCurrent = (PowerManager::getBatteryVoltage() - 3.3) / 0.9;
			double batteryAngle = batteryCurrent * 180;
			double radBattery = ((batteryAngle) * 71) / 4068.0;
			double bx1 = 45 + (sin(radBattery) * 16);
			double by1 = 100 + (cos(radBattery) * 16);
			mDisplay.drawLine(45, 100, (int)bx1, (int)by1, FOREGROUND_COLOR);
			
			//minute pointer
			int currentMinute = MiteOS::currentTime.Minute;
			int minuteAngle = currentMinute * 6;
			double radMinute = ((minuteAngle + 180) * 71) / 4068.0;
			double mx1 = 100 - (sin(radMinute) * 85);
			double my1 = 100 + (cos(radMinute) * 85);
			mDisplay.drawLine(100, 100, (int)mx1, (int)my1, FOREGROUND_COLOR);
			
			//hour pointer
			int currentHour = MiteOS::currentTime.Hour;
			double hourAngle = (currentHour * 30) + currentMinute * 0.5;
			double radHour = ((hourAngle + 180) * 71) / 4068.0;
			double hx1 = 100 - (sin(radHour) * 45);
			double hy1 = 100 + (cos(radHour) * 45);
			mDisplay.drawLine(100, 100, (int)hx1, (int)hy1, FOREGROUND_COLOR);
	}
};

#endif