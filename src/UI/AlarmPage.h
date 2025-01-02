#ifndef ALARM_PAGE_H
#define ALARM_PAGE_H
#include "Page.h"


class AlarmPage : public Page {
	public:
		String pageName() override { return TXT_ALARM; };
		void initPage() override;
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawOverview();
		void drawIcons();
		void setTime();
};
#endif