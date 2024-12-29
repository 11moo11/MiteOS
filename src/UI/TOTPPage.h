#ifndef TOTP_PAGE_H
#define TOTP_PAGE_H
#include "Page.h"

class TOTPPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() override { return false; };
};
#endif