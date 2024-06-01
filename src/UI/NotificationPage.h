#ifndef NOTIFICATION_PAGE_H
#define NOTIFICATION_PAGE_H

#include "Page.h"

class NotificationPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawIcons();
};

#endif