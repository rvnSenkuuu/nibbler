#include "Game.hpp"

constexpr float	MIN_TICK_RATE = 10.0f;
constexpr float	MAX_TICK_RATE = 15.0f;
constexpr int	MIN_WIDTH = 15;
constexpr int	MIN_HEIGHT = 15;
constexpr int	MAX_WIDTH = 50;
constexpr int	MAX_HEIGHT = 50;

static constexpr size_t	max_attempts = 1000;

bool	is_position_free(const Position &pos, const std::vector<Snake> &snakes,
						const std::vector<Position> &obstacles)
{
	for (const auto &snake : snakes) {
		if (snake.occupies(pos))
			return false;
	}

	return std::find(obstacles.begin(), obstacles.end(), pos) == obstacles.end();
}

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

	m_food.set_rng(m_rng);
	m_food.generate_food(m_config, m_snakes, m_obstacles);
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

bool	Game::get_game_config(int argc, char **argv, GameConfig &config)
{
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "--help") {
			usage();
			std::exit(0);
		}
	}

	if (argc <= 2)
		throw std::invalid_argument("Invalid argument count");
	
	config.width = std::stoi(argv[1]);
	config.height = std::stoi(argv[2]);

	if (config.width < MIN_WIDTH || config.width > MAX_WIDTH || config.height < MIN_HEIGHT || config.height > MAX_HEIGHT)
		throw std::invalid_argument("Width or height values too low or too high");

	for (int i = 3; i < argc; i++) {
		std::string	arg = argv[i];

		if (arg == "--multi") {
			config.opt_is_multi = true;
		} else if (arg == "--limit-score") {
			config.opt_limit_score = true;
			if (argv[++i] == nullptr)
				return false;
			config.max_score = std::stoi(argv[i]);
			if (config.max_score <= 0)
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
	m_snakes.clear();
	m_snakes.reserve(m_player_count);

	size_t	row = m_player_count == 2 ? m_config.height / 3 : m_config.height / 2;

	m_snakes.emplace_back(FIRST_SNAKE, Position{m_config.width/ 2 , row});
	if (m_player_count == 2) {
		row = 2 * m_config.height / 3;
		m_snakes.emplace_back(SECOND_SNAKE, Position{m_config.width/ 2 , row});
	}
}

GameData	Game::get_data(void) const
{
	GameData	data;

	data.snakes = m_snakes;
	data.obstacles = m_obstacles;
	data.config = m_config;
	data.food = m_food;
	data.state = m_state;
	data.tick_rate = m_tick_rate;
	data.winner = m_winner;
	return data;
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
			m_snakes[FIRST_SNAKE].change_direction(event);
			break;
		case Event::UP:
		case Event::DOWN:
		case Event::RIGHT:
		case Event::LEFT:
			m_snakes[m_player_count == 2 ? SECOND_SNAKE : FIRST_SNAKE].change_direction(event);
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
			m_tick_rate = std::max(m_tick_rate - 1.0f, MIN_TICK_RATE);
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
	snake.die();
	if (m_config.opt_enable_sounds)
		m_audio.play("DEATH");
}

void	Game::check_collision(Snake &current)
{
	Position	head = current.get_head();
	if (head.x >= m_config.width || head.y >= m_config.height) {
		kill_snake(current);
		return;
	}

	if (std::find(m_obstacles.begin(), m_obstacles.end(), head) != m_obstacles.end()) {
		kill_snake(current);
		return;
	}

	for (auto &snake : m_snakes) {
		auto	begin_it = snake.get_body().begin();
		if (&current == &snake)
			begin_it = std::next(begin_it);

		if (std::any_of(
			begin_it,
			snake.get_body().end(),
			[&head](const Position &pos) { return head == pos; }
		)) {
			kill_snake(current);
			break;
		}
	}
}

bool	Game::is_board_full(void) const
{
	size_t	total = 0;
	for (auto &snake : m_snakes)
		total += snake.length();

	return total >= m_config.width * m_config.height - m_obstacles.size();
}

void	Game::update_game(void)
{
	for (auto &snake : m_snakes) {
		if (snake.get_alive())
			move_snake(snake);
	}

	for (auto &snake : m_snakes) {
		if (snake.get_alive())
			check_collision(snake);
	}

	if (m_config.opt_limit_score) {
		size_t	max = m_config.max_score;
		auto	it = std::find_if(
			m_snakes.begin(),
			m_snakes.end(),
			[&max](const auto &snake) { return snake.get_score() >= max; }
		);

		if (it != m_snakes.end()) {
			m_state = GameState::WON;
			m_winner = it->get_id();
			m_audio.stop();
			return;
		}
	}

	size_t	alive_count = std::count_if(
		m_snakes.begin(),
		m_snakes.end(),
		[](const Snake &snake) { return snake.get_alive(); }
	);

	if (alive_count == 0) {
		m_state = m_player_count == 2 ? GameState::DRAW : GameState::GAME_OVER;
	} else if (m_player_count == 2 && alive_count == 1) {
		m_state = GameState::WON;
		m_winner = m_snakes[0].get_alive() ? m_snakes[0].get_id() : m_snakes[1].get_id();
	} else if (is_board_full()) {
		if (m_player_count == 1) {
			m_state = GameState::WON;
			m_winner = FIRST_SNAKE;
		} else if (m_snakes[0].length() == m_snakes[1].length()) {
			m_state = GameState::DRAW;
		} else {
			m_state = GameState::WON;
			m_winner = m_snakes[0].length() > m_snakes[1].length() ? m_snakes[0].get_id() : m_snakes[1].get_id();
		}
		m_audio.stop();
	}
}

void	Game::move_snake(Snake &snake)
{
	Position	next_pos = snake.next_pos(snake.get_head());
	bool	is_food = (next_pos == m_food.get_pos());

	snake.move(next_pos, is_food);
	if (!is_food)
		return;

	if (m_config.opt_enable_sounds)
		m_audio.play("EAT");

	snake.add_score(m_food.get_given_score());
	m_food.generate_food(m_config, m_snakes, m_obstacles);
}

bool	Game::is_valid_obstacle_pos(const Position &pos) const
{
	if (!is_position_free(pos, m_snakes, m_obstacles))
		return false;

	const int	max_step = 20;
	for (auto &snake : m_snakes) {
		Position	head = snake.get_head();
		for (int i = 0; i < max_step; i++) {
			head = snake.next_pos(head);
			if (head == pos)
				return false;
		}
	}
	return true;
}

int	Game::get_max_obstacles(void) const
{
	const int	board_size = m_config.width * m_config.height;
	size_t	snakes_size = m_snakes[0].length();
	snakes_size += m_config.opt_is_multi ? m_snakes[1].length() : 0;
	
	const int	total_free_slot = board_size - static_cast<int>(snakes_size) - 1;
	if (total_free_slot <= 0)
		return 0;

	return std::min(total_free_slot / 2, board_size * 20 / 100);
}

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