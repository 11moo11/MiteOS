#include "CalendarPage.h"

#include "../MiteOS.h"
#include "../Fonts/FreeMonoBold7pt7b.h"
#include "../Fonts/Teko_Regular8pt7b.h"
#include "../Fonts/Teko_Regular12pt7b.h"

#include "../Managers/FileManager.h"
#include <JSON.h>

#define MONTH_OFFSET pageData.short0
#define PAGE_INDEX pageData.subPageIndex

#define APPOINTMENT_PAGE pageData.short1
#define APPOINTMENT_COUNT pageData.short2

#define PAGE_INDEX_CALENDAR 0
#define PAGE_INDEX_APPOINTMENTS 1

void CalendarPage::initPage() {
    Configuration::init();
    long last_sync = Configuration::preferences.getLong64("lastCalSync", 0);
    printDebug(last_sync);
    printDebug(NOW);
    if(last_sync < NOW - (60 * 24 * 1000)) {
        PhoneConnectionManager::SyncCalendar();
        Configuration::preferences.putLong64("lastCalSync", NOW);
    }
}

void CalendarPage::drawPage() {
    if(PAGE_INDEX == PAGE_INDEX_CALENDAR)
        drawMonth();
    else
        drawAppointment();
}

bool CalendarPage::onButtonPressed(uint8_t buttonIndex) {
    if(PAGE_INDEX == PAGE_INDEX_CALENDAR) {
        if(buttonIndex == BTN_UP) {
            MONTH_OFFSET--;
            return true;
        }else if(buttonIndex == BTN_DOWN) {
            MONTH_OFFSET++;
            return true;
        }
    }else if(PAGE_INDEX == PAGE_INDEX_APPOINTMENTS) {
        if(buttonIndex == BTN_UP) {
            if(APPOINTMENT_PAGE == 0)
                APPOINTMENT_PAGE = ceil(APPOINTMENT_COUNT / 3.0);
            APPOINTMENT_PAGE--;
            return true;
        }else if(buttonIndex == BTN_DOWN) {
            APPOINTMENT_PAGE++;
            if(APPOINTMENT_PAGE >= ceil(APPOINTMENT_COUNT / 3.0)) APPOINTMENT_PAGE = 0;
            return true;
        }
    }

    if(buttonIndex == BTN_CONFIRM) {
        PAGE_INDEX++;
        if(PAGE_INDEX > 1) {
            PAGE_INDEX = 0;
        }
        return true;
    }

    return false;
}

void CalendarPage::drawMonth() {
    drawButtonIcon(BTN_BACK, icon_exit);
    drawButtonIcon(BTN_UP, icon_up);
    drawButtonIcon(BTN_DOWN, icon_down);
    drawButtonIcon(BTN_CONFIRM, icon_switch);

    //mDisplay.drawRect(30, 30, 196, 140, FOREGROUND_COLOR);

    // Horizontal
    mDisplay.drawRect(2,  35, 197, 15, FOREGROUND_COLOR);
    mDisplay.drawRect(2,  50, 197, 21, FOREGROUND_COLOR);
    mDisplay.drawRect(2,  70, 197, 21, FOREGROUND_COLOR);
    mDisplay.drawRect(2,  90, 197, 21, FOREGROUND_COLOR);
    mDisplay.drawRect(2, 110, 197, 21, FOREGROUND_COLOR);
    mDisplay.drawRect(2, 130, 197, 21, FOREGROUND_COLOR);
    mDisplay.drawRect(2, 150, 197, 21, FOREGROUND_COLOR);

    // Vertical
    mDisplay.drawRect(  2, 35, 29, 136, FOREGROUND_COLOR);
    mDisplay.drawRect( 30, 35, 29, 136, FOREGROUND_COLOR);
    mDisplay.drawRect( 58, 35, 29, 136, FOREGROUND_COLOR);
    mDisplay.drawRect( 86, 35, 29, 136, FOREGROUND_COLOR);
    mDisplay.drawRect(114, 35, 29, 136, FOREGROUND_COLOR);
    mDisplay.drawRect(142, 35, 29, 136, FOREGROUND_COLOR);
    mDisplay.drawRect(170, 35, 29, 136, FOREGROUND_COLOR);


    mDisplay.setFont(&FreeMonoBold7pt7b);
    // Day Names
    drawCentreString(TXT_MONDAY_SHORT,     16, 47);
    drawCentreString(TXT_TUESDAY_SHORT,    44, 47);
    drawCentreString(TXT_WEDNESDAY_SHORT,  72, 47);
    drawCentreString(TXT_THURSDAY_SHORT,  100, 47);
    drawCentreString(TXT_FRIDAY_SHORT,    128, 47);
    drawCentreString(TXT_SATURDAY_SHORT,  156, 47);
    drawCentreString(TXT_SUNDAY_SHORT,    184, 47);


    uint8_t y_offset = 67;
    tmElements_t elem = MiteOS::currentTime;
    elem.Month = elem.Month + MONTH_OFFSET;
    while(elem.Month > 12) {
        elem.Year++;
        elem.Month -= 12;
    }

    mDisplay.setFont(&Teko_Regular12pt7b);
    for(uint8_t day = 1; day <= 32; day++) { // Loop through all possible 31 days
        // Set day and recalculate date to check the month
        elem.Day = day;
        time_t base = makeTime(elem);
        tm* time = localtime(&base);
        
        if(time->tm_mon != elem.Month - 1) break; // Check if the day is still in the current month, otherwise abort
        
        uint8_t wday = time->tm_wday == 0 ? 6 : time->tm_wday - 1;
        uint8_t x = 2 + ((196 / 7) * wday) + (196 / 14);
        if(y_offset > 200) {
            mDisplay.setFont(&Teko_Regular8pt7b);
            // If the day is off the calendar, we remove the last row elements that are affected and put both dates in the same field
            mDisplay.fillRect(x - 15, y_offset - 47, 23, 23, BACKGROUND_COLOR);
            
            Page::drawCentreString(String(day - 7), x - 6, y_offset - 38);
            Page::drawCentreString(String(day), x + 4, y_offset - 26);
            mDisplay.drawLine(x + 10, y_offset - 47, x - 13, y_offset - 24, FOREGROUND_COLOR);
        }else{
            // Just draw the day number in the corresponding field
            Page::drawCentreString(String(day), x, y_offset);
        }
        
        // If its the current day, draw the border around it
        if(MiteOS::currentTime.Day == day && MONTH_OFFSET == 0) {
            mDisplay.drawRect(x - 13, y_offset - 16, 27, 19, FOREGROUND_COLOR);
            mDisplay.drawRect(x - 12, y_offset - 16, 25, 19, FOREGROUND_COLOR);
        }
        
        // if its a saturday, we need to go to the next round
        if(time->tm_wday == 0) y_offset += 20;
    }
    
    mDisplay.setFont(&Teko_Regular12pt7b);
    Page::drawCentreString(String(monthStr(elem.Month)) + " " + String(elem.Year + 1970), 100, 30);
    
    // draw Calendar
    /*
    mth = MiteOS::currentTime.Month;
    yr = MiteOS::currentTime.Year + 1970;
    mDisplay.setFont(&Teko_Regular12pt7b);
    textstring = monthStr(MiteOS::currentTime.Month);
    textstring.toUpperCase();
    textstring += " ";
    textstring += MiteOS::currentTime.Day;
    textstring += ", ";
    textstring += MiteOS::currentTime.Year + 1970;
    */
}

