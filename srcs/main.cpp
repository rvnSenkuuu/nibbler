#include "Game.hpp"
#include "Lib.hpp"
#include "Types.hpp"

//TODO: remove this
// #include <termios.h>
// #include <unistd.h>
// #include <fcntl.h>

//TODO: remove this
// class TermRawMode {
// 	public:
// 		TermRawMode(void)
// 		{
// 			tcgetattr(STDIN_FILENO, &m_original);
// 			termios	raw = m_original;
// 			raw.c_lflag &= ~(ICANON | ECHO);
// 			tcsetattr(STDIN_FILENO, TCSANOW, &raw);

// 			m_old_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
// 			fcntl(STDIN_FILENO, F_SETFL, m_old_flags | O_NONBLOCK);
// 		}

// 		~TermRawMode(void)
// 		{
// 			tcsetattr(STDIN_FILENO, TCSANOW, &m_original);
// 			fcntl(STDIN_FILENO, F_SETFL, m_old_flags);
// 		}

// 	private:
// 		termios	m_original;
// 		int	m_old_flags;
// };

int	main(int argc, char **argv)
{
	//TODO: remove this
	// TermRawMode	term_guard;

	try {
		Game	game(argc, argv);
		Lib	lib(DEFAULT_LIB);
		game.run(lib);
		lib.unload();
	} catch (const std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}