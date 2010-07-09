bin_PROGRAMS = \
	bomb-her-man

bomb_her_man_SOURCES = \
	exceptions/exception.hpp \
	exceptions/map/bad-element-exception.hpp \
	exceptions/map/malformed-file-exception.hpp \
	exceptions/too-many-players-exception.hpp \
	exceptions/display/nosdl-exception.hpp \
	exceptions/display/nosvg-exception.hpp \
	atomic-center/bomb.cpp \
	atomic-center/atomic-center.cpp \
	atomic-center/bomb.hpp \
	atomic-center/atomic-center.hpp \
	game/menu.cpp \
	game/player.cpp \
	game/menu.hpp \
	game/player.hpp \
	game/AI.hpp \
	game/game.hpp \
	game/game.cpp \
	game/AI.cpp \
	map/map.cpp \
	map/map-parser.hpp \
	map/map.hpp \
	map/map-parser.cpp \
	map/map-utils.hpp \
	map/map-generator.hpp \
	map/map-generator.cpp \
	config/config.hpp \
	config/config.cpp \
	include/bombherman.hpp \
	include/nls.hpp \
	display/display.hpp \
	display/display.cpp \
	bomb-her-man.cpp

bomb_her_man_LDADD = \
	@LIBINTL@ \
	$(LIBRSVG_LIBS) \
	$(CAIRO_LIBS) \
	$(SDL_LIBS)


ACLOCAL_AMFLAGS = -I m4


if ANIMATION
ANIM_CXXFLAGS=-DANIM_IMAGES=4 -DANIM_TIME=400
else
ANIM_CXXFLAGS=-DANIM_IMAGES=1
endif


AM_CPPFLAGS = \
	-Iinclude/ \
	-DDATADIR=\"$(pkgdatadir)\" \
	-DLOCALEDIR=\"$(localedir)\" \
	$(ANIM_CXXFLAGS) \
	$(CAIRO_CFLAGS) \
	$(LIBRSVG_CFLAGS) \
	$(SDL_CFLAGS) \
	$(DEBUG_CXXFLAGS)
