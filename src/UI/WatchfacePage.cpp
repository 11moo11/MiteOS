#include "WatchfacePage.h"
#include "../MiteOS.h"

#include "Watchfaces/7SEG.h"
#include "Watchfaces/BTTF.h"

RTC_DATA_ATTR uint8_t watchFaceId = 0;

void WatchfacePage::drawPage() {
		#ifdef DEBUG
		Serial.println("Drawing Watchface");
		#endif

    MiteOS::display.fillScreen(BACKGROUND_COLOR);
    MiteOS::display.setTextColor(FOREGROUND_COLOR);

    if(watchFaceId == 0) {
      SEG7().draw();
    }else if(watchFaceId == 1) {
      BTTF().draw();
    }
}

bool WatchfacePage::onButtonPressed(uint8_t buttonIndex) {
  if(buttonIndex == BTN_TOGGLE_BTN) {
    if(DARKMODE) {
      DARKMODE = false;
    }else{
      watchFaceId++;
      DARKMODE = true;
      if(watchFaceId > 1) watchFaceId = 0;
    }
    //DARKMODE = !DARKMODE;
    //MiteOS::refreshPage();
    return true;
  }

  if(buttonIndex == BTN_UP) {
    MiteOS::showPage(PAGE_SETTINGS);
  }
	return false;
}

