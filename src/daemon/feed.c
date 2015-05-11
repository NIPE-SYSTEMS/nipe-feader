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
#include <time.h>
#include "unused.h"
#include "json-c/json.h"
#include "config_parser.h"
#include "rss_to_file.h"
#include "io.h"
#include "feed.h"

static feed_t *feeds = NULL;

void feed_append(feed_t *trailing)
{
	feed_t *iterator = NULL;
	
	if(trailing == NULL)
	{
		return;
	}
	
	if(feeds == NULL)
	{
		feeds = trailing;
	}
	else
	{
		iterator = feeds;
		while(iterator->next != NULL)
		{
			iterator = iterator->next;
		}
		
		iterator->next = trailing;
	}
}

feed_t *feed_new(char *url, unsigned long int interval)
{
	feed_t *feed = NULL;
	
	feed = malloc(sizeof(feed_t));
	if(feed == NULL)
	{
		fprintf(stderr, "Failed to allocate feed structure.\n");
		return NULL;
	}
	
	feed->next = NULL;
	feed->url = strdup(url);
	feed->filename = io_generate_file_path(url);
	feed->interval = interval;
	feed->last_download = 0;
	
	printf("New feed: %s, %li seconds interval > %s\n", feed->url, feed->interval, feed->filename);
	
	return feed;
}

void feed_free(feed_t *feed)
{
	if(feed != NULL && feed->url != NULL)
	{
		free(feed->url);
	}
	
	if(feed != NULL && feed->filename != NULL)
	{
		if(unlink(feed->filename) < 0)
		{
			fprintf(stderr, "Failed to remove feed file.\n");
		}
		
		free(feed->filename);
	}
	
	free(feed);
}

void feed_free_recursive(feed_t *feed)
{
	if(feed->next != NULL)
	{
		feed_free_recursive(feed->next);
	}
	
	feed_free(feed);
}

void feed_cleanup(void)
{
	feed_free_recursive(feeds);
}

static void feed_parse_index_create_feed(json_object *feed)
{
	char *url = NULL;
	unsigned long int interval = 0;
	
	json_object_object_foreach(feed, key, value)
	{
		if(strcmp(key, "url") == 0 && json_object_get_type(value) == json_type_string)
		{
			url = strdup(json_object_get_string(value));
		}
		else if(strcmp(key, "interval") == 0 && json_object_get_type(value) == json_type_int)
		{
			interval = json_object_get_int(value);
		}
	}
	
	if(url != NULL)
	{
		feed_append(feed_new(url, interval));
	}
	
	free(url);
}

int feed_parse_index(void)
{
	json_object *root_element = NULL;
	int feed_amount = 0;
	int i = 0;
	json_object *feed_object = NULL;
	
	root_element = json_object_from_file(config_parser_get_feeds_index());
	if(root_element == NULL)
	{
		fprintf(stderr, "Failed to parse index file.\n");
		return -1;
	}
	
	json_object_object_foreach(root_element, key, value)
	{
		if(strcmp(key, "feeds") == 0 && json_object_get_type(value) == json_type_array)
		{
			feed_amount = json_object_array_length(value);
			for(i = 0; i < feed_amount; i++)
			{
				feed_object = json_object_array_get_idx(value, i);
				if(json_object_get_type(feed_object) == json_type_object)
				{
					feed_parse_index_create_feed(feed_object);
				}
			}
		}
		else
		{
			fprintf(stderr, "Unknown JSON node in config file: (node name: %s)\n", key);
			json_object_put(root_element);
			return -1;
		}
	}
	
	json_object_put(root_element);
	
	return 0;
}

int feed_download(void)
{
	feed_t *iterator = NULL;
	unsigned long int current_time = time(NULL);
	unsigned long int next_interval = 0;
	char next_interval_set = 0;
	
	for(iterator = feeds; iterator != NULL; iterator = iterator->next)
	{
		if((iterator->last_download + iterator->interval) <= current_time)
		{
			rss_to_file(iterator->url, iterator->filename);
			iterator->last_download = current_time;
		}
	}
	
	for(iterator = feeds; iterator != NULL; iterator = iterator->next)
	{
		if(next_interval_set == 0 || ((iterator->last_download + iterator->interval - current_time) < next_interval))
		{
			next_interval = iterator->last_download + iterator->interval - current_time;
			next_interval_set = 1;
		}
	}
	
	return next_interval;
}
