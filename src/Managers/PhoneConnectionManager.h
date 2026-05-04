#ifndef PHONE_CONNECTION_MANAGER_H
#define PHONE_CONNECTION_MANAGER_H

#include "Arduino.h"

#define NOTIFICATION_CNT 20

#define NOTIFICATION_APP_NAME_LENGTH 30
#define NOTIFICATION_TITLE_LENGTH 50
#define NOTIFICATION_MESSAGE_LENGTH 150

#define PLAYBACK_TEXT_LENGTH 30
#define MAX_PLAYBACK_IMAGE_SIZE 100 // cant really increase it that much, as we dont have much ram left with bluetooth active :(

extern RTC_DATA_ATTR long lastNotificationCheck;

struct Notification {
	char app_name[NOTIFICATION_APP_NAME_LENGTH] = "";
	char title[NOTIFICATION_TITLE_LENGTH] = "";
	char message[NOTIFICATION_MESSAGE_LENGTH] = "";
};

struct PlaybackInfo {
	char title[PLAYBACK_TEXT_LENGTH] = "";
	char album[PLAYBACK_TEXT_LENGTH] = "";
	char artist[PLAYBACK_TEXT_LENGTH] = "";
	unsigned char image[MAX_PLAYBACK_IMAGE_SIZE * MAX_PLAYBACK_IMAGE_SIZE / 8 + 1];
	long position = 0;
	long duration = 0;
	bool playing = false;
	uint8_t size = MAX_PLAYBACK_IMAGE_SIZE;
};

class PhoneConnectionManager {
	public:
		static bool SyncNotifications(bool force = false);
		static Notification GetNotification(uint8_t index);
		static uint8_t GetNotificationCount();
		
		static PlaybackInfo RequestPlaybackInfo(bool cached = true);

		static bool GetGPSPosition();
		
		static bool SyncConfiguration();
		static bool SyncCalendar();
};

#endif