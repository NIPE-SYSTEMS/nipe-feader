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
# CFLAGS += -D VALGRIND
# CFLAGS += `pkg-config --cflags libsoup-2.4`
# CFLAGS += `pkg-config --cflags json-glib-1.0`
# CFLAGS += `pkg-config --cflags glib-2.0`
# CFLAGS += `pkg-config --cflags gio-2.0`
CFLAGS += `pkg-config --cflags mrss`
CFLAGS += `pkg-config --cflags libcurl`

# LIBS += `pkg-config --libs libsoup-2.4`
# LIBS += `pkg-config --libs json-glib-1.0`
# LIBS += `pkg-config --libs glib-2.0`
# LIBS += `pkg-config --libs gio-2.0`
LIBS += `pkg-config --libs mrss`
LIBS += `pkg-config --libs libcurl`
LIBS += -lm

.PHONY: all daemon init clean

all: daemon

# Compile main program

daemon: init bin/obj/daemon.o bin/obj/download.o bin/obj/rss_to_json.o bin/obj/io.o bin/obj/rss_to_file.o bin/obj/config_parser.o bin/obj/loop.o bin/obj/feed.o bin/obj/arraylist.o bin/obj/debug.o bin/obj/json_c_version.o bin/obj/json_object.o bin/obj/json_object_iterator.o bin/obj/json_tokener.o bin/obj/json_util.o bin/obj/linkhash.o bin/obj/printbuf.o bin/obj/random_seed.o
	$(CC) bin/obj/daemon.o bin/obj/download.o bin/obj/rss_to_json.o bin/obj/io.o bin/obj/rss_to_file.o bin/obj/config_parser.o bin/obj/loop.o bin/obj/feed.o bin/obj/arraylist.o bin/obj/debug.o bin/obj/json_c_version.o bin/obj/json_object.o bin/obj/json_object_iterator.o bin/obj/json_tokener.o bin/obj/json_util.o bin/obj/linkhash.o bin/obj/printbuf.o bin/obj/random_seed.o $(CFLAGS) -o bin/nipe-feader-daemon $(LIBS)

# Initializes directories

init:
	mkdir -p bin
	mkdir -p bin/obj

# C-objects

bin/obj/daemon.o: src/daemon/daemon.c
	$(CC) $(CFLAGS) -c -o bin/obj/daemon.o src/daemon/daemon.c $(LIBS)

bin/obj/download.o: src/daemon/download.c
	$(CC) $(CFLAGS) -c -o bin/obj/download.o src/daemon/download.c $(LIBS)

bin/obj/rss_to_json.o: src/daemon/rss_to_json.c
	$(CC) $(CFLAGS) -c -o bin/obj/rss_to_json.o src/daemon/rss_to_json.c $(LIBS)

bin/obj/io.o: src/daemon/io.c
	$(CC) $(CFLAGS) -c -o bin/obj/io.o src/daemon/io.c $(LIBS)

bin/obj/rss_to_file.o: src/daemon/rss_to_file.c
	$(CC) $(CFLAGS) -c -o bin/obj/rss_to_file.o src/daemon/rss_to_file.c $(LIBS)

bin/obj/config_parser.o: src/daemon/config_parser.c
	$(CC) $(CFLAGS) -c -o bin/obj/config_parser.o src/daemon/config_parser.c $(LIBS)

bin/obj/loop.o: src/daemon/loop.c
	$(CC) $(CFLAGS) -c -o bin/obj/loop.o src/daemon/loop.c $(LIBS)

bin/obj/feed.o: src/daemon/feed.c
	$(CC) $(CFLAGS) -c -o bin/obj/feed.o src/daemon/feed.c $(LIBS)

bin/obj/arraylist.o: src/daemon/json-c/arraylist.c
	$(CC) $(CFLAGS) -c -o bin/obj/arraylist.o src/daemon/json-c/arraylist.c $(LIBS)

bin/obj/debug.o: src/daemon/json-c/debug.c
	$(CC) $(CFLAGS) -c -o bin/obj/debug.o src/daemon/json-c/debug.c $(LIBS)

bin/obj/json_c_version.o: src/daemon/json-c/json_c_version.c
	$(CC) $(CFLAGS) -c -o bin/obj/json_c_version.o src/daemon/json-c/json_c_version.c $(LIBS)

bin/obj/json_object.o: src/daemon/json-c/json_object.c
	$(CC) $(CFLAGS) -c -o bin/obj/json_object.o src/daemon/json-c/json_object.c $(LIBS)

bin/obj/json_object_iterator.o: src/daemon/json-c/json_object_iterator.c
	$(CC) $(CFLAGS) -c -o bin/obj/json_object_iterator.o src/daemon/json-c/json_object_iterator.c $(LIBS)

bin/obj/json_tokener.o: src/daemon/json-c/json_tokener.c
	$(CC) $(CFLAGS) -c -o bin/obj/json_tokener.o src/daemon/json-c/json_tokener.c $(LIBS)

bin/obj/json_util.o: src/daemon/json-c/json_util.c
	$(CC) $(CFLAGS) -c -o bin/obj/json_util.o src/daemon/json-c/json_util.c $(LIBS)

bin/obj/linkhash.o: src/daemon/json-c/linkhash.c
	$(CC) $(CFLAGS) -c -o bin/obj/linkhash.o src/daemon/json-c/linkhash.c $(LIBS)

bin/obj/printbuf.o: src/daemon/json-c/printbuf.c
	$(CC) $(CFLAGS) -c -o bin/obj/printbuf.o src/daemon/json-c/printbuf.c $(LIBS)

bin/obj/random_seed.o: src/daemon/json-c/random_seed.c
	$(CC) $(CFLAGS) -c -o bin/obj/random_seed.o src/daemon/json-c/random_seed.c $(LIBS)

# Clean

clean:
	rm -Rf bin
