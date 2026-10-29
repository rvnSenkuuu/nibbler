#pragma once

#include <dlfcn.h>
#include <iostream>

#include "GameData.hpp"

constexpr const char	*FIRST_LIB {"./lib/raylib/libraylib.so"};
constexpr const char	*SECOND_LIB {"./lib/sdl2/libsdl2.so"};
constexpr const char	*THIRD_LIB {"./lib/sfml/libsfml.so"};
constexpr const char	*DEFAULT_LIB{FIRST_LIB};

using FnCreate = void	(*)(int width, int height);
using FnRender = void	(*)(const GameData &);
using FnEvent = Event	(*)();
using FnDestroy = void	(*)();

class Lib {
	public:
		Lib(void);
		~Lib(void);

		void	load(const std::string &path);
		void	unload(void);
		[[nodiscard]] bool	switch_lib(const std::string &path);

		FnCreate	create;
		FnRender	render;
		FnEvent	get_event;
		FnDestroy	destroy;

	private:
		Lib(const Lib &) = delete;
		Lib(const std::string &path) = delete;
		Lib	&operator=(const Lib &) = delete;

		void	*m_handle;
		std::string	m_current_lib_path;
};