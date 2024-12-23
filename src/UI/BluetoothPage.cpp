
#include "BluetoothPage.h"
#include "../Managers/BluetoothManager.h"
#include "../Managers/PageManager.h"
#include "../MiteOS.h"

void BluetoothPage::drawPage() {
	const char *items[] = { 
		(btDeviceRegistered ? TXT_UNBOND : TXT_BOND),
		"Test",
		"Sync Settings"
	};
	showMenu(items, 3, true, TXT_BLUETOOTH);
}

bool BluetoothPage::onButtonPressed(uint8_t buttonIndex) {
	if(handleMenuButtons(buttonIndex)) return true;
	
	if(buttonIndex == BTN_CONFIRM) {
		if(pageData.menuIndex == 0) {
			if(btDeviceRegistered) {
				BluetoothManager::unbondDevice();
			}else{
				mDisplay.setFullWindow();
				mDisplay.fillScreen(BACKGROUND_COLOR);
				mDisplay.setTextColor(FOREGROUND_COLOR);
				mDisplay.setFont(&FreeMonoBold9pt7b);
				
				drawCentreString(TXT_BOND_WAIT, 100, 100, false);
				
				mDisplay.display();
				
				BluetoothManager::bondDevice();
			}
		}else if(pageData.menuIndex == 1 || pageData.menuIndex == 0) {
			BluetoothManager::connectDevice();
			uint8_t i = 0;
			while(i < 1) {
				i++;
				if(BluetoothManager::connected) {
					//BluetoothManager::requestNotifications();
					PhoneConnectionManager::RequestPlaybackInfo();
				}
			}
			delay(1000);
		}else if(pageData.menuIndex == 2) {
			BluetoothManager::connectDevice();
			if(BluetoothManager::connected) {
				PhoneConnectionManager::SyncConfiguration();
			}
		}
		
		return true;
	}
	return false;
}
