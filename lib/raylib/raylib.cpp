#include <raylib.h>
#include <iostream>

#include "Types.hpp"
#include "IGraphic.hpp"

const Color	p1_color[2] = {DARKPURPLE, PURPLE};
const Color	p2_color[2] = {DARKBLUE, BLUE};

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

			for (auto &pos : data.obstacles) {
				Rectangle	rec = {
					.x = static_cast<float>(pos.x) * CELL_SIZE,
					.y = static_cast<float>(pos.y) * CELL_SIZE,
					.width = CELL_SIZE,
					.height = CELL_SIZE
				};
				DrawRectangleRoundedLines(rec, 0.15f, 0, 1.5f, WHITE);
			}

			for (size_t i = 0; i < data.snakes.size(); i++) {
				Snake	snake = data.snakes[i];
				draw_snake(snake, i == 0 ? p1_color[0] : p2_color[0], i == 0 ? p1_color[1] : p2_color[1]);
			}

			DrawCircle(
				data.food.pos.x * CELL_SIZE + 15,
				data.food.pos.y * CELL_SIZE + 15, 15.f,
				get_food_color(data.food.type)
			);
			draw_score(data.snakes, p1_color[1], p2_color[1]);
			
			EndDrawing();
		}

		void	render_end_game(const GameData &data)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			const int	font_size = 20;
			const int	center_x = data.config.width * CELL_SIZE / 2;
			const int	start_y = data.config.height * CELL_SIZE / 2 - (100 + font_size) / 2;

			auto	draw_centered = [&](const std::string &text, int y, Color color) {
				int	text_width = MeasureText(text.c_str(), font_size);
				DrawText(text.c_str(), center_x - text_width / 2, y, font_size, color);
			};

			if (data.config.opt_is_multi) {
				std::string	game_winner;
				if (data.state == GameState::DRAW)
					game_winner = "Draw !";
				else
					game_winner = data.winner == data.snakes[0].id ? "P1 won !" : "P2 won !";

				draw_centered(game_winner, start_y, RAYWHITE);
				draw_centered(TextFormat("P1 Score: %zu", data.snakes[0].score), start_y + 30, p1_color[1]);
				draw_centered(TextFormat("P2 Score: %zu", data.snakes[1].score), start_y + 60, p2_color[1]);
				draw_centered("Press 'Enter' for restart the game", start_y + 90, RAYWHITE);
				EndDrawing();
				return;
			}

			const std::string	end_msg = data.state == GameState::WON ? "You Won !" : "Game Over !";
			draw_centered(end_msg, start_y, RAYWHITE);
			draw_centered(TextFormat("Your score: %zu", data.snakes[0].score), start_y + 45, RAYWHITE);
			draw_centered("Press 'Enter' for restart the game", start_y + 100, RAYWHITE);

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
				return Event::W;
			case KEY_S:
				return Event::S;
			case KEY_A:
				return Event::A;
			case KEY_D:
				return Event::D;
			case KEY_UP:
				return Event::UP;
			case KEY_DOWN:
				return Event::DOWN;
			case KEY_LEFT:
				return Event::LEFT;
			case KEY_RIGHT:
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

		private:
			void	draw_snake(const Snake &snake, const Color &head_color, const Color &body_color)
			{
				for (auto &pos : snake.body) {
					Color	color = (pos == snake.body[0]) ? head_color : body_color;
					DrawRectangle(pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
				}
			}

			void	draw_score(const std::vector<Snake> &snakes, Color p1, Color p2)
			{
				DrawText(TextFormat("P1 Score: %zu", snakes[0].score), 10, 10, 20, p1);
				if (snakes.size() == 2)
					DrawText(TextFormat("P2 Score: %zu", snakes[1].score), 10, 40, 20, p2);
			}

			Color	get_food_color(const FoodType &type)
			{
				Color	food_color = RED;	
				if (type == FoodType::FOOD_GREEN)
					food_color = GREEN;
				else if (type == FoodType::FOOD_YELLOW)
					food_color = YELLOW;
				return food_color;
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