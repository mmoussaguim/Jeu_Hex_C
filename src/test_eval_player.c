#include "alltests.h"
#include "server.h"

void test_player3()
{
  struct player *p = malloc(sizeof(*p));
  player__from_library("./install/eval_player.so", p);
  printf("\nTests on eval_player\n");
  srand(time(NULL));
  char const* name = p->get_player_name();
  printf("\nPlayer Name : %s; %s\n", name, TESTCASE(strcmp(name, "PoorQueenbee"), 0));
  struct graph_t * g1 = graph__initialize(3,'h');
  p->initialize_graph(g1);
  graph__display(g1);
  struct move_t mv = p->propose_opening();
  p->initialize_color(BLACK);
  
  printf("\nHe can propose %zu as move\n", mv.m);
  printf("\nHe will always refuse the opening? %d\n", !p->accept_opening(mv));
  p->finalize();
  dlclose(p->lib);
  free(p);
}