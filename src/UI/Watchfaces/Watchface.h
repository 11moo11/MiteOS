#ifndef WATCHFACE_H
#define WATCHFACE_H
#include "../../MiteOS.h"

extern RTC_DATA_ATTR uint8_t watchFaceId;

class Watchface {
	public:
		virtual void draw();
		virtual String watchfaceName() { return "---"; }
};
#endif