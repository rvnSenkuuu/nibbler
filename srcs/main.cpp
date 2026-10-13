#include "Game.hpp"
#include "Lib.hpp"
#include "Types.hpp"

void	usage(void)
{
	std::cout << "Usage: ./nibbler <width> <height> [OPTIONS]\n";
	std::cout << "Options: \n";
	std::cout << "\t--multi\n";
	std::cout << "\tCan play the game with 2 snakes (WASD) for snake 1 and (ARROW KEYS) for snake 2\n\n";
	std::cout << "\t--limit-score [score]\n";
	std::cout << "\t Can specify a score limit\n\n";
	std::cout << "\t--food-variance\n";
	std::cout << "\tThree type of apple (RED, GREEN, YELLOW) each apple give a different amount of points\n";
	
	std::cout << std::endl;
	std::exit(1);
}

int	main(int argc, char **argv)
{
	try {
		Game	game(argc, argv);
		Lib	lib;
		game.run(lib);
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		usage();
		return 1;
	}
	return 0;
}