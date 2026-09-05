#include "Game.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "Usage: ./nibbler <1..3>" << std::endl;
		return 1;
	}
	
	int	lib_index = std::stoi(argv[1], nullptr, 10);
	if (lib_index < 1 || lib_index > 3) {
		std::cerr << "Usage: ./nibbler <1..3>" << std::endl;
		return 1;
	}

	const char	*lib_name = {};
	switch (lib_index) {
	case 1:
		lib_name = "./lib/sdl2/libsdl2.so";
		break;
	case 2:
		lib_name = "./lib/raylib/libraylib.so";
		break;
	case 3:
		lib_name = "./lib/sfml/libsfml.so";
		std::cerr << "TODO: lib.cpp" << std::endl;
		std::abort();
		break;
	default:
		lib_name = "./lib/sdl2/libsdl2.so";
		break;
	}
	
	void *lib_ptr = dlopen(lib_name, RTLD_LAZY);
	if (!lib_ptr) {
		const char *e = dlerror();
		std::cerr << "dlopen failed: " << e << std::endl;
		return 1;
	}

	typedef void	(*Init)(void);
	void *a_ptr = dlsym(lib_ptr, "create_window");
	Init	a = (Init)a_ptr;
	a();

	dlclose(lib_ptr);
	return 0;
}