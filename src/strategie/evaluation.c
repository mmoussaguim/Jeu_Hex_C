#include "evaluation.h"

struct eval_strat * init_eval_strat(struct graph_t *g)
{
    size_t size = graph__size(g);
    int dist[size];
    int *dj;
    struct eval_strat *empty = malloc(sizeof(struct eval_strat));
    for(int c=0;c<2;c++)
    {
        size_t i = 0;
        while(i<size && !graph__vertex_has_color(c,g,i))
            i++;
        empty->first_side[c] = i;
        i = 0;
        dj = dijkstra(g,c,size,empty->first_side[c],&dist[0]);
        while(i<size)
        {
            if(graph__vertex_has_color(c,g,i) && dist[i] != 0)
                {
                    empty->second_side[c] = i;
                    free(dj);
                    break;
                }
            i++;
        }
    }
    return empty;
}

void eval__strat_free(struct eval_strat *e)
{
    free(e);
}

int distance(struct graph_t *g,int startnode,int endnode,int *pred)
{
    int d = 0;
    while(pred[endnode] != startnode)
    {
        if(!graph__vertex_colored(g,endnode))
                d++;
        endnode = pred[endnode];
    }
    return d;
}

int graph__evaluation(struct graph_t *g,struct eval_strat *e,enum color_t id)
{
    struct graph_t *graph = graph__copy(g);
    int ret = two_distance(graph,id,e->first_side[id],e->second_side[id]);
      //- two_distance(graph,(id+1)%2,e->first_side[(id+1)%2],e->second_side[(id+1)%2]);
    graph__free(graph);
    return ret;
}

struct set *path_to_vertex(struct graph_t *g,enum color_t id,int i,int j,int vertex)
{
    int a = i;
    struct set *s = set__empty(compare,copy,delete);
    set__add(s,&a);
    set__add(s,&j);
    set__add(s,&vertex);
    size_t size = graph__size(g);
    int dist[size];
    int *dj = dijkstra(g,id,graph__size(g),vertex,&dist[0]);
    while(dj[a] != vertex)
    {
        a = dj[a];
        set__add(s,&a);
    }
    a = j;
    while(dj[a] != vertex)
    {
        a = dj[a];
        set__add(s,&a);
    }
    free(dj);
    return s;
}

size_t minmax_to_play(struct graph_t *g,struct set* path,struct eval_strat *e,enum color_t id, size_t pm,int depth)
{
    set__crs_start(path);
    int *a = set__crs_data(path);
    int c = *a;
    set__free(path);
    path = path_to_vertex(g,id,e->first_side[id],e->second_side[id],c);
    struct set* op_path = path_to_vertex(g,(id+1)%2,e->first_side[(id+1)%2],e->second_side[(id+1)%2],pm);
    int ret,score_i,score_a;
    score_i = INFINI;
    struct set * s = set__intersection(path,op_path);
    set__free(op_path);
    set__free(path);
    if(!set__is_empty(s))
    {
        set__crs_start(s);
        a = set__crs_data(s);
        ret = *a;
        if(!graph__vertex_colored(g,ret))
            score_i = minimax(ret,depth,-INFINI,INFINI,1,g,id,e,graph__evaluation);
        while(set__crs_has_next(s))
        {
            set__crs_next(s);
            a = set__crs_data(s);
            if(!graph__vertex_colored(g,*a))
            {
                score_a = minimax(*a,depth,-INFINI,INFINI,1,g,id,e,graph__evaluation);
                if(score_a < score_i)
                {
                    ret = *a;
                    score_i = score_a;
                }
            }
        }
    }
    else if(set__is_empty(s) || score_i == INFINI)
    {
        ret = rand()%graph__size(g);
        while(graph__vertex_colored(g,ret))
        {
            ret = rand()%graph__size(g);
        }
    }
    set__free(s);
    return (size_t) ret;
}