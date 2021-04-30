#ifndef __SERVER_H
#define __SERVER_H

#include <stdlib.h>
#include "graph_function.h"
#include "move.h"
// Structure wich memorize function's pointer of a player
struct player {
  char const* (*get_player_name)(void);
  struct move_t (*propose_opening)(void);
  int (*accept_opening)(const struct move_t opening);
  void (*initialize_graph)(struct graph_t* graph);
  void (*initialize_color)(enum color_t id);
  struct move_t (*play)(struct move_t previous_move);
  void (*finalize)(void);
  void * lib;
};

// Structure which gathered data of the game
struct game {
  struct graph_t * board;
  size_t next;
  struct player players[2];
};

// File a player from a library
int player__from_library(char *filename, struct player *);

// Return a boolean which say if the game is won
int is_winning(struct game *, struct move_t, char shape, size_t size);

// Say who's turn to play
struct player * compute_next_player(struct game *);

// Change the next player in the game
void game__change_next(struct game *);

// Free boards and close libraries
void game__close_and_free(struct game *);

#endif
