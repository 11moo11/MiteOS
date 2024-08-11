#ifndef WATCHY_POKEMON_H
#define WATCHY_POKEMON_H

// All credits to https://github.com/Klemek/watchy/tree/master/watchfaces/pokemon-2.0

#include "Watchface.h"

#include "Pokemon/pokemon_yellow.h"
//#include "Pokemon/pokemon_red_blue.h"
//#include "Pokemon/pokemon_red_green.h"

#include "Pokemon/ui_other.h"

#include "Pokemon/ui_en.h"

class PokemonWatchface : public Watchface {

	public:
		void draw();
		
		double randomDay(uint32_t d);
		double randomHour(uint32_t d);
		double randomMinute(uint32_t d);
	
};

#endif