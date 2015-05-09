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
#include "unused.h"
#include "download.h"

static CURL *curl = NULL;
static download_data_t download_data = { NULL, 0 };

static void download_data_init(void)
{
	download_data.length = 0;
	download_data.data = malloc(sizeof(char));
	if(download_data.data == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for data structure.\n");
		return;
	}
	
	download_data.data[0] = '\0';
}

static void download_data_free(void)
{
	if(download_data.data != NULL)
	{
		free(download_data.data);
	}
}

static size_t download_curl_write_function(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t new_size = 0;
	char *old_data = NULL;
	
	UNUSED(userdata);
	
	new_size = download_data.length + (size * nmemb);
	old_data = download_data.data;
	download_data.data = realloc(download_data.data, (new_size + 1) * sizeof(char));
	if(download_data.data == NULL)
	{
		fprintf(stderr, "Failed to reallocate memory for data structure.\n");
		download_data.data = old_data;
		return 0;
	}
	
	memcpy(download_data.data + download_data.length, ptr, size * nmemb);
	download_data.data[new_size] = '\0';
	download_data.length = new_size;
	
	return size * nmemb;
}

void download_init(void)
{
	curl = curl_easy_init();
	if(curl == NULL)
	{
		fprintf(stderr, "Failed to initialize curl handle.\n");
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "nipe-feader");
	}
}

void download_free(void)
{
	download_data_free();
	
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}

download_data_t *download_url(char *url)
{
	CURLcode result;
	double total_time = 0;
	
	if(curl == NULL)
	{
		return NULL;
	}
	
	download_data_free();
	download_data_init();
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_curl_write_function);
	result = curl_easy_perform(curl);
	if(result != CURLE_OK)
	{
		fprintf(stderr, "Failed to perform request: %s\n", curl_easy_strerror(result));
		return NULL;
	}
	else
	{
		result = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
		printf("Downloaded %li bytes in %f seconds: %s\n", download_data.length, total_time, url);
	}
	
	return &download_data;
}
