#ifndef HOBBIT_TIME_H
#define HOBBIT_TIME_H

// Credits to https://github.com/BraininaBowl/Hobbit-Time-for-Watchy/blob/main/HobbitTime.cpp

#include "Watchface.h"
#include "HobbitTime/NunitoSans_Black12pt7b.h"
#include "HobbitTime/NunitoSans_Black18pt7b.h"

class HobbitTimeWatchface : public Watchface {
	public:
		void draw();
};

#endif