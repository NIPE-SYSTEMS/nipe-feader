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
#include "unused.h"
#include <libxml/xmlreader.h>
#include "json-c/json.h"
#include "download.h"

static void printNodes(xmlNode *node, int level)
{
	xmlNode *current_node = NULL;
	int i;
	
	for(current_node = node; current_node; current_node = current_node->next)
	{
		// if(current_node->type == XML_ELEMENT_NODE)
		// {
			
		// }
		
		for(i = 0; i < level; i++)
		{
			printf("    ");
		}
		
		printf("node type: %i, name: %s, content: %s\n", current_node->type, current_node->name, current_node->content);
		
		printNodes(current_node->children, level + 1);
	}
}

static void parseXML(char *data, size_t length)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	
	doc = xmlReadMemory(data, length, "https://github.com/NIPE-SYSTEMS.private.atom?token=ADuE4cnQhiyBZSsYTHG4jJI-ZUM3tSjnks6zW0QxwA==", NULL, XML_PARSE_NOENT);
	if(doc == NULL)
	{
		fprintf(stderr, "Failed to read XML data.\n");
		return;
	}
	
	root_element = xmlDocGetRootElement(doc);
	
	printNodes(root_element, 0);
	
	xmlFreeDoc(doc);
}

int main(int argc, char **argv)
{
	UNUSED(argc);
	UNUSED(argv);
	
	download_data_t *download_data = NULL;
	
	// TODO: XML with libxml2
	
	// struct json_object *object = json_object_new_object();
	// json_object_object_add(object, "abc", json_object_new_string("Testtext1"));
	// json_object_object_add(object, "def", json_object_new_string("Testtext2"));
	// json_object_object_add(object, "ghi", json_object_new_string("Testtext3"));
	// printf("%s\n", json_object_to_json_string(object));
	// json_object_put(object);
	
	download_init();
	// download_url("https://github.com/NIPE-SYSTEMS.private.atom?token=ADuE4cnQhiyBZSsYTHG4jJI-ZUM3tSjnks6zW0QxwA==");
	// download_url("http://heise.de.feedsportal.com/c/35207/f/653902/index.rss");
	download_data = download_url("https://github.com/NIPE-SYSTEMS.private.atom?token=ADuE4cnQhiyBZSsYTHG4jJI-ZUM3tSjnks6zW0QxwA==");
	
	// printf("%s\n", download_data->data);
	
	parseXML(download_data->data, download_data->length);
	
	xmlCleanupParser();
	xmlMemoryDump();
	
	download_free();
	
	return 0;
}
