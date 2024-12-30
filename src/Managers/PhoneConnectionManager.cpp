#include "PhoneConnectionManager.h"

#include "BluetoothManager.h"
#include "HassManager.h"
#include "../Data/Configuration.h"
#include <JSON.h>
#include "../Data/Base64.hpp"
#include "../Managers/FileManager.h"

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

PlaybackInfo PhoneConnectionManager::RequestPlaybackInfo(bool cached) {
	PlaybackInfo pbi;
	
	JSONVar json;

	if(cached && FileManager::exists(PATH_PLAYBACK"dat")) {
		json = JSON.parse(FileManager::readFile(PATH_PLAYBACK"dat"));
	}else{
		BluetoothManager::sendCommand("GET_PLAYBACK_INFO=");

		if(BluetoothManager::lastResponse.length() > 0) {
			json = JSON.parse(BluetoothManager::lastResponse);
			FileManager::writeFile(PATH_PLAYBACK"dat", BluetoothManager::lastResponse);
		}
	}

	if(json.hasOwnProperty("title")) {
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
		if(json.hasOwnProperty("timestamp") && json.hasOwnProperty("playing")) { // Check if the time should have already passed, then just get the current data
			if(json["playing"]) {
				pbi.playing = true;
				long time_passed = max(0, NOW - ((unsigned long) json["timestamp"] + gmtTimeOffset));
				if(pbi.position + time_passed >= pbi.duration) {
					if(cached) {
						return RequestPlaybackInfo(false);
					}
				}
				pbi.position += time_passed;
			}
		}
		
		if(json.hasOwnProperty("art")) {
			String art = JSONVar::stringify(json["art"]);
			art = art.substring(1, art.length() - 1);
			
			// Ineffiecient but works i guess
			int size = decode_base64_length((const unsigned char*) art.c_str());
			int binary_length = decode_base64((const unsigned char*) art.c_str(), pbi.image);
		}
	}else{
		FileManager::deleteFile(PATH_PLAYBACK"dat");
	}
	
	return pbi;
}

void PhoneConnectionManager::SyncConfiguration() {
	printDebug("Requesting Configuration...");

	BluetoothManager::sendCommand("GET_CONFIGURATION=");

	Configuration::init();

	if(BluetoothManager::lastResponse.length() > 0) {
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		
		if(json.hasOwnProperty("hassUrl")) {
			if(json.hasOwnProperty("hassTkn")) {
				HassManager::setURL(json["hassUrl"]);
				HassManager::setToken(json["hassTkn"]);
				
				if(json.hasOwnProperty("entities")) {
					String entities = "";
					for(uint8_t i = 0; i < json["entities"].length(); i++) {
						if(i > 0) entities += ",";
						entities += (String) json["entities"][i]["name"];
						entities += ",";
						entities += (String) json["entities"][i]["key"];
					}
					printDebug(entities);
					HassManager::setEntities(entities);
				}
				Configuration::saveHassConfig();
			}
		}
		
		if(json.hasOwnProperty("totp")) {
			String tokens = "";
			for(uint8_t i = 0; i < json["totp"].length(); i++) {
				if(i > 0) tokens += ",";
				tokens += (String) json["totp"][i]["name"];
				tokens += ",";
				tokens += (String) json["totp"][i]["token"];
			}
			printDebug(tokens);
			Configuration::preferences.putString("totp", tokens);
		}
	}
}
void PhoneConnectionManager::SyncCalendar() {
	printDebug("Requesting Calendar...");

	BluetoothManager::sendCommand("GET_CALENDAR=");
	
	if(BluetoothManager::lastResponse.length() > 0) {
		FileManager::init();

		// Cleanup
		File root = LittleFS.open(PATH_CALENDAR);
		File file = root.openNextFile();

    	while(file) {
			String name = file.name();
			long startTime = name.substring(0, name.indexOf("_")).toInt();

			if(startTime < NOW) {
				String path = file.path();
				file.close();
				printDebug(path);
				LittleFS.remove(path);
			}
			file = root.openNextFile();
		}

		// Write all the data
		JSONVar json = JSON.parse(BluetoothManager::lastResponse);
		if(json.hasOwnProperty("events")) {
			printDebug(json["events"].length());
			if(json["events"].length() > 0) {
				for(uint16_t i = 0; i < json["events"].length(); i++) {
					printDebug(json["events"][i]);

					String id = JSON.stringify(json["events"][i]["startTime"]) + "_" + String((int) json["events"][i]["id"]);

					FileManager::writeFile(String(PATH_CALENDAR) + id, JSON.stringify(json["events"][i]));
				}
			}
		}
	}
}