#pragma once

#include <random>
#include <vector>

#include "Snake.hpp"

class Snake;

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

bool	is_position_free(const Position &pos, std::vector<Snake> &snakes, std::vector<Position> &obstacles);

class Food {
	public:
		Food(void);
		~Food(void);
		Food(const Food &rhs) = default;
		Food	&operator=(const Food &rhs) = default;

		Position	get_pos(void) const;
		FoodType	get_type(void) const;
		int	get_given_score(void) const;

		void	set_rng(std::mt19937 &rng);

		void	generate_food(GameConfig &config, std::vector<Snake> &snakes, std::vector<Position> &obstacles);
		const FoodKind	&get_food_kind(void);

	private:
		std::mt19937	m_rng;
		Position	m_pos;
		FoodType	m_type;
		int	m_given_score;
};