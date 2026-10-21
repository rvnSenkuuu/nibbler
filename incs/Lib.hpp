#pragma once

#include <dlfcn.h>
#include <iostream>

#include "Types.hpp"

#define	FIRST_LIB "./lib/raylib/libraylib.so"
#define	SECOND_LIB "./lib/sdl2/libsdl2.so"
#define	THIRD_LIB "./lib/sfml/libsfml.so"
#define DEFAULT_LIB FIRST_LIB

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