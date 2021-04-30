#ifndef _EVALUATION_H_
#define _EVALUATION_H_

#include <stdio.h>
#include <stdlib.h>
#include "set_sentinel.h"
#include "../graph_function.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#define INFINI 9999
#define MAX 10

/**
 * Structure discribing the four sides of a board
 */
struct eval_strat {
    int first_side[2];
    int second_side[2];
};

/**
 * Function that calculate the shortest path from a start node in a graph
 * Param : g the graph
 * Param : id the color for the player's path
 * Param : n number of vertices
 * Param : startnode
 * Param : distance an allocated array to store the distance between the startnode and the others
 */
int *dijkstra(struct graph_t *g,enum color_t id,int n,int startnode,int *distance);

/**
 * Function that initialize an eval_strat
 * Param : graph g
 */
struct eval_strat * init_eval_strat(struct graph_t *g);

/**
 * Function that free the eval_strat e
 */
void eval__strat_free(struct eval_strat *e);

/**
 * Function that returns a pointer to a set containing the neighbours not colored of the vertex 'position'
 * Param : graph g
 */
struct set *neighbours(struct graph_t *g,size_t position);

/**
 * Function that determines the value of position based on the adjacent celss with the Minimax approach
 * Param : position that will be evaluated
 * Param : the depth of the calculus
 * Param : maximizingPlayer a boolean : 1 for Maximizing / 0 for Minimizing
 * Param : a graph g
 * Param : id the color of the player
 * Param : e an eval_strat containing the fourth sides
 * Param : evaluate_position a function that evaluate the Position/Board
 *                         //FOR THE ALPHA BETA PRUNING//
 * Param : alpha initialized on -INFINI
 * Param : beta initialized on +IINFINI
 */
int minimax(size_t position, int depth, int alpha, int beta, int maximizingPlayer,struct graph_t *g,enum color_t id,struct eval_strat *e, int (*evaluate_position)(struct graph_t*,struct eval_strat*,enum color_t));

/**
 * Function that returns the distance between two nodes (startnode) and (endnode)
 * Param : pred it's the DIJSKTRA results containing the predecessors
 * Param : g a graph
 */
int distance(struct graph_t *g,int startnode,int endnode,int *pred);

/**
 * Function that returns the shortest past between nodes (u and v)
 * Param : g a graph
 * Param : id the color of player
 */
struct set *path__vertices(struct graph_t *g,enum color_t id,size_t u, size_t v);

/**
 * Function that returns the nearest neighbour to vertex from the set s
 * Param : id the color of the player
 * Param : p a graph
 */
int aux(struct graph_t *g,struct set *s,int vertex,enum color_t id);

/**
 * Function that returns the two-distances evaluation for two nodes (u and v) -> d(u,v)
 * Param : id player's color
 * Param : g a graph
 */
int two_distance(struct graph_t *g, enum color_t id,size_t u,size_t v);

/**
 * Function that returns the two-distances evaluation fro the board depending on player's color id
 * Param : e eval_start for the four sides
 * Param ! g a graph
 */
int graph__evaluation(struct graph_t *g,struct eval_strat *e,enum color_t id);

/**
 * Function that provides the next move for a player id with a minimax approach
 * Param : g a graph
 * Param : path the shortest's path for the player id
 * Param : an eval_strat e
 * Param : a player's color id
 * Param : pm the previous move for the opponent
 * Param : depth the depth for minimax
 */
size_t minmax_to_play(struct graph_t *g,struct set *path,struct eval_strat *e,enum color_t id, size_t pm,int depth);

/**
 * Function that returns the shortest's path from the node i to the node j passing by vertex
 * Param : g a graph
 * Param : a player's color id
 * Param : i,j,vertex : vertices
 */
struct set *path_to_vertex(struct graph_t *g,enum color_t id,int i,int j,int vertex);
#endif 