#include "Game.hpp"

Game::Game(int argc, char **argv) : m_rng(std::random_device{}()), m_score(0), m_game_over(false),
									m_tick_rate(10.0f), m_last_update_time(std::chrono::steady_clock::now())
{
	if (argc != 3)
		throw std::logic_error("Usage: ./nibbler <width> <height>");

	this->m_width = std::stoi(argv[1]);
	this->m_height = std::stoi(argv[2]);

	if (this->m_width < 10 || this->m_height < 10)
		throw std::invalid_argument("Width or height values too low");

	m_snake.push_back({m_width / 2, m_height / 2});
	m_snake.push_back({m_width / 2, m_height / 2 + 1});
	m_snake.push_back({m_width / 2, m_height / 2 + 2});
	m_snake.push_back({m_width / 2, m_height / 2 + 3});

	m_grid.resize(m_height, std::vector<GridType>(m_width, GridType::EMPTY));
}

Game::~Game(void)
{
}

void	Game::run(void)
{
	while (!this->m_game_over) {
		auto	current_time = std::chrono::steady_clock::now();
		float	delta_time = std::chrono::duration<float>(current_time - m_last_update_time).count();

		if (delta_time >= 1.0f / m_tick_rate) {
			this->generate_food(); // Used for testing; remove later
			this->update();
			this->render();
			this->m_last_update_time = current_time;
		}
	}
}

void	Game::update(void)
{
	for (auto &row : this->m_grid)
		std::fill(row.begin(), row.end(), GridType::EMPTY);

	Position	head_pos = this->m_snake[0];
	this->m_grid[head_pos.x][head_pos.y] = GridType::SNAKE_HEAD;
	for (size_t i = 1; i < this->m_snake.size(); i++) {
		const Position	&pos = this->m_snake[i];
		this->m_grid[pos.x][pos.y] = GridType::SNAKE_BODY;
	}
	this->m_grid[this->m_food.x][this->m_food.y] = GridType::FOOD;
}

//TODO: Rendering shouldn’t be handled by the core
void	Game::render(void)
{
	std::cout << "\033[2J\033[H";
	std::cout << "Score = " << m_score << "\n\n";
	
	for (auto &row: this->m_grid) {
		for (const auto &cell: row) {
			switch (cell) {
			case GridType::SNAKE_HEAD:
				std::cout << 'O';
				break;
			case GridType::SNAKE_BODY:
				std::cout << 'o'; 
				break;
			case GridType::FOOD:
				std::cout << '@';
				break;
			default:
				std::cout << ' ';
				break;
            }
		}
		std::cout << '\n';
	}
	std::cout.flush();
}

void	Game::generate_food(void)
{
	std::uniform_int_distribution<int>	x_pos(0, this->m_width - 1);
	std::uniform_int_distribution<int>	y_pos(0, this->m_height - 1);

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