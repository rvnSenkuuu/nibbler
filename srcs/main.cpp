#include "Game.hpp"
#include "Lib.hpp"
#include "Types.hpp"

int	main(int argc, char **argv)
{
	try {
		Game	game(argc, argv);
		Lib	lib;
		game.run(lib);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}