SOURCES=$(shell find -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS+= -g -DGIT_ROOT=\"$(shell pwd)/..\" -I. -I./include
LDFLAGS+=
ifdef STATIC
LDFLAGS+=-static
static-sdl=static-
static-pkg=--static
endif

WARNINGS_CFLAGS=-Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo
CXXFLAGS+=$(WARNING_CFLAGS)

SDL_CXXFLAGS=$(shell sdl-config --cflags)
SDL_LDFLAGS=-lSDL_ttf $(shell sdl-config --$(static-sdl)libs)
CXXFLAGS+=$(SDL_CXXFLAGS)
LDFLAGS+=$(SDL_LDFLAGS)

SVG_CXXFLAGS=$(shell pkg-config --cflags $(static-pkg) cairo librsvg-2.0)
SVG_LDFLAGS=$(shell pkg-config --libs $(static-pkg) cairo librsvg-2.0)
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

ifndef DESTDIR
DESTDIR=/usr/local
endif

ifndef BINDIR
BINDIR=$(DESTDIR)/usr/bin
endif

ifndef DOCDIR
DOCDIR=$(DESTDIR)/usr/share/doc
endif

INSTALL=install
INSTALLBIN = -s -m 755
INSTALLDOC = -d -m 644

RM=rm
ECHO=echo
CD=cd
MKDIR=mkdir

DOC_MAKER=doxygen

all: bomb-her-man

doc: ../doc

bomb-her-man: $(OBJECTS)
	@ $(ECHO) Linking bomb-her-man
	@ $(LD) -o bomb-her-man $(OBJECTS) $(LDFLAGS)

../doc:
	@ $(ECHO) Generating documentation in $(shell pwd)/../doc
	@ $(CD) .. && $(MKDIR) -p doc && $(DOC_MAKER) > /dev/null

clean:
	@ $(ECHO) Cleaning temporary files
	@ $(RM) -f bomb-her-man $(shell find -name '*.o') 2>/dev/null

clean-doc:
	@ $(ECHO) Cleaning documentation
	@ $(RM) -rf ../doc

install: all
	@ $(ECHO) Installing bomb-her-man in $(BINDIR)
	@ $(INSTALL) $(INSTALLBIN) bomb-her-man $(BINDIR)/bomb-her-man

install-doc : doc
	@ $(ECHO) Installing documentation in $(DOCDIR)/bomb-her-man
	@ $(MKDIR) -p $(DOCDIR)/bomb-her-man/html $(DOCDIR)/bomb-her-man/latex
	@ $(INSTALL) $(INSTALLDOC) ../doc $(DOCDIR)/bomb-her-man

.cpp.o:
	@ $(ECHO) Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

