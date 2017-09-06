/*
  ====== main.c ======
  Eva Lott
  Worms game using ncurses.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <curses.h>

#include "worms.h"
#include "helper.h"

#define TIMESTEP 30000

void set_timer (void);
void set_signals (void);

WINDOW *mainwin;
int oldcur;

int main (void)
{
	//Seed RNG,set timer and register signal handlers
	srand((unsigned)time(NULL));
	set_timer();
	set_signals();
	
	//Initialize ncurses
	if ((mainwin = initscr()) == NULL) {
		perror("error initialising ncurses");
		exit(EXIT_FAILURE);
	}
	noecho();
	keypad(mainwin,TRUE);
	oldcur = curs_set(0);

	//Initialize worm and draw it
	init_worm();
	draw();

	//Loop and get user input
	while (1) {
		int key = getch();

		switch (key) {

			case KEY_UP:
			case 'Y':
			case 'y':
				change_dir(UP);
			break;

			case KEY_DOWN:
			case 'N':
			case 'n':
				change_dir(DOWN);
			break;

			case KEY_LEFT:
			case 'G':
			case 'g':
				change_dir(LEFT);
			break;

			case KEY_RIGHT:
			case 'J':
			case 'j':
				change_dir(RIGHT);
			break;

			case 'Q':
			case 'q':
				quit(USER_QUIT);
			break;
		}
	}

	//We never get here
	return EXIT_SUCCESS;
}

//Sets up the game timer
void set_timer (void)
{
	struct itimerval it;
	
	//Clear itimerval struct members
	timerclear(&it.it_interval);
	timerclear(&it.it_value);

	//Set timer
	it.it_interval.tv_usec = TIMESTEP;
	it.it_value.tv_usec	= TIMESTEP;
	setitimer(ITIMER_REAL,&it,NULL);
}

//Sets up signal handlers we need
void set_signals (void)
{
	struct sigaction sa;

	//Fill in sigaction struct
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	//Set signal handlers
	sigaction(SIGTERM,&sa,NULL);
	sigaction(SIGINT,&sa,NULL);
	sigaction(SIGALRM,&sa,NULL);

	//Ignore SIGTSTP
	sa.sa_handler = SIG_IGN;
	sigaction(SIGTSTP,&sa,NULL);
}
