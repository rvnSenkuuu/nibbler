#include <raylib.h>
#include <iostream>

#include "Types.hpp"
#include "IGraphic.hpp"

static constexpr int	cell_size = 30;

class GraphicLib : public IGraphic {
	public:
		GraphicLib(void) { }
		~GraphicLib(void) { }

		void	create_window(int width, int height) override
		{
			SetTraceLogLevel(LOG_NONE);
			InitWindow(width * cell_size, height * cell_size, "Nibbler");
		}

		void	render_game(const GameState &state) override
		{
			BeginDrawing();
			ClearBackground(BLACK);

			for (size_t i = 0; i < state.snake.size(); i++) {
				const auto	&pos = state.snake.at(i);
				Color	color = i == 0 ? DARKGREEN : GREEN;
				DrawRectangle(pos.x * cell_size, pos.y * cell_size, cell_size, cell_size, color);
			}

			DrawRectangle(state.food.x * cell_size, state.food.y * cell_size, cell_size, cell_size, RED);
			DrawText(TextFormat("Score: %zu", state.score), 10, 10, 20, RAYWHITE);

			EndDrawing();
		}

		Event	get_event(void) override
		{
			int	key = GetKeyPressed();
			switch (key) {
			case KEY_ESCAPE:
				return Event::QUIT;
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

	void	render_game(const GameState &state)
	{
		lib.render_game(state);
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