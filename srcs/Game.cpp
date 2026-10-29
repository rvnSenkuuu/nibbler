#include "Game.hpp"

constexpr float	MIN_TICK_RATE{10.0f};
constexpr float	MAX_TICK_RATE{15.0f};
constexpr int	MIN_WIDTH{15};
constexpr int	MIN_HEIGHT{15};
constexpr int	MAX_WIDTH{50};
constexpr int	MAX_HEIGHT{50};

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
	const std::optional<GameConfig>	config = get_game_config(argc, argv);
	if (!config)
		throw std::invalid_argument("Failed to get game configuration");
	m_config = *config;

	if (m_config.opt_sounds)
		m_audio.init();

	init_snakes();
	if (m_config.opt_have_obstacles)
		generate_obstacle();

	m_food.generate_food(m_config, m_rng, m_snakes, m_obstacles);
}

void	Game::reset_game(void)
{
	init_snakes();
	m_obstacles.clear();
	if (m_config.opt_have_obstacles)
		generate_obstacle();
	if (m_config.opt_sounds)
		m_audio.reset();
	m_food.generate_food(m_config, m_rng, m_snakes, m_obstacles);
	m_state = GameState::RUNNING;
	m_winner = -1;
	m_tick_rate = MIN_TICK_RATE;
	m_last_update_time = std::chrono::steady_clock::now();
}

std::optional<GameConfig>	Game::get_game_config(int argc, char **argv)
{
	for (int i{1}; i < argc; i++) {
		if (std::string(argv[i]) == "--help") {
			usage();
			std::exit(0);
		}
	}

	if (argc < 3)
		return std::nullopt;
	
	GameConfig	config;

	if (!get_option_value(argv[1], config.width) || !get_option_value(argv[2], config.height))
		return std::nullopt;

	if (config.width < MIN_WIDTH || config.width > MAX_WIDTH ||
		config.height < MIN_HEIGHT || config.height > MAX_HEIGHT)
		return std::nullopt;

	for (int i{3}; i < argc; i++) {
		const std::string_view	arg = argv[i];

		if (arg == "--multi") {
			config.opt_multi = true;
			m_player_count = 2;
		} else if (arg == "--limit-score") {
			if (i + 1 >= argc || !get_option_value(argv[i + 1], config.max_score) || config.max_score <= 0)
				return std::nullopt;
			config.opt_limit_score = true;
			i++;
		} else if (arg == "--food-variance") {
			config.opt_food_variance = true;
		} else if (arg == "--food-disappear") {
			config.opt_food_disappear = true;
		} else if (arg == "--enable-obstacles") {
			if (i + 1 >= argc || !get_option_value(argv[i + 1], config.obstacles_count) || config.obstacles_count < 0)
				return std::nullopt;
			config.opt_have_obstacles = true;
			i++;
		} else if (arg == "--enable-sounds") {
			config.opt_sounds = true;
		} else if (arg == "--enable-trap") {
			config.opt_trap = true;
		} else {
			return std::nullopt;
		}
	}

	if (config.opt_trap && !config.opt_multi)
		return std::nullopt;

	return config;
}

