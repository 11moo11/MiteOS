#include "Page.h"

class WatchfacePage : public Page {
	public:
		WatchfacePage() {};
		void drawPage();
		void onButtonPressed(uint8_t buttonIndex);
};