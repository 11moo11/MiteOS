#include "PhoneConnectionManager.h"

#include "BluetoothManager.h"
#include "../Data/Configuration.h"
#include <JSON.h>
#include "../Data/Base64.hpp"

RTC_DATA_ATTR int8_t notificationRequeryCounter = -1;

void PhoneConnectionManager::SyncNotifications(bool force) {
	if(force) notificationRequeryCounter = -1;
	
	if(notificationRequeryCounter > 0) {
		notificationRequeryCounter--;
		return;
	}
	notificationRequeryCounter = 15;
	
	BluetoothManager::requestNotifications();
	
	if(!BluetoothManager::connected) return;
	
	if(BluetoothManager::lastResponse.length() > 0) {
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		
		Configuration::init();
		
		if(json.hasOwnProperty("count")) {
			uint8_t count = uint8_t(json["count"]);
			
			Notification n;
			
			for(uint8_t i = 0; i < NOTIFICATION_CNT; i++) {
				if(count > i) {
					String str = JSONVar::stringify(json["nBundleList"][i]["appName"]);
					str.substring(1, str.length() - 1).toCharArray(n.app_name, NOTIFICATION_APP_NAME_LENGTH, 0);
					
					str = JSONVar::stringify(json["nBundleList"][i]["title"]);
					str.substring(1, str.length() - 1).toCharArray(n.title, NOTIFICATION_TITLE_LENGTH, 0);
					
					str = JSONVar::stringify(json["nBundleList"][i]["text"]);
					str.substring(1, str.length() - 1).toCharArray(n.message, NOTIFICATION_MESSAGE_LENGTH, 0);
				}
				Configuration::saveNotification(i, n);
			}
			
			Configuration::preferences.putUInt("notiCnt", count);
		}
	}
}

Notification PhoneConnectionManager::GetNotification(uint8_t index) {
	return Configuration::loadNotification(index);
}

uint8_t PhoneConnectionManager::GetNotificationCount() {
	Configuration::init();
	return min((uint8_t) NOTIFICATION_CNT, (uint8_t) Configuration::preferences.getUInt("notiCnt", (uint8_t) 0));
}

void PhoneConnectionManager::RequestPlaybackInfo() {
	BluetoothManager::sendCommand("GET_PLAYBACK_INFO=");
	
	if(BluetoothManager::lastResponse.length() > 0) {
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		
		if(json.hasOwnProperty("art")) {
			String art = JSONVar::stringify(json["art"]);
			art = art.substring(1, art.length() - 1);
			
			// Ineffiecient but works i guess
			int size = decode_base64_length((const unsigned char*) art.c_str());
			unsigned char binary[size];
			int binary_length = decode_base64((const unsigned char*) art.c_str(), binary);
			
			
			mDisplay.epd2.asyncPowerOn();
			mDisplay.setFullWindow();
			mDisplay.fillScreen(BACKGROUND_COLOR);
			mDisplay.setTextColor(FOREGROUND_COLOR);
			
			int16_t byteWidth = (48 + 7) / 8; // Bitmap scanline pad = whole byte
			uint8_t b = 0;
			
			mDisplay.startWrite();
			for (int16_t j = 0; j < 48; j++) {
				for (int16_t i = 0; i < 48; i++) {
					if (i & 7)
						b <<= 1;
					else
						b = binary[j * byteWidth + i / 8];
					
					if(b & 0x80) {
						mDisplay.fillRect(4 + (i * 4), 4 + (j * 4), 4, 4, FOREGROUND_COLOR);
					}
				}
			}
			mDisplay.endWrite();
			
			mDisplay.display(true);
			
			delay(15000);
		}
	}
	
}