#ifndef FURLY_PAGE_H
#define FURLY_PAGE_H

#include "Page.h"

#define GENDER_MALE 0
#define GENDER_FEMALE 1

class FurlyPage : public Page {
	public:
    	void initPage() override;
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawFurly(uint8_t x, uint8_t y, uint8_t gender = 0);
		void drawMainPage();
		void drawStepProgressBar();
		void drawInfoPage();
		
		uint8_t getLevel();
		uint32_t getLevelSteps();
};

#endif