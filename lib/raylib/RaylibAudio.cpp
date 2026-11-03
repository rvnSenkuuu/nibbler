#include <raylib.h>
#include <iostream>
#include <array>
#include <algorithm>

#include "IAudio.hpp"

constexpr const char	*EAT_SOUND{"./assets/sounds/eat.wav"};
constexpr const char	*DEATH_SOUND{"./assets/sounds/death.wav"};
constexpr const char	*SHRINK_SOUND{"./assets/sounds/shrink.wav"};
constexpr const char	*BACKGROUND_SOUND{"./assets/sounds/background.mp3"};

constexpr const char	*SOUNDS[__SOUND_COUNT] = {
	EAT_SOUND,
	DEATH_SOUND,
	SHRINK_SOUND
};

class AudioLib : public IAudio {
	public:
		AudioLib(void) { }
		~AudioLib(void) { }

		void	init(void) override
		{
			SetTraceLogLevel(LOG_NONE);
			InitAudioDevice();
			m_volume = 1.0f;

			for (int i{0}; i < __SOUND_COUNT; i++) {
				Sound	sound = LoadSound(SOUNDS[i]);
				if (!sound.stream.buffer)
					throw std::runtime_error("Failed to load sound : " + std::string(SOUNDS[i]));
				SetSoundVolume(sound, m_volume);
				m_sounds.at(i) = sound;
			}
			
			m_background = LoadMusicStream(BACKGROUND_SOUND);
			m_background.looping = true;
			SetMusicVolume(m_background, m_volume);
			PlayMusicStream(m_background);
		}

		void	play(const SoundId id) override
		{
			PlaySound(m_sounds.at(id));
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
				UnloadSound(sound);
			CloseAudioDevice();
		}

	private:
		AudioLib(const AudioLib &) = delete;
		AudioLib(const std::string &path) = delete;
		AudioLib	&operator=(const AudioLib &) = delete;

		float	m_volume;
		std::array<Sound, __SOUND_COUNT>	m_sounds;
		Music	m_background;
};

AudioLib	lib;

extern "C" {
	void	init(void)
	{
		lib.init();
	}

	void	play(const SoundId id)
	{
		lib.play(id);
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