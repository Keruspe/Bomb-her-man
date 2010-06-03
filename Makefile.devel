SOURCES=$(shell find src -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)

CXXFLAGS+= -g -Isrc -Isrc/include
LDFLAGS+= --as-needed
ifdef STATIC
LDFLAGS+= -static
static-sdl=static-
static-pkg=--static
endif

WARNINGS_CFLAGS=-Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo -Werr
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

ifdef NOANIM
CXXFLAGS+= -DANIM_IMAGES=1
else
CXXFLAGS+= -DANIM_IMAGES=4 -DANIM_TIME=400
endif

CXX=g++
LD=g++

ifndef DESTDIR
DESTDIR=/usr/local
endif

ifndef ROOTDIR
ROOTDIR=$(DESTDIR)
endif

ifndef BINDIR
BINDIR=$(DESTDIR)/usr/bin
endif

ifndef DOCDIR
DOCDIR=$(DESTDIR)/usr/share/doc
endif

ifndef DATADIR
DATADIR=$(DESTDIR)/usr/share
endif

ifndef DEVEL
CXXFLAGS+= -DDATADIR=\"$(ROOTDIR)/usr/share/bomb-her-man\"
else
CXXFLAGS+= -DDATADIR=\"$(shell pwd)/data\"
endif

INSTALL=install
INSTALLBIN = -s -m 755
INSTALLDIR = -d -m 755
INSTALLFILE = -m 644

RM=rm
ECHO=echo
CD=cd
MKDIR=mkdir
EXIT=exit

DOC_MAKER=doxygen

all: bomb-her-man

bomb-her-man: $(OBJECTS)
	@ $(ECHO) Linking bomb-her-man
	@ $(LD) -o bomb-her-man $(OBJECTS) $(LDFLAGS)

doc:
	@ $(ECHO) Generating documentation in $(shell pwd)/doc
	@ $(MKDIR) -p doc && $(DOC_MAKER) > /dev/null

clean:
	@ $(ECHO) Cleaning temporary files
	@ $(RM) -f bomb-her-man $(shell find -name '*.o') 2>/dev/null

clean-doc:
	@ $(ECHO) Cleaning documentation
	@ $(RM) -rf doc

install: all install-data
	@ $(ECHO) Installing bomb-her-man in $(BINDIR)
	@ $(INSTALL) $(INSTALLDIR) $(BINDIR) 2>/dev/null
	@ $(INSTALL) $(INSTALLBIN) bomb-her-man $(BINDIR)/bomb-her-man 2>/dev/null || ($(ECHO) "You must be root to install the game" && $(EXIT) -1)

install-data:
	@ $(ECHO) Installing datas in $(DATADIR)/bomb-her-man
	@ for i in $(shell find data -type d); do $(INSTALL) $(INSTALLDIR) $(DATADIR)/$${i/data/bomb-her-man}; done 2>/dev/null
	@ for i in $(shell find data -type f); do $(INSTALL) $(INSTALLFILE) $$i $(DATADIR)/$${i/data/bomb-her-man}; done 2>/dev/null || ($(ECHO) "You must be root to install datas" && $(EXIT) -1)

install-doc : doc
	@ $(ECHO) Installing documentation in $(DOCDIR)/bomb-her-man
	@ for i in $(shell find doc -type d); do $(INSTALL) $(INSTALLDIR) $$i $(DOCDIR)/$${i/doc/bomb-her-man}; done 2>/dev/null
	@ for i in $(shell find doc -type f); do $(INSTALL) $(INSTALLFILE) $$i $(DOCDIR)/$${i/doc/bomb-her-man}; done 2>/dev/null || ($(ECHO) "You must be root to install documentation" && $(EXIT) -1)

.cpp.o:
	@ $(ECHO) Compiling $<
	@ $(CXX) -c $< -o $@ $(CXXFLAGS)

