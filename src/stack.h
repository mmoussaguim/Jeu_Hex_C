#ifndef __STACK_H
#define __STACK_H

#include <stdlib.h>

#define SUCCESS 0
#define FAILURE 1

struct stack;

/**
 * Function that create an empty stack
 */
struct stack * stack__empty();


/**
 * Function that return true if the stack is empty and false otherwise
 */
int stack__is_empty(struct stack *);

/**
 * Function that push a number in the stack
 */
int stack__push(struct stack *, size_t);

/**
 * Function that free the top of the stack and return its value
 */
size_t stack__pop(struct stack *);

/**
 * Function that free the stack
 */
void stack__free(struct stack *);

#endif
