#pragma once

#include <dlfcn.h>
#include <iostream>

#include "Types.hpp"

#define	LIB1 "./lib/raylib/libraylib.so"
#define	LIB2 "./lib/sdl2/libsdl2.so"
#define	LIB3 "./lib/sfml/libsfml.so"
#define DEFAULT_LIB LIB1

class Lib {
	public:
		Lib(const std::string &path);
		~Lib();

		void	load(const std::string &path);
		void	unload(void);
		void	switch_lib(const std::string &path);

	private:
		Lib() = default;
		Lib(const Lib &) = default;
		Lib	&operator=(const Lib &) = default;

		using FnCreate = void	(*)(int width, int height);
		using FnRender = void	(*)(const GameState &);
		using FnEvent = Event	(*)();
		using FnDestroy = void	(*)();

		FnCreate	create;
		FnRender	render;
		FnEvent	get_event;
		FnDestroy	destroy;
		void	*handle;
};