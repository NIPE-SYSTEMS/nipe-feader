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
#include <signal.h>
#include "unused.h"
#include "loop.h"
#include "feed.h"

static char running = 0;

static void loop_signal_handler_alarm(int signal_number)
{
	UNUSED(signal_number);
	
	// printf("Got SIGALRM.\n");
}

static void loop_signal_handler_usr1(int signal_number)
{
	UNUSED(signal_number);
	
	printf("Got SIGUSR1.\n");
}

static void loop_signal_handler_usr2(int signal_number)
{
	UNUSED(signal_number);
	
	printf("Got SIGUSR2.\n");
}

static void loop_signal_handler_interrupt(int signal_number)
{
	UNUSED(signal_number);
	
	printf("Got SIGINT. Stopping main loop...\n");
	
	running = 0;
}

void loop_register_signals(void)
{
	printf("Registering signals...\n");
	
	signal(SIGALRM, loop_signal_handler_alarm);
	signal(SIGUSR1, loop_signal_handler_usr1);
	signal(SIGUSR2, loop_signal_handler_usr2);
	signal(SIGINT, loop_signal_handler_interrupt);
}

void loop_main_loop(void)
{
	running = 1;
	
	while(running == 1)
	{
		alarm(feed_download());
		pause();
	}
}
