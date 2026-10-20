#include <raylib.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "IAudio.hpp"

#define EAT_SOUND "./assets/sounds/eat.wav"
#define DEATH_SOUND "./assets/sounds/death.wav"

class AudioLib : public IAudio {
	public:
		AudioLib(void) { }
		~AudioLib(void) { }

		void	init(void) override
		{
			SetTraceLogLevel(LOG_NONE);
			InitAudioDevice();

			auto	load = [&](const std::string &type, const std::string &file) {
				Sound	sound = LoadSound(file.c_str());
				m_sounds.insert({type, sound});
			};

			load("EAT", EAT_SOUND);
			load("DEATH", DEATH_SOUND);
		}

		void	play(const std::string &sound) override
		{
			auto	it = m_sounds.find(sound);
			PlaySound(it->second);
		}

		void	destroy(void) override
		{
			for (auto &sound : m_sounds)
				UnloadSound(sound.second);
			CloseAudioDevice();
		}

	private:
		AudioLib(const AudioLib &) = delete;
		AudioLib(const std::string &path) = delete;
		AudioLib	&operator=(const AudioLib &) = delete;

		std::unordered_map<std::string, Sound>	m_sounds;
};

AudioLib	lib;

extern "C" {
	void	init(void)
	{
		lib.init();
	}

	void	play(const std::string &sound)
	{
		lib.play(sound);
	}

	void	destroy(void)
	{
		lib.destroy();
	}
}