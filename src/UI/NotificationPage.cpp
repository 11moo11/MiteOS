#include "NotificationPage.h"

#include "../Managers/PhoneConnectionManager.h"
#include "../MiteOS.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "../Images/menu_icons.h"

void NotificationPage::drawPage() {
	/*
	mDisplay.setCursor(0, 20);
	mDisplay.setFont(&FreeSans9pt7b);
	mDisplay.println("Waiting for Phone...");
	
	mDisplay.display(true);
	*/
	PhoneConnectionManager::SyncNotifications();
	
	mDisplay.fillScreen(BACKGROUND_COLOR);
	
	uint8_t cnt = PhoneConnectionManager::GetNotificationCount();
	if(pageData.subPageIndex >= cnt) pageData.subPageIndex = 0;
	
	if(cnt > 0) {
		mDisplay.setCursor(0, 40);
		Notification n = PhoneConnectionManager::GetNotification(pageData.subPageIndex);
		mDisplay.setTextWrap(true);
		mDisplay.setFont(&FreeSansBold9pt7b);
		mDisplay.println(n.app_name);
		mDisplay.println(n.title);
		
		mDisplay.setFont(&FreeSans9pt7b);
		mDisplay.println(n.message);
		drawCentreString(String(pageData.subPageIndex + 1) + " / " + String(cnt), 100, 15, false);
		
		drawIcons();
	}
}

void NotificationPage::drawIcons() {
	drawButtonIcon(BTN_BACK, icon_home);
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
	} else if(buttonIndex == BTN_HOME) {
		PageManager::showPage(GLOBAL_PAGE_WATCHFACE);
		return true;
	} else if(buttonIndex == BTN_CONFIRM) {
		PhoneConnectionManager::SyncNotifications(true);
		return true;
	}
}