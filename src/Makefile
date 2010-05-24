
SOURCES=$(shell find -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)

SDL_CXXFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=-lSDL_ttf $(shell sdl-config --libs)

SVG_CFLAGS=$(shell pkg-config --cflags cairo librsvg-2.0)
SVG_LDFLAGS=$(shell pkg-config --libs cairo librsvg-2.0)

CXXFLAGS=-Wall -Wextra -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo -g -DGIT_ROOT=\"$(shell pwd)/..\" -I. -I./include $(SDL_CXXFLAGS) $(SVG_CFLAGS)
LDFLAGS=$(SDL_LDFLAGS) $(SVG_LDFLAGS)

CXX=g++ $(CXXFLAGS)
LD=g++

all: $(OBJECTS)
	$(LD) -o main $(OBJECTS) $(LDFLAGS)

clean:
	-rm -f $(OBJECTS) 2>/dev/null

.cpp.o:
	$(CXX) -c $< -o $@