void CalendarPage::drawAppointment() {
    drawButtonIcon(BTN_BACK, icon_exit);
    drawButtonIcon(BTN_UP, icon_up);
    drawButtonIcon(BTN_DOWN, icon_down);
    drawButtonIcon(BTN_CONFIRM, icon_switch);
    
    FileManager::init();
    File root = LittleFS.open(PATH_CALENDAR);
    File file = root.openNextFile();

    mDisplay.setFont(&Teko_Regular12pt7b);
    uint8_t offset_y = 45;
    APPOINTMENT_COUNT = 0;
    while(file) {
        APPOINTMENT_COUNT++;
        if(offset_y < 180 && APPOINTMENT_COUNT - 1 >= APPOINTMENT_PAGE * 3) {
            String name = file.name();
            long startTime = name.substring(0, name.indexOf("_")).toInt();

            if(file.available()) {
                String line = file.readStringUntil('\n');
                
                // {"id":34,"title":"My Appointment","startTime":1736353800000,"endTime":1736361000000,"allDay":false,"calendarId":"13","calendarName":"calendar name"}
                JSONVar json = JSON.parse(line);
                if(json.hasOwnProperty("startTime")) {
                    mDisplay.setCursor(10, offset_y);
                    mDisplay.setTextWrap(false);
                    mDisplay.println((String) json["title"]);

                    tmElements_t startTime;
                    breakTime((unsigned long) json["startTime"], startTime);

                    mDisplay.setCursor(10, offset_y + 20);
                    mDisplay.print(String(startTime.Day) + ". " + String(monthShortStr(startTime.Month)) + " " + String(startTime.Year + 1970) );
                    if(!json["allDay"]) {
                        tmElements_t endTime;
                        breakTime((unsigned long) json["endTime"], endTime);

                        mDisplay.print("   ");
                        mDisplay.print(String(startTime.Hour) + ":" + (startTime.Minute < 10 ? "0" : "") + String(startTime.Minute));
                        mDisplay.print(" - ");
                        mDisplay.println(String(endTime.Hour) + ":" + (startTime.Minute < 10 ? "0" : "") + String(endTime.Minute));
                        //mDisplay.println(json["title"]);
                    }

                    offset_y += 50;
                }
            }
            file.close();
        }
        file = root.openNextFile();
    }

    drawCentreString(String(APPOINTMENT_PAGE + 1) + " / " + String((int) ceil(APPOINTMENT_COUNT / 3.0)), 100, 15, false);
}