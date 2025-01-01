#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>
#include "../Images/menu_icons.h"

#define BTN_BACK 1
#define BTN_MENU 2
#define BTN_UP 3
#define BTN_DOWN 4

#define BTN_TOGGLE_BTN BTN_BACK
#define BTN_NEXT_PAGE BTN_DOWN
#define BTN_CONFIRM BTN_MENU
#define BTN_HOME BTN_BACK

#define BTN_SINGLE_TAP 5 // Currently not used, maybe in the future
#define BTN_DOUBLE_TAP 6

typedef struct PageData {
	uint8_t pageIndex; // Is used by the os
	uint8_t menuIndex; // Is used by the os
	uint8_t menuOffset; // Is used by the os
	
	// All these can be used by custom implementations, they are just here to have a central spot for the page that gets saved
	uint8_t subPageIndex;
	
	String string0;
	String string1;
	String string2;

	uint8_t number0;
	uint8_t number1;
	uint8_t number2;
	uint8_t number3;
	uint8_t number4;
	uint8_t number5;
	uint8_t number6;
	uint8_t number7;
	uint8_t number8;
	uint8_t number9;

	int16_t short0;
	int16_t short1;
	int16_t short2;
	
	long largenumber0;
	long largenumber1;
	long largenumber2;
	long largenumber3;
	long largenumber4;
	long largenumber5;
	long largenumber6;
	long largenumber7;
	long largenumber8;
	long largenumber9;
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
		static void drawScrollBar(uint8_t index, uint8_t count);
		static void drawScrollBar(uint8_t index, uint8_t count, uint8_t bar_size);
		static void drawCentreString(String buf, int x, int y, bool textWrap = true);
		static void drawCentreString(const char *buf, int x, int y, bool textWrap = true);
		static void print(const char *buf);
		static void print(const char *buf, uint16_t fgc, uint16_t bgc);
		static void println(const char *buf);
		static void println(const char *buf, uint16_t fgc, uint16_t bgc);
		
		static void drawCircle(int centerX, int centerY, int diameter, int lineThickness, int percentage);
		static void drawCircle(int centerX, int centerY, int diameter, int lineThickness, int percentage, uint16_t color);
		
		static void drawProgressBar(int x, int y, int w, int h, float progress);
		static void drawProgressBar(int x, int y, int w, int h, float progress, uint16_t color);
		
		static void drawDitherBox(int x, int y, int w, int h);
		static void drawDitherBox(int x, int y, int w, int h, uint8_t fillVal, uint8_t emptyVal);
		static void drawDitherBox(int x, int y, int w, int h, uint8_t fillVal, uint8_t emptyVal, uint16_t color);
};

#endif