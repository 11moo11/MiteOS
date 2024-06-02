
#include "BluetoothPage.h"
#include "../Managers/BluetoothManager.h"
#include "../Managers/PageManager.h"

void BluetoothPage::drawPage() {
	const char *items[] = { 
		(btDeviceRegistered ? TXT_REBOND : TXT_BOND),
		"Test"
	};
	showMenu(items, 2, true, TXT_BLUETOOTH);
}

bool BluetoothPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex)) return true;
	
	if(buttonIndex == BTN_CONFIRM) {
		if(pageData.menuIndex == 0) {
			BluetoothManager::bondDevice();
		}else if(pageData.menuIndex == 1 || pageData.menuIndex == 0) {
			BluetoothManager::connectDevice();
			uint8_t i = 0;
			while(i < 1) {
				i++;
				if(BluetoothManager::connected) {
					BluetoothManager::requestNotifications();
				}
			}
			delay(1000);
		}
		
		return true;
	}
	return false;
}
