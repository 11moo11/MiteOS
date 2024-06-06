#include "PhoneConnectionManager.h"

#include "BluetoothManager.h"
#include "../Data/Configuration.h"
#include <JSON.h>

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

#include "../../Base64.h"

unsigned char reverse(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void PhoneConnectionManager::RequestPlaybackInfo() {
	BluetoothManager::sendCommand("GET_PLAYBACK_INFO=");
	
	if(BluetoothManager::lastResponse.length() > 0) {
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		
		//if(json.hasOwnProperty("art")) {
			String str = JSON.stringify(json["art"]);
			str = str.substring(1, str.length() - 1);
			Serial.println(str);
			
			size_t size = base64::decodeLength(str.c_str());
			uint8_t* buf = new uint8_t[size];
			base64::decode(str.c_str(), buf);
			
			Serial.print("Size: ");
			Serial.println(size);
			for(int i = 0; i < size; i++) {
				buf[i] = reverse(buf[i]);
			}
			
			
			mDisplay.epd2.asyncPowerOn();
			mDisplay.setFullWindow();
			mDisplay.fillScreen(BACKGROUND_COLOR);
			mDisplay.setTextColor(FOREGROUND_COLOR);
			mDisplay.clearScreen();
			mDisplay.drawBitmap(50, 50, buf, 100, 100, FOREGROUND_COLOR);
			
			mDisplay.display(true);
			
			delay(5000);
		//}
	}
	
}