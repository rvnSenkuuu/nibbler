#pragma once

#include <dlfcn.h>
#include <iostream>

constexpr const char	*AUDIO_LIB{"./lib/raylib/libraylib_audio.so"};

using FnInitAudio = void	(*)(void);
using FnPlaySound = void	(*)(const std::string &sound);
using FnUpdateSound = void	(*)(void);
using FnResetSound = void	(*)(void);
using FnStopSound = void	(*)(void);
using FnDestroyAudio = void	(*)(void);

class Audio {
	public:
		Audio(void);
		~Audio(void);

		void	load(const std::string &path);
		void	unload(void);

		FnInitAudio	init;
		FnPlaySound	play;
		FnUpdateSound	update;
		FnResetSound	reset;
		FnStopSound	stop;
		FnDestroyAudio	destroy;

	private:
		Audio(const Audio &) = delete;
		Audio(const std::string &path) = delete;
		Audio	&operator=(const Audio &) = delete;

		void	*m_handle;
};