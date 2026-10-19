#include <raylib.h>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "IAudio.hpp"

class AudioLib : public IAudio {
	public:
		AudioLib(void) { }
		~AudioLib(void) { }

		void	init(void) override
		{
			InitAudioDevice();
			load_sounds();
		}

		void	load_sounds()
		{
			Sound	eat = LoadSound("./assets/sounds/eat.wav");
			m_sounds.insert({"EAT", eat});
		}

		void	play(const std::string &sound)
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