#ifndef ACTIVITY_PAGE_H
#define ACTIVITY_PAGE_H

#include "Page.h"

class ActivityPage : public Page {
	public:
		void drawPage() override;
	private:
		void drawCircle();
		void drawText();
};

#endif