#include <raylib.h>
#include <iostream>

#include "Types.hpp"
#include "IGraphic.hpp"

class GraphicLib : public IGraphic {
	public:

		GraphicLib(void) { }
		~GraphicLib(void) { }

		void	create_window(int width, int height)
		{
			InitWindow(width, height, "Nibbler");
			SetTargetFPS(60);
		}

		void	render_game(const GameState &state)
		{
			(void)state;
			std::cerr << "TODO: " << __func__ << std::endl;
		}

		Event	get_event(void)
		{
			std::cerr << "TODO: " << __func__ << std::endl;
			return Event::NONE;
		}

		void	destroy_window(void)
		{
			CloseWindow();
		}
};

GraphicLib	lib;

#ifdef __cplusplus
extern "C" {
#endif

void	create_window(int width, int height);
void	render_game(const GameState &state);
Event	get_event(void);
void	destroy_window(void);

#ifdef __cplusplus
}
#endif


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