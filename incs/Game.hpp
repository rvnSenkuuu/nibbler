#pragma once

#include <iostream>
#include <exception>
#include <deque>
#include <algorithm>
#include <random>
#include <chrono>

#include "Lib.hpp"
#include "Types.hpp"

class Game {
	public:
		Game(int argc, char **argv);
		~Game();

		GameState	get_state(void) const;

		void	run(Lib &lib);
		void	reset_game(void);
		void	generate_food(void);
		void	check_collision(void);
		void	move_snake();
		void	change_snake_dir(Event event);
	
	private:
		Game() = default;
		Game(const Game &rhs) = default;
		Game	&operator=(const Game &rhs) = default;

		[[nodiscard]] bool	is_opposite_dir(Event a, Event b);

	private:
		size_t	m_width;
		size_t	m_height;
		std::deque<Position>	m_snake;
		Position	m_food;
		Event	m_direction;
		std::mt19937	m_rng;
		size_t	m_score;
		bool	m_game_over;

	    float	m_tick_rate;
    	std::chrono::steady_clock::time_point	m_last_update_time;

};