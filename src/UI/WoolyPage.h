#ifndef WOOLY_PAGE_H
#define WOOLY_PAGE_H

#include "Page.h"

class WoolyPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawWooly();
		void drawMainPage();
};

#endif