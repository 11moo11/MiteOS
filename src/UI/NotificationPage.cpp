#include "NotificationPage.h"

#include "../Managers/PhoneConnectionManager.h"
#include "../MiteOS.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

void NotificationPage::drawPage() {
	mDisplay.setCursor(0, 20);
	mDisplay.setFont(&FreeSans9pt7b);
	mDisplay.println("Waiting for Phone...");
	
	mDisplay.display(true);
	
	PhoneConnectionManager::SyncNotifications();
	
	mDisplay.fillScreen(BACKGROUND_COLOR);
	
	mDisplay.setCursor(0, 20);
	mDisplay.println(PhoneConnectionManager::GetNotificationCount());
	
	Notification n = PhoneConnectionManager::GetNotification(0);
	mDisplay.setTextWrap(true);
	mDisplay.setFont(&FreeSansBold9pt7b);
	mDisplay.println(n.app_name);
	mDisplay.println(n.title);
	
	mDisplay.setFont(&FreeSans9pt7b);
	mDisplay.println(n.message);
}