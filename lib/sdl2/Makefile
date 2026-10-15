NAME = libsdl2.so
CXX = c++
DEPS_DIR = $(HOME)/goinfre/nibbler_deps

CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2 -fPIC \
             -I../../incs -I$(DEPS_DIR)/include -I$(DEPS_DIR)/include/SDL2 -MMD -MP
LDFLAGS = -shared -L$(DEPS_DIR)/lib -lSDL2 -lSDL2_ttf -Wl,-rpath,$(DEPS_DIR)/lib

SRCS = Sdl2.cpp

OBJSDIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS))
D_FILES = $(OBJS:.o=.d)

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJSDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

sinclude $(D_FILES)

.PHONY: all