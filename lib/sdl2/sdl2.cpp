#include <SDL2/SDL.h>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

void	create_window(int width, int height);
void	destroy_window(void);

#ifdef __cplusplus
}
#endif

SDL_Window *window;

void	create_window(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return;
	}

	window = SDL_CreateWindow("Nibbler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (!window) {
		std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
		return;
	}
}

void	destroy_window()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}