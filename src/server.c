#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "move.h"
#include "graph_function.h"
#include "player.h"
#include "server.h"

// Global variables which store option informations
static int m = 10;     // -m
static char T = 'h';  // -t
static size_t M = 10;

////////////////////////////////////////////////////////////////
// Function for parsing the options of the program
// Currently available options are :
// -t <shape> : sets the shape of the board
// -m <size> : sets size of the board
static void parse_opts(int argc, char* argv[])
{
  int opt;
  while ((opt = getopt(argc, argv, "t:m:")) != -1) {
    switch (opt) {
    case 'm':
      m = atoi(optarg);
      break;
    case 't':
      T = optarg[0];
      break;
    default: /* '?' */
      fprintf(stderr, "Usage: %s [-m size][-t shape] \n",
              argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}

// Create a player from pointer to a library given in parameter


int main(int argc, char *argv[])
{
  parse_opts(argc,argv);

  if (optind+1 >= argc) {
    fprintf(stderr, "Expected path to library for both of 2 players\n");
    exit(EXIT_FAILURE);
  }

  if (m < 0) {
    fprintf(stderr, "Expected a positive integer for the size\n");
    exit(EXIT_FAILURE);
  } else
    M = (size_t) m;

  if (T != 'c' && T != 'h' && T != 't') {
    fprintf(stderr, "Unexpected character for the shape, please enter one of the following ones:\n\t* t : for triangular graph\n\t* c : for square qraph\n\t* h : for hexagonale graph\n");
    exit(EXIT_FAILURE);
  }
  
  // Initialze the game
  struct game game;
  game.board = graph__initialize(M,T);
  for (size_t i=0; i<2; i++) {
    player__from_library(argv[optind+i], game.players+i);
  }
  game.next = 0;

  // Give a copy of the board to each player
  struct graph_t * tmp;
  for (size_t i=0; i<2; i++) {
    tmp = graph__copy(game.board);
    game.players[i].initialize_graph(tmp);
  }
  
  struct move_t move = game.players[0].propose_opening();
  if (game.players[1].accept_opening(move)) { // 2nd player plays next
    game.players[0].initialize_color(BLACK);
    game.players[1].initialize_color(WHITE);
    game.next = WHITE;
  } else { // 2nd player refuses and 1st player plays next
    game.players[0].initialize_color(WHITE);
    game.players[1].initialize_color(BLACK);
    game.next = BLACK;
  }
  graph__colors_vertex(game.board, BLACK, move.m);

  struct player * p;
  size_t turn = 1;
  size_t game_over = graph__is__full(game.board);
  while(!game_over) {
    printf("turn : %lu\n", turn++);
    p = compute_next_player(&game);
    printf("%s plays\n",p->get_player_name());
    move = p->play(move);
    printf("move : %lu\n\n", move.m);
    if (is_winning(&game, move, T, M)) {
        break;
    }
    game_over = graph__is__full(game.board);

    game__change_next(&game);
      }

  // Show the result
  if(!game_over) {
    printf("%s won !!!!\n",p->get_player_name());
    graph__print(game.board,T);
  } else {
    printf("Equality, there is no winner\n");
  }


  // Close the game
  for (size_t i=0; i<2; i++)
    game.players[i].finalize();
  game__close_and_free(&game);

  return EXIT_SUCCESS;
}
