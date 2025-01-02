#ifndef HASS_PAGE_H
#define HASS_PAGE_H

#include "Page.h"

class HassPage : public Page {
	public:
		String pageName() override { return TXT_HASS; };
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void displayPage();
		String getElement(uint8_t index);
		void loadData();
};

#endif