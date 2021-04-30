#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "set_sentinel.h"

void * copy (void const *a)
{
  int *c=malloc(sizeof(int));
  *c=*(int *)a;
  return c;
}

void delete (void *a)
{
  int * i = (int *)a;
  free(i);
}

int compare(const void* a,const void *b)
{
  return (*(int *)a - *(int *)b);
}

// returns the expected position of c in sorted array s
// the returned value p is such that either s[p]==c,
// or c should be inserted in position p in s
size_t find(void * const s[], void *c,int (*cmp)(const void*, const void*))
{
  size_t i=0;
  while (s[i]!=SET__BOUND && cmp(s[i],c)<0)
    {
      i++;
    }
  return i;
}
 
// translates elements in s[begin..end] to s[begin+1..end+1]
// assuming all indices are valid (with s[end]=SENTINEL)
// s[begin] is left unchanged
void shift_right(void *s[], int begin, int end)
{
  for (int i=end;i>=begin;i--)
    {
      s[i+1]=s[i];
    }
}

// translates elements in s[begin..end] tp [begin-1..end-1]
// translates elements in s[begin..end] to [begin-1..end-1]
// assuming all indices are valid (with s[end]=SENTINEL)
// s[end] is left unchanged
void shift_left(void * s[], int begin)
{
  while(s[begin]!=SET__BOUND)
    {
      s[begin-1]=s[begin];
      begin++;
    }
    s[begin-1]=s[begin];
}

struct set* set__empty(int (*cmp)(const void*, const void*),
void * (*copy)(void const *),void (*del)(void *)) // allocates a set
{
  struct set *empty=malloc(sizeof(struct set));
  empty->s[0]=SET__BOUND;
  empty->cmp=cmp;
  empty->copy=copy;
  empty->del=del;
  return empty;
}

int set__is_empty(const struct set*s)
{
  if(s->s[0]==SET__BOUND)
    return 1;
  return 0;
}

int set__add(struct set *s, void *i)      //Not enough space -- Already in
{
  int a=find(s->s,i,s->cmp);
  int end=0;
  while (s->s[end]!=SET__BOUND)
    end++;
  if (end>=SET__SIZE-1 || set__find(s,i))//|| i<0 )
    return 1;
  else 
    {
      shift_right(s->s,a,end);
      s->s[a]=s->copy(i);
    }
  return 0;
}

int set__remove(struct set *s, void *i)
{
  if (set__find(s,i))
  {
    int a=find(s->s,i,s->cmp);
    s->del(s->s[a]);
    shift_left(s->s,a+1);
    return 0;
  }
  return 1;
}

size_t set__size(const struct set *s)
{
  size_t size=0;
  while(s->s[size]!=SET__BOUND)
    size++;
  return size;
}

int set__find(const struct set *s,void *i)
{
  for (int j=0;s->s[j]!=SET__BOUND ;j++)
  {
    if(s->cmp(s->s[j],i)==0)
      return 1;
    if (s->cmp(s->s[j], i) > 0)
      return 0;
  }
  return 0;
  /*size_t a=find(s->s,i);
  printf("%d\n",a);
  if (set__is_empty(s))
    return 0;
  if( a <= SET__SIZE -1 && compare(s->s[a],i)==0 )
    return 1;
  return 0;*/
}

void set__free(struct set*s)
{
  size_t size=set__size(s);
  for (size_t i=0; i<size; i++)
  {
    s->del(s->s[i]);
  }
  free(s);
}
  
struct set* set__union(struct set*a, struct set*b)
{
  int *v;
  struct set* empty=set__empty(compare,copy,delete);
  if(!set__is_empty(a))
  {
    set__crs_start(a);
    v=set__crs_data(a);
    set__add(empty,v);
    while(set__crs_has_next(a))
    {
      set__crs_next(a);
      v = set__crs_data(a);
      set__add(empty,v);
    }
  }
  if(!set__is_empty(b))
  {
    set__crs_start(b);
    v =set__crs_data(b);
    set__add(empty,v);
    while(set__crs_has_next(b))
    {
      set__crs_next(b);
      v = set__crs_data(b);
      set__add(empty,v);
    }
  }
  return empty;
}

struct set*set__intersection(struct set* a,struct set *b)
{
  struct set* c = set__empty(compare,copy,delete);
  set__crs_start(a);
  int *i = set__crs_data(a);
  while(set__crs_has_next(a))
  {
    if(set__find(b,i))
    {
      set__add(c,i);
    }
    set__crs_next(a);
    i = set__crs_data(a);
  }
  if(set__find(b,i))
  {
    set__add(c,i);
  }
  return c;
}

void set__display(struct set *s)
{
  int *c;
  if (!set__is_empty(s))
  {
    set__crs_start(s);
    while (set__crs_has_next(s))
    {
      c = set__crs_data(s);
      printf("%d | ",*c);
      set__crs_next((struct set*)s);
    }
    c = set__crs_data(s);
    printf("%d | -\n",*c);
  }
  else
  {
    printf("Set is empty !\n");
  }
}


//////////////////////// CUURSEUR /////////////////////////////

void set__crs_start(struct set*s)
{
  s->curseur=0;
}

void set__crs_next(struct set*s)
{
  s->curseur++;
}

int set__crs_has_next(const struct set*s)
{
  int i=s->curseur+1;
  return ((i < SET__SIZE) && ( s->s[i] != SET__BOUND));
}

void* set__crs_data(const struct set*s)
{
  return s->s[s->curseur];
}