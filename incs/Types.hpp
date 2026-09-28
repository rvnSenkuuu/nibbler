#pragma once

#include <deque>

struct Position {
	int	x, y;
	inline bool	operator==(const Position &rhs) const
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
	QUIT
};

struct GameState {
	std::deque<Position>	snake;
	Position	food;
	size_t	score;
	int	width;
	int	height;
	bool	game_over;
};