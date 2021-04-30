#include "alltests.h"

int test__edge(struct graph_t *g, size_t i, size_t j)
{
  if (TESTCASE(graph__edge(g,i,j),1))
    return 1;
  else
  {
      printf("FAILED : An edge expected between %ld and %ld\n",i,j);
      return 0;
  }
}

int test__no_edge(struct graph_t *g, size_t i, size_t j)
{
  if (TESTCASE(graph__edge(g,i,j),0))
    return 1;
  else
  {
      printf("FAILED : No edge expected between %ld and %ld\n",i,j);
      return 0;
  }
}

int test__vertex_colored(struct graph_t *g,enum color_t id,size_t i)
{
  if(id == BLACK)
  {
    if(TESTCASE(graph__vertex_has_color(id,g,i),1))
    {
      return 1;
    }
    else
    {
      printf("FAILED : Vertex %ld should be BLACK",i);
    }
  }
  if(id == WHITE)
  {
    if(TESTCASE(graph__vertex_has_color(id,g,i),1))
    {
      return 1;
    }
    else
    {
      printf("FAILED : Vertex %ld should be WHITE",i);
    }
  }
  return 0;
}

void test__graph_hexagonal()
{
  struct graph_t * g1 = graph__initialize(8,'h');
  printf("\nHexagonal Graph\n");
  int test = 0;
  test = test + test__no_edge(g1,0,10) + test__no_edge(g1,8,9)
   + test__no_edge(g1,70,80) + test__edge(g1,8,17) 
   + test__edge(g1,80,71) + test__edge(g1,79,71);
  if (test == 6)
    printf("Edge test : PASSED\n");
  test = 
  test__vertex_colored(g1,BLACK,0) + test__vertex_colored(g1,BLACK,7) 
  + test__vertex_colored(g1,BLACK,80) + test__vertex_colored(g1,BLACK,73) 
  + test__vertex_colored(g1,BLACK,76) + test__vertex_colored(g1,WHITE,8) 
  + test__vertex_colored(g1,WHITE,9) + test__vertex_colored(g1,WHITE,17) 
  + test__vertex_colored(g1,WHITE,72) + test__vertex_colored(g1,WHITE,71);
  if (test == 10)
    printf("Vertices color test : PASSED\n");
  graph__print(g1, 'h');
  graph__free(g1);
}

void test__graph_square()
{
  struct graph_t * g2 = graph__initialize(7,'c');
  printf("\nSquare Graph\n");
  int test = 0;
  test = test + test__no_edge(g2,0,5) + test__no_edge(g2,7,8)
   + test__no_edge(g2,63,54) + test__edge(g2,8,0) 
   + test__edge(g2,56,48) + test__edge(g2,63,55);
  if (test == 6)
    printf("Edge test : PASSED\n");
  test = test__vertex_colored(g2,BLACK,0) + test__vertex_colored(g2,BLACK,3) + test__vertex_colored(g2,BLACK,6)
   + test__vertex_colored(g2,BLACK,63) + test__vertex_colored(g2,BLACK,57) + test__vertex_colored(g2,WHITE,7) 
   + test__vertex_colored(g2,WHITE,8) + test__vertex_colored(g2,WHITE,15) + test__vertex_colored(g2,WHITE,55)
  + test__vertex_colored(g2,WHITE,56);
  if (test == 10)
    printf("Vertices color test : PASSED\n");
  graph__print(g2, 'c');
  graph__free(g2);
}

void test__graph_triangular()
{
  printf("\nTriangular Graph\n");
  struct graph_t * g3 = graph__initialize(4,'t');
    int test = 0;
  test = test + test__no_edge(g3,20,21) + test__no_edge(g3,20,22)
   + test__no_edge(g3,12,19) + test__no_edge(g3,0,5)
   + test__no_edge(g3,7,13) + test__edge(g3,19,27) 
   + test__edge(g3,11,18) + test__edge(g3,94,95)  
   + test__edge(g3,90,91) + test__edge(g3,91,94);
  if (test == 10)
    printf("Edge test : PASSED\n");
  test = 
  test__vertex_colored(g3,BLACK,0) + test__vertex_colored(g3,BLACK,1) 
  + test__vertex_colored(g3,BLACK,2) + test__vertex_colored(g3,BLACK,3) 
  + test__vertex_colored(g3,BLACK,4) + test__vertex_colored(g3,BLACK,5) 
  + test__vertex_colored(g3,BLACK,6) + test__vertex_colored(g3,BLACK,7) 
  + test__vertex_colored(g3,BLACK,12) + test__vertex_colored(g3,BLACK,13) 
  + test__vertex_colored(g3,BLACK,88) + test__vertex_colored(g3,BLACK,89)
  + test__vertex_colored(g3,BLACK,90) + test__vertex_colored(g3,BLACK,91) 
  + test__vertex_colored(g3,BLACK,92) + test__vertex_colored(g3,BLACK,93) 
  + test__vertex_colored(g3,BLACK,94) + test__vertex_colored(g3,BLACK,95) 
  + test__vertex_colored(g3,BLACK,83) + test__vertex_colored(g3,BLACK,82)  
  + test__vertex_colored(g3,WHITE,11) + test__vertex_colored(g3,WHITE,18) 
  + test__vertex_colored(g3,WHITE,19) + test__vertex_colored(g3,WHITE,20) 
  + test__vertex_colored(g3,WHITE,27) + test__vertex_colored(g3,WHITE,28)
  + test__vertex_colored(g3,WHITE,35) + test__vertex_colored(g3,WHITE,36) 
  + test__vertex_colored(g3,WHITE,43) + test__vertex_colored(g3,WHITE,44) 
  + test__vertex_colored(g3,WHITE,51) + test__vertex_colored(g3,WHITE,52)  
  + test__vertex_colored(g3,WHITE,59) + test__vertex_colored(g3,WHITE,60) 
  + test__vertex_colored(g3,WHITE,67) + test__vertex_colored(g3,WHITE,68) 
  + test__vertex_colored(g3,WHITE,75) + test__vertex_colored(g3,WHITE,76)
  + test__vertex_colored(g3,WHITE,77) + test__vertex_colored(g3,WHITE,84);
  if (test == 40)
    printf("Vertices color test : PASSED\n");
  graph__print(g3, 't');
  graph__free(g3);
}

void test_graph()
{
  test__graph_hexagonal();
  test__graph_square();
  test__graph_triangular();
}
