#include "StarryHorizonWatchface.h"

struct xyPoint rotatePointAround(int x, int y, int ox, int oy, double angle) {
  // rotate X,Y point around given origin point by a given angle
  // based on https://gist.github.com/LyleScott/e36e08bfb23b1f87af68c9051f985302#file-rotate_2d_point-py-L38
  double qx = (double)ox + (cos(angle) * (double)(x - ox)) + (sin(angle) * (double)(y - oy));
  double qy = (double)oy + (-sin(angle) * (double)(x - ox)) + (cos(angle) * (double)(y - oy));
  struct xyPoint newPoint;
  newPoint.x = (int)qx;
  newPoint.y = (int)qy;
  return newPoint;
}

void StarryHorizonWatchface::draw() {
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.fillCircle(100, horizonY + planetR, planetR, FOREGROUND_COLOR);
	drawGrid();
	drawStars(STARS);
	drawTime();
	drawDate();
}

void StarryHorizonWatchface::drawGrid() {
	int prevY = horizonY;
	for(int i = 0; i < 40; i+= 1) {
	int y = prevY + int(abs(sin(double(i) / 10) * 10));
	if(y <= 200) {
		mDisplay.drawFastHLine(0, y, 200, BACKGROUND_COLOR);
	}
	prevY = y;
	}
	int vanishY = horizonY - 25;
	for (int x = -230; x < 430; x += 20) {
		mDisplay.drawLine(x, 200, 100, vanishY, BACKGROUND_COLOR);
	}
}
void StarryHorizonWatchface::drawStars(const Star stars[]) {
	// draw field of stars
	// rotate stars so that they make an entire revolution once per hour
	int minute = (int)MiteOS::currentTime.Minute;
	double minuteAngle = ((2.0 * M_PI) / 60.0) * (double)minute;
//          printf("Minute %d, angle %f\n", (int)currentTime.Minute, minuteAngle);

	for(int starI = 0; starI < STAR_COUNT; starI++) {
	int starX = stars[starI].x;
	int starY = stars[starI].y;
	int starR = stars[starI].r;

	struct xyPoint rotated = rotatePointAround(starX, starY, 100, 100, minuteAngle);
	if(rotated.x < 0 || rotated.y < 0 || rotated.x > 200 || rotated.y > horizonY) {
		continue;
	}
	if(starR == 0) {
		mDisplay.drawPixel(rotated.x, rotated.y, FOREGROUND_COLOR);
	} else {
		mDisplay.fillCircle(rotated.x, rotated.y, starR, FOREGROUND_COLOR);
	}
	}
}
void StarryHorizonWatchface::drawTime() {
	mDisplay.setFont(&MADE_Sunflower_PERSONAL_USE39pt7b);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	mDisplay.setTextWrap(false);
	char* timeStr;
	asprintf(&timeStr, "%d:%02d", MiteOS::currentTime.Hour, MiteOS::currentTime.Minute);
	Page::drawCentreString(timeStr, 100, 115, false);
	free(timeStr);
}

void StarryHorizonWatchface::drawDate() {
	String monthStr = monthShortStr(MiteOS::currentTime.Month);
	String dayOfWeek = dayShortStr(MiteOS::currentTime.Wday);
	mDisplay.setFont(&FreeSansBold9pt7b);
	mDisplay.setTextColor(FOREGROUND_COLOR);
	mDisplay.setTextWrap(false);
	char* dateStr;
	asprintf(&dateStr, "%s %s %d", dayOfWeek.c_str(), monthStr.c_str(), MiteOS::currentTime.Day);
	Page::drawCentreString(dateStr, 100, 140, true);
	free(dateStr);
}
