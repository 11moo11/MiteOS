#include "WatchfacePage.h"
#include "../MiteOS.h"

#include "Watchfaces/7SEG.h"
#include "Watchfaces/BTTF.h"

RTC_DATA_ATTR uint8_t watchFaceId = 0;

void WatchfacePage::drawPage() {
		#ifdef DEBUG
		Serial.println("Drawing Watchface");
		#endif

    MiteOS::display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    MiteOS::display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    if(watchFaceId == 0) {
      SEG7().draw();
    }else if(watchFaceId == 1) {
      BTTF().draw();
    }
}

bool WatchfacePage::onButtonPressed(uint8_t buttonIndex) {
  if(accSensor.isDoubleClick()) {
    
  }else{
    if(buttonIndex == BTN_BACK) {
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
  }
	return false;
}

