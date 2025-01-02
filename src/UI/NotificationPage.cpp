#include "NotificationPage.h"

#include "../Managers/PhoneConnectionManager.h"
#include "../MiteOS.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "../Images/menu_icons.h"

void NotificationPage::drawPage() {
	if(notificationRequeryCounter <= 0) {
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_SYNCING, 100, 100, false);
		
		mDisplay.display(true);
		
		PhoneConnectionManager::SyncNotifications();
	}
	
	mDisplay.fillScreen(BACKGROUND_COLOR);
	
	uint8_t cnt = PhoneConnectionManager::GetNotificationCount();
	if(pageData.subPageIndex >= cnt) pageData.subPageIndex = 0;
	
	if(cnt > 0) {
		mDisplay.setCursor(0, 40);
		Notification n = PhoneConnectionManager::GetNotification(pageData.subPageIndex);
		mDisplay.setTextWrap(true);
		mDisplay.setFont(&FreeSansBold9pt7b);
		println(n.app_name);
		println(n.title);
		
		mDisplay.setFont(&FreeSans9pt7b);
		println(n.message);
		
		drawCentreString(String(pageData.subPageIndex + 1) + " / " + String(cnt), 100, 15, false);
		
		drawIcons();
	}else{
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_NO_NOTIFICATION, 100, 100, true);
		drawButtonIcon(BTN_BACK, icon_exit);
		drawButtonIcon(BTN_CONFIRM, icon_refresh);
	}
}

void NotificationPage::drawIcons() {
	drawButtonIcon(BTN_BACK, icon_exit);
	drawButtonIcon(BTN_UP, icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
	drawButtonIcon(BTN_CONFIRM, icon_refresh);
}

bool NotificationPage::onButtonPressed(uint8_t buttonIndex) {
	if(buttonIndex == BTN_UP) {
		if(pageData.subPageIndex == 0) pageData.subPageIndex = PhoneConnectionManager::GetNotificationCount();
		pageData.subPageIndex--;
		return true;
	}else if(buttonIndex == BTN_DOWN) {
		pageData.subPageIndex++;
		if(pageData.subPageIndex >= PhoneConnectionManager::GetNotificationCount()) pageData.subPageIndex = 0;
		return true;
	} else if(buttonIndex == BTN_CONFIRM) {
		notificationRequeryCounter = -1;
		return true;
	}
	return false;
}