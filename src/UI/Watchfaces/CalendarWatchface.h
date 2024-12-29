#ifndef WATCHY_CALENDAR_H
#define WATCHY_CALENDAR_H

// Credits to https://github.com/uCBill/Calendar
// Calendar watchface by Bill Eichner
// Reworked by Adammantium

#include "Watchface.h"
#include "Calendar/calendar_img.h"
#include "../../Fonts/Teko_Regular12pt7b.h"
#include "../../Fonts/Teko_Regular8pt7b.h"

class CalendarWatchface : public Watchface {
	public:
		void draw() { //override this method to customize how the watch face looks
			int16_t  x1, y1;
			uint16_t w, h;
			String textstring;
			int frame;
			int mth;
			int yr;
			int x;
			int y;
			
			// draw background
			mDisplay.drawBitmap(0,0, calendar_grid, 200, 200, FOREGROUND_COLOR);
			mDisplay.setFont(&Teko_Regular12pt7b);
			mDisplay.setTextWrap(false);
			
			
			uint8_t y_offset = 80;
			tmElements_t elem = MiteOS::currentTime;
			for(uint8_t day = 1; day <= 32; day++) { // Loop through all possible 31 days
				// Set day and recalculate date to check the month
				elem.Day = day;
				time_t base = makeTime(elem);
				tm* time = localtime(&base);
				
				if(time->tm_mon != elem.Month - 1) break; // Check if the day is still in the current month, otherwise abort
				
				uint8_t x = 1 + (200 / 7) * time->tm_wday + (200 / 14);
				if(y_offset > 200) {
					// If the day is off the calendar, we remove the last row elements that are affected and put both dates in the same field
					mDisplay.setFont(&Teko_Regular8pt7b);
					mDisplay.fillRect(x - 15, y_offset - 47, 23, 23, BACKGROUND_COLOR);
					
					Page::drawCentreString(String(day - 7), x - 6, y_offset - 38);
					Page::drawCentreString(String(day), x + 4, y_offset - 26);
					mDisplay.drawLine(x + 10, y_offset - 47, x - 13, y_offset - 24, FOREGROUND_COLOR);
				}else{
					// Just draw the day number in the corresponding field
					Page::drawCentreString(String(day), x, y_offset);
				}
					
				// If its the current day, draw the border around it
				if(MiteOS::currentTime.Day == day) {
					mDisplay.drawBitmap(x - 18, y_offset - 24 - (y_offset > 200 ? 28 : 0), block34x34, 35, 34, FOREGROUND_COLOR);
				}
				
				// if its a saturday, we need to go to the next round
				if(time->tm_wday == 6) y_offset += 28;
			}
			
			mDisplay.setFont(&Teko_Regular12pt7b);
			
			// draw Calendar
			mth = MiteOS::currentTime.Month;
			yr = MiteOS::currentTime.Year + 1970;
			mDisplay.setFont(&Teko_Regular12pt7b);
			textstring = monthStr(MiteOS::currentTime.Month);
			textstring.toUpperCase();
			textstring += " ";
			textstring += MiteOS::currentTime.Day;
			textstring += ", ";
			textstring += MiteOS::currentTime.Year + 1970;
			
			mDisplay.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
			mDisplay.setCursor(100 - w / 2, 38); // date position
			mDisplay.print(textstring);
			// END draw Calendar
			
			// highlight calendar day
			textstring = Lang::dayShortStr(MiteOS::currentTime.Wday);
			
			// draw time
			mDisplay.setFont(&Teko_Regular12pt7b);
			int displayHour;
			if (MiteOS::currentTime.Hour > 12) {
				textstring = ((MiteOS::currentTime.Hour+11)%12)+1;
			} else if (MiteOS::currentTime.Hour < 1) {
				textstring = 12;
			} else {
				textstring = MiteOS::currentTime.Hour;
			}
			textstring += ":";
			if (MiteOS::currentTime.Minute < 10) {
				textstring += "0";
			} else {
				textstring += "";
			}
			textstring += MiteOS::currentTime.Minute;
			//This option displays am or pm to 12 hour clock
			if (MiteOS::currentTime.Hour >= 12) {
				textstring += "PM";
			} else {
				textstring += "AM";
			}
			
			mDisplay.setCursor(5, 19);
			mDisplay.print(textstring);
			//END time

			//  Draw Battery Percent
			int8_t batteryLevel = 0;
			float VBAT = PowerManager::getBatteryVoltage();

			if(VBAT >= 4.2){
				batteryLevel = 100.0;
			}
			else if (VBAT >= 3.3) {
				batteryLevel = 100.0*(VBAT-3.3)/0.9;
			}
			textstring = "BAT:";
			textstring += batteryLevel;
			textstring += "%";
			mDisplay.getTextBounds(textstring, 0, 0, &x1, &y1, &w, &h);
			mDisplay.setCursor(195-w, 19);
			mDisplay.print(textstring);
			//  END OF Draw Battery
		}
};
#endif