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
#include <unistd.h>
#include <math.h>
#include "io.h"
#include "config_parser.h"

static char io_tempdir[] = IO_TEMPDIR_NAME;
static int io_tempdir_created = 0;
static int io_temp_path_file_created = 0;
static int io_pid_file_created = 0;

int io_write(char *path, char *data, size_t length)
{
	FILE *fd = NULL;
	
	fd = fopen(path, "w");
	if(fd == NULL)
	{
		fprintf(stderr, "Failed to open file: %s\n", path);
		return -1;
	}
	
	fwrite(data, sizeof(char), length, fd);
	fclose(fd);
	
	return 0;
}

int io_mk_tempdir(void)
{
	if(mkdtemp(io_tempdir) == NULL)
	{
		fprintf(stderr, "Failed to make temp directory.\n");
		return -1;
	}
	
	io_tempdir_created = 1;
	
	if(io_write(config_parser_get_temp_path_file(), io_tempdir, sizeof(io_tempdir) / sizeof(io_tempdir[0])) < 0)
	{
		fprintf(stderr, "Failed to create temp path file.\n");
		return -1;
	}
	
	io_temp_path_file_created = 1;
	
	return 0;
}

void io_mk_tempdir_cleanup(void)
{
	if(io_tempdir_created == 1 && rmdir(io_tempdir) < 0)
	{
		fprintf(stderr, "Failed to remove temp directory.\n");
	}
	
	if(io_temp_path_file_created == 1 && unlink(config_parser_get_temp_path_file()) < 0)
	{
		fprintf(stderr, "Failed to remove temp path file.\n");
	}
}

void io_create_pid_file(void)
{
	pid_t pid = 0;
	int digits = 1;
	char *pid_string = NULL;
	
	pid = getpid();
	if(pid != 0)
	{
		digits = floor(log10(abs(pid))) + 1;
	}
	
	pid_string = malloc((digits + 1) * sizeof(char));
	if(pid_string == NULL)
	{
		fprintf(stderr, "Failed to allocate pid.\n");
		return;
	}
	
	sprintf(pid_string, "%i", pid);
	
	if(io_write(config_parser_get_pid_file(), pid_string, digits) < 0)
	{
		fprintf(stderr, "Failed to create pid file.\n");
		return;
	}
	
	io_pid_file_created = 1;
	
	free(pid_string);
}

void io_pid_file_cleanup(void)
{
	if(io_pid_file_created == 1 && unlink(config_parser_get_pid_file()) < 0)
	{
		fprintf(stderr, "Failed to remove pid file.\n");
	}
}

char *io_generate_file_path(char *url)
{
	size_t i = 0;
	char *filename = NULL;
	char *temp_filename = strdup(url);
	if(temp_filename == NULL)
	{
		fprintf(stderr, "Failed to allocate feed temp filename.\n");
		return NULL;
	}
	
	for(i = 0; i < strlen(url); i++)
	{
		switch(temp_filename[i])
		{
			case '/': case '*': case '?': case '<': case '>': case '|':
			{
				temp_filename[i] = '.';
				break;
			}
		}
	}
	
	filename = malloc((strlen(io_tempdir) + 1 + strlen(temp_filename) + 1) * sizeof(char));
	if(filename == NULL)
	{
		fprintf(stderr, "Failed to allocate feed filename.\n");
		return NULL;
	}
	
	sprintf(filename, "%s/%s", io_tempdir, temp_filename);
	
	free(temp_filename);
	
	return filename;
}
