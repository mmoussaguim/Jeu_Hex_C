#include "stack_struct.h"

struct stack * stack__empty()
{
  struct stack * s = malloc(sizeof(struct stack));
  s->top = NULL;
  return s;
}

int stack__is_empty(struct stack *s)
{
  return s->top == NULL;
}

int stack__push(struct stack * s, size_t d)
{
  struct stack__element * e = malloc(sizeof(struct stack__element));
  if (e == NULL)
    return FAILURE;
  e->next = s->top;
  e->data = d;
  s->top = e;

  return SUCCESS;
}

size_t stack__pop(struct stack * s)
{
  struct stack__element * tmp = s->top;
  size_t d = tmp->data;
  s->top = tmp->next;
  free(tmp);
  return d;
}

void stack__free(struct stack * s)
{
  struct stack__element * tmp = s->top;
  struct stack__element * tmp2;
  while (tmp != NULL) {
    tmp2 = tmp->next;
    free(tmp);
    tmp = tmp2;
  }
  free(s);
}
