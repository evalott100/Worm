/*
	====== worms.c ======
	Eva Lott
	Implementation of worms functions.
*/

#include <stdlib.h>
#include <curses.h>

#include "worms.h"
#include "helper.h"

#define MOVESCORE 1
#define FOODSCORE 10
#define WORMSIZE  8

static WORM *worm;
static int dir = DOWN;
static int rows, cols;
int score = 0;

//Initialises the worm, and draws it for the first time
void init_worm (void)
{
	WORM *temp;
	int x = 1, y = 1, i;

	//Allocate memory for worm
	for (i = 0; i < WORMSIZE; i++) {
		if (i == 0) {
			//If this is the first worm part
			if ((worm = malloc(sizeof(WORM))) == NULL)
				error_quit("couldn't allocate memory for worm");
			temp = worm;
		}
		else {
			//If this is not the first worm part
			if ((temp->next = malloc(sizeof(WORM))) == NULL)
				error_quit("couldn't allocate memory for worm");
			temp = temp->next;
		}

		//Set coordinates for this worm part
		temp->x = x;
		temp->y = y++;
	}
	temp->next = NULL;


	// Get window size
	get_term_size(&rows,&cols);  
}

//Draws game arena
void draw (void)
{
	WORM *temp = worm;

	//Draw border around arena
	box(stdscr,0,0);

	//Draw the worm
	while (temp) {
		move(temp->y,temp->x);
		addch(WORMBIT);
		temp = temp->next;
	}

	//Draw initial worm food
	place_food();
}

//Moves the worm
void move_worm (void)
{
	WORM *temp = worm;
	int x, y, ch;

	//Move to last worm bit
	while (temp->next != NULL)
		temp = temp->next;

	//Make a new worm bit
	if ((temp->next = malloc(sizeof(WORM))) == NULL)
		error_quit("couldn't allocate memory in MoveWorm()");

	//Determine position of new worm bit
	switch (dir) {
		case DOWN:
			x = temp->x;
			y = temp->y + 1;
		break;
		
		case UP:
			x = temp->x;
			y = temp->y - 1;
		break;
		
		case LEFT:
			x = temp->x - 1;
			y = temp->y;
		break;
		
		case RIGHT:
			x = temp->x + 1;
			y = temp->y;
		break;
	}

	//Fill in new worm bit structure
	temp = temp->next;
	temp->next = NULL;
	temp->x	= x;
	temp->y	= y;

	//See if we hit anything
	move(y,x);
	switch (ch = inch()) {
		case EMPTY:
			score += MOVESCORE;

			//Delete first worm bit
			temp = worm->next;
			move(worm->y,worm->x);
			addch(EMPTY);
			free(worm);
		worm = temp; //Fallthrough
		
		case WORMFOOD:
			//Add new wormbit
			move(y,x);
			addch(WORMBIT);
			if (ch == WORMFOOD) {
				//Place some new food
				place_food();
				score += FOODSCORE;
			}
			refresh();
		break;

		case WORMBIT:
		quit(HITSELF);

		default:
		quit(HITWALL);
	}
}

//Place food in the arena
void place_food (void)
{
	int x, y;

	do {
		x = rand() % (cols - 3) + 1;
		y = rand() % (rows - 3) + 1;
		move(y,x);
	}
	while (inch() != EMPTY);

	addch(WORMFOOD);
}

//Changes the direction of travel
void change_dir (int d)
{
	WORM *temp = worm;

	//Move to last worm bit
	while (temp->next != NULL)
		temp = temp->next;

	//Determine next position
	switch (d) {
		case LEFT:
			if (dir == RIGHT)
				return;
			move(temp->y,temp->x - 1);
		break;
	  
		case RIGHT:
			if (dir == LEFT)
				return;
			move(temp->y,temp->x + 1);
		break;

		case UP:
			if (dir == DOWN)
				return;
			move(temp->y - 1,temp->x);
		break;

		case DOWN:
			if (dir == UP)
				return;
		move(temp->y + 1,temp->x);
		break;
	}

	dir = d;
}

//Releases memory used by worm
void free_worm (void)
{
	WORM *temp = worm;
	while (worm) {
		temp = worm->next;
		free(worm);
		worm = temp;
	}
}
