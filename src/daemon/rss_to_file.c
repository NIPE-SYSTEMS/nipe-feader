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
#include <string.h>
#include "unused.h"
#include "download.h"
#include "rss_to_json.h"
#include "io.h"
#include "rss_to_file.h"

int rss_to_file(char *url, char *path)
{
	download_data_t *download_data = NULL;
	char *json_string = NULL;
	int error = 0;
	
	download_data = download_url(url);
	if(download_data == NULL)
	{
		return -1;
	}
	
	json_string = rss_to_json(download_data->data, download_data->length);
	if(json_string == NULL)
	{
		return -1;
	}
	
	if(io_write(path, json_string, strlen(json_string)) < 0)
	{
		error = 1;
	}
	
	free(json_string);
	
	if(error == 1)
	{
		return -1;
	}
	
	return 0;
}