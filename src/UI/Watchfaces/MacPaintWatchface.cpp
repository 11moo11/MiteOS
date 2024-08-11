#include "MachPaintWatchface.h"

const unsigned char *numbers [10] = {numbers0, numbers1, numbers2, numbers3, numbers4, numbers5, numbers6, numbers7, numbers8, numbers9};

void MacPaintWatchface::draw() {
	mDisplay.drawBitmap(0, 0, window, DISPLAY_WIDTH, DISPLAY_HEIGHT, FOREGROUND_COLOR);

	//Hour
	mDisplay.drawBitmap(35, 70, numbers[MiteOS::currentTime.Hour/10], 38, 50, FOREGROUND_COLOR); //first digit
	mDisplay.drawBitmap(70, 70, numbers[MiteOS::currentTime.Hour%10], 38, 50, FOREGROUND_COLOR); //second digit

	//Colon
	mDisplay.drawBitmap(100, 80, colon, 11, 31, FOREGROUND_COLOR); //second digit

	//Minute
	mDisplay.drawBitmap(115, 70, numbers[MiteOS::currentTime.Minute/10], 38, 50, FOREGROUND_COLOR); //first digit
	mDisplay.drawBitmap(153, 70, numbers[MiteOS::currentTime.Minute%10], 38, 50, FOREGROUND_COLOR); //second digit
}