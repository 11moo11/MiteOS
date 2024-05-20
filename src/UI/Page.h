#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>

#define BTN_BACK 1
#define BTN_MENU 2
#define BTN_UP 3
#define BTN_DOWN 4

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
    Page() {};
		virtual void drawPage() {};
		virtual bool onButtonPressed(uint8_t buttonIndex) { return false; };
		void showMenu(char *menuItems[], bool partialRefresh);
};

#endif