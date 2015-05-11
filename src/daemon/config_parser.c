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
#include "json-c/json.h"
#include "config_parser.h"

static char *config_parser_feeds_index = NULL;
static char *config_parser_temp_path_file = NULL;
static char *config_parser_pid_file = NULL;

int config_parser_parse_file(char *path)
{
	json_object *root_element = NULL;
	
	root_element = json_object_from_file(path);
	if(root_element == NULL)
	{
		fprintf(stderr, "Failed to parse JSON config file.\n");
		return -1;
	}
	
	json_object_object_foreach(root_element, key, value)
	{
		if(strcmp(key, "feeds_index") == 0 && json_object_get_type(value) == json_type_string)
		{
			free(config_parser_feeds_index);
			config_parser_feeds_index = strdup(json_object_get_string(value));
		}
		else if(strcmp(key, "temp_path_file") == 0 && json_object_get_type(value) == json_type_string)
		{
			free(config_parser_temp_path_file);
			config_parser_temp_path_file = strdup(json_object_get_string(value));
		}
		else if(strcmp(key, "pid_file") == 0 && json_object_get_type(value) == json_type_string)
		{
			free(config_parser_pid_file);
			config_parser_pid_file = strdup(json_object_get_string(value));
		}
		else
		{
			fprintf(stderr, "Unknown JSON node in config file: %s (node name: %s)\n", path, key);
			json_object_put(root_element);
			return -1;
		}
	}
	
	json_object_put(root_element);
	
	return 0;
}

void config_parser_free(void)
{
	free(config_parser_feeds_index);
	free(config_parser_temp_path_file);
}

char *config_parser_get_feeds_index(void)
{
	if(config_parser_feeds_index == NULL)
	{
		return CONFIG_PARSER_DEFAULT_FEEDS_INDEX;
	}
	
	return config_parser_feeds_index;
}

char *config_parser_get_temp_path_file(void)
{
	if(config_parser_temp_path_file == NULL)
	{
		return CONFIG_PARSER_DEFAULT_TEMP_PATH_FILE;
	}
	
	return config_parser_temp_path_file;
}

char *config_parser_get_pid_file(void)
{
	if(config_parser_pid_file == NULL)
	{
		return CONFIG_PARSER_DEFAULT_PID_FILE;
	}
	
	return config_parser_pid_file;
}