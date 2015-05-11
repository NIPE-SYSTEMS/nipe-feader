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

#ifndef __FEED_H__
#define __FEED_H__

struct _feed_t
{
	struct _feed_t *next;
	char *url;
	char *filename;
	unsigned long int interval;
	unsigned long int last_download;
};
typedef struct _feed_t feed_t;

void feed_append(feed_t *trailing);
feed_t *feed_new(char *url, unsigned long int interval);
void feed_free(feed_t *feed);
void feed_free_recursive(feed_t *feed);
void feed_cleanup(void);
int feed_parse_index(void);
int feed_download(void);

#endif /* __FEED_H__ */
