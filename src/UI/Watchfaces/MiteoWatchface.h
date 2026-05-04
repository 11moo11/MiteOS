#ifndef WATCHY_MITEO_H
#define WATCHY_MITEO_H

#include "Watchface.h"


class MiteoWatchface : public Watchface {

	public:
		String watchfaceName() { return "Miteo"; }

		void draw();
		void drawBackground();
		void drawForeground();
};

#endif