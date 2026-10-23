#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

#include "Snake.hpp"
#include "Food.hpp"
#include "Lib.hpp"
#include "AudioManager.hpp"
#include "GameData.hpp"

void	usage(void);
bool	is_position_free(const Position &pos, std::vector<Snake> &snakes,
						std::vector<Position> &obstacles);

class Game {
	public:
		Game(int argc, char **argv);
		~Game(void) = default;

		GameData	get_data(void) const;

		[[nodiscard]] bool	is_board_full(void) const;
		[[nodiscard]] bool	get_game_config(int argc, char **argv, GameConfig &config);
		void	init_snakes();
		void	run(Lib &lib);
		void	update_game(void);
		void	reset_game(void);
		void	generate_obstacle(void);
		void	check_collision(Snake &current);
		void	move_snake(Snake &snake);
		void	kill_snake(Snake &snake);
		void	handle_event(Lib &lib, Event &event);

	private:
		Game(const Game &rhs) = delete;
		Game	&operator=(const Game &rhs) = default;

		[[nodiscard]] bool	is_valid_obstacle_pos(const Position &pos) const;
		int	get_max_obstacles(void) const;

	private:
		size_t	m_player_count;
		int	m_winner;
		std::vector<Snake>	m_snakes;
		std::vector<Position>	m_obstacles;
		Food	m_food;
		std::mt19937	m_rng;
		GameState	m_state;
		GameConfig	m_config;
		AudioManager	m_audio;

		float	m_tick_rate;
		std::chrono::steady_clock::time_point	m_last_update_time;
};