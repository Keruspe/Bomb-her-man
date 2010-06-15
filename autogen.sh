#!/bin/sh

autoreconf -i -Wall
intltoolize
./configure $@
exit 0
