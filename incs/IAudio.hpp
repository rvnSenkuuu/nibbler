#pragma once

class IAudio {
	public:
		virtual ~IAudio() { }

		virtual void	init(void) = 0;
		virtual void	destroy(void) = 0;
		// virtual void	play_eat() = 0;
		// virtual void	play_die() = 0;
		// virtual void	play_background() = 0;
};