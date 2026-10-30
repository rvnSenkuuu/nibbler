#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <charconv>
#include <algorithm>
#include <iterator>
#include <random>
#include <optional>
#include <system_error>
#include <chrono>
#include <thread>

#include "Snake.hpp"
#include "Food.hpp"
#include "Lib.hpp"
#include "Audio.hpp"
#include "GameData.hpp"

constexpr float	MIN_TICK_RATE{10.0f};
constexpr float	MAX_TICK_RATE{15.0f};
constexpr int	MIN_WIDTH{15};
constexpr int	MIN_HEIGHT{15};
constexpr int	MAX_WIDTH{50};
constexpr int	MAX_HEIGHT{50};

void	usage(void);

template<typename T>
bool	get_option_value(const char *arg, T &option)
{
	const std::string_view	str(arg);
	const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), option);
	
	return ec == std::errc() && ptr == str.data() + str.size();
}

class Game {
	public:
		Game(int argc, char **argv);
		~Game(void) = default;

		GameData	get_data(void) const;

		[[nodiscard]] bool	is_board_full(void) const;
		[[nodiscard]] std::optional<GameConfig>	get_game_config(int argc, char **argv);
		void	init_snakes();
		void	run(Lib &lib);
		void	update_game(void);
		void	start_new_game(void);
		void	generate_obstacle(void);
		void	check_collision(Snake &current);
		void	move_snake(Snake &snake);
		void	kill_snake(Snake &snake);
		void	handle_event(Lib &lib, Event &event);

	private:
		Game(const Game &rhs) = delete;
		Game	&operator=(const Game &rhs) = delete;

		[[nodiscard]] bool	is_valid_obstacle_pos(const Position &pos) const;
		int	get_max_obstacles(void) const;
		
		void	end_game(GameState state, int winner);
		void	end_by_board_full(void);
		void	handle_disappearing_food(std::chrono::steady_clock::time_point &current_time);

	private:
		size_t	m_player_count{1};
		int	m_winner{-1};
		std::vector<Snake>	m_snakes;
		std::vector<Position>	m_obstacles;
		Food	m_food;
		std::mt19937	m_rng{std::random_device{}()};
		GameState	m_state{GameState::RUNNING};
		GameConfig	m_config;
		Audio	m_audio;

		float	m_tick_rate{MIN_TICK_RATE};
		std::chrono::steady_clock::time_point	m_last_update_time{std::chrono::steady_clock::now()};
};