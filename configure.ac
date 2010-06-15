AC_PREREQ(2.65)
AC_INIT([bomb-her-man],[0.1],[""])

AC_CONFIG_SRCDIR([src/bomb-her-man.cpp])
AC_CONFIG_HEADERS([src/include/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(bomb-her-man, 0.1)
AC_LANG([C++])


AC_PROG_CXX

GETTEXT_PACKAGE=bomb-her-man
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
	[The prefix for our gettext translation domains.])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])

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

AC_MSG_CHECKING([whether to activate debug])
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug], [Activate debug]),
	[debug=$enableval
	AC_MSG_RESULT([$enableval])],
	[debug=no
	AC_MSG_RESULT([no])])

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

#if test "x$build_doc" = "xyes"; then
#	AC_ARG_VAR([DOXYGEN],[the 'doxygen' program used to build documentation])
#	AC_PATH_PROG([DOXYGEN],[doxygen],[])
#	if test "x$DOXYGEN" = "x"; then
#		AC_MSG_NOTICE([==> doxygen command not found!])
#		AC_MSG_NOTICE([==> Set DOXYGEN variable if present in non-standard path!])
#		AC_MSG_ERROR([doxygen is mandatory to build documentation : will stop here!])
#	fi
#fi

if test "x$debug" = "xyes"; then
	DEBUG_CXXFLAGS="-g -Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo -Werror"
	AC_SUBST(DEBUG_CXXFLAGS)
fi

AM_CONDITIONAL(ANIMATION, [test "x$build_anim" = "xyes"])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	data/Makefile
	data/graphics/Makefile
	po/Makefile.in
])
AC_OUTPUT
