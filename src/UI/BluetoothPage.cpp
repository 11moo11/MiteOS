
#include "BluetoothPage.h"
#include "../Managers/BluetoothManager.h"
#include "../Managers/PageManager.h"
#include "../MiteOS.h"

void BluetoothPage::drawPage() {
	const char *items[] = { 
		(btDeviceRegistered ? TXT_UNBOND : TXT_BOND),
		TXT_SYNC" "TXT_SETTINGS,
		TXT_SYNC" "TXT_CALENDAR
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
		}else if(pageData.menuIndex == 1) {
			drawSuccess(PhoneConnectionManager::SyncConfiguration());
		}else if(pageData.menuIndex == 2) {
			drawSuccess(PhoneConnectionManager::SyncCalendar());
		}
		
		return true;
	}
	return false;
}
