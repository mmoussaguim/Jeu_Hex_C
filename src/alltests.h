#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dlfcn.h>
#include "graph_function.h"
#include "player.h"

#define TESTCASE(value, expected) (value == expected ? "PASSED" : "FAILED")

void test_graph();
void test_player1();
void test_player2();
void test_player3();
void test_stack();
void test_server();
