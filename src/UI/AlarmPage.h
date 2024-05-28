#ifndef ALARM_PAGE_H
#define ALARM_PAGE_H
#include "Page.h"

class AlarmPage : public Page {
	public:
		void initPage() override;
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
	private:
		void drawOverview();
		void drawIcons();
};
#endif