#include "lib.hpp"

void	create_window(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return;
	}
	SDL_Quit();
}