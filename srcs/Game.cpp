#include "Game.hpp"

#define MIN_TICK_RATE 10.0f
#define MAX_TICK_RATE 15.0f
#define MIN_WIDTH 15
#define MIN_HEIGHT 15
#define MAX_WIDTH 50
#define MAX_HEIGHT 50

static const size_t	max_attempts = 1000;

Game::Game(int argc, char **argv) : m_player_count(1), m_winner(-1), m_rng(std::random_device{}()), m_state(GameState::RUNNING),
									m_config(), m_tick_rate(MIN_TICK_RATE), m_last_update_time(std::chrono::steady_clock::now())
{
	if (!get_game_config(argc, argv, m_config))
		throw std::invalid_argument("Failed to get game configuration");
	
	if (m_config.opt_is_multi)
		m_player_count = 2;

	if (m_config.opt_enable_sounds)
		m_audio.init();

	init_snakes();
	if (m_config.opt_have_obstacles)
		generate_obstacle();

	m_food.generate_food(m_config, m_snakes, m_obstacles);
}

bool	Game::get_game_config(int argc, char **argv, GameConfig &config)
{
	if (argc < 3)
		throw std::invalid_argument("Invalid argument count");
	
	config.width = std::stoi(argv[1]);
	config.height = std::stoi(argv[2]);

	if ((config.width < MIN_WIDTH && config.width > MAX_WIDTH) || (config.height < MIN_HEIGHT && config.height > MAX_HEIGHT))
		throw std::invalid_argument("Width or height values too low or too high");

	for (int i = 3; i < argc; i++) {
		std::string	arg = argv[i];
		
		if (arg == "--help") {
			usage();
			std::exit(0);
		} else if (arg == "--multi") {
			config.opt_is_multi = true;
		} else if (arg == "--limit-score") {
			config.opt_limit_score = true;
			if (argv[++i] == nullptr)
				return false;
			config.max_score = std::stoi(argv[i]);
			if (config.max_score < 0)
				return false;
		} else if (arg == "--food-variance") {
			config.opt_food_variance = true;
		} else if (arg == "--enable-obstacles") {
			config.opt_have_obstacles = true;
			if (argv[++i] == nullptr)
				return false;
			config.obstacles_count = std::stoi(argv[i]);
			if (config.obstacles_count < 0)
				return false;
		} else if (arg == "--enable-sounds") {
			config.opt_enable_sounds = true;
		} else {
			return false;
		}
	}

	return true;
}

void	Game::init_snakes(void)
{
	m_snakes.reserve(m_player_count);
	m_snakes.clear();

	size_t	row = m_player_count == 2 ? m_config.height / 3 : m_config.height / 2;

	Snake	p1;
	p1.direction = Event::LEFT;
	p1.score = 0;
	p1.id = __FIRST_SNAKE;
	p1.alive = true;
	p1.body.push_back({m_config.width / 2, row});
	p1.body.push_back({m_config.width / 2 + 1, row});
	p1.body.push_back({m_config.width / 2 + 2, row});
	p1.body.push_back({m_config.width / 2 + 3, row});

	m_snakes.push_back(p1);

	if (m_player_count == 2) {
		row = 2 * m_config.height / 3;

		Snake	p2;
		p2.direction = Event::LEFT;
		p2.score = 0;
		p2.id = __SECOND_SNAKE;
		p2.alive = true;
		p2.body.push_back({m_config.width / 2, row});
		p2.body.push_back({m_config.width / 2 + 1, row});
		p2.body.push_back({m_config.width / 2 + 2, row});
		p2.body.push_back({m_config.width / 2 + 3, row});

		m_snakes.push_back(p2);
	}
}

GameData	Game::get_data(void) const
{
	return (GameData) {
		.snakes = m_snakes,
		.obstacles = m_obstacles,
		.config = m_config,
		.food = m_food,
		.state = m_state,
		.tick_rate = m_tick_rate,
		.winner = m_winner
	};
}

void	Game::handle_event(Lib &lib, Event &event)
{
	if (m_state == GameState::GAME_OVER || m_state == GameState::WON || m_state == GameState::DRAW) {
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
				lib.create(m_config.width, m_config.height);
			break;
		case Event::LIB2:
			if (lib.switch_lib(SECOND_LIB))
				lib.create(m_config.width, m_config.height);
			break;
		case Event::LIB3:
			if (lib.switch_lib(THIRD_LIB))
				lib.create(m_config.width, m_config.height);
			break;
		case Event::SPEED_UP:
			m_tick_rate = std::min(m_tick_rate + 1.0f, MAX_TICK_RATE);
			break;
		case Event::SPEED_DOWN:
			m_tick_rate = std::min(m_tick_rate - 1.0f, MIN_TICK_RATE);
			break;
		default:
			break;
		}
	}
}

void	Game::run(Lib &lib)
{
	lib.create(m_config.width, m_config.height);
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
				m_last_update_time = current_time;
			}
			if (m_config.opt_enable_sounds)
				m_audio.update();
		}
		lib.render(get_data());
	}
}

void	Game::kill_snake(Snake &snake)
{
	m_state = GameState::GAME_OVER;
	snake.alive = false;
	if (m_config.opt_enable_sounds)
		m_audio.play("DEATH");
}

void	Game::check_collision(Snake &current)
{
	Position	head = current.body.front();
	if (head.x >= m_config.width || head.y >= m_config.height) {
		kill_snake(current);
		return;
	}

	if (std::find(m_obstacles.begin(), m_obstacles.end(), head) != m_obstacles.end()) {
		kill_snake(current);
		return;
	}

	for (auto &snake : m_snakes) {
		auto	begin_it = snake.body.begin();
		if (&current == &snake)
			begin_it = std::next(begin_it);

		if (std::any_of(
			begin_it,
			snake.body.end(),
			[&head](const Position &pos) { return head == pos; }
		)) {
			kill_snake(current);
			break;
		}
	}
}

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

