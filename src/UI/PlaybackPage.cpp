#include "PlaybackPage.h"

#include "../MiteOS.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "../Fonts/FreeSans8pt7b.h"
#include "../Fonts/FreeSans6pt7b.h"

#include "../Images/menu_icons.h"

#define IMAGE_SCALE 1
#define IMAGE_BORDER ((DISPLAY_WIDTH - (PLAYBACK_IMAGE_SIZE * IMAGE_SCALE)) / 2)
#define FORCE_REQUERY pageData.number3

void PlaybackPage::drawPage() {
	if(pageData.number1 != 1) {
		mDisplay.setFont(&FreeSansBold9pt7b);
		drawCentreString(TXT_SYNCING, 100, 100, false);
		
		mDisplay.display(true);
	}
	
	PlaybackInfo pbi = PhoneConnectionManager::RequestPlaybackInfo(FORCE_REQUERY == 0);
	FORCE_REQUERY = 0;
	mDisplay.fillScreen(BACKGROUND_COLOR);
	
	if(strlen(pbi.title) > 2) {
		pageData.number1 = 1;
		int16_t byteWidth = (PLAYBACK_IMAGE_SIZE + 7) / 8; // Bitmap scanline pad = whole byte
		uint8_t b = 0;
		
		mDisplay.startWrite();
		for (int16_t j = 0; j < PLAYBACK_IMAGE_SIZE; j++) {
			for (int16_t i = 0; i < PLAYBACK_IMAGE_SIZE; i++) {
				if (i & 7)
					b <<= 1;
				else
					b = pbi.image[j * byteWidth + i / 8];
				
				if((b & 0x80)) {
					mDisplay.fillRect(IMAGE_BORDER + (i * IMAGE_SCALE), 5 + (j * IMAGE_SCALE), IMAGE_SCALE, IMAGE_SCALE, GxEPD_WHITE);
				}else{
					mDisplay.fillRect(IMAGE_BORDER + (i * IMAGE_SCALE), 5 + (j * IMAGE_SCALE), IMAGE_SCALE, IMAGE_SCALE, GxEPD_BLACK);
				}
			}
		}
		mDisplay.endWrite();
		
		// Progress Bar
		float percentage = ((float) pbi.position) / pbi.duration;
		drawProgressBar(50, 105, 100, 12, percentage, FOREGROUND_COLOR);
		
		// Progress Texts
		mDisplay.setFont(&FreeSans8pt7b);
		
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
		
		mDisplay.setTextWrap(false);
		mDisplay.setCursor(3, 117);
		mDisplay.print(curPos);
		
		mDisplay.setCursor(153, 117);
		mDisplay.print(durationStr);
		
		// Song Info
		drawCentreString(pbi.title, 100, 140, true);
		mDisplay.setFont(&FreeSans6pt7b);
		drawCentreString(pbi.album, 100, 155, true);
		drawCentreString(pbi.artist, 100, 170, true);
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
	if(buttonIndex == BTN_BACK) {
		PageManager::showPage(GLOBAL_PAGE_APPS);
		return true;
	}
	return false;
}