#include "Page.h"

#include "../MiteOS.h"
#include <GxEPD2_EPD.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "../Images/menu_icons.h"
#include "glcdfont.c"

void Page::showMenu(const char *menuItems[], uint8_t itemCount, bool partialRefresh, String title) {
	mDisplay.setFullWindow();
	mDisplay.fillScreen(BACKGROUND_COLOR);
	mDisplay.setFont(&FreeMonoBold9pt7b);
	
	if(title.length() > 0) {
		mDisplay.setRotation(3);
		drawCentreString(title, 100, 13, false);
		mDisplay.setRotation(0);
	}
	
	int16_t x1, y1;
	uint16_t w, h;
	int16_t yPos;
	
	if(pageData.menuIndex < 0 || pageData.menuIndex > 250) pageData.menuIndex = itemCount - 1;
	else if(pageData.menuIndex >= itemCount) pageData.menuIndex = 0;
	
	for (int i = 0; i < itemCount; i++) {
		yPos = MENU_HEIGHT + (MENU_HEIGHT * i);
		mDisplay.setCursor(25, yPos);
		if (i == pageData.menuIndex) {
			mDisplay.getTextBounds(menuItems[i], 25, yPos, &x1, &y1, &w, &h);
			mDisplay.fillRect(x1 - 1, y1 - 10, 150, h + 15, FOREGROUND_COLOR);
			mDisplay.setTextColor(BACKGROUND_COLOR);
			print(menuItems[i], BACKGROUND_COLOR, FOREGROUND_COLOR);
		} else {
			mDisplay.setTextColor(FOREGROUND_COLOR);
			print(menuItems[i]);
		}
	}
	
	drawButtonIcon(BTN_BACK, icon_left);
	drawButtonIcon(BTN_MENU, icon_checkmark);
	drawButtonIcon(BTN_UP,   icon_up);
	drawButtonIcon(BTN_DOWN, icon_down);
}

bool Page::handleMenuButtons(uint8_t buttonIndex) {
	if(buttonIndex == BTN_UP) {
		pageData.menuIndex -= 1;
		return true;
	}else if(buttonIndex == BTN_DOWN) {
		pageData.menuIndex += 1;
		return true;
	}
	return false;
}


void Page::drawButtonIcon(uint8_t buttonIndex, const uint8_t bitmap[]) {
	switch(buttonIndex) {
		case BTN_BACK:
			mDisplay.drawBitmap(5, 5, bitmap, 20, 20, FOREGROUND_COLOR);
			break;
		
		case BTN_MENU:
			mDisplay.drawBitmap(5, 175, bitmap, 20, 20, FOREGROUND_COLOR);
			break;

		case BTN_UP:
			mDisplay.drawBitmap(175, 5, bitmap, 20, 20, FOREGROUND_COLOR);
			break;
		
		case BTN_DOWN:
			mDisplay.drawBitmap(175, 175, bitmap, 20, 20, FOREGROUND_COLOR);
			break;
	}
}

void Page::drawCentreString(String buf, int x, int y, bool textWrap) {
	drawCentreString(buf.c_str(), x, y);
}

void Page::drawCentreString(const char *buf, int x, int y, bool textWrap) {
	int16_t x1, y1;
	uint16_t w, h;
	
	String str = buf; str.replace("ä", "a"); str.replace("Ä", "A"); str.replace("ö", "Ö"); str.replace("o", "O"); str.replace("ü", "u"); str.replace("Ü", "U");
	
	mDisplay.setTextWrap(false);
	mDisplay.getTextBounds(str, x, y, &x1, &y1, &w, &h); //calc width of new string
	mDisplay.setCursor(x - w / 2, y);
	
	print(buf);
}

size_t utf8_strlen(const char * str) {
	int len = 0;
	while (*str) len += (*str++ & 0xc0) != 0x80;
	return len;
}


void Page::println(const char *buf) {
	println(buf, FOREGROUND_COLOR, BACKGROUND_COLOR);
}

