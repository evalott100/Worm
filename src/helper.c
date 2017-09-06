/*
  ====== helper.c ======
  Eva Lott
  Helper functions.
*/

#include <stdlib.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>

#include "helper.h"
#include "worms.h"

//Quit on error
void error_quit (char *msg)
{
	extern WINDOW *mainwin;
	extern int oldcur;

	//Clean up nicely
	delwin(mainwin);
	curs_set(oldcur);
	endwin();
	refresh();
	free_worm();

	//Output error message and exit
	perror(msg);
	exit(EXIT_FAILURE);
}

//Quit successfully
void quit (int reason)
{
	extern WINDOW *mainwin;
	extern int oldcur;
	extern int score;

	//Clean up nicely
	delwin(mainwin);
	curs_set(oldcur);
	endwin();
	refresh();
	free_worm();

	//Output farewell message
	switch (reason) {
   		case HITWALL:
			printf("Score : %d\n",score);
		break;

		case HITSELF:
			printf("Score : %d\n",score);
		break;

		default:
			printf("Score : %d\n",score);
		break;
	}

	exit(EXIT_SUCCESS);
}

//Returns the x-y size of the terminal
void get_term_size(int *rows, int *cols)
{
	struct winsize ws;

	//Get terminal size
	if (ioctl(0,TIOCGWINSZ,&ws) < 0) {
		perror("couldn't get window size");
		exit(EXIT_FAILURE);
	}
	
	//Update globals
	*rows = ws.ws_row;
	*cols = ws.ws_col;
}

//Signal handler
void handler (int signum)
{
	extern WINDOW *mainwin;
	extern int oldcur;

	//Switch on signal number
	switch (signum) {
		//Received from the timer
		case SIGALRM:
			move_worm();
		return;

		case SIGTERM:
		case SIGINT:
			//Clean up nicely
			delwin(mainwin);
			curs_set(oldcur);
			endwin();
			refresh();
			free_worm();
		exit(EXIT_SUCCESS);
	}
}