bool	Game::is_board_full(void)
{
	size_t	total = 0;
	for (auto &snake : m_snakes) {
		total += snake.body.size();
	}

	return total >= m_config.width * m_config.height - m_obstacles.size();
}

void	Game::update_game(void)
{
	for (auto &snake : m_snakes) {
		if (snake.alive)
			move_snake(snake);
	}

	for (auto &snake : m_snakes) {
		if (snake.alive)
			check_collision(snake);
	}

	if (m_config.opt_limit_score) {
		size_t	max = m_config.max_score;
		auto	it = std::find_if(
			m_snakes.begin(),
			m_snakes.end(),
			[&max](const auto &snake) { return snake.score >= max; }
		);

		if (it != m_snakes.end()) {
			m_state = GameState::WON;
			m_winner = it->id;
			m_audio.stop();
		}
	}

	size_t	alive_count = std::count_if(
		m_snakes.begin(),
		m_snakes.end(),
		[](const Snake &snake) { return snake.alive; }
	);

	if (alive_count == 0) {
		m_state = m_player_count == 2 ? GameState::DRAW : GameState::GAME_OVER;
		if (m_config.opt_enable_sounds) {
			m_audio.stop();
			m_audio.play("DEATH");
		}
	} else if (m_player_count == 2 && alive_count == 1) {
		m_state = GameState::WON;
		m_winner = m_snakes[0].alive ? m_snakes[0].id : m_snakes[1].id;
		m_audio.stop();
	} else if (is_board_full()) {
		if (m_player_count == 1) {
			m_state = GameState::WON;
			m_winner = __FIRST_SNAKE;
		} else if (m_snakes[0].body.size() == m_snakes[1].body.size()) {
			m_state = GameState::DRAW;
		} else {
			m_state = GameState::WON;
			m_winner = m_snakes[0].body.size() > m_snakes[1].body.size() ? m_snakes[0].id : m_snakes[1].id;
		}
		m_audio.stop();
	}
}

Position	Game::get_next_pos(Position &pos, Event dir)
{
	size_t	dx = 0, dy = 0;

	switch (dir) {
	case Event::UP:
		dy = -1;
		break;
	case Event::DOWN:
		dy = 1;
		break;
	case Event::LEFT:
		dx = -1;
		break;
	case Event::RIGHT:
		dx = 1;
		break;
	default:
		break;
	}

	pos.x = pos.x + dx;
	pos.y = pos.y + dy;
	return pos;
}

void	Game::move_snake(Snake &snake)
{
	Position	head = snake.body.front();
	Position	next_pos = get_next_pos(head, snake.direction);

	bool	is_food = (next_pos == m_food.get_pos());
	snake.body.push_front(next_pos);

	if (!is_food) {
		snake.body.pop_back();
		return;
	}

	if (m_config.opt_enable_sounds)
		m_audio.play("EAT");
	snake.score += m_food.get_given_score();
	m_food.generate_food(m_config, m_snakes, m_obstacles);
}

bool	Game::is_position_free(const Position &pos)
{
	for (const auto &snake : m_snakes) {
		if (std::find(snake.body.begin(), snake.body.end(), pos) != snake.body.end())
			return false;
	}

	return std::find(m_obstacles.begin(), m_obstacles.end(), pos) == m_obstacles.end();
}

bool	Game::is_valid_obstacle_pos(const Position &pos)
{
	if (!is_position_free(pos))
		return false;

	const int	max_step = 10;
	for (const auto &snake : m_snakes) {
		Position	head = snake.body.front();
		for (int i = 0; i < max_step; i++) {
			head = get_next_pos(head, snake.direction);
			if (head == pos)
				return false;
		}
	}
	return true;
}

int	Game::get_max_obstacles(void)
{
	const int	board_size = m_config.width * m_config.height;
	size_t	snakes_size = m_snakes[0].body.size();
	snakes_size += m_config.opt_is_multi ? m_snakes[1].body.size() : 0;
	
	const int	total_free_slot = board_size - static_cast<int>(snakes_size) - 1;
	if (total_free_slot <= 0)
		return 0;

	return std::min(total_free_slot / 2, board_size * 20 / 100);
};

void	Game::generate_obstacle(void)
{
	int	max_obstacles = get_max_obstacles();
	if (m_config.obstacles_count > max_obstacles)
		throw std::logic_error("Too many obstacles for the board");

	m_obstacles.clear();
	m_obstacles.reserve(m_config.obstacles_count);

	std::uniform_int_distribution<size_t>	x_pos(0, m_config.width - 1);
	std::uniform_int_distribution<size_t>	y_pos(0, m_config.height - 1);

	size_t	i = 0;
	while (m_obstacles.size() < static_cast<size_t>(m_config.obstacles_count) && i < max_attempts) {
		Position	pos = {x_pos(m_rng), y_pos(m_rng)};
		if (is_valid_obstacle_pos(pos))
			m_obstacles.push_back(pos);
		i++;
	}
}

void	Game::reset_game(void)
{
	init_snakes();
	if (m_config.opt_have_obstacles)
		generate_obstacle();
	if (m_config.opt_enable_sounds)
		m_audio.reset();
	m_food.generate_food(m_config, m_snakes, m_obstacles);
	m_state = GameState::RUNNING;
	m_winner = -1;
	m_tick_rate = MIN_TICK_RATE;
	m_last_update_time = std::chrono::steady_clock::now();
}