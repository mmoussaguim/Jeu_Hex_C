#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

#include "move.h"
#include "graph_function.h"
#include "player.h"
#include "server.h"
#include "stack.h"


int player__from_library(char *filename, struct player *p)
{
  p->lib = dlopen(filename, RTLD_LAZY);
  if (!p->lib) {
    fputs(dlerror(), stderr);
    exit(EXIT_FAILURE);
  }
  p->get_player_name = dlsym(p->lib, "get_player_name");
  p->propose_opening = dlsym(p->lib, "propose_opening");
  p->accept_opening = dlsym(p->lib, "accept_opening");
  p->initialize_graph = dlsym(p->lib, "initialize_graph");
  p->initialize_color = dlsym(p->lib, "initialize_color");
  p->play = dlsym(p->lib, "play");
  p->finalize = dlsym(p->lib, "finalize");
  return 0;
}

void game__close_and_free(struct game *g)
{
  graph__free(g->board);
  for (size_t i = 0; i<2; i++)
    dlclose((g->players+i)->lib);
}

struct player * compute_next_player(struct game * g)
{
  return g->players + g->next;
}

void game__change_next(struct game *g)
{
  if (g->next == 0)
    g->next = 1;
  else
    g->next = 0;
}


// is winning

static size_t starting_vertex_num(char shape, size_t size, enum color_t c)
{
  if (c == BLACK)
    return 0;
  if (shape == 'h' || shape == 'c')
    return size+1;
  return (size/2 + 1)*(size/2 + 2) - 1;
}

static size_t ending_vertex_num(char shape, size_t size, enum color_t c)
{
  if (c == BLACK) {
    if (shape == 't')
      return 6*size*size;
    return (size+1)*(size+1) - 1;
  }
  if (shape == 'h' || shape == 'c')
    return size;
  return size * (size+1);
}
  
static int are_connected(struct graph_t *g, size_t begin, size_t end, size_t size, enum color_t c)
{

  gsl_spmatrix *mark = gsl_spmatrix_alloc(1,graph__size(g));
  struct stack *s = stack__empty();
  stack__push(s, begin);

  size_t tmp = begin;
  size_t min_neighbour;
  size_t max_neighbour;
   
  while (!stack__is_empty(s)) {
    tmp = stack__pop(s);    // pop the top of the stack
    gsl_spmatrix_set(mark, 0, tmp, 1);    // mark the current vertice
    // Neighbour are between tmp - 2*size and tmp + 2*size but vertices are between 0 and size*size
    if (tmp < 2*size)
      min_neighbour = 0;
    else
      min_neighbour = tmp - 2*size;

    if (tmp >= size * (size-2))
      max_neighbour = (size+1)*(size+1);
    else
      max_neighbour = tmp + 2*size ;
    // Finds neighbours of the current vertice
    for (; min_neighbour < max_neighbour; min_neighbour++) {
      // a neighbour has an edge with the current vertice, but we search a one with the same color but not marked
      if (graph__edge(g, tmp, min_neighbour) && graph__vertex_has_color(c, g, min_neighbour) && !gsl_spmatrix_get(mark, 0, min_neighbour)) {
	if (min_neighbour == end) {// the path between begin and end has been found
	  stack__free(s);
	  gsl_spmatrix_free(mark);
	  return 1;
	}
	stack__push(s, min_neighbour);
      }
    }
  }

  stack__free(s);
  gsl_spmatrix_free(mark);

  return 0;
}

int is_winning(struct game *g, struct move_t move, char shape, size_t size)
{
  // the current player try to cheat and play an invalid move so the game end and he loses
  if (move.c == NO_COLOR || graph__vertex_colored(g->board, move.m)) {
    game__change_next(g);
    return 1;
  }

  graph__colors_vertex(g->board, move.c, move.m);

  return are_connected(g->board, starting_vertex_num(shape, size, move.c), ending_vertex_num(shape, size, move.c), size, move.c);
  //return 0;
}

