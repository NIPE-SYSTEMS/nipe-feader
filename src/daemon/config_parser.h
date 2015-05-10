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

#ifndef __CONFIG_PARSER_H__
#define __CONFIG_PARSER_H__

#define CONFIG_PARSER_DEFAULT_FEEDS_INDEX "/home/hendrik/Programme/nipe-feader/index.json"

int config_parser_parse_file(char *path);
void config_parser_free(void);
char *config_parser_get_feeds_index(void);

#endif /* __CONFIG_PARSER_H__ */
