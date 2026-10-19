NAME = libraylib.so
NAME_AUDIO = libraylib_audio.so
CXX = c++
DEPS_DIR = $(HOME)/goinfre/nibbler_deps

CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2 -fPIC \
			-I../../incs -I$(DEPS_DIR)/include -MMD -MP
LDFLAGS = -shared -L$(DEPS_DIR)/lib -lraylib -Wl,-rpath,$(DEPS_DIR)/lib

SRCS = Raylib.cpp
SRCS_AUDIO = RaylibAudio.cpp

OBJSDIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS))
OBJS_AUDIO = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS_AUDIO))
D_FILES = $(OBJS:.o=.d) $(OBJS_AUDIO:.o=.d)

RM = rm -rf

all: $(NAME) $(NAME_AUDIO)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(NAME)

$(NAME_AUDIO): $(OBJS_AUDIO)
	$(CXX) $(OBJS_AUDIO) $(LDFLAGS) -o $(NAME_AUDIO)

$(OBJSDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

sinclude $(D_FILES)

.PHONY: all