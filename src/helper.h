/*
  ====== helper.h ======
  Eva Lott
  Interface to helper functions.
*/

#ifndef HELPER_H
#define HELPER_H

//Macros
#define USER_QUIT 1
#define HITSELF   2
#define HITWALL   3

//Function prototypes
void error_quit (char *msg);
void quit (int reason);
void get_term_size (int *rows, int *cols);
void handler (int signum);
void change_dir (int d);

#endif //HELPER_H
