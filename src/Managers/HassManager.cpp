#include "HassManager.h"
#include <HTTPClient.h>
#include "WifiConnectionManager.h"
#include "../Data/Configuration.h"
#include "../MiteOS.h"
#include "../WebRequest/WebRequest.h"

String HassManager::url = "";
String HassManager::token = "";
String HassManager::entities = "";

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
	}else if(entity.startsWith("input_boolean.")) {
		return toggle("input_boolean", entity);
	}
	return false;
}

bool HassManager::toggle(String type, String entity) {
	if(HassManager::url.length() == 0) {
		Configuration::loadHassConfig();
	}
	if(HassManager::url.length() == 0) return false;
	
	String serverPath = url + "/api/services/" + type + "/toggle";
	
	WebRequestData data = WebRequest::POST(serverPath, "{\"entity_id\": \"" + entity + "\"}", "Bearer " + token);
	
	return data.isSuccess();
}