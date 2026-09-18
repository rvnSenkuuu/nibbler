#include "Game.hpp"

int	main(int argc, char **argv)
{
	try {
		Game	game(argc, argv);
		//Allocate lib
		game.run(); //Put lib as parameter
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}