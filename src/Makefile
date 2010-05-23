
SOURCES=$(shell find -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)

SDL_CXXFLAGS=$(shell sdlmm-config --cflags) $(shell sdl-config --cflags)
SDL_LDFLAGS=$(shell sdlmm-config --libs) -lSDL_ttf $(shell sdl-config --libs)

CXXFLAGS=-Wall -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo -g -DGIT_ROOT=\"$(shell pwd)/..\" -I. -I./include $(SDL_CXXFLAGS)
LDFLAGS=$(SDL_LDFLAGS)

CXX=g++ $(CXXFLAGS)
LD=g++

all: $(OBJECTS)
	$(LD) -o main $(OBJECTS) $(LDFLAGS)

clean:
	-rm $(OBJECTS) 2>/dev/null

.cpp.o:
	$(CXX) -c $< -o $@
