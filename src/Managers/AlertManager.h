#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H
#include <Arduino.h>

class AlertManager {
	public:
		static void showShortAlert(); // TODO
		static void showPermanentAlert(String title, const uint8_t icon[], String subtitle = "");
		static void vibMotor(uint8_t intervalMs = 100, uint8_t length = 20);
};

#endif