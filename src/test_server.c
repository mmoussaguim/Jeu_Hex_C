#include <assert.h>
#include <stdio.h>

#include "move.h"
#include "graph_function.h"
#include "player.h"
#include "server.h"
#include "stack.h"

static void test_is_winning(void)
{
  printf("%s", __func__);
  
  // Initialze the game
  size_t size = 5;
  char shape = 'c';
  struct game game__c;
  game__c.board = graph__initialize(size, shape);
  game__c.next = BLACK;

  struct move_t move = {size + 2, BLACK};

  // The game is not ended
  assert(!is_winning(&game__c, move, shape, size));

  // The second player play an already played move, this end the game
  move.c = WHITE;
  assert(is_winning(&game__c, move, shape, size));
  
  size_t end = (size + 1)*(size + 1) - 3*(size + 1) + 1;

  for (size_t i = 2*(size+1)+1; i<=end; i += size+1)
    graph__colors_vertex(game__c.board, BLACK, i);
  move.c = BLACK;
  move.m = end + size + 1;

  assert(is_winning(&game__c, move, shape, size)); // The black player has won
  graph__free(game__c.board);

  printf("\t\tOK\n");
}

void test_server(void)
{
  printf("%s\n", __func__);
  test_is_winning();
}
