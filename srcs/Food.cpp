#include "Game.hpp"
#include "Food.hpp"

static constexpr FoodKind	food_kinds[] = {
	{FoodType::FOOD_RED, 10, 6},
	{FoodType::FOOD_GREEN, 30, 3},
	{FoodType::FOOD_YELLOW, 50, 1},
};

Position	Food::get_pos(void) const
{
	return m_pos;
}

FoodType	Food::get_type(void) const
{
	return m_type;
}

int	Food::get_given_score(void) const
{
	return m_given_score;
}

void	Food::set_rng(const std::mt19937 &rng)
{
	m_rng = rng;
}

bool	Food::generate_food(GameConfig &config, const std::vector<Snake> &snakes, const std::vector<Position> &obstacles)
{
	std::uniform_int_distribution<size_t>	x_pos(0, config.width - 1);
	std::uniform_int_distribution<size_t>	y_pos(0, config.height - 1);

	for (size_t i = 0; i < max_spawn_attempts; i++) {
		const Position	pos = {x_pos(m_rng), y_pos(m_rng)};
		if (!is_position_free(pos, snakes, obstacles))
			continue;
			
		const FoodKind	&kind = config.opt_food_variance ? get_food_kind() : food_kinds[0];
		m_pos = pos;
		m_type = kind.type;
		m_given_score = kind.score;
		return true;
	}
	return false;
}

const FoodKind	&Food::get_food_kind(void)
{
	int	total = 0;
	for (const auto &kind : food_kinds)
		total += kind.weight;

	std::uniform_int_distribution<int>	dist(0, total - 1);
	int	roll = dist(m_rng);

	for (const auto &kind : food_kinds) {
		if (roll < kind.weight)
			return kind;
		roll -= kind.weight;
	}
	
	return food_kinds[0]; 
}