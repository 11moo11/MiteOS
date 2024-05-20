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
    MiteOS::display.drawBitmap(0, 0, bttf, 200, 200, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
}


void BTTF::printCentered(uint16_t x, uint16_t y, String text){
    int16_t  x1, y1;
    uint16_t w, h;

    MiteOS::display.getTextBounds(text, 40, 100, &x1, &y1, &w, &h);
    MiteOS::display.setCursor(x-w/2, y+h/2);
    MiteOS::display.println(text);
}


void BTTF::drawTime(){
    MiteOS::display.setFont(&FONT_7_SEG_LARGE);
    MiteOS::display.setTextColor(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

    String hourStr = String(MiteOS::currentTime.Hour);
    hourStr = MiteOS::currentTime.Hour < 10 ? "0" + hourStr : hourStr;
    printCentered(101, 42, hourStr);

    String minStr = String(MiteOS::currentTime.Minute);
    minStr = MiteOS::currentTime.Minute < 10 ? "0" + minStr : minStr;
    printCentered(165, 42, minStr);
}


void BTTF::drawDate(){
    MiteOS::display.setFont(&FONT_7_SEG_MEDIUM);
    MiteOS::display.setTextColor(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

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
    MiteOS::display.setFont(&FONT_7_SEG_MEDIUM);
    MiteOS::display.setTextColor(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

    
    int8_t bat = MiteOS::getBatteryPercentage();
    String batStr = String(bat);
    batStr = bat < 10 ? "0" + batStr : batStr;
    batStr = bat < 100 ? "0" + batStr : batStr;
    printCentered(151, 169, batStr);
}


void BTTF::drawSteps(){
    MiteOS::display.setFont(&FONT_7_SEG_MEDIUM);
    MiteOS::display.setTextColor(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

    uint32_t steps = accSensor.getCounter();
    String stepStr = String(steps);
    for(int i=1; i<5; i++){
        stepStr = steps < pow(10, i) ? "0" + stepStr : stepStr;
    }

    printCentered(62, 169, stepStr);
}