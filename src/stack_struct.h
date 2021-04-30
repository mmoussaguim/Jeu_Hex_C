#ifndef __STACK_STRUCT_H
#define __STACK_STRUCT_H

#include "stack.h"

struct stack {
  struct stack__element * top;
};

struct stack__element {
  size_t data;
  struct stack__element * next;
};
  
#endif
