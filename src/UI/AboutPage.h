#ifndef ABOUT_PAGE_H
#define ABOUT_PAGE_H
#include "Page.h"

class AboutPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() override { return false; };
};
#endif