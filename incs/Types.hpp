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

enum class FoodType : int {
	FOOD_RED,
	FOOD_GREEN,
	FOOD_YELLOW
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

struct FoodKind {
	FoodType	type;
	int	score;
	int	weight;
};

static const FoodKind	food_kinds[] = {
	{FoodType::FOOD_RED, 10, 6},
	{FoodType::FOOD_GREEN, 30, 3},
	{FoodType::FOOD_YELLOW, 50, 1},
};

struct Food {
	Position	pos;
	FoodType	type;
	int	given_score;
};

struct GameData {
	std::vector<Snake>	snakes;
	GameConfig	config;
	Food	food;
	GameState	state;
	int	winner;
};