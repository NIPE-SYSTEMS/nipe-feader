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
#include <curl/curl.h>
#include <mrss.h>
#include "unused.h"
#include "json-c/json.h"
#include "rss_to_json.h"

char *rss_to_json(char *data, size_t length)
{
	mrss_t *mrss_data = NULL;
	mrss_error_t mrss_error;
	mrss_item_t *mrss_item = NULL;
	json_object *root_object = NULL;
	json_object *item_array = NULL;
	json_object *item_object = NULL;
	char *return_string = NULL;
	
	mrss_error = mrss_parse_buffer(data, length, &mrss_data);
	if(mrss_error != MRSS_OK)
	{
		fprintf(stderr, "Failed to parse RSS: %s\n", mrss_strerror(mrss_error));
		return NULL;
	}
	
	root_object = json_object_new_object();
	json_object_object_add(root_object, "title", json_object_new_string(SAFE_STRING(mrss_data->title)));
	json_object_object_add(root_object, "description", json_object_new_string(SAFE_STRING(mrss_data->description)));
	json_object_object_add(root_object, "link", json_object_new_string(SAFE_STRING(mrss_data->link)));
	json_object_object_add(root_object, "language", json_object_new_string(SAFE_STRING(mrss_data->language)));
	json_object_object_add(root_object, "rating", json_object_new_string(SAFE_STRING(mrss_data->rating)));
	json_object_object_add(root_object, "copyright", json_object_new_string(SAFE_STRING(mrss_data->copyright)));
	json_object_object_add(root_object, "pub_date", json_object_new_string(SAFE_STRING(mrss_data->pubDate)));
	json_object_object_add(root_object, "last_build_date", json_object_new_string(SAFE_STRING(mrss_data->lastBuildDate)));
	json_object_object_add(root_object, "docs", json_object_new_string(SAFE_STRING(mrss_data->docs)));
	json_object_object_add(root_object, "managingeditor", json_object_new_string(SAFE_STRING(mrss_data->managingeditor)));
	json_object_object_add(root_object, "webmaster", json_object_new_string(SAFE_STRING(mrss_data->webMaster)));
	json_object_object_add(root_object, "generator", json_object_new_string(SAFE_STRING(mrss_data->generator)));
	json_object_object_add(root_object, "ttl", json_object_new_int(mrss_data->ttl));
	json_object_object_add(root_object, "about", json_object_new_string(SAFE_STRING(mrss_data->about)));
	
	item_array = json_object_new_array();
	mrss_item = mrss_data->item;
	
	while(mrss_item)
	{
		item_object = json_object_new_object();
		json_object_object_add(item_object, "title", json_object_new_string(SAFE_STRING(mrss_item->title)));
		json_object_object_add(item_object, "description", json_object_new_string(SAFE_STRING(mrss_item->description)));
		json_object_object_add(item_object, "link", json_object_new_string(SAFE_STRING(mrss_item->link)));
		json_object_object_add(item_object, "author", json_object_new_string(SAFE_STRING(mrss_item->author)));
		json_object_object_add(item_object, "comments", json_object_new_string(SAFE_STRING(mrss_item->comments)));
		json_object_object_add(item_object, "pub_date", json_object_new_string(SAFE_STRING(mrss_item->pubDate)));
		json_object_object_add(item_object, "guid", json_object_new_string(SAFE_STRING(mrss_item->guid)));
		json_object_object_add(item_object, "guid_is_permalink", json_object_new_int(mrss_item->guid_isPermaLink));
		
		json_object_array_add(item_array, item_object);
		
		mrss_item = mrss_item->next;
	}
	
	json_object_object_add(root_object, "items", item_array);
	
	return_string = strdup(json_object_to_json_string_ext(root_object, JSON_C_TO_STRING_PRETTY | JSON_C_TO_STRING_SPACED));
	json_object_put(root_object);
	
	mrss_free(mrss_data);
	
	return return_string;
}
