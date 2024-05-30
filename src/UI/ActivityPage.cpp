#include "ActivityPage.h"

#include "../MiteOS.h"
#include "../Fonts/Seven_Seg18pt7b.h"
#include "../Fonts/icons.h"

void ActivityPage::drawPage() {
	drawCircle();
	drawText();
}

void ActivityPage::drawCircle() {
	int lineThickness = 10;
	int diameter = 160;
	int percentage = (ActivityManager::getStepCount() / 8000.0) * 100;
	int centerX = 20;
	int centerY = 20;
	
	percentage = max(0, min(100, percentage));
	
	int radius = diameter / 2;
	double angleStep = 0.007; // Schrittweite für 1 Grad in Bogenmaß
	double endAngle = 2 * M_PI * (percentage / 100.0); // Endwinkel basierend auf dem Prozentsatz
	
	if(percentage > 0) {
		// Generierung der Kreislinie
		for (double angle = 0; angle <= endAngle; angle += angleStep) {
			for (int r = radius - lineThickness; r <= radius; ++r) {
				int x = static_cast<int>(radius + r * cos(angle));
				int y = static_cast<int>(radius + r * sin(angle));
				mDisplay.drawPixel(centerX + y, centerY + diameter - x, FOREGROUND_COLOR);
			}
		}
	}
	
	for(double angle = 0; angle <= 2 * M_PI; angle += M_PI / 10) {
		for (int r = radius - lineThickness; r <= radius; ++r) {
			int x = static_cast<int>(radius + r * cos(angle));
			int y = static_cast<int>(radius + r * sin(angle));
			mDisplay.drawPixel(centerX + y, centerY + diameter - x, FOREGROUND_COLOR);
		}
	}
}

void ActivityPage::drawText() {
	mDisplay.setFont(&Seven_Seg18pt7b);
	drawCentreString(String(ActivityManager::getStepCount()), 100, 100, false);
	drawCentreString(ActivityManager::getWalkedDistanceStr(), 100, 130, false);
	mDisplay.drawBitmap(90, 50, steps, 19, 23, FOREGROUND_COLOR);
}