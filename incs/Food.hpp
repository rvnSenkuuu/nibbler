#pragma once

#include <random>
#include <vector>

#include "Types.hpp"

enum class FoodType : int {
	FOOD_RED,
	FOOD_GREEN,
	FOOD_YELLOW
};

struct FoodKind {
	FoodType	type;
	int	score;
	int	weight;
};

class Snake;

bool	is_position_free(const Position &pos, const std::vector<Snake> &snakes,
						const std::vector<Position> &obstacles);

class Food {
	public:
		Food(void) = default;
		~Food(void) = default;
		Food(const Food &rhs) = default;
		Food	&operator=(const Food &rhs) = default;

		Position	get_pos(void) const;
		FoodType	get_type(void) const;
		int	get_given_score(void) const;

		bool	generate_food(GameConfig &config, std::mt19937 &rng,
			std::vector<Snake> &snakes, const std::vector<Position> &obstacles);
		
		private:
			const FoodKind	&get_food_kind(std::mt19937 &rng);

	private:
		// std::mt19937	m_rng;
		Position	m_pos;
		FoodType	m_type;
		int	m_given_score;
};