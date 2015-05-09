# Copyright (C) 2015 NIPE-SYSTEMS
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc

CFLAGS += -Wall
CFLAGS += -Wextra
# CFLAGS += `pkg-config --cflags libsoup-2.4`
# CFLAGS += `pkg-config --cflags json-glib-1.0`
# CFLAGS += `pkg-config --cflags glib-2.0`
# CFLAGS += `pkg-config --cflags gio-2.0`
CFLAGS += `pkg-config --cflags libcurl`

# LIBS += `pkg-config --libs libsoup-2.4`
# LIBS += `pkg-config --libs json-glib-1.0`
# LIBS += `pkg-config --libs glib-2.0`
# LIBS += `pkg-config --libs gio-2.0`
LIBS += `pkg-config --libs libcurl`

.PHONY: all daemon init clean

all: daemon

# Compile main program

daemon: init bin/obj/daemon.o bin/obj/download.o
	$(CC) bin/obj/daemon.o bin/obj/download.o $(CFLAGS) -o bin/daemon $(LIBS)

# Initializes directories

init:
	mkdir -p bin
	mkdir -p bin/obj

# C-objects

bin/obj/daemon.o: src/daemon/daemon.c
	$(CC) $(CFLAGS) -c -o bin/obj/daemon.o src/daemon/daemon.c $(LIBS)

bin/obj/download.o: src/daemon/download.c
	$(CC) $(CFLAGS) -c -o bin/obj/download.o src/daemon/download.c $(LIBS)

# Clean

clean:
	rm -Rf bin
