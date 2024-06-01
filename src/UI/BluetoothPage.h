#ifndef BLUETOOTH_PAGE_H
#define BLUETOOTH_PAGE_H

#include "Page.h"

class BluetoothPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
};

#endif