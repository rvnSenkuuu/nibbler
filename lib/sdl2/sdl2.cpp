#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Types.hpp"
#include "IGraphic.hpp"

static constexpr SDL_Color	BLACK_C = {0, 0, 0, 255};
static constexpr SDL_Color	DARKGREEN_C = {0, 117, 44, 255};
static constexpr SDL_Color	GREEN_C = {0, 228, 48, 255};
static constexpr SDL_Color	DARKBLUE_C = {0, 82, 172, 255};
static constexpr SDL_Color	BLUE_C = {0, 121, 241, 255};
static constexpr SDL_Color	RED_C = {255, 0, 0, 255};
static constexpr SDL_Color	RAYWHITE_C = {245, 245, 245, 255};

class GraphicLib : public IGraphic {
	public:
		GraphicLib(void) : m_window(nullptr), m_renderer(nullptr), m_font(nullptr) { };
		~GraphicLib(void) {  };

		void	create_window(int width, int height) override
		{
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
				throw std::runtime_error(SDL_GetError());

			if (TTF_Init() < 0) {
				const std::string	err = SDL_GetError();
				destroy_window();
				throw std::runtime_error(err);
			}

			m_font = TTF_OpenFont(FONT_PATH, 20);
			if (!m_font) {
				const std::string	err = SDL_GetError();
				destroy_window();
				throw std::runtime_error(err);
			}
			
			m_window = SDL_CreateWindow("Nibbler",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width * CELL_SIZE, height * CELL_SIZE, 0
			);
			if (!m_window) {
				const std::string	err = SDL_GetError();
				destroy_window();
				throw std::runtime_error(err);
			}
			
			m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
			if (!m_renderer) {
				const std::string	err = SDL_GetError();
				destroy_window();
				throw std::runtime_error(err);
			}
		}

		void	render_game(const GameData &data) override
		{
			SDL_SetRenderDrawColor(m_renderer, 0x18, 0x18, 0x18, 0xFF);
			SDL_RenderClear(m_renderer);

			for (size_t i = 0; i < data.snakes.size(); i++) {
				Snake	snake = data.snakes[i];
				draw_snake(snake, i == 0 ? p1_color[0] : p2_color[0], i == 0 ? p1_color[1] : p2_color[1]);
			}

			draw_cell(data.food.x, data.food.y, RED_C);
			draw_text("P1 Score: " + std::to_string(data.snakes[0].score), 10, 10, GREEN_C);
			if (data.snakes.size() == 2)
				draw_text("P2 Score: " + std::to_string(data.snakes[0].score), 10, 40, BLUE_C);
			SDL_RenderPresent(m_renderer);
		}

		void	render_end_game(const GameData &data)
		{
			SDL_SetRenderDrawColor(m_renderer, BLACK_C.r, BLACK_C.g, BLACK_C.b, BLACK_C.a);
			SDL_RenderClear(m_renderer);

			const int	center_x = data.width * CELL_SIZE / 2;
			const int	start_y = data.height * CELL_SIZE / 2 - (100 + 20) / 2;

			if (data.snakes.size() == 2) {
				std::string	game_winner;
				if (data.state == GameState::DRAW)
					game_winner = "Draw !";
				else
					game_winner = data.winner == data.snakes[0].id ? "P1 won !" : "P2 won !";

				draw_text_centered(game_winner, center_x, start_y, RAYWHITE_C);
				draw_text_centered("P1 Score: " + std::to_string(data.snakes[0].score), center_x, start_y + 30, p1_color[1]);
				draw_text_centered("P2 Score: " + std::to_string(data.snakes[1].score), center_x, start_y + 60, p2_color[1]);
				draw_text_centered("Press 'Enter' for restart the game", center_x, start_y + 90, RAYWHITE_C);
				
				SDL_RenderPresent(m_renderer);
				return;
			}

			const std::string	end_msg = data.state == GameState::WON ? "You Won !" : "Game Over !";
			draw_text_centered(end_msg, center_x, start_y, RAYWHITE_C);
			draw_text_centered("Your score: " + std::to_string(data.snakes[0].score), center_x, start_y + 45, RAYWHITE_C);
			draw_text_centered("Press 'Enter' for restart the game", center_x, start_y + 100, RAYWHITE_C);

			SDL_RenderPresent(m_renderer);
		}

		Event	get_event(void) override
		{
			SDL_Event	event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT)
					return Event::QUIT;
				if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_1:
						return Event::LIB1;
					case SDL_SCANCODE_2:
						return Event::LIB2;
					case SDL_SCANCODE_3:
						return Event::LIB3;
					case SDL_SCANCODE_W:
						return Event::W;
					case SDL_SCANCODE_S:
						return Event::S;
					case SDL_SCANCODE_A:
						return Event::A;
					case SDL_SCANCODE_D:
						return Event::D;
					case SDL_SCANCODE_UP:
						return Event::UP;
					case SDL_SCANCODE_DOWN:
						return Event::DOWN;
					case SDL_SCANCODE_LEFT:
						return Event::LEFT;
					case SDL_SCANCODE_RIGHT:
						return Event::RIGHT;
					case SDL_SCANCODE_ESCAPE:
						return Event::QUIT;
					case SDL_SCANCODE_KP_ENTER:
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
			if (m_renderer) {
				SDL_DestroyRenderer(m_renderer);
				m_renderer = nullptr;
			}
			if (m_font) {
				TTF_CloseFont(m_font);
				m_font = nullptr;
			}
			if (m_window) {
				SDL_DestroyWindow(m_window);
				m_window = nullptr;
			}
			TTF_Quit();
			SDL_Quit();
		}

		private:
			GraphicLib(const GraphicLib &other) = delete;
			GraphicLib	&operator=(const GraphicLib &other) = delete;

			void	draw_snake(Snake &snake, const SDL_Color head_color, const SDL_Color body_color)
			{
				for (auto &pos : snake.body) {
					const SDL_Color	color = (pos == snake.body[0]) ? head_color : body_color; 
					const SDL_Rect	rect = {
						.x = static_cast<int>(pos.x * CELL_SIZE),
						.y = static_cast<int>(pos.y * CELL_SIZE),
						.w = CELL_SIZE,
						.h = CELL_SIZE
					};
					SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
					SDL_RenderFillRect(m_renderer, &rect);
				}
			}

			void	draw_cell(int x, int y, const SDL_Color &color)
			{
				const SDL_Rect	rect = {
					.x = x * CELL_SIZE,
					.y = y * CELL_SIZE,
					.w = CELL_SIZE,
					.h = CELL_SIZE};
				SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(m_renderer, &rect);
			}

			void	draw_text(const std::string &text, int x, int y, const SDL_Color &color)
			{
				SDL_Surface	*surface = TTF_RenderText_Solid(m_font, text.c_str(), color);
				SDL_Texture	*texture = SDL_CreateTextureFromSurface(m_renderer, surface);
				const SDL_Rect	rect = {
					.x = x,
					.y = y,
					.w = surface->w,
					.h = surface->h
				};
				
				SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture);
			}

			void	draw_text_centered(const std::string &text, int center_x, int y, const SDL_Color &color)
			{
				int	w = 0;
				int	h = 0;
				TTF_SizeText(m_font, text.c_str(), &w, &h);
				draw_text(text, center_x - w / 2, y, color);
			}

			SDL_Window	*m_window;
			SDL_Renderer	*m_renderer;
			TTF_Font	*m_font;

			const SDL_Color	p1_color[2] = {DARKGREEN_C, GREEN_C};
			const SDL_Color	p2_color[2] = {DARKBLUE_C, BLUE_C};
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