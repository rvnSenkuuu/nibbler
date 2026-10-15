#pragma once

#include "Types.hpp"

class IGraphic {
	public:
		virtual ~IGraphic() { }

		virtual void	create_window(int width, int height) = 0;
		virtual void	render_game(const GameData &state) = 0;
		virtual Event	get_event(void) = 0;
		virtual void	destroy_window(void) = 0;
};