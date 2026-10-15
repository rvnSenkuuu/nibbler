NAME = libsfml.so
AUDIO_NAME = libsfmlaudio.so
CXX = c++
DEPS_DIR = $(HOME)/goinfre/nibbler_deps

CXXFLAGS = -Wall -Werror -Wextra -std=c++17 -O2 -fPIC \
			-I../../incs -I$(DEPS_DIR)/include -MMD -MP

LDFLAGS = -shared -L$(DEPS_DIR)/lib -lsfml-graphics -lsfml-window -lsfml-system \
             -Wl,-rpath,$(DEPS_DIR)/lib

AUDIO_LDFLAGS = -shared -L$(DEPS_DIR)/lib -lsfml-audio -lsfml-system \
                   -Wl,-rpath,$(DEPS_DIR)/lib

SRCS = Sfml.cpp
AUDIO_SRCS = SfmlAudio.cpp

OBJSDIR = .objs/
OBJS = $(patsubst %.cpp, $(OBJSDIR)%.o, $(SRCS))
AUDIO_OBJS = $(AUDIO_SRCS:%.cpp=$(OBJSDIR)%.o)
D_FILES = $(OBJS) $(AUDIO_OBJS)
D_FILES := $(D_FILES:.o=.d)

RM = rm -rf

all: $(NAME) $(AUDIO_NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(NAME)

$(AUDIO_NAME): $(AUDIO_OBJS)
	$(CXX) $^ $(AUDIO_LDFLAGS) -o $@

$(OBJSDIR)%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

libsfmlaudio.so: SfmlAudio.cpp
	$(CXX) -shared -fPIC SfmlAudio.cpp -o libsfmlaudio.so

sinclude $(D_FILES)

.PHONY: all