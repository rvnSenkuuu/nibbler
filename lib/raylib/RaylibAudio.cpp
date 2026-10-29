#include <raylib.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "IAudio.hpp"

constexpr const char	*EAT_SOUND{"./assets/sounds/eat.wav"};
constexpr const char	*DEATH_SOUND{"./assets/sounds/death.wav"};
constexpr const char	*SHRINK_SOUND{"./assets/sounds/shrink.wav"};
constexpr const char	*BACKGROUND_SOUND{"./assets/sounds/background.mp3"};

class AudioLib : public IAudio {
	public:
		AudioLib(void) { }
		~AudioLib(void) { }

		void	init(void) override
		{
			SetTraceLogLevel(LOG_NONE);
			InitAudioDevice();
			m_volume = 1.0f;

			auto	load = [&](const std::string &type, const std::string &file) {
				Sound	sound = LoadSound(file.c_str());
				SetSoundVolume(sound, m_volume);
				m_sounds.insert({type, sound});
			};

			load("EAT", EAT_SOUND);
			load("DEATH", DEATH_SOUND);
			load("SHRINK", SHRINK_SOUND);
			
			m_background = LoadMusicStream(BACKGROUND_SOUND);
			m_background.looping = true;
			SetMusicVolume(m_background, m_volume);
			PlayMusicStream(m_background);
		}

		void	play(const std::string &sound) override
		{
			auto	it = m_sounds.find(sound);
			PlaySound(it->second);
		}

		void	update(void) override
		{
			UpdateMusicStream(m_background);
		}

		void	stop(void) override
		{
			StopMusicStream(m_background);
		}

		void	reset(void) override
		{
			stop();
			PlayMusicStream(m_background);
		}

		void	destroy(void) override
		{
			UnloadMusicStream(m_background);
			for (auto &sound : m_sounds)
				UnloadSound(sound.second);
			CloseAudioDevice();
		}

	private:
		AudioLib(const AudioLib &) = delete;
		AudioLib(const std::string &path) = delete;
		AudioLib	&operator=(const AudioLib &) = delete;

		float	m_volume;
		std::unordered_map<std::string, Sound>	m_sounds;
		Music	m_background;
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

	void	update(void)
	{
		lib.update();
	}

	void	stop(void)
	{
		lib.stop();
	}

	void	reset(void)
	{
		lib.reset();
	}

	void	destroy(void)
	{
		lib.destroy();
	}
}