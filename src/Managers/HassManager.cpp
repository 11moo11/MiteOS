#include "HassManager.h"
#include <HTTPClient.h>
#include "NetworkManager.h"

String HassManager::url = "";
String HassManager::token = "";

bool HassManager::toggleLight(String entity) {
	return toggle("light", entity);
}

bool HassManager::toggleSwitch(String entity) {
	return toggle("switch", entity);
}

bool HassManager::toggle(String entity) {
	if(entity.startsWith("switch.")) {
		return toggleSwitch(entity);
	}else if(entity.startsWith("light.")) {
		return toggleLight(entity);
	}
	return false;
}

bool HassManager::toggle(String type, String entity) {
	if (NetworkManager::connectWifi()) {
		HTTPClient http;

		String serverPath = url + "/api/services/" + type + "/toggle";
		
		http.begin(serverPath.c_str());
		http.addHeader("Content-Type", F("application/json"));
		http.addHeader("Authorization", "Bearer " + token);
		
		int httpResponseCode = http.POST("{\"entity_id\": \"" + entity + "\"}");
		Serial.println(httpResponseCode);
		http.end();
		
		return httpResponseCode == 200;
	}
	return false;
}