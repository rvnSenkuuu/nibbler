#pragma once

#include <deque>

#define CELL_SIZE 30
#define FONT_PATH "./assets/font.ttf"

struct Position {
	size_t	x;
	size_t	y;
	bool	operator==(const Position &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
};

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
	QUIT
};

struct GameData {
	std::deque<Position>	snake;
	Position	food;
	size_t	score;
	size_t	width;
	size_t	height;
	GameState	state;
};