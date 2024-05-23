#ifndef TIMER_PAGE_H
#define TIMER_PAGE_H
#include "Page.h"

class TimerPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
	private:
		void drawTime();
		void drawIcons();
		void startTimer();
};
#endif