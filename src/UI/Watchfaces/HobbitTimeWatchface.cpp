#include "HobbitTimeWatchface.h"

void HobbitTimeWatchface::draw() { //override this method to customize how the watch face looks
	String textstring = "";
	const char *lines [25][3] = {
		{"Midnight","snack"},
		{"Sleep"},
		{"Sleep"},
		{"Sleep"},
		{"Sleep"},
		{"Sleep"},
		{"Almost","breakfast"},
		{"Breakfast"},
		{"Almost","second","breakfast"},
		{"Second","breakfast"},
		{"Almost","elevenses"},
		{"Elevenses"},
		{"Luncheon"},
		{"After","lunch","nap"},
		{"Afternoon","tea"},
		{"Three-ish"},
		{"Almost","dinner"},
		{"Dinner"},
		{"Almost","supper"},
		{"Supper"},
		{"Eight-ish"},
		{"Nine-ish"},
		{"Sleep"},
		{"Sleep"},
		{"Midnight","snack"}
	};
	
	const char linecount[25] = { 2, 1, 1, 1, 1, 1, 2, 1, 3, 2, 2, 1, 1, 3, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2 };
	
	//drawtime
	mDisplay.setFont(&NunitoSans_Black12pt7b);
	textstring = MiteOS::currentTime.Hour;
	textstring += ":";
	if (MiteOS::currentTime.Minute<10){
		textstring += "0";
	}
	textstring += MiteOS::currentTime.Minute;
	Page::drawCentreString(textstring, 100, 195);

	//drawlabel
	mDisplay.setFont(&NunitoSans_Black18pt7b);
	if (linecount[MiteOS::currentTime.Hour] == 1) {
		Page::drawCentreString(lines[MiteOS::currentTime.Hour][0], 100, 100);
	} else if (linecount[MiteOS::currentTime.Hour] == 2){
		Page::drawCentreString(lines[MiteOS::currentTime.Hour][0], 100, 86);
		Page::drawCentreString(lines[MiteOS::currentTime.Hour][1], 100, 114);
	} else if (linecount[MiteOS::currentTime.Hour] == 3) {
		Page::drawCentreString(lines[MiteOS::currentTime.Hour][0], 100, 72);
		Page::drawCentreString(lines[MiteOS::currentTime.Hour][1], 100, 100);
		Page::drawCentreString(lines[MiteOS::currentTime.Hour][2], 100, 128);
	}
}