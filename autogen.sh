#!/bin/sh

autoreconf -i -Wall
gettextize --symlink --no-changelog
./configure $@
exit 0
