#pragma once

#include <deque>
#include <vector>

#define CELL_SIZE 30
#define FONT_PATH "./assets/font.ttf"

#define __FIRST_SNAKE 0
#define __SECOND_SNAKE 1

enum class GridType : int {
	EMPTY,
	SNAKE_HEAD,
	SNAKE_BODY,
	FOOD
};

enum class Event : int {
	NONE,
	UP,
	LEFT,
	RIGHT,
	DOWN,
	W,
	S,
	A,
	D,
	LIB1,
	LIB2,
	LIB3,
	RESTART,
	QUIT
};

enum class GameState : int {
	RUNNING,
	GAME_OVER,
	WON,
	DRAW,
	QUIT
};

struct Position {
	size_t	x;
	size_t	y;
	bool	operator==(const Position &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
};

struct GameConfig {
	bool	opt_is_multi;
	bool	opt_limit_score;
	bool	opt_food_variance;
	bool	opt_have_obstacle;
	int	max_score;
};

struct Snake {
	std::deque<Position>	body;
	size_t	score;
	Event	direction;
	int	id;
	bool	alive;
};

struct GameData {
	std::vector<Snake>	snakes;
	GameConfig	config;
	Position	food;
	size_t	width;
	size_t	height;
	GameState	state;
	int	winner;
};