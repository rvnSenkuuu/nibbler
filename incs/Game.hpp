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
		~Game() { };

		GameData	get_data(void) const;

		[[nodiscard]] bool	is_board_full(void);
		[[nodiscard]] bool	get_game_config(int argc, char **argv, GameConfig &config);
		void	init_snakes();
		void	run(Lib &lib);
		void	update_game(void);
		void	reset_game(void);
		void	generate_food(void);
		void	generate_obstacle(void);
		void	check_collision(Snake &current);
		void	move_snake(Snake &snake);
		Position	get_next_pos(Position &pos, Event dir);
		void	change_snake_dir(size_t snake_index, Event event);
		void	handle_event(Lib &lib, Event &event);

		const FoodKind	&get_food_kind(void);
	
	private:
		Game() = default;
		Game(const Game &rhs) = default;
		Game	&operator=(const Game &rhs) = default;

		[[nodiscard]] bool	is_opposite_dir(Event a, Event b);
		[[nodiscard]] bool	is_position_free(const Position &pos);
		[[nodiscard]] bool	is_valid_obstacle_pos(const Position &pos);
		Event	get_event_dir(Event event);

	private:
		size_t	m_player_count;
		int	m_winner;
		std::vector<Snake>	m_snakes;
		std::vector<Position>	m_obstacles;
		Food	m_food;
		std::mt19937	m_rng;
		GameState	m_state;
		GameConfig	m_config;

		float	m_tick_rate;
		std::chrono::steady_clock::time_point	m_last_update_time;
};