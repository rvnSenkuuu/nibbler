#pragma once

#include <vector>

#include "Types.hpp"
#include "Snake.hpp"
#include "Food.hpp"

struct GameData {
	const std::vector<Snake>	&snakes;
	const std::vector<Position>	&obstacles;
	const GameConfig	&config;
	const Food	&food;
	GameState	state;
	float	tick_rate;
	int	winner;
};