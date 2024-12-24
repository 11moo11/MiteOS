
#ifndef HASS_MANAGER_H
#define HASS_MANAGER_H

#include <Arduino.h>

class HassManager {
	private:
		static String url;
		static String token;
		static String entities;
	public:
		static void setURL(String url) { HassManager::url = url; }
		static void setToken(String token) { HassManager::token = token; }
		static void setEntities(String entities) { HassManager::entities = entities; }
		static String getURL() { return HassManager::url; }
		static String getToken() { return HassManager::token; }
		static String getEntities() { return HassManager::entities; }
		
		static bool toggleLight(String entity);
		static bool toggleSwitch(String entity);
		static bool toggle(String entity);
		static bool toggle(String type, String entity);
};
#endif