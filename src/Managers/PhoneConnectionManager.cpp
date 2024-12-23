#include "PhoneConnectionManager.h"

#include "BluetoothManager.h"
#include "HassManager.h"
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

PlaybackInfo PhoneConnectionManager::RequestPlaybackInfo() {
	BluetoothManager::sendCommand("GET_PLAYBACK_INFO=");
	
	PlaybackInfo pbi;
	
	if(BluetoothManager::lastResponse.length() > 0) {
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		
		
		if(json.hasOwnProperty("title")) {
			String str = JSONVar::stringify(json["title"]);
			str.substring(1, str.length() - 1).toCharArray(pbi.title, PLAYBACK_TEXT_LENGTH, 0);
		}
		if(json.hasOwnProperty("album")) {
			String str = JSONVar::stringify(json["album"]);
			str.substring(1, str.length() - 1).toCharArray(pbi.album, PLAYBACK_TEXT_LENGTH, 0);
		}
		if(json.hasOwnProperty("artist")) {
			String str = JSONVar::stringify(json["artist"]);
			str.substring(1, str.length() - 1).toCharArray(pbi.artist, PLAYBACK_TEXT_LENGTH, 0);
		}
		
		if(json.hasOwnProperty("position")) {
			pbi.position = long(json["position"]);
		}
		if(json.hasOwnProperty("duration")) {
			pbi.duration = long(json["duration"]);
		}
		
		if(json.hasOwnProperty("art")) {
			String art = JSONVar::stringify(json["art"]);
			art = art.substring(1, art.length() - 1);
			
			// Ineffiecient but works i guess
			int size = decode_base64_length((const unsigned char*) art.c_str());
			int binary_length = decode_base64((const unsigned char*) art.c_str(), pbi.image);
		}
	}
	return pbi;
}

void PhoneConnectionManager::SyncConfiguration() {
	printDebug("Requesting Configuration...");

	BluetoothManager::sendCommand("GET_CONFIGURATION=");

	PlaybackInfo pbi;
	
	if(BluetoothManager::lastResponse.length() > 0) {
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		
		if(json.hasOwnProperty("hassUrl")) {
			if(json.hasOwnProperty("hassTkn")) {
				HassManager::setURL(json["hassUrl"]);
				HassManager::setToken(json["hassTkn"]);
				
				Configuration::saveHassConfig();
			}
		}
	}
}