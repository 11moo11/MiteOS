#include "AlertManager.h"
#include "../MiteOS.h"
#include <Fonts/FreeMonoBold18pt7b.h>
#include "../Images/menu_icons.h"

void AlertManager::showShortAlert() {

}

void AlertManager::showPermanentAlert(String title, const uint8_t icon[], String subtitle) {
	mDisplay.epd2.asyncPowerOn();
	mDisplay.setFullWindow();
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	
	mDisplay.setFont(&FreeMonoBold18pt7b);
	mDisplay.drawBitmap(80, 30, icon, 40, 40, FOREGROUND_COLOR);
	Page::drawCentreString(title, 100, 120, false);
	Page::drawCentreString(subtitle, 100, 150, false);
	
	Page::drawButtonIcon(BTN_DOWN, icon_checkmark);
	
	mDisplay.display(true);
	
	delay(500);
	
	pinMode(DOWN_BTN_PIN, INPUT);
	pinMode(UP_BTN_PIN, INPUT);
	pinMode(MENU_BTN_PIN, INPUT);
	pinMode(BACK_BTN_PIN, INPUT);
	
	AlertManager::vibMotor(100, 10);

	bool canceled = false;
	uint16_t counter = 0;
	while(!canceled) {
		counter++;
		if(counter % 10 == 0) {
			AlertManager::vibMotor(100, 2);
		}

		if(digitalRead(DOWN_BTN_PIN) == HIGH 
		|| digitalRead(UP_BTN_PIN) == HIGH
		|| digitalRead(MENU_BTN_PIN) == HIGH
		|| digitalRead(BACK_BTN_PIN) == HIGH) {
			canceled = true;
		}else if(counter > 600) { // Cancel after 60 seconds automatically
			canceled = true;
		}else{
			delay(100);
		}
	}

	mDisplay.setFullWindow();
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	mDisplay.display();
}

void AlertManager::vibMotor(uint8_t intervalMs, uint8_t length) {
	pinMode(VIB_MOTOR_PIN, OUTPUT);
	bool motorOn = false;
	
	if(length % 2 == 1) length++;
	
	for (int i = 0; i < length; i++) {
		motorOn = !motorOn;
		digitalWrite(VIB_MOTOR_PIN, motorOn);
		delay(intervalMs);
	}
}