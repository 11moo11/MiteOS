#ifndef PHONE_CONNECTION_MANAGER_H
#define PHONE_CONNECTION_MANAGER_H

#include "Arduino.h"

#define NOTIFICATION_CNT 5

#define NOTIFICATION_APP_NAME_LENGTH 30
#define NOTIFICATION_TITLE_LENGTH 50
#define NOTIFICATION_MESSAGE_LENGTH 150

#define PLAYBACK_TEXT_LENGTH 30
#define PLAYBACK_IMAGE_SIZE 48

extern RTC_DATA_ATTR int8_t notificationRequeryCounter;

struct Notification {
	char app_name[NOTIFICATION_APP_NAME_LENGTH] = "";
	char title[NOTIFICATION_TITLE_LENGTH] = "";
	char message[NOTIFICATION_MESSAGE_LENGTH] = "";
};

struct PlaybackInfo {
	char title[PLAYBACK_TEXT_LENGTH] = "";
	char album[PLAYBACK_TEXT_LENGTH] = "";
	char artist[PLAYBACK_TEXT_LENGTH] = "";
	unsigned char image[PLAYBACK_IMAGE_SIZE * PLAYBACK_IMAGE_SIZE / 8 + 1];
	long position = 0;
	long duration = 0;
};

class PhoneConnectionManager {
	public:
		static void SyncNotifications(bool force = false);
		static Notification GetNotification(uint8_t index);
		static uint8_t GetNotificationCount();
		
		static PlaybackInfo RequestPlaybackInfo();
};

#endif