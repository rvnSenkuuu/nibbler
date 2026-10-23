#include <SFML/Graphics.hpp>

#include "Types.hpp"
#include "IGraphic.hpp"

const sf::Color	RAYWHITE_C(245, 245, 245);
const sf::Color	RED(255, 0, 0);
const sf::Color	DARKBLUE_C(0, 82, 172);
const sf::Color	BLUE_C(0, 121, 241);
const sf::Color	DARKBROWN_C(76, 63, 47);
const sf::Color	BROWN_C(127, 106, 79);

class GraphicLib : public IGraphic {
	public:

		GraphicLib(void) : m_window(nullptr), m_font(nullptr) { };
		~GraphicLib(void) { };

		void	create_window(int width, int height) override
		{
			m_window = new sf::RenderWindow(
				sf::VideoMode(width * CELL_SIZE, height * CELL_SIZE),
				"Nibbler",
				sf::Style::Close
			);

			m_font = new sf::Font();
			if (!m_font->loadFromFile(FONT_PATH)) {
				destroy_window();
				throw std::runtime_error("Failed to load font");
			}
		}
		
		void	render_game(const GameData &data) override
		{
			m_window->clear(sf::Color::Black);

			for (auto &snake : data.snakes)
				draw_snake(snake, 
					snake.get_id() == FIRST_SNAKE ? p1_color[0] : p2_color[0],
					snake.get_id() == FIRST_SNAKE ? p1_color[1] : p2_color[1]
				);

			sf::Color	food_color = get_food_color(data.food.get_type());
			sf::RectangleShape	food(sf::Vector2f(CELL_SIZE, CELL_SIZE));
			food.setFillColor(food_color);
			const Position	&pos = data.food.get_pos();
			food.setPosition(sf::Vector2f(
				static_cast<float>(pos.x * CELL_SIZE), 
				static_cast<float>(pos.y * CELL_SIZE))
			);
			m_window->draw(food);

			draw_score(data.snakes, p1_color[1], p2_color[1]);
			draw_speed(data);

			m_window->display();
		}

		void	render_end_game(const GameData &data)
		{
			const int	font_size = 20;
			const float	center_x = data.config.width * CELL_SIZE / 2.f;
			const float	start_y = data.config.height * CELL_SIZE / 2.f - (100 + font_size) / 2.f;

			auto	draw_centered = [&](const std::string &str, float y, sf::Color color) {
				sf::Text	text;
				text.setFont(*m_font);
				text.setString(str);
				text.setCharacterSize(font_size);
				text.setFillColor(color);

				sf::FloatRect	bounds = text.getLocalBounds();
				text.setPosition(center_x - bounds.width / 2.f, y);

				m_window->draw(text);
			};

			m_window->clear(sf::Color::Black);

			if (data.config.opt_is_multi) {
				std::string	game_winner;
				if (data.state == GameState::DRAW)
					game_winner = "Draw !";
				else
					game_winner = data.winner == data.snakes[0].get_id() ? "P1 won !" : "P2 won !";

				draw_centered(game_winner, start_y, sf::Color::White);
				draw_centered("P1 Score: " + std::to_string(data.snakes[0].get_score()), start_y + 30, p1_color[1]);
				draw_centered("P2 Score: " + std::to_string(data.snakes[1].get_score()), start_y + 60, p2_color[1]);
				draw_centered("Press 'Enter' for restart the game", start_y + 90, sf::Color::White);
				
				m_window->display();
				return;
			}

			const std::string	end_msg = data.state == GameState::WON ? "You Won !" :  "Game Over !";
			draw_centered(end_msg, start_y, sf::Color::White);
			draw_centered("Your score: " + std::to_string(data.snakes[0].get_score()), start_y + 45, p1_color[1]);
			draw_centered("Press 'Enter' for restart the game", start_y + 100, sf::Color::White);

			m_window->display();
		}

