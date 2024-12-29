#ifndef CALENDAR_PAGE_H
#define CALENDAR_PAGE_H

#include "Page.h"

class CalendarPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawMonth();
        void drawAppointment();
};

#endif