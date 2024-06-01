
#include "BluetoothPage.h"
#include "../Managers/BluetoothManager.h"
#include "../Managers/PageManager.h"

void BluetoothPage::drawPage() {
	const char *items[] = { "Test" };
	showMenu(items, 1);
}

bool BluetoothPage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_CONFIRM) {
		BluetoothManager::initBLE();
		
		BluetoothManager::startBLEAdvertising();
		while(!BluetoothManager::connected) {
			//BluetoothManager::pServer->startAdvertising();
			delay(100);
		}
		delay(5000);
		while(BluetoothManager::connected) {
			BluetoothManager::test();
			delay(5000);
		}
		return true;
	}else if(buttonIndex == BTN_HOME) {
		PageManager::nextPage();
		return true;
	}
	return false;
}
