#include "set.h"
#ifndef SET__SIZE
#define SET__SIZE 30
#endif
#define SET__BOUND NULL

int compare(const void* a,const void *b);
void * copy (void const *a);
void delete (void *a);
// returns the expected position of c in sorted array s
// the returned value p is such that either s[p]==c,
// or c should be inserted in position p in s
size_t find(void * const s[], void *i,int (*cmp)(const void*, const void*));
// translates elements in s[begin..end] to s[begin+1..end+1]
// assuming all indices are valid (with s[end]=SENTINEL)
// s[begin] is left unchanged
void shift_right(void *s[], int begin, int end);

// translates elements in s[begin..end] tp [begin-1..end-1]
// translates elements in s[begin..end] to [begin-1..end-1]
// assuming all indices are valid (with s[end]=SENTINEL)
// s[end] is left unchanged
void shift_left(void *s[], int begin);

struct set {
  void* s[SET__SIZE];
  int curseur;
  int (*cmp)(const void*, const void*);
  void * (*copy)(void const *);
  void (*del)(void *);
};
