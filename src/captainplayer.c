#include <string.h>
#include <time.h>
#include "player.h"
#include "graph_function.h"
#include "strategie/strategie.h"

struct person{
    char *name;
    struct graph_t *graph;
    size_t *vertices;
    enum color_t color;
    struct strategy *s;
    int attack;
};

struct person player2;

char const* get_player_name()
{
  struct person *p = &player2;
  p->name = "CaptainMFR";
  return p->name;
}

struct move_t propose_opening()
{
    player2.attack = 1;
    size_t size = graph__size(player2.graph);
    size_t op = size/2;
    while(op > size || graph__vertex_colored(player2.graph,op))
    {
      op = op+2;
    }
    struct move_t opening = {.m = op};
    graph__colors_vertex(player2.graph,BLACK,op);
    player2.s->id = BLACK;
    player2.s->my_pm = op;
    return opening;
}

int accept_opening(const struct move_t opening)
{
    player2.attack = 1; //Refusing the oppening and playing an attack strategie
    graph__colors_vertex(player2.graph,BLACK,opening.m);
    player2.s->my_pm = opening.m;
    player2.s->id = BLACK;
    return 0;
}

void initialize_graph(struct graph_t* graph)
{
  //A copy is made by a server, it's a copy for this player
  srand(time(NULL));
  player2.s = strategy__empty();
  struct person *p = &player2;
  p->graph = graph;
  init__strategy_border(player2.s,player2.graph);
}

void initialize_color(enum color_t id)
{
  struct person *p = &player2;
  if (id != BLACK)
  {
    player2.attack = 0;
    player2.s->id = id;
    size_t size = graph__size(player2.graph);
    size_t op = size/2;
    while((op > 0) & graph__vertex_colored(player2.graph,op))
    {
      op = op-2;
    }
    player2.s->my_pm = op;
  }
  player2.s->id = id;
  p->color = id;
}

struct move_t play(struct move_t previous_move)
{
    struct move_t next;
    if(!graph__vertex_colored(player2.graph,player2.s->my_pm))
    {
      next.m = player2.s->my_pm;
      next.c = player2.color;
      graph__colors_vertex(player2.graph,player2.color,next.m);
      return next;
    }
    size_t vertices = graph__size(player2.graph);
    graph__colors_vertex(player2.graph,(player2.color+1)%2,previous_move.m);
    size_t move = strategy_play(player2.s,player2.graph);
    while(graph__vertex_colored(player2.graph,move))
    {
        move = rand()%(vertices-1);
    }
    graph__colors_vertex(player2.graph,player2.color,move);
    next.m = move;
    next.c = player2.color;
    return next;
}

void finalize()
{
  close_and_free__strategy(player2.s);
  graph__free(player2.graph);
}
