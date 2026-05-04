#ifndef WATCHFACE_H
#define WATCHFACE_H
#include "../../MiteOS.h"

extern RTC_DATA_ATTR uint8_t watchFaceId;

class Watchface {
	public:
		virtual void draw();
		virtual String watchfaceName() { return "---"; }
		static String getTimeStr() {
			char buf[50];
			time_t base = makeTime(MiteOS::currentTime);
			struct tm* tm = localtime(&base);
			if(HOUR_12_24==12){
				strftime(buf, sizeof(buf), "%I:%M", tm);
			} else {
				strftime(buf, sizeof(buf), "%H:%M", tm);
			}
			return String(buf);
		}
};
#endif