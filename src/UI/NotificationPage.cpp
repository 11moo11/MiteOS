#include "NotificationPage.h"

#include "../Managers/BluetoothManager.h"
#include "../MiteOS.h"

void NotificationPage::drawPage() {
	mDisplay.println("Waiting for Phone...");
	
	mDisplay.display(false);
	
	BluetoothManager::requestNotifications();
	
	mDisplay.clearScreen();
	
	mDisplay.println(BluetoothManager::tmp_buffer);
}