#ifndef ACTIVITY_PAGE_H
#define ACTIVITY_PAGE_H

#include "Page.h"

class ActivityPage : public Page {
	public:
		String pageName() override { return TXT_ACTIVITY; };
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
	private:
		void drawText();
		void drawStepList();
};

#endif