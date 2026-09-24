#pragma once

#include <iostream>
#include <exception>
#include <vector>
#include <deque>
#include <algorithm>
#include <random>
#include <chrono>

#include "Event.hpp"

struct Position {
	int	x, y;
	inline bool	operator==(const Position &rhs)
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

class Game {
	public:
		Game(int argc, char **argv);
		~Game();

		void	run(void);
		void	resync_grid(void);
		void	render(void);
		void	generate_food(void);
		void	check_collission(void);
		void	move_snake(Event event);
	
	private:
		Game() = default;
		Game(const Game &rhs) = default;
		Game	&operator=(const Game &rhs) = default;

	private:
		int	m_width;
		int	m_height;
		std::deque<Position>	m_snake;
		std::vector<std::vector<GridType>>	m_grid;
		Position	m_food;
		std::mt19937	m_rng;
		size_t	m_score;
		bool	m_game_over;

	    float	m_tick_rate;
    	std::chrono::steady_clock::time_point	m_last_update_time;

};