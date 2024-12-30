#ifndef PLAYBACK_PAGE_H
#define PLAYBACK_PAGE_H

#include "Page.h"
#include "../Managers/PhoneConnectionManager.h"

class PlaybackPage : public Page {
	public:
		void drawPage() override;
		bool onButtonPressed(uint8_t buttonIndex) override;
		bool isPageable() { return false; };
	private:
		void drawIcons(PlaybackInfo pbi);
};

#endif