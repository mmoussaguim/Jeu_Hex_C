#ifndef _GRAPH_FUNCTION_H_
#define _GRAPH_FUNCTION_H_

#include "move.h"
#include "graph.h"
#define BLEUCLAIR "\033[01;34m"
#define ROUGE "\033[00;31m"
#define BLANC "\033[37m"
#define BLEU "\033[00;34m"
#define NORMAL "\033[00m"

/**
 * Function that provide the number of vertices of the graph given in parameter
 */
size_t graph__size(struct graph_t * graph);

/**
 * Function that returns the width of the graph given in parameter
 */
size_t graph__width(struct graph_t *graph,char t);

/**
 * Function that free the memory allocated
 * Returns 0 if there is no probleme
 */

int graph__free(struct graph_t *);

/**
 * Function that return 1 if there is an edge between vertex i and j in the graph; 0 else
 */
int graph__edge(struct graph_t * graph, size_t i, size_t j);

/**
 * Function that verify if the vertex is colored
 * Returns 1 if it's colored, 0 else
 */
int graph__vertex_colored(struct graph_t * graph, size_t i);

/**
 * Function that verify if the vertex i has color id
 * Returns 1 if it's colored with id, 0 else
 */
int graph__vertex_has_color(enum color_t id,struct graph_t * graph, size_t i);

/**
 * Function which colors the vertex i for the player id
 */
void graph__colors_vertex(struct graph_t * graph,enum color_t id , size_t i);


void graph__remove_color_vertex(struct graph_t * graph ,enum color_t id , size_t i);


/**
 * Function that verify if the graph is full
 * Returns 1 if it's full, 0 else
 */
int graph__is__full(struct graph_t * graph);
/**
 * Function that create a copy of the graph given in parameter
 * Returns a new pointer allocated for the graph copied
 */
struct graph_t *graph__copy(struct graph_t *graph);

/**
 * Function that initialize a graph (Creates the edge depending of the type given)
 * Param : a graph
 * Param : a letter describing the type of the graph
 * ---> t : for triangular graph
 * ---> c : for square qraph
 * ---> h : for hexagonale graph
 */
struct graph_t * graph__initialize(size_t size,char t);

/**
 * Function that display a graph given in parameter
 */
void graph__display(struct graph_t *);

void graph__print(struct graph_t *graph, char t);

#endif // _GRAPH_FUNCTION_H_
