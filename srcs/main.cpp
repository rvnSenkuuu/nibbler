#include "Game.hpp"
#include "Lib.hpp"
#include "Types.hpp"

void	usage(void)
{
	std::cout << "Usage: ./nibbler <width> <height> [OPTIONS]\n";
	std::cout << "Options:\n";
	std::cout << "\t--multi\n";
    std::cout << "\tPlay with two snakes: Snake 1 uses WASD, Snake 2 uses the arrow keys\n\n";
	std::cout << "\t--limit-score [score]\n";
    std::cout << "\tSet a score limit\n\n";
	std::cout << "\t--food-variance\n";
    std::cout << "\tSpawns three types of apples (RED, GREEN, YELLOW), each worth a different amount of points\n\n";
	std::cout << "\t--enable-obstacles [obstacles count]\n";
	std::cout << "\tAdds random obstacles. The snake dies if it collides with one\n\n";
	std::cout << "\t--enable-sounds\n";
	std::cout << "\tEnable background music, death and eat sound\n";
	std::cout << std::endl;
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