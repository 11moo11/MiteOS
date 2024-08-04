#ifndef ACTIVITY_PAGE_H
#define ACTIVITY_PAGE_H

#include "Page.h"

class ActivityPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
	private:
		void drawText();
		void drawStepList();
};

#endif