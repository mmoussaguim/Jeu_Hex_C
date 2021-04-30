#include "strategie.h"
#define INFINI 9999
#define MAX 10

struct strategy *strategy__empty()
{
    struct strategy *empty = malloc(sizeof(struct strategy));
    return empty;
}

int has__neighboor_in_set(struct graph_t *g,struct set*s,size_t i)
{
    set__crs_start(s);
    int *a = set__crs_data(s);
    if(graph__edge(g,i,*a))
    {
        return 1;
    }
    while(set__crs_has_next(s))
    {
        set__crs_next(s);
        a = set__crs_data(s);
        if(graph__edge(g,i,*a))
            return 1;       
    }
    return 0;
}

void init__strategy_border(struct strategy *s,struct graph_t *g)
{
    struct set *set_vertices;
    int *a,b;
    for(int c=0; c<2; c++)
    {
        set_vertices = set__empty(compare,copy,delete);
        size_t vertices = graph__size(g);
        for (size_t i=0; i<vertices; i++)
        {
            if(graph__vertex_has_color(c,g,i))
                set__add(set_vertices,&i);
        }
        size_t border_size = set__size(set_vertices)/2;
        s->first__border[c] = set__empty(compare,copy,delete);
        set__crs_start(set_vertices);
        a = set__crs_data(set_vertices);
        b = *a;
        set__add(s->first__border[c],&b);
        set__remove(set_vertices,&b);
        while(set__size(set_vertices) != border_size)
        {
            set__crs_start(set_vertices);
            int *v = set__crs_data(set_vertices);
            if(has__neighboor_in_set(g,s->first__border[c],*v))
                {
                    set__add(s->first__border[c],v);
                    set__remove(set_vertices,v);
                }
            while(set__crs_has_next(set_vertices))
            {
                set__crs_next(set_vertices);
                v = set__crs_data(set_vertices);
                if(has__neighboor_in_set(g,s->first__border[c],*v))
                {
                    set__add(s->first__border[c],v);
                    set__remove(set_vertices,v);
                }
            }
        }
        s->second__border[c] = set_vertices;
    }
}

int depth(int *pred,int start,int end)
{
    int d = 0;
    while(pred[end] != start)
    {
        d++;
        end = pred[end];
    }
    return d;
}

int short_node_from_set(int *pred,int startnode, struct set *s)
{
    set__crs_start(s);
    int newdep;
    int *c;
    int *vertex = set__crs_data(s);
    int dep = depth(pred,startnode,*vertex);
    while(set__crs_has_next(s))
    {
        set__crs_next(s);
        c = set__crs_data(s);
        newdep = depth(pred,startnode,*c);
        if (newdep < dep)
        {
            *vertex = *c;
            dep = newdep;
        }
    }
    return *vertex;
}

struct set * path(int *pred,int startnode,int end)
{
    struct set *path = set__empty(compare,copy,delete);
    set__add(path,&startnode);
    set__add(path,&end);
    while(pred[end] != startnode)
    {
        end = pred[end];
        set__add(path,&end);
    }
    return path;
}

struct set *full_path(struct graph_t *g,struct strategy*s)
{
    int *dist = malloc(sizeof(int)*graph__size(g));
    int *pred = dijkstra(g,s->id,graph__size(g),s->my_pm,dist);
    int shortend = short_node_from_set(pred,s->my_pm,s->first__border[s->id]);
    struct set* u = path(pred,s->my_pm,shortend);
    int shortend2 = short_node_from_set(pred,s->my_pm,s->second__border[s->id]);
    struct set* t = path(pred,s->my_pm,shortend2);
    struct set * path = set__union(u,t);
    free(pred);
    free(dist);
    set__free(u);
    set__free(t);
    return path;
}

size_t strategy_play(struct strategy *s,struct graph_t *g)
{
    struct set * path = full_path(g,s);
    struct set * tmp_path;
    int tmp_dep;
    int depth = set__size(path);
    set__crs_start(path);
    int *c = set__crs_data(path);
    if(!graph__vertex_colored(g,*c))
    {
        graph__colors_vertex(g,2%(s->id+1),*c);
        tmp_path = full_path(g,s);
        tmp_dep = set__size(tmp_path);
        set__free(tmp_path);
        graph__remove_color_vertex(g,2%(s->id+1),*c);
        if(tmp_dep > depth)
        {
            size_t ret =(size_t) *c;
            set__free(path);
            return ret;
        }
    }
    while(set__crs_has_next(path))
    {
        set__crs_next(path);
        c = set__crs_data(path);
        if(!graph__vertex_colored(g,*c))
        {
            graph__colors_vertex(g,2%(s->id+1),*c);
            tmp_path = full_path(g,s);
            tmp_dep = set__size(tmp_path);
            set__free(tmp_path);
            graph__remove_color_vertex(g,2%(s->id+1),*c);
            if(tmp_dep > depth)
            {            
                size_t ret =(size_t) *c;
                set__free(path);
                return ret;
            }
        }
    }
    set__crs_start(path);
    c = set__crs_data(path);
    if(!graph__vertex_colored(g,*c))
    {
        size_t ret =(size_t) *c;
        set__free(path);
        return ret;
    }
    while(set__crs_has_next(path))
    {
        set__crs_next(path);
        c = set__crs_data(path);
        if(!graph__vertex_colored(g,*c))
        {
            size_t ret = (size_t) *c;
            set__free(path);
            return ret;
        }
    }
    set__free(path);
    return rand()%graph__size(g);
}

void close_and_free__strategy(struct strategy* s)
{
    for(size_t i=0; i<2; i++)
    {
        set__free(s->first__border[i]);
        set__free(s->second__border[i]);
    }
    free(s);
}
