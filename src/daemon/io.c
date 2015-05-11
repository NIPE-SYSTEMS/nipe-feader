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
static io_temp_file_t *io_temp_files = NULL;

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
	if(io_tempdir_created == 1)
	{
		if(rmdir(io_tempdir) < 0)
		{
			fprintf(stderr, "Failed to remove temp directory.\n");
		}
	}
	
	if(io_temp_path_file_created == 1)
	{
		if(unlink(config_parser_get_temp_path_file()) < 0)
		{
			fprintf(stderr, "Failed to remove temp path file.\n");
		}
	}
}

FILE *io_open_new_tempfile(void)
{
	FILE *tempfile_fp = NULL;
	int tempfile_fd = 0;
	char *tempfile_path = NULL;
	size_t tempfile_path_length = 0;
	
	if(io_tempdir_created != 1)
	{
		fprintf(stderr, "Failed to create temp file: No temp directory created.\n");
		return NULL;
	}
	
	printf("%s\n", io_tempdir);
	
	tempfile_path_length = (sizeof(io_tempdir) / sizeof(io_tempdir[0])) + 1 + 6 + 1;
	
	tempfile_path = malloc(tempfile_path_length * sizeof(char));
	if(tempfile_path == NULL)
	{
		fprintf(stderr, "Failed to allocate temp file path.\n");
		return NULL;
	}
	
	strcpy(tempfile_path, io_tempdir);
	strcat(tempfile_path, "/XXXXXX");
	
	printf("Temp file path: %s\n", tempfile_path);
	
	if((tempfile_fd = mkstemp(tempfile_path)) < 0)
	{
		fprintf(stderr, "Failed to make temp file.\n");
		free(tempfile_path);
		return NULL;
	}
	
	if((tempfile_fp = fdopen(tempfile_fd, "w+")) == NULL)
	{
		fprintf(stderr, "Failed to open temp file.\n");
		free(tempfile_path);
		return NULL;
	}
	
	if(io_temp_files == NULL)
	{
		io_temp_files = malloc(sizeof(io_temp_file_t));
		if(io_temp_files == NULL)
		{
			fprintf(stderr, "Failed to allocate temp file structure.\n");
			fclose(tempfile_fp);
			free(tempfile_path);
			return NULL;
		}
		
		io_temp_files->next = NULL;
		io_temp_files->path = tempfile_path;
		io_temp_files->fp = tempfile_fp;
		io_temp_files->fp_open = 1;
	}
	
	return io_temp_files;
}

void io_close_tempfile(FILE **tempfile_fp)
{
	fclose(*tempfile_fp);
	*tempfile_fp = NULL;
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
	if(io_pid_file_created == 1)
	{
		if(unlink(config_parser_get_pid_file()) < 0)
		{
			fprintf(stderr, "Failed to remove pid file.\n");
		}
	}
}
