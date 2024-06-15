#include "PlaybackPage.h"

#include "../Managers/PhoneConnectionManager.h"
#include "../MiteOS.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#include "../Images/menu_icons.h"

#define IMAGE_SCALE 2
#define IMAGE_BORDER ((DISPLAY_WIDTH - (PLAYBACK_IMAGE_SIZE * IMAGE_SCALE)) / 2)

void PlaybackPage::drawPage() {
	if(pageData.number1 != 1) {
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_SYNCING, 100, 100, false);
		
		mDisplay.display(true);
	}
	
	PlaybackInfo pbi = PhoneConnectionManager::RequestPlaybackInfo();
	mDisplay.fillScreen(BACKGROUND_COLOR);
	
	if(strlen(pbi.title) > 2) {
		pageData.number1 = 1;
		int16_t byteWidth = (48 + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t b = 0;
		
		mDisplay.startWrite();
		for (int16_t j = 0; j < 48; j++) {
			for (int16_t i = 0; i < 48; i++) {
				if (i & 7)
					b <<= 1;
				else
					b = pbi.image[j * byteWidth + i / 8];
				
				if(!(b & 0x80)) {
					mDisplay.fillRect(IMAGE_BORDER + (i * IMAGE_SCALE), 5 + (j * IMAGE_SCALE), IMAGE_SCALE, IMAGE_SCALE, FOREGROUND_COLOR);
				}
			}
		}
		mDisplay.endWrite();
		
		// Progress Bar
		mDisplay.drawRect(40, 105, 120, 12, FOREGROUND_COLOR);
		float percentage = ((float) pbi.position) / pbi.duration;
		mDisplay.fillRect(40, 105, 120 * percentage, 12, FOREGROUND_COLOR);
		
		// Progress Texts
		mDisplay.setFont(&FreeSans9pt7b);
		
		String curPos = "";
		curPos += String(pbi.position / 60);
		curPos += ":";
		if(pbi.position % 60 < 10) curPos += "0";
		curPos += String(pbi.position % 60);
		
		
		String durationStr = "";
		durationStr += String(pbi.duration / 60);
		durationStr += ":";
		if(pbi.duration % 60 < 10) durationStr += "0";
		durationStr += String(pbi.duration % 60);
		
		mDisplay.setCursor(3, 117);
		mDisplay.print(curPos);
		
		mDisplay.setCursor(163, 117);
		mDisplay.print(durationStr);
		
		
		// Song Info
		drawCentreString(pbi.title, 100, 140, true);
		drawCentreString(pbi.album, 100, 155, true);
		drawCentreString(pbi.artist, 100, 170, true);
	}else{
		pageData.number1 = 0;
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_NO_PLAYBACK, 100, 100, false);
	}
	drawIcons();
}

void PlaybackPage::drawIcons() {
	if(pageData.number1 == 1) {
		drawButtonIcon(BTN_UP, icon_next);
		drawButtonIcon(BTN_BACK, icon_previous);
		drawButtonIcon(BTN_CONFIRM, icon_play);
	}else{
		drawButtonIcon(BTN_CONFIRM, icon_refresh);
	}
	
	drawButtonIcon(BTN_DOWN, icon_down);
}

bool PlaybackPage::onButtonPressed(uint8_t buttonIndex) {
	if(pageData.number1 == 1) {
		if(buttonIndex == BTN_UP) {
			BluetoothManager::sendCommand("NEXT_PLAYBACK=");
			delay(500);
			return true;
		} else if(buttonIndex == BTN_BACK) {
			BluetoothManager::sendCommand("PREVIOUS_PLAYBACK=");
			delay(500);
			return true;
		} else if(buttonIndex == BTN_CONFIRM) {
			BluetoothManager::sendCommand("TOGGLE_PLAYBACK=");
			delay(500);
			return true;
		}
	}else{
		if(buttonIndex == BTN_CONFIRM) {
			return true; // Just refresh the page
		}
	}
	return false;
}