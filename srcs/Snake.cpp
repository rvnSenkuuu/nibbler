#include "Snake.hpp"
#include <algorithm>

Snake::Snake(int id, const Position &start)
{
	reset(id, start);
}

void	Snake::reset(int id, const Position &start)
{
	m_direction = Event::LEFT;
	m_score = 0;
	m_id = id;
	m_alive = true;
	m_body.clear();
	m_body.push_back({start.x, start.y});
	m_body.push_back({start.x + 1, start.y});
	m_body.push_back({start.x + 2, start.y});
	m_body.push_back({start.x + 3, start.y});
}

Event	Snake::normalize_direction(Event event)
{
	switch (event) {
	case Event::W:
		return Event::UP;
	case Event::S:
		return Event::DOWN;
	case Event::A:
		return Event::LEFT;
	case Event::D:
		return Event::RIGHT;
	default:
		return event;
	}
}

bool	Snake::occupies(const Position &pos) const
{
	return std::find(m_body.begin(), m_body.end(), pos) != m_body.end();
}

bool	Snake::is_opposite_dir(Event a, Event b)
{
	a = normalize_direction(a);
	b = normalize_direction(b);

	return (a == Event::UP && b == Event::DOWN) || (a == Event::DOWN && b == Event::UP) ||
		(a == Event::LEFT && b == Event::RIGHT) || (a == Event::RIGHT && b == Event::LEFT);
}

void	Snake::change_direction(Event event)
{
	if (m_alive && !is_opposite_dir(m_direction, event))
		m_direction = normalize_direction(event);
}

Position	Snake::next_pos(Position head) const
{
	switch (m_direction) {
	case Event::UP:
		head.y--;
		break;
	case Event::DOWN:
		head.y++;
		break;
	case Event::LEFT:
		head.x--;
		break;
	case Event::RIGHT:
		head.x++;
		break;
	default:
		break;
	}

	return head;
}

void	Snake::move(Position next_pos, bool grow)
{
	m_body.push_front(next_pos);
	if (!grow)
		m_body.pop_back();
}

int	Snake::get_id(void) const
{
	return m_id;
}

bool	Snake::get_alive(void) const
{
	return m_alive;
}

Position	Snake::get_head(void) const
{
	return m_body.front();
}

size_t	Snake::length(void) const
{
	return m_body.size();
}

size_t	Snake::get_score(void) const
{
	return m_score;
}

Event	Snake::get_direction(void) const
{
	return m_direction;
}

const std::deque<Position>	&Snake::get_body(void) const
{
	return m_body;
}

void	Snake::add_score(size_t score)
{
	m_score += score;
}

void	Snake::die()
{
	m_alive = false;
}