#include "Game.hpp"

Game::Game(int argc, char **argv) : m_direction(Event::LEFT), m_rng(std::random_device{}()), m_score(0), m_state(GameState::RUNNING),
									m_tick_rate(10.0f), m_last_update_time(std::chrono::steady_clock::now())
{
	if (argc != 3)
		throw std::logic_error("Usage: ./nibbler <width> <height>");

	m_width = std::stoi(argv[1]);
	m_height = std::stoi(argv[2]);

	//TODO: Check for higher value
	if (m_width < 10 || m_height < 10)
		throw std::invalid_argument("Width or height values too low");

	m_snake.push_back({m_width / 2, m_height / 2});
	m_snake.push_back({m_width / 2 + 1, m_height / 2});
	m_snake.push_back({m_width / 2 + 2, m_height / 2});
	m_snake.push_back({m_width / 2 + 3, m_height / 2});
	generate_food();
}

Game::~Game(void) { }

GameData	Game::get_data(void) const
{
	GameData	data = {
		.snake = m_snake,
		.food = m_food,
		.score = m_score,
		.width = m_width,
		.height = m_height,
		.state = m_state};

	return data;
}

void	Game::handle_event(Lib &lib, Event &event)
{
	if (m_state == GameState::GAME_OVER || m_state == GameState::WON) {
		if (event == Event::RESTART)
			reset_game();
	} else if (m_state == GameState::RUNNING) {
		switch (event) {
		case Event::QUIT:
			return;
		case Event::UP:
		case Event::DOWN:
		case Event::RIGHT:
		case Event::LEFT:
			change_snake_dir(event);
			break;
		case Event::LIB1:
			if (lib.switch_lib(FIRST_LIB))
				lib.create(m_width, m_height);
			break;
		case Event::LIB2:
			if (lib.switch_lib(SECOND_LIB))
				lib.create(m_width, m_height);
			break;
		case Event::LIB3:
			if (lib.switch_lib(THIRD_LIB))
				lib.create(m_width, m_height);
			break;
		default:
			break;
		}
	}
}

void	Game::run(Lib &lib)
{
	lib.create(m_width, m_height);
	while (true) {
		Event	event = lib.get_event();
		if (event == Event::QUIT)
			return;
		
		handle_event(lib, event);
		if (m_state == GameState::RUNNING) {
			auto	current_time = std::chrono::steady_clock::now();
			float	delta_time = std::chrono::duration<float>(current_time - m_last_update_time).count();
			if (delta_time >= 1.0f / m_tick_rate) {
				move_snake();
				check_collision();
				m_last_update_time = current_time;
			}
		}
		lib.render(get_data());
	}
}

void	Game::check_collision(void)
{
	Position	head = m_snake.front();
	if (head.x >= m_width || head.y >= m_height) {
		m_state = GameState::GAME_OVER;
		return;
	}

	for (size_t i = 1; i < m_snake.size(); i++) {
		if (head == m_snake[i]) {
			m_state = GameState::GAME_OVER;
			return;
		}
	}
}

bool	Game::is_opposite_dir(Event a, Event b)
{
	return (a == Event::UP && b == Event::DOWN) ||
	(a == Event::DOWN && b == Event::UP) ||
	(a == Event::LEFT && b == Event::RIGHT) ||
	(a == Event::RIGHT && b == Event::LEFT);
}

void	Game::change_snake_dir(Event event)
{
	if (event == Event::UP ||
		event == Event::DOWN ||
		event == Event::RIGHT ||
		event == Event::LEFT) {
		if (!is_opposite_dir(m_direction, event))
			m_direction = event;
	}
}

void	Game::move_snake()
{
	Position	head = m_snake.front();

	switch (m_direction) {
	case Event::UP:
		head.y--;
		break;
	case Event::DOWN:
		head.y++;
		break;
	case Event::RIGHT:
		head.x++;
		break;
	case Event::LEFT:
		head.x--;
		break;
	default:
		break;
	}

	bool	is_food = (head == m_food);
	m_snake.push_front(head);

	if (is_food) {
		m_score += 10;
		if (m_snake.size() == m_width * m_height)
			m_state = GameState::WON;
		else
			generate_food();
	} else {
		m_snake.pop_back();
	}
}

void	Game::generate_food(void)
{
	std::uniform_int_distribution<size_t>	x_pos(0, m_width - 1);
	std::uniform_int_distribution<size_t>	y_pos(0, m_height - 1);

	bool	valid_pos = false;
	Position	new_food;
	while (!valid_pos) {
		new_food = {x_pos(m_rng), y_pos(m_rng)};
		
		valid_pos = true;
		for (auto &snake_part : m_snake) {
			if (snake_part == new_food) {
				valid_pos = false;
				break;
			}
		}
	}
	m_food = new_food;
}

void	Game::reset_game(void)
{
	m_direction = Event::LEFT;
	m_snake.clear();
	m_snake.push_back({m_width / 2, m_height / 2});
	m_snake.push_back({m_width / 2 + 1, m_height / 2});
	m_snake.push_back({m_width / 2 + 2, m_height / 2});
	m_snake.push_back({m_width / 2 + 3, m_height / 2});
	m_state = GameState::RUNNING;
	m_score = 0;
	m_last_update_time = std::chrono::steady_clock::now();
	generate_food();
}