#include <string.h>
#include "player.h"
#include "graph_function.h"
#define MAX_STRING 20
#define MAX_N 6

struct person{
  char* name;
  struct graph_t *graph;
  enum color_t color;
};

struct person player1 = {"Player1", NULL, 2};

char const* get_player_name()
{
  return player1.name;
}

struct move_t propose_opening()
{
    size_t vertices = graph__size(player1.graph);
    size_t mv = rand()%vertices;
    while((graph__vertex_colored(player1.graph,mv)))
      {
	       mv = rand()%vertices;
      }
    struct move_t opening = { mv, 0};
    graph__colors_vertex(player1.graph,0,mv);
    return opening;
}

int accept_opening(const struct move_t opening)
{
    (void)opening;
    return 1;
}

void initialize_graph(struct graph_t* graph)
{
  //A copy is made by a server, it's a copy for this player
  player1.graph = graph;
}

void initialize_color(enum color_t id)
{
  player1.color = id;
}


struct move_t play(struct move_t previous_move)
{
  
  struct move_t next = {.m = 0, .c = player1.color};
  size_t pm = previous_move.m;
  struct graph_t *graph = player1.graph;
  size_t vertices = graph__size(graph);
  graph__colors_vertex(graph,previous_move.c,pm);
  size_t move;;
  size_t tab[MAX_N];
  size_t count = 0;
  for(size_t i = 0; i < vertices; i++)
    {
      if((!graph__vertex_colored(graph, i)) && (graph__edge(graph, pm, i)))
	    {
	      tab[count] = i;
	      count++;
	    }
    }
  if(count)
    {
      if(previous_move.c == BLACK)
	{
	  for(size_t j = 0; j < count; j++)
	    {
	      if((tab[j] < pm -1) || (tab[j] > pm +1))
		{
		  next.m = tab[j];
		  break;
		}
	    }
	  if(!next.m)
	    {
	      for(size_t j = 0; j < count; j++)
		{
		  if((tab[j] == pm -1) || (tab[j] == pm +1))
		    {
		      next.m = tab[j];
		      break;
		    }
		}
	    }
	}
      else
	{
	  for(size_t j = 0; j < count; j++)
	    {
	      
	      if((tab[j] == pm -1) || (tab[j] == pm +1))
		{
		  next.m = tab[j];
		  break;
		}
	    }
	  if(!next.m)
	    {
	      for(size_t j = 0; j < count; j++)
		{
		  if((tab[j] < pm -1) || (tab[j] > pm +1))
		    {
		      next.m = tab[j];
		      break;
		    }
		}
	    }
	}
    }
  else
    {
      move = rand()%vertices;
      while((graph__vertex_colored(graph,move)))
	      {
	        move = rand()%vertices;
	      }
      next.m = move;
    }
  graph__colors_vertex(graph,player1.color,next.m);
  return next;
}

void finalize()
{
  graph__free(player1.graph);
}
