NAME = nibbler
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2 -Iincs -MMD -MP
LDFLAGS = -ldl
RM = rm -rf

SRCS = srcs/main.cpp
INCS = incs/Game.hpp

OBJSDIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS))
D_FILES = $(OBJS:.o=.d)

LIBS_DIRS = lib/raylib lib/sfml lib/sdl2

all: deps $(NAME) libs

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
	$(RM) ./lib/sdl2/.objs ./lib/raylib/libsdl2.so
	$(RM) ./lib/sfml/.objs ./lib/raylib/libsfml.so

re: fclean
	$(MAKE) all

sinclude $(D_FILES)

.PHONY: all deps libs clean fclean re