void	Game::init_snakes(void)
{
	m_snakes.clear();
	m_snakes.reserve(m_player_count);

	size_t	row{m_player_count == 2 ? m_config.height / 3 : m_config.height / 2};

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
	if (m_state != GameState::RUNNING) {
		if (event == Event::RESTART)
			reset_game();
		return;
	}

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
	case Event::P1_SHRINK:
		if (m_config.opt_trap)
			m_snakes[FIRST_SNAKE].shrink(m_obstacles, m_audio);
		break;
	case Event::P2_SHRINK:
		if (m_config.opt_trap)
			m_snakes[SECOND_SNAKE].shrink(m_obstacles, m_audio);
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

void	Game::run(Lib &lib)
{
	lib.create(m_config.width, m_config.height);
	while (true) {
		Event	event{lib.get_event()};
		if (event == Event::QUIT)
			return;
		
		handle_event(lib, event);
		if (m_state == GameState::RUNNING) {
			auto	current_time{std::chrono::steady_clock::now()};
			float	delta_time{std::chrono::duration<float>(
				current_time - m_last_update_time).count()};
			
			if (delta_time >= 1.0f / m_tick_rate) {
				if (m_config.opt_food_disappear)
					handle_disappearing_food(current_time);

				update_game();
				m_last_update_time = current_time;
			}
			
			if (m_config.opt_sounds)
				m_audio.update();
		}
		lib.render(get_data());
	}
}

void	Game::kill_snake(Snake &snake)
{
	snake.die();
	if (m_config.opt_sounds)
		m_audio.play("DEATH");
}

void	Game::check_collision(Snake &current)
{
	const Position	head{current.get_head()};
	if (head.x >= m_config.width || head.y >= m_config.height) {
		kill_snake(current);
		return;
	}

	if (std::find(m_obstacles.begin(), m_obstacles.end(), head) != m_obstacles.end()) {
		kill_snake(current);
		return;
	}

	for (auto &snake : m_snakes) {
		auto	begin_it{snake.get_body().begin()};
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
	size_t	total{0};
	for (const auto &snake : m_snakes)
		total += snake.length();

	return total + 1 >= m_config.width * m_config.height - m_obstacles.size();
}

void	Game::end_game(GameState state, int winner)
{
	m_state = state;
	m_winner = winner;
	if (m_config.opt_sounds)
		m_audio.stop();
}

void	Game::end_by_board_full(void)
{
    if (m_player_count == 1)
        end_game(GameState::WON, FIRST_SNAKE);
	else if (m_snakes[0].length() == m_snakes[1].length())
        end_game(GameState::DRAW, -1);
	else
        end_game(GameState::WON, m_snakes[0].length() > m_snakes[1].length()
			? m_snakes[0].get_id() : m_snakes[1].get_id());
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
		size_t	max{static_cast<std::size_t>(m_config.max_score)};
		auto	it = std::find_if(
			m_snakes.begin(),
			m_snakes.end(),
			[&max](const auto &snake) { return snake.get_alive() && snake.get_score() >= max; }
		);

		if (it != m_snakes.end()) {
			end_game(GameState::WON, it->get_id());
			return;
		}
	}

	size_t	alive_count = std::count_if(
		m_snakes.begin(),
		m_snakes.end(),
		[](const Snake &snake) { return snake.get_alive(); }
	);


	if (alive_count == 0)
		end_game(m_player_count == 2 ? GameState::DRAW : GameState::GAME_OVER, -1);
	else if (m_player_count == 2 && alive_count == 1)
		end_game(GameState::WON, m_snakes[0].get_alive() ? m_snakes[0].get_id() : m_snakes[1].get_id());
	else if (is_board_full())
		end_by_board_full();
}

void	Game::move_snake(Snake &snake)
{
	Position	next_pos{snake.next_pos(snake.get_head())};
	bool	is_food{next_pos == m_food.get_pos()};

	snake.move(next_pos, is_food);
	if (!is_food)
		return;

	if (m_config.opt_sounds)
		m_audio.play("EAT");

	snake.add_score(m_food.get_given_score());
	if (!m_food.generate_food(m_config, m_rng, m_snakes, m_obstacles))
		end_game(GameState::WON, snake.get_id());
}

bool	Game::is_valid_obstacle_pos(const Position &pos) const
{
	if (!is_position_free(pos, m_snakes, m_obstacles))
		return false;

	const int	max_step{20};
	for (auto &snake : m_snakes) {
		Position	head{snake.get_head()};
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
	const size_t	board_size{m_config.width * m_config.height};
	size_t	snakes_size{m_snakes[0].length()};
	snakes_size += m_config.opt_multi ? m_snakes[1].length() : 0;
	
	const size_t	total_free_slot{board_size - static_cast<int>(snakes_size) - 1};
	if (total_free_slot <= 0)
		return 0;

	return std::min(total_free_slot / 2, board_size * 20 / 100);
}

void	Game::generate_obstacle(void)
{
	if (m_config.obstacles_count > get_max_obstacles())
		throw std::logic_error("Too many obstacles for the board");

	m_obstacles.clear();
	m_obstacles.reserve(m_config.obstacles_count);

	std::vector<Position>	slot;
	slot.reserve(m_config.width * m_config.height);
	for (size_t i{0}; i < m_config.width * m_config.height; i++) {
		const Position	pos{i % m_config.width, i / m_config.width};
		if (is_valid_obstacle_pos(pos))
			slot.push_back(pos);
		}

	if (slot.size() < static_cast<std::size_t>(m_config.obstacles_count))
		throw std::logic_error("Not enough valid cells for the requested obstacles");
	
	std::shuffle(slot.begin(), slot.end(), m_rng);
	m_obstacles.assign(slot.begin(), slot.begin() + m_config.obstacles_count);
}

void	Game::handle_disappearing_food(std::chrono::steady_clock::time_point &current_time)
{
	float	food_delta_time{std::chrono::duration<float>(
		current_time - m_food.get_spawn_time()).count()};
	if (food_delta_time >= FOOD_LIFETIME &&
		!m_food.generate_food(m_config, m_rng, m_snakes, m_obstacles))
			end_by_board_full();	
}