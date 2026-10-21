#pragma once

#include <dlfcn.h>
#include <iostream>

#define AUDIO_LIB "./lib/raylib/libraylib_audio.so"

using FnInitAudio = void	(*)(void);
using FnPlaySound = void	(*)(const std::string &sound);
using FnUpdateSound = void	(*)(void);
using FnResetSound = void	(*)(void);
using FnStopSound = void	(*)(void);
using FnDestroyAudio = void	(*)(void);

class AudioManager {
	public:
		AudioManager(void);
		~AudioManager(void);

		void	load(const std::string &path);
		void	unload(void);

		FnInitAudio	init;
		FnPlaySound	play;
		FnUpdateSound	update;
		FnResetSound	reset;
		FnStopSound	stop;
		FnDestroyAudio	destroy;

	private:
		AudioManager(const AudioManager &) = delete;
		AudioManager(const std::string &path) = delete;
		AudioManager	&operator=(const AudioManager &) = delete;

		void	*m_handle;
};