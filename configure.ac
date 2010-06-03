AC_PREREQ([2.65])
AC_INIT(bomb-her-man, 0.1, "")

AC_CONFIG_SRCDIR([src/bomb-her-man.cpp])
AC_CONFIG_HEADERS([src/include/config.h])

AM_INIT_AUTOMAKE(bomb-her-man, 0.1)
AC_LANG([C++])

AC_MSG_CHECKING([whether to activate anim])
AC_ARG_ENABLE([anim],
	AS_HELP_STRING([--disable-anim], [Disactivate animations]),
	[build_anim=$enableval
	AC_MSG_RESULT([$enableval])],
	[build_anim=yes
	AC_MSG_RESULT([yes])])

#AC_MSG_CHECKING([whether to build the docs])
#AC_ARG_ENABLE([doc],
#	AS_HELP_STRING([--enable-doc], [Build documentation]),
#	[build_doc=$enableval
#	AC_MSG_RESULT([$enableval])],
#	[build_doc=no
#	AC_MSG_RESULT([no])])

AC_MSG_CHECKING([whether to activate anim])
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [Activate debug]),
	[debug=$enableval
	AC_MSG_RESULT([$enableval])],
	[debug=yes
	AC_MSG_RESULT([yes])])

AC_PROG_CXX
AC_PROG_INSTALL

AC_SEARCH_LIBS([TTF_OpenFont], [SDL_ttf], [], [AC_MSG_ERROR(SDL_ttf lib required)])

PKG_CHECK_MODULES(SDL, sdl)
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)

PKG_CHECK_MODULES(CAIRO, cairo)
AC_SUBST(CAIRO_CFLAGS)
AC_SUBST(CAIRO_LIBS)

PKG_CHECK_MODULES(LIBRSVG, librsvg-2.0)
AC_SUBST(LIBRSVG_CFLAGS)
AC_SUBST(LIBRSVG_LIBS)

#if test "x$build_doc" = "x"; then
#	AC_PROGS_DOXYGEN
#fi

if test "x$debug" = "xyes"; then
	DEBUG_CXXFLAGS="-g -Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo"
	AC_SUBST(DEBUG_CXXFLAGS)
fi

AM_CONDITIONAL(ANIMATION, [test "x$build_anim" = "xyes"])

AC_OUTPUT([
	Makefile
	src/Makefile
	data/Makefile
])
