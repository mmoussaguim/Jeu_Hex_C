#include <stdio.h>
#include <stdlib.h>
#include "../graph_function.h"
#include "set_sentinel.h"

#define INFINI 9999

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

struct eval_strat;

int *dijkstra(struct graph_t *g,enum color_t id,int n,int startnode,int *distance)
{
    // PLUS COURT CHEMIN AVEC DES SOMMETS VIDES ET DES SOMMETS DE COULEURS ID
    int size = graph__size(g);
    int * pred = malloc(sizeof(int)*size);
    int cost[size][size];
    int visited[size],count,mindistance,nextnode,i,j;
    
    //pred[] stores the predecessor of each node
    //count gives the number of nodes seen so far
    //create the cost matrix
    for(i=0;i<size;i++)
        for(j=0;j<size;j++)
            if(!graph__edge(g,i,j))
                cost[i][j]=INFINI;
            else
            {
                if(graph__vertex_has_color((id+1)%2,g,i) || graph__vertex_has_color((id+1)%2,g,j))
                    {
                        cost[i][j] = INFINI;
                    }
                else if(graph__vertex_has_color(id,g,i) && graph__vertex_has_color(id,g,j))
                    {
                        cost[i][j] = 0;
                    }
                else if(graph__vertex_has_color(id,g,j) && !graph__vertex_colored(g,i))
                    cost[i][j] = 0;
                else
                    cost[i][j] = 1;
            }
    
    //initialize pred[],distance[] and visited[]
    for(i=0;i<n;i++)
    {
        *(distance+i)=cost[startnode][i];
        *(pred+i) = startnode;
        visited[i]=0;
    }
    
    *(distance+startnode)=0;
    visited[startnode]=1;
    count=1;
    
    while(count<size-1)
    {
        mindistance=INFINI;
        
        //nextnode gives the node at minimum distance
        for(i=0;i<size;i++)
            {
                if(*(distance+i)<mindistance && !visited[i])
                {
                    mindistance=*(distance+i);
                    nextnode=i;
                }
            }
            
            //check if a better path exists through nextnode            
            visited[nextnode]=1;
            for(i=0;i<size;i++)
                if(!visited[i])
                    if(mindistance+cost[nextnode][i]<*(distance+i))
                    {
                        *(distance+i)=mindistance+cost[nextnode][i];
                        *(pred+i)=nextnode;
                    }
        count++;
    }
    return pred;
}

struct set *neighbours(struct graph_t *g,size_t position)
{
    struct set *neighbours = set__empty(compare,copy,delete);
    for(size_t i=0; i<graph__size(g); i++)
    {
        if(graph__edge(g,i,position) && !graph__vertex_colored(g,i))
            set__add(neighbours,&i);
    }
    return neighbours;
}

int minimax(size_t position, int depth, int alpha, int beta, int maximizingPlayer,struct graph_t *g,enum color_t id,struct eval_strat *e, int (*evaluate_position)(struct graph_t*,struct eval_strat*,enum color_t))
{
    if (depth == 0) //|| game over in position)
        return evaluate_position(g,e,id); //To implement

    int *child,eval,maxEval,minEval;
    struct set *nb = neighbours(g,position); //neighbours connected

    if(set__is_empty(nb))
    {
        set__free(nb);
        return evaluate_position(g,e,id);
    }

    if (maximizingPlayer == 1)
        {   
            maxEval = -INFINI;
            set__crs_start(nb);
            child = set__crs_data(nb);
            graph__colors_vertex(g,id,*child);
            eval = minimax(*child, depth - 1, alpha, beta, 0 ,g,id,e,evaluate_position);
            graph__remove_color_vertex(g,id,*child);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            while(set__crs_has_next(nb)) //for (each child of position)
            {
                set__crs_next(nb);
                child = set__crs_data(nb);
                graph__colors_vertex(g,id,*child);
                eval = minimax(*child, depth - 1, alpha, beta, 0 ,g,id,e,evaluate_position);
                graph__remove_color_vertex(g,id,*child);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha)
                    {
                        break;
                    }
            }
            set__free(nb);
            return maxEval;
        }
    else
        {
            minEval = INFINI;
            nb = neighbours(g,position);
            set__crs_start(nb);
            child = set__crs_data(nb);
            graph__colors_vertex(g,(id+1)%2,*child);
            eval = minimax(*child, depth - 1, alpha, beta, 0 ,g,id,e,evaluate_position);
            graph__remove_color_vertex(g,(id+1)%2,*child);
            minEval = max(minEval, eval);
            beta = min(beta, eval);
            while(set__crs_has_next(nb)) //(each child of position)
            {
                set__crs_next(nb);
                child = set__crs_data(nb);
                graph__colors_vertex(g,(id+1)%2,*child);
                eval = minimax(*child, depth - 1, alpha, beta ,1,g,id,e,evaluate_position);
                graph__remove_color_vertex(g,(id+1)%2,*child);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha)
                {
                    break;
                }
            }
            set__free(nb);
            return minEval;
        }
}

struct set *path__vertices(struct graph_t *g,enum color_t id,size_t u, size_t v)
{
    struct set *s = set__empty(compare,copy,delete);
    size_t size = graph__size(g);
    int dist[size];
    int *dj = dijkstra(g,id,graph__size(g),v,&dist[0]);
    set__add(s,&u);
    set__add(s,&v);
    while(dj[u] != (int) v)
    {
        u = dj[u];
        set__add(s,&u);
    }
    free(dj);
    return s;
}

int aux(struct graph_t *g,struct set *s,int vertex,enum color_t id)
{
    int ret;
    size_t size = graph__size(g);
    int dist[size];
    int *dj = dijkstra(g,id,graph__size(g),vertex,&dist[0]);
    int *a;
    if(!set__is_empty(s))
    {
        set__crs_start(s);
        while(set__crs_has_next(s))
        {
            a = set__crs_data(s);
            if(!graph__vertex_colored(g,*a) && dist[*a] == 1)
                {
                    free(dj);
                    ret = *a;
                    return ret;
                }
            set__crs_next(s);
        }
        a = set__crs_data(s);
        if(!graph__vertex_colored(g,*a) && dist[*a] == 1)
            {
                free(dj);
                ret = *a;
                return ret;
            }
    }
    free(dj);
    return INFINI;
}

int two_distance(struct graph_t *g, enum color_t id,size_t u,size_t v)
{
    if(graph__edge(g,u,v))
        return 1;
    struct set *nb;
    size_t size = graph__size(g);
    int dist[size],*dj;
    dj = dijkstra(g,id,graph__size(g),v,&dist[0]);
    nb = path__vertices(g,id,u,v);
    size_t u1 = aux(g,nb,u,id);
    if( u1 == v )
    {
        set__free(nb);
        free(dj);
        return 0;
    }
    if(u1 == INFINI )
    {
        set__free(nb);
        free(dj);
        return 1;
    }
    set__free(nb);
    graph__colors_vertex(g,(id+1)%2,u1);
    free(dj);
    dj = dijkstra(g,id,graph__size(g),v,&dist[0]);
    nb = path__vertices(g,id,u,v);
    size_t u2 = aux(g,nb,u,id);
    set__free(nb);
    free(dj);
    if(u2 == INFINI)
        return 1;    
    return 1 + two_distance(g,id,u2,v);
}
