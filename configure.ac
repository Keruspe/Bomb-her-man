AC_PREREQ([2.65])
AC_INIT(bomb-her-man, 0.1, "")

AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(bomb-her-man, 0.1)
AC_LANG([C++])

AC_MSG_CHECKING([for root directory])
AC_ARG_WITH([rootdir],
	AS_HELP_STRING([--with-rootdir], [Specify the final destination dir]),
	ROOTDIR=$withval,
	ROOTDIR="/usr/local")
AC_MSG_RESULT([$ROOTDIR])
AC_SUBST([ROOTDIR])

AC_MSG_CHECKING([for prefix])
AC_ARG_WITH([prefix],
	AS_HELP_STRING([--with-prefix], [Specify the prefix dir]),
	PREFIX=$withval,
	PREFIX="/usr/local/usr")
AC_MSG_RESULT([$PREFIX])
AC_SUBST([PREFIX])

AC_MSG_CHECKING([whether to activate anim])
AC_ARG_ENBALE([anim],
	AS_HELP_STRING([--enable-anim], [Activate animations]),
	[build_anim=$enableval
	AC_MSG_RESULT([$enableval])],
	[build_anim=yes
	AC_MSG_RESULT([yes])])

AC_MSG_CHECKING([whether to build the docs])
AC_ARG_ENBALE([doc],
	AS_HELP_STRING([--enable-doc], [Build documentation]),
	[build_doc=$enableval
	AC_MSG_RESULT([$enableval])],
	[build_doc=no
	AC_MSG_RESULT([no])])

AC_PROG_CXX
AC_PROG_INSTALL

CXXFLAGS="$CXXFLAGS -g -Isrc -Isrc/include -Wall -Wextra -Weffc++ -Wstrict-null-sentinel -Wno-non-template-friend -Wold-style-cast -Wno-pmf-conversions -Wsign-promo -Werr"
AC_SUBST(CXXFLAGS)

AC_SDL_CHECK_LIB(SDL_ttf, TTF_OpenFont, [], [AC_MSG_ERROR(SDL_ttf lib required)])

PKG_CHECK_MODULES(CAIRO, cairo)
AC_SUBST(CAIRO_CXXFLAGS)
AC_SUBST(CAIRO_LIBS)

PKG_CHECK_MODULES(LIBRSVG, librsvg-2.0)
AC_SUBST(LIBRSVG_CXXFLAGS)
AC_SUBST(LIBRSVG_LIBS)

if [ "x$build_doc" == "x" ]; then
	AC_PROGS_DOXYGEN
fi

AC_OUTPUT([Makefile])
