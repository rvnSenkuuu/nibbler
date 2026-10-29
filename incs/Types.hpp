#pragma once

#include <deque>
#include <vector>

constexpr int	CELL_SIZE{20};
constexpr const char	*FONT_PATH{"./assets/font.ttf"};

constexpr int FIRST_SNAKE{0};
constexpr int SECOND_SNAKE{1};

constexpr size_t	max_spawn_attempts{1000};

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
	P1_SHRINK,
	P2_SHRINK,
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
	size_t	width = 0;
	size_t	height = 0;

	bool	opt_multi = false;
	bool	opt_limit_score = false;
	bool	opt_food_variance = false;
	bool	opt_food_disappear = false;
	bool	opt_have_obstacles = false;
	bool	opt_sounds = false;
	bool	opt_trap = false;
	int	max_score = 0;
	int	obstacles_count = 0;
};