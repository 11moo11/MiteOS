#ifndef WATCHFACEPAGE_H
#define WATCHFACEPAGE_H
#include "Page.h"
#include "Watchfaces/Watchface.h"

#define WATCHFACE_COUNT 8

class WatchfacePage : public Page {
	public:
		WatchfacePage() {};
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		Watchface* getWatchface(uint8_t index);
};

extern RTC_DATA_ATTR uint8_t watchFaceId;
#endif