		Event	get_event(void) override
		{
			sf::Event	event;
			while (m_window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					return Event::QUIT;
				if (event.type == sf::Event::KeyPressed) {
					switch (event.key.code) {
					case sf::Keyboard::Num1:
						return Event::LIB1;
					case sf::Keyboard::Num2:
						return Event::LIB2;
					case sf::Keyboard::Num3:
						return Event::LIB3;
					case sf::Keyboard::W:
						return Event::W;
					case sf::Keyboard::S:
						return Event::S;
					case sf::Keyboard::A:
						return Event::A;
					case sf::Keyboard::D:
						return Event::D;
					case sf::Keyboard::Up:
						return Event::UP;
					case sf::Keyboard::Down:
						return Event::DOWN;
					case sf::Keyboard::Left:
						return Event::LEFT;
					case sf::Keyboard::Right:
						return Event::RIGHT;
					case sf::Keyboard::Escape:
						return Event::QUIT;
					case sf::Keyboard::Enter:
						return Event::RESTART;
					case sf::Keyboard::Equal:
						return Event::SPEED_UP;
					case sf::Keyboard::Hyphen:
						return Event::SPEED_DOWN;
					default:
						break;
					}
				}
			}
			return Event::NONE;
		}

		void	destroy_window(void) override
		{
			if (m_window) {
				m_window->close();
				delete m_window;
				m_window = nullptr;
			}
			delete m_font;
			m_font = nullptr;
		}

		private:
			GraphicLib(const GraphicLib &other) = delete;
			GraphicLib	&operator=(const GraphicLib &other) = delete;

			void	draw_score(const std::vector<Snake> &snakes, sf::Color p1, sf::Color p2)
			{
				sf::Text	score;
				score.setFont(*m_font);
				score.setCharacterSize(20);
				score.setString("P1 Score: " + std::to_string(snakes[0].get_score()));
				score.setFillColor(p1);
				score.setPosition(10, 10);
				m_window->draw(score);
				
				if (snakes.size() == 2) {
					score.setString("P2 Score: " + std::to_string(snakes[1].get_score()));
					score.setFillColor(p2);
					score.setPosition(10, 40);
					m_window->draw(score);
				}
			}

			void	draw_speed(const GameData &data)
			{
				char speed[32];
				snprintf(speed, sizeof(speed), "Speed: %.2f", data.tick_rate);

				sf::Text	text;
				text.setFont(*m_font);
				text.setString(speed);
				text.setCharacterSize(20);
				text.setFillColor(RAYWHITE_C);

				sf::FloatRect	bounds = text.getLocalBounds();
				text.setOrigin(bounds.width, 0.f);
				text.setPosition(data.config.width * CELL_SIZE - 10.f, 10.f);
				m_window->draw(text);
			}

			void	draw_snake(const Snake &snake, const sf::Color &head_color, const sf::Color &body_color)
			{
				for (auto &pos : snake.get_body()) {
					sf::RectangleShape	cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
					sf::Color	color = (pos == snake.get_head()) ? head_color : body_color;
					cell.setFillColor(color);
					cell.setPosition(sf::Vector2f(
						static_cast<float>(pos.x * CELL_SIZE),
						static_cast<float>(pos.y * CELL_SIZE))
					);
					m_window->draw(cell);
				}
			}

			sf::Color	get_food_color(const FoodType &type)
			{
				sf::Color	food_color = sf::Color::Red;	
				if (type == FoodType::FOOD_GREEN)
					food_color = sf::Color::Green;
				else if (type == FoodType::FOOD_YELLOW)
					food_color = sf::Color::Yellow;
				return food_color;
			}

			sf::RenderWindow	*m_window;
			sf::Font	*m_font;

			const sf::Color	p1_color[2] = {DARKBROWN_C, BROWN_C};
			const sf::Color	p2_color[2] = {DARKBLUE_C, BLUE_C};
};

GraphicLib	lib;

extern "C" {
	void	create_window(int width, int height)
	{
		lib.create_window(width, height);
	}

	void	render_game(const GameData &data)
	{
		if (data.state == GameState::GAME_OVER || data.state == GameState::WON || data.state == GameState::DRAW) {
			lib.render_end_game(data);
			return;
		}
		lib.render_game(data);
	}

	Event	get_event(void)
	{
		return lib.get_event();
	}

	void	destroy_window(void)
	{
		lib.destroy_window();		
	}
}