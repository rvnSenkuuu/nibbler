#include "Lib.hpp"

Lib::Lib(void) : m_current_lib_path(DEFAULT_LIB)
{
	load(DEFAULT_LIB);
}

Lib::~Lib()
{
	if (m_handle)
		unload();
}

void	Lib::load(const std::string &path)
{
	m_handle = dlopen(path.c_str(), RTLD_LAZY);
	if (!m_handle)
		throw std::runtime_error(dlerror());

	auto	sym = [&](const char *name) {
		void	*sym_ptr{dlsym(m_handle, name)};
		if (!sym_ptr) {
			std::string	err = dlerror();
			dlclose(m_handle);
			throw std::runtime_error(err);
		}
		return sym_ptr;
	};

	create = reinterpret_cast<FnCreate>(sym("create_window"));
	get_event = reinterpret_cast<FnEvent>(sym("get_event"));
	render = reinterpret_cast<FnRender>(sym("render_game"));
	destroy = reinterpret_cast<FnDestroy>(sym("destroy_window"));
	m_current_lib_path = path;
}

void	Lib::unload(void)
{
	destroy();
	if (dlclose(m_handle) != 0)
		throw std::runtime_error(dlerror());
	m_handle = nullptr;
	create = nullptr;
	render = nullptr;
	get_event = nullptr;
	destroy = nullptr;
}

bool	Lib::switch_lib(const std::string &path)
{
	if (path == m_current_lib_path)
		return false;

	unload();
	load(path);
	return true;
}