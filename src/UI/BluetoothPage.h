#ifndef BLUETOOTH_PAGE_H
#define BLUETOOTH_PAGE_H

#include "Page.h"

class BluetoothPage : public Page {
	public:
		String pageName() override { return TXT_BLUETOOTH; };
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
};

#endif