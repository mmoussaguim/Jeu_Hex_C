#include <assert.h>
#include <stdio.h>

#include "stack_struct.h"

static void test_stack__empty(void)
{
  printf("%s", __func__);

  struct stack * s = stack__empty();

  assert(s != NULL);
  assert(s->top == NULL);

  stack__free(s);
  
  printf("\tOK\n");
}

static void test_stack__is_empty(void)
{
  printf("%s", __func__);

  struct stack * s = stack__empty();

  assert(stack__is_empty(s));

  struct stack__element *e = malloc(sizeof(struct stack__element));
  e->next = NULL;
  e->data = 1;
  s->top = e;

  assert(!stack__is_empty(s));

  stack__free(s);

  printf("\tOK\n");
}

static void test_stack__push(void)
{
  printf("%s", __func__);

  struct stack * s = stack__empty();

  assert(stack__push(s, 1) == SUCCESS);
  assert(stack__push(s, 2) == SUCCESS);
  assert(stack__push(s, 3) == SUCCESS);
  assert(stack__push(s, 6) == SUCCESS);
  assert(stack__push(s, 2) == SUCCESS);

  assert(s->top->data == 2);
  assert(s->top->next->data == 6);
  assert(s->top->next->next->data == 3);
  assert(s->top->next->next->next->data == 2);
  assert(s->top->next->next->next->next->data == 1);

  stack__free(s);

  printf("\tOK\n");
}

static void test_stack__pop(void)
{
  printf("%s", __func__);

  struct stack * s = stack__empty();

  assert(stack__push(s, 1) == SUCCESS);
  assert(stack__push(s, 2) == SUCCESS);
  assert(stack__push(s, 3) == SUCCESS);
  assert(stack__push(s, 6) == SUCCESS);
  assert(stack__push(s, 2) == SUCCESS);

  assert(s->top->data == 2);
  assert(s->top->next->data == 6);
  assert(s->top->next->next->data == 3);
  assert(s->top->next->next->next->data == 2);
  assert(s->top->next->next->next->next->data == 1);

  assert(stack__pop(s) == 2);
  assert(stack__pop(s) == 6);
  assert(stack__pop(s) == 3);
  assert(stack__pop(s) == 2);
  assert(stack__pop(s) == 1);

  stack__free(s);

  printf("\t\tOK\n");
}


void test_stack(void)
{
  printf("%s\n", __func__);

  test_stack__empty();
  test_stack__is_empty();
  test_stack__push();
  test_stack__pop();
}
/*
int main(void)
{
  test_stack();
  return 0;
}
*/
