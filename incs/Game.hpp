#pragma once

#include <iostream>
#include <exception>
#include <deque>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

#include "Lib.hpp"
#include "Types.hpp"

void	usage(void);

class Game {
	public:
		Game(int argc, char **argv);
		~Game();

		GameData	get_data(void) const;

		bool	config_game(int argc, char **argv, GameConfig &config);
		void	init_snakes();
		void	run(Lib &lib);
		void	update_game(void);
		void	reset_game(void);
		void	generate_food(void);
		void	check_collision(Snake &current);
		void	move_snake(Snake &snake);
		void	change_snake_dir(size_t snake_index, Event event);
		void	handle_event(Lib &lib, Event &event);

		const FoodKind	&get_food_kind(void);
	
	private:
		Game() = default;
		Game(const Game &rhs) = default;
		Game	&operator=(const Game &rhs) = default;

		[[nodiscard]] bool	is_opposite_dir(Event a, Event b);
		Event	get_event_dir(Event event);

	private:
		size_t	m_player_count;
		int	m_winner;
		std::vector<Snake>	m_snakes;
		Food	m_food;
		std::mt19937	m_rng;
		GameState	m_state;
		GameConfig	m_config;

		float	m_tick_rate;
		std::chrono::steady_clock::time_point	m_last_update_time;
};