#ifndef _STRATEGIE_H_
#define _STRATEGIE_H_
#include <stdio.h>
#include <stdlib.h>
#include "set_sentinel.h"
#include "../graph_function.h"

#define MAXLINE 15
#define MAXINT 1024
#define MAXCARRIERS 6

/**
 * Structure defining :
 * -> The four sides of the board game
 * -> id : Player's color using this strategy
 * -> my_pm : The previous move of this player
 * -> path : the path that he dicides to pursuit
 */
struct strategy {
    struct set *first__border[2];
    struct set *second__border[2];
    enum color_t id;
    size_t my_pm;
    struct set* path;
};

/**
 * Function that calculates the shortest's path from a start node in a graph
 * Param : g the graph
 * Param : id the color for the player's path
 * Param : n number of vertices
 * Param : startnode
 * Param : distance an allocated array to store the distance between the startnode and the others
 */
int *dijkstra(struct graph_t *g,enum color_t id,int n,int startnode,int *distance);

/**
 * Function that returns an empty strategy allocated
 */
struct strategy *strategy__empty();

/**
 * Function that returns if there exists a neighbour to i in set s
 */
int has__neighboor_in_set(struct graph_t *g,struct set*s,size_t i);

/**
 * Function that intializes the sides from the graph 'g' in the strategy 's'
 */
void init__strategy_border(struct strategy *s,struct graph_t *g);

/**
 * Function that calculates the depth from the node 'start' to the node 'end'
 * Param : pred an array of predecessors generated from DIJSKTRA
 */
int depth(int *pred,int start,int end);

/**
 * Function that returns the nearest node to stratnode in the set s
 * Param : pred an array of predecessors generated from DIJSKTRA
 */
int short_node_from_set(int *pred,int startnode, struct set *s);

/**
 * Function that returns the path from the startnode to the end based on predecessors stored on pred with DIJSKTRA
 */
struct set * path(int *pred,int startnode,int end);

/**
 * Function that returns the full path for the player stored in strategy 's' from his first side's board to the second side's board
 */
struct set *full_path(struct graph_t *g,struct strategy*s);

/**
 * Function that provides the next move for the player stored in the strategy s with shortest's path approach
 */

size_t strategy_play(struct strategy *s,struct graph_t *g);

/**
 * Function that returns an opening suggestion
 */
int opening(struct graph_t *g,struct strategy *s);

/**
 * Function that closes and frees the strategy 's'
 */
void close_and_free__strategy(struct strategy* s);

#endif //_STRATEGIE_H_
