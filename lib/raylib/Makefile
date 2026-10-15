NAME = libraylib.so
CXX = c++
DEPS_DIR = $(HOME)/goinfre/nibbler_deps

CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2 -fPIC \
             -I../../incs -I$(DEPS_DIR)/include -MMD -MP
LDFLAGS = -shared -L$(DEPS_DIR)/lib -lraylib -Wl,-rpath,$(DEPS_DIR)/lib

SRCS = Raylib.cpp

OBJSDIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS))
D_FILES = $(OBJS:.o=.d)

RM         = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJSDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

sinclude $(D_FILES)

.PHONY: all