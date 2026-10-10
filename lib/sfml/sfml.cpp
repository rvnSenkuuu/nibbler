#include <SFML/Graphics.hpp>

#include "Types.hpp"
#include "IGraphic.hpp"

const sf::Color	RAYWHITE_C(245, 245, 245);
const sf::Color	RED_C(255, 0, 0);
const sf::Color	DARKGREEN_C(0, 117, 44);
const sf::Color	GREEN_C(0, 228, 48);

class GraphicLib : public IGraphic {
	public:

		GraphicLib(void) : m_window(nullptr), m_font(nullptr) { };
		~GraphicLib(void) { };

		void	create_window(int width, int height) override
		{
			m_window = new sf::RenderWindow(
				sf::VideoMode(width * CELL_SIZE, height * CELL_SIZE),
				"Nibbler",
				sf::Style::Default);

			m_font = new sf::Font();
			if (!m_font->loadFromFile(FONT_PATH)) {
				destroy_window();
				throw std::runtime_error("Failed to load font");
			}
		}

		void	render_game(const GameData &data) override
		{
			m_window->clear(sf::Color::Black);

			sf::Text	score;
			score.setFont(*m_font);
			score.setString("Score: " + std::to_string(data.score));
			score.setCharacterSize(20);
			score.setFillColor(RAYWHITE_C);
			score.setPosition(15, 15);
			m_window->draw(score);

			for (size_t i = 0; i < data.snake.size(); i++) {
				const auto	&pos = data.snake.at(i);
				draw_cell(pos.x, pos.y, i == 0 ? DARKGREEN_C : GREEN_C);
			}

			sf::RectangleShape	food(sf::Vector2f(CELL_SIZE, CELL_SIZE));
			food.setFillColor(RED_C);
			food.setPosition(sf::Vector2f(static_cast<float>(data.food.x * CELL_SIZE), 
				static_cast<float>(data.food.y * CELL_SIZE)));
			m_window->draw(food);

			m_window->display();
		}

		void	render_end_game(const GameData &data)
		{
			const int	font_size = 20;
			const float	center_x = data.width * CELL_SIZE / 2.f;
			const float	start_y = data.height * CELL_SIZE / 2.f - (100 + font_size) / 2.f;

			auto	draw_centered = [&](const std::string &str, float y) {
				sf::Text	text;

				text.setFont(*m_font);
				text.setString(str);
				text.setCharacterSize(font_size);
				text.setFillColor(RAYWHITE_C);

				sf::FloatRect	bounds = text.getLocalBounds();
				text.setPosition(center_x - bounds.width / 2.f, y);

				m_window->draw(text);
			};

			m_window->clear(sf::Color::Black);

			const std::string	end_msg = data.state == GameState::WON ? "You Won !" :  "Game Over !";
			draw_centered(end_msg, start_y);
			draw_centered("Your score: " + std::to_string(data.score), start_y + 45);
			draw_centered("Press 'Enter' for restart the game", start_y + 100);

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
						return Event::UP;
					case sf::Keyboard::S:
						return Event::DOWN;
					case sf::Keyboard::A:
						return Event::LEFT;
					case sf::Keyboard::D:
						return Event::RIGHT;
					case sf::Keyboard::Escape:
						return Event::QUIT;
					case sf::Keyboard::Enter:
						return Event::RESTART;
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

			void	draw_cell(int x, int y, sf::Color color)
			{
				sf::RectangleShape	cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
				cell.setFillColor(color);
				cell.setPosition(sf::Vector2f(static_cast<float>(x * CELL_SIZE),
					static_cast<float>(y * CELL_SIZE)));
				m_window->draw(cell);
			}

			sf::RenderWindow	*m_window;
			sf::Font	*m_font;
};

GraphicLib	lib;

extern "C" {
	void	create_window(int width, int height)
	{
		lib.create_window(width, height);
	}

	void	render_game(const GameData &data)
	{
		if (data.state == GameState::GAME_OVER || data.state == GameState::WON) {
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