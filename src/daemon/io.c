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
#include "io.h"

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
