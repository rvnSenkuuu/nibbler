#pragma once

#include <dlfcn.h>
#include <iostream>

#include "Types.hpp"

#define	LIB1 "./lib/raylib/libraylib.so"
#define	LIB2 "./lib/sdl2/libsdl2.so"
#define	LIB3 "./lib/sfml/libsfml.so"
#define DEFAULT_LIB LIB1

// TODO: Convert this class to canonical form.
struct Lib {
	using FnCreate = void	(*)(int width, int height);
	using FnRender = void	(*)(const GameState &);
	using FnEvent = Event	(*)();
	using FnDestroy = void	(*)();

	FnCreate	create;
	FnRender	render;
	FnEvent	get_event;
	FnDestroy	destroy;
	void	*handle;

	Lib(const std::string &path)
	{
		this->load(path);
	}

	void	load(const std::string &path)
	{
		this->handle = dlopen(path.c_str(), RTLD_LAZY);
		if (!this->handle)
			throw std::runtime_error(dlerror());

		auto	sym = [&](const char *name) {
			void	*sym_ptr = dlsym(this->handle, name);
			if (!sym_ptr) {
				dlclose(this->handle);
				throw std::runtime_error(dlerror());
			}
			return sym_ptr;
		};

		this->create = reinterpret_cast<FnCreate>(sym("create_window"));
		this->get_event = reinterpret_cast<FnEvent>(sym("get_event"));
		this->render = reinterpret_cast<FnRender>(sym("render_game"));
		this->destroy = reinterpret_cast<FnDestroy>(sym("destroy_window"));
	}

	void	unload(void)
	{
		this->destroy();
		if (dlclose(this->handle) != 0)
			throw std::runtime_error(dlerror());
		this->handle = nullptr;
		this->create = nullptr;
		this->render = nullptr;
		this->get_event = nullptr;
		this->destroy = nullptr;
	}

	void	switch_lib(const std::string &path)
	{
		this->unload();
		this->load(path);
	}
};