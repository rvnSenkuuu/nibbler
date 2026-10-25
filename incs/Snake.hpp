#pragma once

#include <deque>

#include "Types.hpp"

class Snake {
	public:
		Snake(int id, const Position &start);
		Snake(const Snake &rhs) = default;
		Snake	&operator=(const Snake &rhs) = default;
		~Snake(void) = default;

		void	reset(int id, const Position &start);
		void	change_direction(Event event);
		[[nodiscard]] bool	occupies(const Position &pos) const;
		Position	next_pos(Position head) const;
		void	move(Position next_pos, bool grow);

		int		get_id(void) const;
		bool	get_alive(void) const;
		Position	get_head(void) const;
		size_t	length() const;
		size_t	get_score() const;
		Event	get_direction(void) const;
		const std::deque<Position>	&get_body(void) const;

		void	add_score(size_t score);
		void	die();

	private:
		Snake(void) = default;
		
		Event	normalize_direction(Event event);
		[[nodiscard]] bool	is_opposite_dir(Event a, Event b);

	private:
		std::deque<Position>	m_body;
		size_t	m_score;
		Event	m_direction;
		int	m_id;
		bool	m_alive;
};