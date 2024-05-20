#ifndef PAGE_H
#define PAGE_H

#include <GxEPD2_EPD.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <config.h>
#include "../MiteOS.h"

#define BTN_BACK
#define BTN_MENU
#define BTN_UP
#define BTN_DOWN

typedef struct PageData {
  uint8_t pageIndex;
  uint8_t menuIndex;

  String string1;
  String string2;

  uint8_t number1;
  uint8_t number2;
  uint32_t number3;
} PageData;


class Page {
	public:
		virtual void drawPage();
		virtual void onButtonPressed(uint8_t buttonIndex);
		void showMenu(char *menuItems[], byte menuIndex, bool partialRefresh);
};

#endif