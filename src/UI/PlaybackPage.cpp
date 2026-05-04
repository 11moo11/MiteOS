#include "PlaybackPage.h"

#include "../MiteOS.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../Fonts/FreeSans8pt7b.h"
#include "../Fonts/FreeSans6pt7b.h"

#include "../Images/menu_icons.h"

#define IMAGE_SCALE 1
#define FORCE_REQUERY pageData.number3

void PlaybackPage::initPage() {
	FORCE_REQUERY = 1;
}

void PlaybackPage::drawPage() {
	if(pageData.number1 != 1) {
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_SYNCING, 100, 100, false);
		
		mDisplay.display(true);
	}
	
	PlaybackInfo pbi = PhoneConnectionManager::RequestPlaybackInfo(FORCE_REQUERY == 0);
	printDebug("meep1");
	FORCE_REQUERY = 0;
	mDisplay.fillScreen(BACKGROUND_COLOR);
	
	if(strlen(pbi.title) > 2) {
		pageData.number1 = 1;
		int16_t byteWidth = (pbi.size + 7) / 8; // Bitmap scanline pad = whole byte
		int16_t imageBorder = ((DISPLAY_WIDTH - (pbi.size  * IMAGE_SCALE)) / 2);
		uint8_t b = 0;
		
		if(pbi.size > 0) {
			mDisplay.startWrite();
			for (int16_t j = 0; j < pbi.size; j++) {
				for (int16_t i = 0; i < pbi.size; i++) {
					if (i & 7)
						b <<= 1;
					else
						b = pbi.image[j * byteWidth + i / 8];
					
					if((b & 0x80)) {
						mDisplay.fillRect(imageBorder + (i * IMAGE_SCALE), 20 + (j * IMAGE_SCALE), IMAGE_SCALE, IMAGE_SCALE, GxEPD_WHITE);
					}else{
						mDisplay.fillRect(imageBorder + (i * IMAGE_SCALE), 20 + (j * IMAGE_SCALE), IMAGE_SCALE, IMAGE_SCALE, GxEPD_BLACK);
					}
				}
			}
			mDisplay.endWrite();
		}
		
		// Progress Bar
		float percentage = 0;
		if(pbi.duration > 0)
			percentage = ((float) pbi.position) / pbi.duration;
		drawProgressBar(30, 180, 140, 12, percentage, FOREGROUND_COLOR);
		
		// Progress Texts
		mDisplay.setFont(&FreeSans8pt7b);
		
		if(pbi.duration > 0 && pbi.position > pbi.duration) {
			pbi.position = pbi.duration;
			FORCE_REQUERY = 1;
		}

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
		
		drawCentreString(curPos, 25, 170);
		drawCentreString(durationStr, 175, 170);
		
		// Song Info
		drawCentreString(pbi.title, 100, 150, true);
		mDisplay.setFont(&FreeSans6pt7b);
		//drawCentreString(pbi.album, 100, 155, true);
		drawCentreString(pbi.artist, 100, 169, true);
	}else{
		pageData.number1 = 0;
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_NO_PLAYBACK, 100, 100, false);
	}
	drawIcons(pbi);
}

void PlaybackPage::drawIcons(PlaybackInfo pbi) {
	if(pageData.number1 == 1) {
		drawButtonIcon(BTN_DOWN, icon_next);
		drawButtonIcon(BTN_MENU, icon_previous);
		drawButtonIcon(BTN_UP, icon_play);
	}else{
		drawButtonIcon(BTN_CONFIRM, icon_refresh);
	}
	
	drawButtonIcon(BTN_BACK, icon_exit);
}

bool PlaybackPage::onButtonPressed(uint8_t buttonIndex) {
	if(pageData.number1 == 1) {
		if(buttonIndex == BTN_DOWN) {
			BluetoothManager::sendCommand("NEXT_PLAYBACK=");
			FORCE_REQUERY = 1;
			delay(500);
			return true;
		} else if(buttonIndex == BTN_MENU) {
			BluetoothManager::sendCommand("PREVIOUS_PLAYBACK=");
			FORCE_REQUERY = 1;
			delay(500);
			return true;
		} else if(buttonIndex == BTN_UP) {
			BluetoothManager::sendCommand("TOGGLE_PLAYBACK=");
			FORCE_REQUERY = 1;
			delay(500);
			return true;
		}
	}else{
		if(buttonIndex == BTN_CONFIRM) {
			FORCE_REQUERY = 1;
			return true; // Just refresh the page
		}
	}
	
	return false;
}