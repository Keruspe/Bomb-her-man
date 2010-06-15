#!/bin/sh

autoreconf -i -Wall
./configure $@
exit 0
