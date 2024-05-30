#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>

#define BTN_BACK 1
#define BTN_MENU 2
#define BTN_UP 3
#define BTN_DOWN 4

#define BTN_TOGGLE_BTN BTN_BACK
#define BTN_NEXT_PAGE BTN_DOWN
#define BTN_CONFIRM BTN_MENU
#define BTN_HOME BTN_BACK

#define BTN_SINGLE_TAP 5
#define BTN_DOUBLE_TAP 6

typedef struct PageData {
  uint8_t pageIndex;
  uint8_t menuIndex;
  
  uint8_t subPageIndex;
  
  String string1;
  String string2;

  uint8_t number1;
  uint8_t number2;
  uint32_t number3;
} PageData;


class Page {
  public:
    Page() {};
    virtual void initPage() {};
		virtual void drawPage() {};
		virtual bool onButtonPressed(uint8_t buttonIndex) { return false; };
    virtual bool isPageable() { return true; };
		void showMenu(const char *menuItems[], uint8_t itemCount, bool partialRefresh = true, String title = "");
    bool handleMenuButtons(uint8_t buttonIndex);
    
		static void drawButtonIcon(uint8_t buttonIndex, const uint8_t bitmap[]);
    static void drawCentreString(String buf, int x, int y, bool textWrap = true);
		static void drawCentreString(const char *buf, int x, int y, bool textWrap = true);
};

#endif