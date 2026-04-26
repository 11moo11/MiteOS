#include "BadForEyeWatchface.h"
#include "../../MiteOS.h"

const unsigned char *badforeye_numbers [10] = {
	badforeye_numbers0,
	badforeye_numbers1,
	badforeye_numbers2,
	badforeye_numbers3,
	badforeye_numbers4,
	badforeye_numbers5,
	badforeye_numbers6,
	badforeye_numbers7,
	badforeye_numbers8,
	badforeye_numbers9
};

void BadForEyeWatchface::draw(){
    mDisplay.fillScreen(BACKGROUND_COLOR);
    mDisplay.drawBitmap(0, 0, badforeye_window, DISPLAY_WIDTH, DISPLAY_HEIGHT, FOREGROUND_COLOR);

    //Hour
    mDisplay.drawBitmap(50, 10, badforeye_numbers[MiteOS::currentTime.Hour/10], 39, 80, BACKGROUND_COLOR); //first digit
    mDisplay.drawBitmap(110, 10, badforeye_numbers[MiteOS::currentTime.Hour%10], 39, 80, BACKGROUND_COLOR); //second digit
    

    //Minute
    mDisplay.drawBitmap(50, 110, badforeye_numbers[MiteOS::currentTime.Minute/10], 39, 80, BACKGROUND_COLOR); //first digit
    mDisplay.drawBitmap(110, 110, badforeye_numbers[MiteOS::currentTime.Minute%10], 39, 80, BACKGROUND_COLOR); //second digit
}