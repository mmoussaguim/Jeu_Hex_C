#include <stdlib.h>
#ifndef _SET_H_  // Type set variable - Pas d'allocation dynamique - Allocation par le client
#define _SET_H_

struct set;

struct set* set__empty(int (*cmp)(const void*, const void*),void * (*copy)(void const *),void (*del)(void *)); // allocates a set
int set__is_empty(const struct set*s);
int set__add(struct set *s, void *);     //Not enough space -- Already in
int set__remove(struct set *s, void *);  
size_t set__size(const struct set *s);
int set__find(const struct set *s,void *);
struct set* set__intersection(struct set*a, struct set*b);
struct set* set__union(struct set*a, struct set*b);
void set__add_to_set(struct set*a,struct set*b); // a = a union b
void set__display(struct set*s);
void set__free(struct set*s);

void set__crs_start(struct set*);
void set__crs_next(struct set*);
int set__crs_has_next(const struct set*);
void* set__crs_data(const struct set*); 

struct set * set__filter(const struct set *s,int (*p)(void *));
void set__debug_data(const struct set *s, void (*p)(void *));
#endif //_SET_H
