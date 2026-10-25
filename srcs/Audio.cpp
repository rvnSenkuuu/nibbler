#include "Audio.hpp"

Audio::Audio(void)
{
	load(AUDIO_LIB);
}

Audio::~Audio()
{
	if (m_handle)
		unload();
}

void	Audio::load(const std::string &path)
{
	m_handle = dlopen(path.c_str(), RTLD_LAZY);
	if (!m_handle)
		throw std::runtime_error(dlerror());

	auto	sym = [&](const char *name) {
		void	*sym_ptr = dlsym(m_handle, name);
		if (!sym_ptr) {
			std::string	err = dlerror();
			dlclose(m_handle);
			throw std::runtime_error(err);
		}
		return sym_ptr;
	};

	init = reinterpret_cast<FnInitAudio>(sym("init"));
	play = reinterpret_cast<FnPlaySound>(sym("play"));
	update = reinterpret_cast<FnUpdateSound>(sym("update"));
	reset = reinterpret_cast<FnResetSound>(sym("reset"));
	stop = reinterpret_cast<FnStopSound>(sym("stop"));
	destroy = reinterpret_cast<FnDestroyAudio>(sym("destroy"));
}

void	Audio::unload(void)
{
	destroy();
	if (dlclose(m_handle) != 0)
		throw std::runtime_error(dlerror());
	m_handle = nullptr;
	init = nullptr;
	play = nullptr;
	update = nullptr;
	reset = nullptr;
	stop = nullptr;
	destroy = nullptr;
}