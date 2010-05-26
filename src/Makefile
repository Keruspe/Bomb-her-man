
SOURCES=$(shell find -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS=-Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo -g -DGIT_ROOT=\"$(shell pwd)/..\" -I. -I./include
LDFLAGS=

SDL_CXXFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=-lSDL_ttf $(shell sdl-config --libs)
CXXFLAGS+=$(SDL_CXXFLAGS)
LDFLAGS+=$(SDL_LDFLAGS)

SVG_CXXFLAGS=$(shell pkg-config --cflags cairo librsvg-2.0)
SVG_LDFLAGS=$(shell pkg-config --libs cairo librsvg-2.0)
CXXFLAGS+=$(SVG_CXXFLAGS)
LDFLAGS+=$(SVG_LDFLAGS)

FLAC_CXXFLAGS=$(shell pkg-config --cflags flac++)
FLAC_LDFLAGS=$(shell pkg-config --libs flac++)
CXXFLAGS+=$(FLAC_CXXFLAGS)
LDFLAGS+=$(FLAC_LDFLAGS)

CXX=g++
LD=g++

ECHO=echo

all: $(OBJECTS)
	@ $(ECHO) Linking bomb-her-man
	@ $(LD) -o bomb-her-man $(OBJECTS) $(LDFLAGS)

clean:
	@ $(ECHO) Cleaning temporary files
	@ -rm -f $(OBJECTS) 2>/dev/null

.cpp.o:
	@ $(ECHO) Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)
