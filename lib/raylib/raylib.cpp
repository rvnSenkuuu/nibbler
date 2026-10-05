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

		void	render_game(const GameState &state) override
		{
			BeginDrawing();
			ClearBackground(BLACK);

			for (size_t i = 0; i < state.snake.size(); i++) {
				const auto	&pos = state.snake.at(i);
				Color	color = i == 0 ? DARKGREEN : GREEN;
				DrawRectangle(pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
			}

			DrawRectangle(state.food.x * CELL_SIZE, state.food.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
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