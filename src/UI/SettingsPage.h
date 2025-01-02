#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H
#include "Page.h"
#include <WiFiManager.h>

class SettingsPage : public Page {
	public:
		String pageName() override { return TXT_SETTINGS; };
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() override { return false; };
	private:
		void setTime();
		void showAccelerometer();
};
#endif