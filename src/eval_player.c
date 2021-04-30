#include <string.h>
#include <time.h>
#include "player.h"
#include "graph_function.h"
#include "strategie/evaluation.h"

struct person{
    char *name;
    struct graph_t *graph;
    size_t *vertices;
    enum color_t color;
    struct eval_strat *e;
    struct set *path;
};

struct person player4;

char const* get_player_name()
{
  struct person *p = &player4;
  p->name = "PoorQueenbee";
  return p->name;
}

struct move_t propose_opening()
{
    size_t size = graph__size(player4.graph);
    size_t op = size/2;
    while(op > size || graph__vertex_colored(player4.graph,op))
    {
      op = op+2;
    }
    struct move_t opening = {.m = op};
    graph__colors_vertex(player4.graph,BLACK,op);
    return opening;
}

int accept_opening(const struct move_t opening)
{
    graph__colors_vertex(player4.graph,BLACK,opening.m);
    return 0;
}

void initialize_graph(struct graph_t* graph)
{
  //A copy is made by a server, it's a copy for this player
  srand(time(NULL));
  player4.e = init_eval_strat(graph);
  struct person *p = &player4;
  p->graph = graph;
}

void initialize_color(enum color_t id)
{
  struct person *p = &player4;
  player4.path = path_to_vertex(player4.graph,id,player4.e->first_side[id],player4.e->second_side[id],player4.e->second_side[id]);
  p->color = id;
}

struct move_t play(struct move_t previous_move)
{
    struct move_t next;
    graph__colors_vertex(player4.graph,(player4.color+1)%2,previous_move.m);
    // if(set__is_empty(player4.e->path))
    // {
    //     set__free(player4.e->path);    
    //     player4.e->path = path_to_vertex(player4.graph,player4.color,player4.e->first_side[player4.color],player4.e->second_side[player4.color],player4.e->first_side[player4.color]);
    // }
    size_t move = minmax_to_play(player4.graph,player4.path,player4.e,player4.color,previous_move.m,1);
    if( move > graph__size(player4.graph) || graph__vertex_colored(player4.graph,move))
    {
        move = rand()%graph__size(player4.graph);
        while(graph__vertex_colored(player4.graph,move))
        {
            move = rand()%graph__size(player4.graph);
        }
    }
    player4.path = path_to_vertex(player4.graph,player4.color,player4.e->first_side[player4.color],player4.e->first_side[player4.color],move);
    graph__colors_vertex(player4.graph,player4.color,move);
    next.m = move;
    next.c = player4.color;
    return next;
}

void finalize()
{
  set__free(player4.path);
  eval__strat_free(player4.e);
  graph__free(player4.graph);
}
