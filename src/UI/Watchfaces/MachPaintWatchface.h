#ifndef WATCHY_MACPAINT_H
#define WATCHY_MACPAINT_H

// Credits to https://github.com/sqfmi/Watchy/tree/master/examples/WatchFaces/MacPaint

#include "Watchface.h"
#include "MacPaint/macpaint.h"

class MacPaintWatchface : public Watchface {
	public:
		void draw();
};

#endif