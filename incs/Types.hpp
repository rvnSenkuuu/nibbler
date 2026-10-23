#pragma once

#include <deque>
#include <vector>

#define CELL_SIZE 20
#define FONT_PATH "./assets/font.ttf"

constexpr int FIRST_SNAKE  = 0;
constexpr int SECOND_SNAKE  = 1;

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
	SPEED_UP,
	SPEED_DOWN,
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
	size_t	width;
	size_t	height;

	bool	opt_is_multi;
	bool	opt_limit_score;
	bool	opt_food_variance;
	bool	opt_have_obstacles;
	bool	opt_enable_sounds;
	int	max_score;
	int	obstacles_count;
};