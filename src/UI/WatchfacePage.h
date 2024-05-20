#ifndef WATCHFACEPAGE_H
#define WATCHFACEPAGE_H
#include "Page.h"

class WatchfacePage : public Page {
	public:
		WatchfacePage() {};
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
};
#endif