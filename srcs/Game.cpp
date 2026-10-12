#include "Game.hpp"

Game::Game(int argc, char **argv) : m_player_count(1), m_rng(std::random_device{}()), m_state(GameState::RUNNING),
									m_tick_rate(10.0f), m_last_update_time(std::chrono::steady_clock::now())
{
	if (argc < 3 || argc > 4)
		throw std::invalid_argument("Usage: ./nibbler <width> <height> [--multi]");

	m_width = std::stoi(argv[1]);
	m_height = std::stoi(argv[2]);

	if (argc == 4) {
		if (std::string(argv[3]) != "--multi")
			throw std::invalid_argument("Unknown option (expected --multi)");
		m_player_count = 2;
	}

	//TODO: Check for higher value
	if (m_width < 10 || m_height < 10)
		throw std::invalid_argument("Width or height values too low");

	init_snakes();
	generate_food();
}

Game::~Game(void) { }

void	Game::init_snakes(void)
{
	m_snakes.reserve(m_player_count);
	m_snakes.clear();

	size_t	row = m_player_count == 2 ? m_height / 3 : m_height / 2;

	Snake	p1;
	p1.direction = Event::LEFT;
	p1.score = 0;
	p1.alive = true;
	p1.body.push_back({m_width / 2, row});
	p1.body.push_back({m_width / 2 + 1, row});
	p1.body.push_back({m_width / 2 + 2, row});
	p1.body.push_back({m_width / 2 + 3, row});

	m_snakes.push_back(p1);

	if (m_player_count == 2) {
		row = 2 * m_height / 3;

		Snake	p2;
		p2.direction = Event::LEFT;
		p2.score = 0;
		p2.alive = true;
		p2.body.push_back({m_width / 2, row});
		p2.body.push_back({m_width / 2 + 1, row});
		p2.body.push_back({m_width / 2 + 2, row});
		p2.body.push_back({m_width / 2 + 3, row});

		m_snakes.push_back(p2);
	}
}

GameData	Game::get_data(void) const
{
	GameData	data = {
		.snakes = m_snakes,
		.food = m_food,
		.width = m_width,
		.height = m_height,
		.state = m_state
	};

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
		case Event::W:
		case Event::S:
		case Event::A:
		case Event::D:
			change_snake_dir(__FIRST_SNAKE, event);
			break;
		case Event::UP:
		case Event::DOWN:
		case Event::RIGHT:
		case Event::LEFT:
			change_snake_dir(m_player_count == 2 ? __SECOND_SNAKE : __FIRST_SNAKE, event);
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
				update_game();
				// check_collision();
				m_last_update_time = current_time;
			}
		}
		lib.render(get_data());
	}
}

// void	Game::check_collision(void)
// {
// 	Position	head = m_snake.front();
// 	if (head.x >= m_width || head.y >= m_height) {
// 		m_state = GameState::GAME_OVER;
// 		return;
// 	}

// 	for (size_t i = 1; i < m_snake.size(); i++) {
// 		if (head == m_snake[i]) {
// 			m_state = GameState::GAME_OVER;
// 			return;
// 		}
// 	}
// }

Event	Game::get_event_dir(Event event)
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

bool	Game::is_opposite_dir(Event a, Event b)
{
	a = get_event_dir(a);
	b = get_event_dir(b);

	return (a == Event::UP && b == Event::DOWN) || (a == Event::DOWN && b == Event::UP) ||
		(a == Event::LEFT && b == Event::RIGHT) || (a == Event::RIGHT && b == Event::LEFT);
}

void	Game::change_snake_dir(size_t snake_index, Event event)
{
	Snake	&snake = m_snakes[snake_index];
	if (snake.alive && !is_opposite_dir(snake.direction, event))
		snake.direction = get_event_dir(event);
}

void	Game::update_game(void)
{
	for (auto &snake : m_snakes) {
		if (snake.alive)
			move_snake(snake);
	}
}

void	Game::move_snake(Snake &snake)
{
	Position	head = snake.body.front();

	switch (snake.direction) {
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

	bool	is_food = (head == m_food);
	snake.body.push_front(head);

	if (!is_food) {
		snake.body.pop_back();
		return;
	}

	snake.score += 10;
	if (snake.body.size() == m_width * m_height) {
		m_state = GameState::WON;
	} else {
		generate_food();
	}
}

void	Game::generate_food(void)
{
	std::uniform_int_distribution<size_t>	x_pos(0, m_width - 1);
	std::uniform_int_distribution<size_t>	y_pos(0, m_height - 1);

	bool	valid_pos = false;
	Position	pos;
	while (!valid_pos) {
		pos = {x_pos(m_rng), y_pos(m_rng)};
		valid_pos = true;
		for (auto &snake : m_snakes) {
			if (std::find(snake.body.begin(), snake.body.end(), pos) != snake.body.end()) {
				valid_pos = false;
				break;
			}
		}
	}
	m_food = pos;
}

void	Game::reset_game(void)
{
	init_snakes();
	m_state = GameState::RUNNING;
	m_last_update_time = std::chrono::steady_clock::now();
	generate_food();
}