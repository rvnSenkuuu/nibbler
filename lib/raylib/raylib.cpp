#include <raylib.h>
#include <iostream>

#include "Types.hpp"
#include "IGraphic.hpp"

class GraphicLib : public IGraphic {
	public:
		GraphicLib(void) { }
		~GraphicLib(void) { }

		void	create_window(int width, int height) override
		{
			SetTraceLogLevel(LOG_NONE);
			InitWindow(width * CELL_SIZE, height * CELL_SIZE, "Nibbler");
		}

		void	render_game(const GameData &data) override
		{
			BeginDrawing();
			ClearBackground(BLACK);

			for (size_t i = 0; i < data.snake.size(); i++) {
				const auto	&pos = data.snake.at(i);
				Color	color = i == 0 ? DARKGREEN : GREEN;
				DrawRectangle(pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
			}

			DrawCircle(data.food.x * CELL_SIZE + 15, data.food.y * CELL_SIZE + 15, 15.f, RED);
			DrawText(TextFormat("Score: %zu", data.score), 10, 10, 20, RAYWHITE);

			EndDrawing();
		}

		void	render_end_game(const GameData &data)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			const int	font_size = 20;
			const int	center_x = data.width * CELL_SIZE / 2;
			const int	start_y = data.height * CELL_SIZE / 2 - (100 + font_size) / 2;

			auto	draw_centered = [&](const std::string &text, int y) {
				int	text_width = MeasureText(text.c_str(), font_size);
				DrawText(text.c_str(), center_x - text_width / 2, y, font_size, RAYWHITE);
			};

			const std::string	end_msg = data.state == GameState::WON ? "You Won !" : "Game Over !";
			draw_centered(end_msg, start_y);
			draw_centered(TextFormat("Your score: %zu", data.score), start_y + 45);
			draw_centered("Press 'Enter' for restart the game", start_y + 100);

			EndDrawing();
		}

		Event	get_event(void) override
		{
			int	key = GetKeyPressed();
			switch (key) {
			case KEY_ESCAPE:
				return Event::QUIT;
			case KEY_ENTER:
				return Event::RESTART;
			case KEY_W:
				return Event::UP;
			case KEY_S:
				return Event::DOWN;
			case KEY_A:
				return Event::LEFT;
			case KEY_D:
				return Event::RIGHT;
			case KEY_ONE:
				return Event::LIB1;
			case KEY_TWO:
				return Event::LIB2;
			case KEY_THREE:
				return Event::LIB3;
			default:
				break;
			}
			return Event::NONE;
		}

		void	destroy_window(void) override
		{
			CloseWindow();
		}
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