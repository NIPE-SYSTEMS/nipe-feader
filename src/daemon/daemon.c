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
#include "download.h"

int main(int argc, char **argv)
{
	download_data_t *download_data = NULL;
	
	download_init();
	download_data = download_url("https://github.com/NIPE-SYSTEMS.private.atom?token=ADuE4cnQhiyBZSsYTHG4jJI-ZUM3tSjnks6zW0QxwA==");
	download_data = download_url("http://heise.de.feedsportal.com/c/35207/f/653902/index.rss");
	
	download_free();
	
	return 0;
}
