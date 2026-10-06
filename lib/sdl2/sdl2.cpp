#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Types.hpp"
#include "IGraphic.hpp"

#define FONT_PATH "./assets/font.ttf"

static constexpr SDL_Color	BLACK_C = {0, 0, 0, 255};
static constexpr SDL_Color	DARKGREEN_C = {0, 117, 44, 255};
static constexpr SDL_Color	GREEN_C = {0, 228, 48, 255};
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
				destroy_window();
				throw std::runtime_error(SDL_GetError());
			}

			m_font = TTF_OpenFont(FONT_PATH, 20);
			if (!m_font) {
				destroy_window();
				throw std::runtime_error(SDL_GetError());
			}
			
			m_window = SDL_CreateWindow("Nibbler",
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				width * CELL_SIZE, height * CELL_SIZE, 0);
			if (!m_window) {
				destroy_window();
				throw std::runtime_error(SDL_GetError());
			}
			
			m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
			if (!m_renderer) {
				destroy_window();
				throw std::runtime_error(SDL_GetError());
			}
		}

		void	render_game(const GameState &state) override
		{
			SDL_SetRenderDrawColor(m_renderer, 0x18, 0x18, 0x18, 0xFF);
			SDL_RenderClear(m_renderer);

			for (size_t i = 0; i < state.snake.size(); i++) {
				const auto	&pos = state.snake.at(i);
				draw_cell(pos.x, pos.y, i == 0 ? DARKGREEN_C : GREEN_C);
			}

			draw_cell(state.food.x, state.food.y, RED_C);
			draw_score(state.score);
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
						return Event::UP;
					case SDL_SCANCODE_S:
						return Event::DOWN;
					case SDL_SCANCODE_A:
						return Event::LEFT;
					case SDL_SCANCODE_D:
						return Event::RIGHT;
					case SDL_SCANCODE_ESCAPE:
						return Event::QUIT;
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

			void	draw_cell(int x, int y, const SDL_Color &color)
			{
				const SDL_Rect	rect = {
					.x = x * CELL_SIZE,
					.y = y * CELL_SIZE,
					.w = CELL_SIZE,
					.h = CELL_SIZE
				};
				SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(m_renderer, &rect);
			}
			void	draw_score(const size_t score)
			{
				const std::string	text = "Score: " + std::to_string(score);
				SDL_Surface	*surface = TTF_RenderText_Solid(m_font, text.c_str(), RAYWHITE_C);
				SDL_Texture	*texture = SDL_CreateTextureFromSurface(m_renderer, surface);
				const SDL_Rect	rect = {
					.x = 10,
					.y = 10,
					.w = surface->w,
					.h = surface->h
				};
				SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture);
			}

			SDL_Window	*m_window;
			SDL_Renderer	*m_renderer;
			TTF_Font	*m_font;
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