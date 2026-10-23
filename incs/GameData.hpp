#pragma once

#include <vector>

#include "Types.hpp"
#include "Snake.hpp"
#include "Food.hpp"

struct GameData {
	std::vector<Snake>	snakes;
	std::vector<Position>	obstacles;
	GameConfig	config;
	Food	food;
	GameState	state;
	float	tick_rate;
	int	winner;
};