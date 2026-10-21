#pragma once

#include <random>

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

class Food {
	public:
		Food(void);
		Food(std::mt19937 rng);
		~Food();
		Food(Position pos, FoodType type, int given_score) = delete;
		Food(const Food &rhs) = default;
		Food	&operator=(const Food &rhs) = default;

		[[nodiscard]] bool	is_position_free(const Position &pos, std::vector<Snake> &snakes, std::vector<Position>	&obstacles);

		Position	get_pos(void) const;
		FoodType	get_type(void) const;
		int	get_given_score(void) const;

		void	generate_food(GameConfig &config, std::vector<Snake> &snakes, std::vector<Position> &obstacles);
		const FoodKind	&get_food_kind(void);

	private:
		std::mt19937	m_rng;
		Position	m_pos;
		FoodType	m_type;
		int	m_given_score;
};