
SOURCES=$(shell find -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS+= -g -DGIT_ROOT=\"$(shell pwd)/..\" -I. -I./include
LDFLAGS+=

WARNINGS_CFLAGS=-Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo
CXXFLAGS+=$(WARNING_CFLAGS)

SDL_CXXFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=-lSDL_ttf $(shell sdl-config --libs)
CXXFLAGS+=$(SDL_CXXFLAGS)
LDFLAGS+=$(SDL_LDFLAGS)

SVG_CXXFLAGS=$(shell pkg-config --cflags cairo librsvg-2.0)
SVG_LDFLAGS=$(shell pkg-config --libs cairo librsvg-2.0)
CXXFLAGS+=$(SVG_CXXFLAGS)
LDFLAGS+=$(SVG_LDFLAGS)

#FLAC_CXXFLAGS=$(shell pkg-config --cflags flac++)
#FLAC_LDFLAGS=$(shell pkg-config --libs flac++)

ifdef ANIM
CXXFLAGS+=-DANIM_IMAGES=4 -DANIM_TIME=300
else
CXXFLAGS+=-DANIM_IMAGES=1
endif

CXX=g++
LD=g++

RM=rm
ECHO=echo
CD=cd
MKDIR=mkdir

DOC_MAKER=doxygen

all: $(OBJECTS)
	@ $(ECHO) Linking bomb-her-man
	@ $(LD) -o bomb-her-man $(OBJECTS) $(LDFLAGS)

clean:
	@ $(ECHO) Cleaning temporary files
	@ $(RM) -f bomb-her-man $(shell find -name '*.o') 2>/dev/null

.cpp.o:
	@ $(ECHO) Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

doc:
	@ $(ECHO) Generating documentation in $(shell pwd)/../doc
	@ $(CD) .. && $(MKDIR) -p doc && $(DOC_MAKER) > /dev/null
