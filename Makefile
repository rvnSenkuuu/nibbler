NAME = nibbler
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2 -Iincs -MMD -MP -g3 -fsanitize=address,leak
LDFLAGS = -ldl -rdynamic
RM = rm -rf

SRCS = srcs/main.cpp \
		srcs/Game.cpp \
		srcs/Food.cpp \
		srcs/Snake.cpp \
		srcs/Lib.cpp \
		srcs/AudioManager.cpp

INCS = incs/Game.hpp \
		incs/Food.hpp \
		incs/Snake.hpp \
		incs/Types.hpp \
		incs/IGraphic.hpp \
		incs/Lib.hpp \
		incs/IAudio.hpp \
		incs/AudioManager.hpp

OBJSDIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS))
D_FILES = $(OBJS:.o=.d)

LIBS_DIRS = lib/raylib lib/sfml lib/sdl2

all: deps 
	$(MAKE) $(NAME)
	$(MAKE) libs

deps:
	@bash scripts/install_deps.sh

$(NAME): $(OBJS) $(INCS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJSDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

libs:
	$(MAKE) -C ./lib/raylib
	$(MAKE) -C ./lib/sdl2
	$(MAKE) -C ./lib/sfml

clean:
	$(RM) $(OBJSDIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) ./lib/raylib/.objs ./lib/raylib/libraylib.so
	$(RM) ./lib/sdl2/.objs ./lib/sdl2/libsdl2.so
	$(RM) ./lib/sfml/.objs ./lib/sfml/libsfml.so ./lib/sfml/libsfmlaudio.so

clean_deps: fclean
	$(RM) "${HOME}/goinfre/nibbler_deps"

re: fclean
	$(MAKE) all

sinclude $(D_FILES)

.PHONY: all deps libs clean fclean clean_deps re