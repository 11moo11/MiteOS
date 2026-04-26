#ifndef WATCHY_TETRIS_H
#define WATCHY_TETRIS_H

#include "Watchface.h"
#include "Tetris/Tetris.h"

class TetrisWatchface : public Watchface {
	public:
		String watchfaceName() { return "Tetris"; }

		void draw();
		void drawNumber(int x, int y, int value, int max_digits);
		double random();
};

#endif