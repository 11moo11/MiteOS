#ifndef APP_PAGE_H
#define APP_PAGE_H

#include "Page.h"

class AppPage : public Page {
	public:
		String pageName() override { return TXT_APPS; };
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void displayPage();
};

#endif