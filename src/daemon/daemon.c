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

int main(int argc, char **argv)
{
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
	
	// download_init();
	// rss_to_file("http://www.nipe-systems.de/blog/rss.php", "nipe.json");
	// rss_to_file("https://github.com/NIPE-SYSTEMS.private.atom?token=ADuE4cnQhiyBZSsYTHG4jJI-ZUM3tSjnks6zW0QxwA==", "github.json");
	// rss_to_file("http://heise.de.feedsportal.com/c/35207/f/653902/index.rss", "heise.json");
	// rss_to_file("http://rss.golem.de/rss.php?feed=ATOM1.0", "golem.json");
	// download_free();
	
	printf("feeds index: %s\n", config_parser_get_feeds_index());
	config_parser_free();
	
	return 0;
}
