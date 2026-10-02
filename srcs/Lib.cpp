#include "Lib.hpp"

Lib::Lib(void) : m_current_lib_path(DEFAULT_LIB)
{
	this->load(DEFAULT_LIB);
}

Lib::~Lib()
{
	if (this->m_handle)
		this->unload();
}

void	Lib::load(const std::string &path)
{
	this->m_handle = dlopen(path.c_str(), RTLD_LAZY);
	if (!this->m_handle)
		throw std::runtime_error(dlerror());

	auto	sym = [&](const char *name) {
		void	*sym_ptr = dlsym(this->m_handle, name);
		if (!sym_ptr) {
			dlclose(this->m_handle);
			throw std::runtime_error(dlerror());
		}
		return sym_ptr;
	};

	this->create = reinterpret_cast<FnCreate>(sym("create_window"));
	this->get_event = reinterpret_cast<FnEvent>(sym("get_event"));
	this->render = reinterpret_cast<FnRender>(sym("render_game"));
	this->destroy = reinterpret_cast<FnDestroy>(sym("destroy_window"));
	this->m_current_lib_path = path;
}

void	Lib::unload(void)
{
	this->destroy();
	if (dlclose(this->m_handle) != 0)
		throw std::runtime_error(dlerror());
	this->m_handle = nullptr;
	this->create = nullptr;
	this->render = nullptr;
	this->get_event = nullptr;
	this->destroy = nullptr;
}

bool	Lib::switch_lib(const std::string &path)
{
	if (path == this->m_current_lib_path)
		return false;

	this->unload();
	this->load(path);
	return true;
}