void Page::println(const char *buf, uint16_t fgc, uint16_t bgc) {
	print(buf, fgc, bgc);
	mDisplay.println();
}

void Page::print(const char *buf) {
	print(buf, FOREGROUND_COLOR, BACKGROUND_COLOR);
}
/*
Function to print a String with UTF8 encoding
Replaces umlauts if possible otherwise just uses the non Umlaut Character as replacement
There is probably an easier and more performant way to do it, but i couldnt find it :/
*/
void Page::print(const char *buf, uint16_t fgc, uint16_t bgc) {
	if(utf8_strlen(buf) == strlen(buf)) { // No need to loop through each character if there is no utf8 involved
		mDisplay.print(buf);
		return;
	}
	
	int character = 0;
	for(int i = 0; i < strlen(buf); i++) {
		if(buf[i] >= 0b11000000) {
			character |= buf[i];
			character = character << 8;
			continue;
		}
		character |= buf[i];
		
		char replaceChar = buf[i];
		switch(character) {
			case 'Ä': replaceChar = 0x8E; break;
			case 'ä': replaceChar = 0x84; break;
			case 'Ö': replaceChar = 0x99; break;
			case 'ö': replaceChar = 0x94; break;
			case 'Ü': replaceChar = 0x9A; break;
			case 'ü': replaceChar = 0x81; break;
			case 'ß': replaceChar = 0xE1; break;
			default: break;
		}
		
		if(replaceChar == buf[i]) {
			mDisplay.print(replaceChar);
		}else{
			int16_t x1, y1;
			uint16_t w, h;
			mDisplay.getTextBounds(String(replaceChar).c_str(), (int16_t) 50, (int16_t) 50, &x1, &y1, &w, &h);
			if(w > 0) { // If the character is available in the font, use it
				mDisplay.print(replaceChar);
			} else { // Otherwise fallback to the default font
				mDisplay.getTextBounds("O", (int16_t) 50, (int16_t) 50, &x1, &y1, &w, &h); // Get the current font's size and resize the default one to match somewhat, using O as a big character to better match the size
				
				uint8_t size_x = h / 7, size_y = h / 7;
				uint8_t x = mDisplay.getCursorX();
				uint8_t y = mDisplay.getCursorY() - size_y * 8 + 3;
				
				mDisplay.startWrite();
				for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
					uint8_t line = pgm_read_byte(&font[replaceChar * 5 + i]);
					for (int8_t j = 0; j < 8; j++, line >>= 1) {
						if (size_x == 1 && size_y == 1)
							mDisplay.writePixel(x + i, y + j, (line & 1 ? fgc : bgc));
						else
							mDisplay.writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, (line & 1 ? fgc : bgc));
					}
				}
				if (size_x == 1 && size_y == 1)
					mDisplay.writeFastVLine(x + 5, y, 8, bgc);
				else
					mDisplay.writeFillRect(x + 5 * size_x, y, size_x, 8 * size_y, bgc);
				
				mDisplay.endWrite();
				
				mDisplay.setCursor(x + size_x * 6, mDisplay.getCursorY()); // Advance x one char
				
				//mDisplay.print(replaceChar);
			}
		}
		
		character = 0;
	}
}

void Page::drawCircle(int centerX, int centerY, int diameter, int lineThickness, int percentage) {
	Page::drawCircle(centerX, centerY, diameter, lineThickness, percentage, FOREGROUND_COLOR);
}

void Page::drawCircle(int centerX, int centerY, int diameter, int lineThickness, int percentage, uint16_t color) {
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
				mDisplay.drawPixel(centerX + y, centerY + diameter - x, color);
			}
		}
	}
	
	for(double angle = 0; angle <= 2 * M_PI; angle += M_PI / 10) {
		for (int r = radius - lineThickness; r <= radius; ++r) {
			int x = static_cast<int>(radius + r * cos(angle));
			int y = static_cast<int>(radius + r * sin(angle));
			mDisplay.drawPixel(centerX + y, centerY + diameter - x, color);
		}
	}
}