#ifndef PHONE_CONNECTION_MANAGER_H
#define PHONE_CONNECTION_MANAGER_H

#include "Arduino.h"

#define NOTIFICATION_CNT 5

#define NOTIFICATION_APP_NAME_LENGTH 30
#define NOTIFICATION_TITLE_LENGTH 50
#define NOTIFICATION_MESSAGE_LENGTH 150

extern RTC_DATA_ATTR int8_t notificationRequeryCounter;

struct Notification {
	char app_name[NOTIFICATION_APP_NAME_LENGTH];
	char title[NOTIFICATION_TITLE_LENGTH];
	char message[NOTIFICATION_MESSAGE_LENGTH];
};

class PhoneConnectionManager {
	public:
		static void SyncNotifications(bool force = false);
		static Notification GetNotification(uint8_t index);
		static uint8_t GetNotificationCount();
		
		static void RequestPlaybackInfo();
};

#endif