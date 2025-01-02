#ifndef TIMER_PAGE_H
#define TIMER_PAGE_H
#include "Page.h"

class TimerPage : public Page {
	public:
		String pageName() override { return TXT_TIMER; };
		void initPage() override;
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawTime();
		void drawIcons();
		void startTimer();
};
#endif