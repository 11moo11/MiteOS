#ifndef WATCHY_BADFOREYE_H
#define WATCHY_BADFOREYE_H

#include "Watchface.h"
#include "BadForEye/BadForEyeResource.h"

class BadForEyeWatchface : public Watchface {
	public:
		String watchfaceName() { return "BadForEye"; }

		void draw();
};

#endif