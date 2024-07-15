
#ifndef HASS_MANAGER_H
#define HASS_MANAGER_H

#include <Arduino.h>

class HassManager {
	private:
		static String url;
		static String token;
	public:
		static void setURL(String url) { HassManager::url = url; }
		static void setToken(String token) { HassManager::token = token; }
		
		static bool toggleLight(String entity);
		static bool toggleSwitch(String entity);
		static bool toggle(String entity);
		static bool toggle(String type, String entity);
};
#endif