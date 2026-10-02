#include "Lib.hpp"

Lib::Lib(const std::string &path)
{
	this->load(path);
}

Lib::~Lib()
{
	this->unload();
}

void	Lib::load(const std::string &path)
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

void	Lib::unload(void)
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

void	Lib::switch_lib(const std::string &path)
{
	this->unload();
	this->load(path);
}