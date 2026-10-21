#pragma once

class IAudio {
	public:
		virtual ~IAudio() { }

		virtual void	init(void) = 0;
		virtual void	play(const std::string &sound) = 0;
		virtual void	update(void) = 0;
		virtual void	stop(void) = 0;
		virtual void	reset(void) = 0;
		virtual void	destroy(void) = 0;
};