/*
  ====== worms.h =======
  Eva Lott
  Interface to worms functions
*/

#ifndef WORMS_H
#define WORMS_H

struct worm_part {
	struct worm_part *next;
	int x, y;
};
typedef struct worm_part WORM;

#define DOWN     1
#define UP       2
#define LEFT     3
#define RIGHT    4
#define WORMBIT  'O'
#define EMPTY    ' '
#define WORMFOOD 'X'

void init_worm (void);
void move_worm (void);
void place_food (void);
void draw (void);
void free_worm (void);

#endif //WORMS_H
