#include "Game.hpp"

Game::Game(int argc, char **argv) : m_direction(Event::LEFT), m_rng(std::random_device{}()), m_score(0), m_game_over(false),
									m_tick_rate(10.0f), m_last_update_time(std::chrono::steady_clock::now())
{
	if (argc != 3)
		throw std::logic_error("Usage: ./nibbler <width> <height>");

	this->m_width = std::stoi(argv[1]);
	this->m_height = std::stoi(argv[2]);

	//TODO: Check for higher value
	if (this->m_width < 10 || this->m_height < 10)
		throw std::invalid_argument("Width or height values too low");

	m_snake.push_back({m_width / 2, m_height / 2});
	m_snake.push_back({m_width / 2 + 1, m_height / 2});
	m_snake.push_back({m_width / 2 + 2, m_height / 2});
	m_snake.push_back({m_width / 2 + 3, m_height / 2});

	m_grid.resize(m_width, std::vector<GridType>(m_height, GridType::EMPTY));
}

Game::~Game(void) { }

GameState	Game::get_state(void) const
{
	GameState	state = {
		.snake = this->m_snake,
		.food = this->m_food,
		.score = this->m_score,
		.width = this->m_width,
		.height = this->m_height,
		.game_over = this->m_game_over};

	return state;
}

void	Game::run(Lib &lib)
{
	this->generate_food();
	lib.create(this->m_width, this->m_height);
	while (!this->m_game_over) {
		auto	current_time = std::chrono::steady_clock::now();
		float	delta_time = std::chrono::duration<float>(current_time - this->m_last_update_time).count();

		if (delta_time >= 1.0f / m_tick_rate) {

			// lib.get_event();
			this->move_snake();
			this->check_collision();
			if (this->m_game_over)
				break;
			this->resync_grid();
			// this->render();
			lib.render(this->get_state());
			this->m_last_update_time = current_time;
		}
	}
}

void	Game::check_collision(void)
{
	Position	head = this->m_snake.front();
	if (head.x >= this->m_width || head.y >= this->m_height) {
		this->m_game_over = true;
		return;
	}

	for (size_t i = 1; i < this->m_snake.size(); i++) {
		if (head == this->m_snake[i]) {
			this->m_game_over = true;
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
		if (!is_opposite_dir(this->m_direction, event))
			this->m_direction = event;
	}
}

void	Game::move_snake(void)
{
	Position	head = this->m_snake.front();

	switch (this->m_direction) {
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

	bool	is_food = (head == this->m_food);
	this->m_snake.push_front(head);

	if (is_food) {
		this->m_score += 10;
		this->generate_food();
	} else {
		this->m_snake.pop_back();
	}
}

void	Game::resync_grid(void)
{
	for (auto &col : this->m_grid)
		std::fill(col.begin(), col.end(), GridType::EMPTY);

	Position	head_pos = this->m_snake.front();
	this->m_grid[head_pos.x][head_pos.y] = GridType::SNAKE_HEAD;
	for (size_t i = 1; i < this->m_snake.size(); i++) {
		const Position	&pos = this->m_snake[i];
		this->m_grid[pos.x][pos.y] = GridType::SNAKE_BODY;
	}
	
	this->m_grid[this->m_food.x][this->m_food.y] = GridType::FOOD;
}

//TODO: Rendering shouldn’t be handled by the core
// void	Game::render(void)
// {
// 	std::cout << "\033[2J\033[H";
// 	std::cout << "Score = " << m_score << "\n\n";
	
// 	for (size_t i = 0; i < this->m_width; i++)
// 		std::cout << "=";
// 	std::cout << std::endl;

// 	for (size_t y = 0; y < this->m_height; y++) {
// 		std::cout << '|';
// 		for (size_t x = 0; x < this->m_width; x++) {
// 			switch (this->m_grid[x][y]) {
// 			case GridType::SNAKE_HEAD:
// 				std::cout << 'O';
// 				break;
// 			case GridType::SNAKE_BODY:
// 				std::cout << 'o'; 
// 				break;
// 			case GridType::FOOD:
// 				std::cout << '@';
// 				break;
// 			default:
// 				std::cout << ' ';
// 				break;
//             }
// 		}
// 		std::cout << "|\n";
// 	}

// 	for (size_t i = 0; i < this->m_width; i++)
// 		std::cout << "=";
// 	std::cout << std::endl;

// 	std::cout.flush();
// }

void	Game::generate_food(void)
{
	if (this->m_snake.size() == this->m_width * this->m_height)
		return;

	std::uniform_int_distribution<size_t>	x_pos(0, this->m_width - 1);
	std::uniform_int_distribution<size_t>	y_pos(0, this->m_height - 1);

	bool	valid_pos = false;
	Position	new_food;
	while (!valid_pos) {
		new_food = {x_pos(m_rng), y_pos(m_rng)};
		
		valid_pos = true;
		for (auto &snake_part : this->m_snake) {
			if (snake_part == new_food) {
				valid_pos = false;
				break;
			}
		}
	}

	this->m_food = new_food;
}