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

		GameData	get_data(void) const;

		void	init_snakes();
		void	run(Lib &lib);
		void	update_game(void);
		void	reset_game(void);
		void	generate_food(void);
		void	check_collision(void);
		void	move_snake(Snake &snake);
		void	change_snake_dir(size_t snake_index, Event event);
		void	handle_event(Lib &lib, Event &event);
	
	private:
		Game() = default;
		Game(const Game &rhs) = default;
		Game	&operator=(const Game &rhs) = default;

		[[nodiscard]] bool	is_opposite_dir(Event a, Event b);
		Event	get_dir(Event event);

	private:
		size_t	m_width;
		size_t	m_height;

		size_t	m_player_count;
		std::vector<Snake>	m_snakes;
		Position	m_food;
		std::mt19937	m_rng;
		GameState	m_state;

		float	m_tick_rate;
		std::chrono::steady_clock::time_point	m_last_update_time;
};