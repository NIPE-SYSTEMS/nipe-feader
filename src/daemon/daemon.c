/**
 * Copyright (C) 2015 NIPE-SYSTEMS
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "unused.h"
#include "json-c/json.h"
#include <mrss.h>
#include "download.h"
#include "rss_to_file.h"
#include "config_parser.h"
#include "io.h"
#include "loop.h"
#include "feed.h"

int main(int argc, char **argv)
{
	// FILE *tmp_file = NULL;
	
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s [path to config.json]\n", ((argc > 0)?(argv[0]):("nipe-feader-daemon")));
		return 1;
	}
	
	if(config_parser_parse_file(argv[1]) < 0)
	{
		fprintf(stderr, "Failed to parse config file. Aborting.\n");
		config_parser_free();
		return 2;
	}
	
	// fork at this point
	
	if(io_mk_tempdir() < 0)
	{
		config_parser_free();
		io_mk_tempdir_cleanup();
		return 3;
	}
	
	io_create_pid_file();
	
	loop_register_signals();
	
	printf("feeds index: %s\n", config_parser_get_feeds_index());
	printf("temp path file: %s\n", config_parser_get_temp_path_file());
	printf("pid file: %s\n", config_parser_get_pid_file());
	
	download_init();
	
	feed_parse_index();
	
	loop_main_loop();
	
	printf("Exiting...\n");
	
	feed_cleanup();
	download_free();
	io_mk_tempdir_cleanup();
	io_pid_file_cleanup();
	config_parser_free();
	
	return 0;
}
