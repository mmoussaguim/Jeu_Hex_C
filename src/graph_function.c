#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "graph_function.h"

size_t graph__size(struct graph_t * graph)
{
    return graph->num_vertices;
}

int is_perfect_square(int a)
{
    if (a >= 0)
    {
        return (floor(sqrt(a)) == sqrt(a));
    }
    return 0;
}

size_t graph__width(struct graph_t *graph,char t)
{
    switch(t){
    case 'h':
        {
            size_t width = graph__size(graph);
            if (is_perfect_square(width)){
            return sqrt(graph__size(graph)) - 1;
            }
            break;
        }
    case 'c':
        {
            size_t width = graph__size(graph);
            if (is_perfect_square(width)){
            return sqrt(graph__size(graph)) - 1;
            }
            break;
        }
    case 't':
        {
            size_t width = graph__size(graph)/6;
            if (is_perfect_square(width)){
            return sqrt(width);
            }
            break;
        }
    default:
        {
            return 0;
        }
    }
    return 0;
}

int graph__free(struct graph_t *graph)
{
    if (graph == NULL)
        return 1;
    gsl_spmatrix_free(graph->t);
    gsl_spmatrix_free(graph->o);
    free(graph);
    return 0;
}

int graph__edge(struct graph_t * graph, size_t i, size_t j)
{
  gsl_spmatrix *t = graph->t;
  return (gsl_spmatrix_get(t,i,j) == 1);
}

int graph__vertex_colored(struct graph_t * graph, size_t i)
{
    return (graph__vertex_has_color(0,graph,i) || graph__vertex_has_color(1,graph,i));
}

int graph__vertex_has_color(enum color_t id,struct graph_t *graph, size_t i)
{
    gsl_spmatrix *o = graph->o;
    return gsl_spmatrix_get(o,id,i);
}

void graph__colors_vertex(struct graph_t * graph ,enum color_t id , size_t i)
{
    gsl_spmatrix *o = graph->o;
    gsl_spmatrix_set(o,id,i,1);
}

void graph__remove_color_vertex(struct graph_t * graph ,enum color_t id , size_t i)
{
    gsl_spmatrix *o = graph->o;
    gsl_spmatrix_set(o,id,i,0);
}

int graph__is__full(struct graph_t * graph)
{
  size_t vertices = graph__size(graph);
  for(size_t i = 0; i < vertices; i++)
    {
      if(!graph__vertex_colored(graph, i))
	return 0;
    }
  return 1;
}

struct graph_t *graph__copy(struct graph_t *graph)
{
    struct graph_t *copy = malloc(sizeof(struct graph_t));
    size_t size = graph__size(graph);
    copy->num_vertices = size;
    copy->t = gsl_spmatrix_alloc(size,size);
    copy->o = gsl_spmatrix_alloc(2,size);
    gsl_spmatrix_memcpy(copy->t,graph->t);
    gsl_spmatrix_memcpy(copy->o,graph->o);
    return copy;

}

void graph__color_hex_square(struct graph_t *graph)
{
    size_t vertices = graph__size(graph);
    size_t width = graph__width(graph,'h');
    gsl_spmatrix *o = graph->o;
    for(size_t i=0; i < width ;i++)
        {
            gsl_spmatrix_set(o,BLACK,i,1);
            gsl_spmatrix_set(o,BLACK,vertices-i-1,1);
        }
    for(size_t i=width ; i < vertices-1 ; i= i + width + 1)
        {
            gsl_spmatrix_set(o,WHITE,i,1);
            gsl_spmatrix_set(o,WHITE,i+1,1);
        }
}

size_t index__first_in_line(struct graph_t *graph, size_t line)
{
    size_t width = graph__width(graph,'t');
    size_t size = graph__size(graph);
    size_t total_line = 4*width - 1;
    size_t nbr_vtx = 2*width;
    if (line < width)
    {
        return line*(line+1);
    }
    if ((width <= line ) && (line <= total_line - width))
    {
        return width*(width+1) + (line-width)*nbr_vtx;
    }
    else
    {
        return (size - (total_line-line)*(total_line-line+1));
    }    
}

void graph__color_triangular(struct graph_t* graph)
{
    size_t width = graph__width(graph,'t');
    size_t total_line = 4*width -1;
    gsl_spmatrix *o = graph->o;
    size_t index_end,index;
    for(size_t line=0 ; line < total_line ; line++)
    {
        index =  index__first_in_line(graph,line);
        if (line < width)
        {   
            index_end = index + (2*(line+1) -1);
            if((line > 0) && (line < width/2))
            {
                gsl_spmatrix_set(o,BLACK,index_end,1);    
                gsl_spmatrix_set(o,BLACK,index_end-1,1);    
            }
            if (line == width/2)
            {
                gsl_spmatrix_set(o,WHITE,index_end,1);
            }
            if (line > width/2)
            {
                gsl_spmatrix_set(o,WHITE,index_end,1);
                gsl_spmatrix_set(o,WHITE,index_end-1,1);    
            }
            gsl_spmatrix_set(o,BLACK,index,1);
            gsl_spmatrix_set(o,BLACK,index+1,1);
        }
        if (line >= width)
        {   
            index_end = index + 2*width-1;
            if (line <= total_line - width/2 - 1)
                gsl_spmatrix_set(o,WHITE,index,1);
            if (line < total_line - width)
                gsl_spmatrix_set(o,WHITE,index_end,1);
        }
        if ((line >= total_line - width) && (line < total_line - width/2 - 1))
        {
            gsl_spmatrix_set(o,WHITE,index+1,1);
        }
        if (line > total_line - width/2 -1)
        {
            gsl_spmatrix_set(o,BLACK,index,1);
            gsl_spmatrix_set(o,BLACK,index+1,1);
        }
        if (line >= total_line - width)
        {
            index_end = index + (2*(total_line-line) -1);
            gsl_spmatrix_set(o,BLACK,index_end-1,1);
            gsl_spmatrix_set(o,BLACK,index_end,1);
        }
    }
}

void graph__initialize_hexagonal(struct graph_t *graph)
{
    size_t size = graph__size(graph);
    graph->t = gsl_spmatrix_alloc(size,size);
    graph->o = gsl_spmatrix_alloc(2,size);
    size_t num_line = graph__width(graph,'h')+1;
    gsl_spmatrix *t = graph->t;
    gsl_spmatrix_set_zero(t);
    for(size_t i=0 ; i < size ; i++)
        {
            if( (i - num_line +1 > 0) && (i - num_line +1 < size) && ((i-num_line+1)%num_line != 0))
            {
                gsl_spmatrix_set(t,i-num_line+1,i,1);
                gsl_spmatrix_set(t,i,i-num_line+1,1);
            }
            if((i+1 < size) && ((i+1)%num_line != 0))
            {
                gsl_spmatrix_set(t,i,i+1,1);
                gsl_spmatrix_set(t,i+1,i,1);
            }
            if( i + num_line < size)
            {
                gsl_spmatrix_set(t,i,i+num_line,1);
                gsl_spmatrix_set(t,i+num_line,i,1);
            }
        }
    graph__color_hex_square(graph);
}

void graph__initialize_square(struct graph_t * graph)
{
    size_t size = graph__size(graph);
    graph->t = gsl_spmatrix_alloc(size,size);
    graph->o = gsl_spmatrix_alloc(2,size);
    size_t num_line = graph__width(graph,'c')+1;
    gsl_spmatrix *t = graph->t;
    gsl_spmatrix_set_zero(t);
    for(size_t i = 0; i < size; i++)
      {
	if((i+1 < size) && ((i+1)%num_line != 0))
	  {
	    gsl_spmatrix_set(t,i,i+1,1);
	    gsl_spmatrix_set(t,i+1,i,1);
	  }
	if(i + num_line < size)
	  {
	     gsl_spmatrix_set(t,i,i+num_line,1);
	     gsl_spmatrix_set(t,i+num_line,i,1);
	  }
      }
    graph__color_hex_square(graph);
}

void graph__initialize_triangular(struct graph_t *graph)
{
    size_t size = graph__size(graph);
    graph->t = gsl_spmatrix_alloc(size,size);
    graph->o = gsl_spmatrix_alloc(2,size);
    size_t width = graph__width(graph,'t');
    gsl_spmatrix *t = graph->t;
    gsl_spmatrix_set_zero(t);
    size_t i = 0;
    size_t a = 2,vtx_line = 2,line = 0;
    while(i < width*(width-1))
    {
        while(i < a)
        {
            if(i < a-1)
            {
                if (i%2 == 0)
                    gsl_spmatrix_set(t,i,i+1,1);
            }
            gsl_spmatrix_set(t,i,i+vtx_line+1,1);
            i++;
        }
        vtx_line = vtx_line + 2;
        a = i + vtx_line;
        line ++;
    }
    vtx_line = 2*width;
    a = i + vtx_line;
    while(i < size - (width*(width+1)))
    {
        while((i < a) && (i<size))
        {
            if(i < a-1)
            {
                if ((line%2 == 1) && (i%2 == 0))
                    gsl_spmatrix_set(t,i,i+1,1);
                else if((line%2 == 0) && (i%2 ==1))
                    gsl_spmatrix_set(t,i,i+1,1);
            }
            gsl_spmatrix_set(t,i,i+vtx_line,1);
            i++;
        }
        a = i + vtx_line;
        line ++;
    }
    while(i < size)
    {
        while(i < a-1)
        {
            if ((i%2 == 0))
                gsl_spmatrix_set(t,i,i+1,1);
            if( i != a - vtx_line)
            {
                gsl_spmatrix_set(t,i,i+vtx_line-1,1);
            }
            i++;
        }
        i++;
        vtx_line = vtx_line - 2;
        a = i + vtx_line;
        line ++;
    }
    graph__color_triangular(graph);
}

struct graph_t * graph__initialize(size_t size,char t)
{
    struct graph_t *graph = malloc(sizeof(struct graph_t));
    switch (t) {
    case 'h':
        {
            graph->num_vertices = (size+1)*(size+1);
            graph__initialize_hexagonal(graph);
            break;
        }
    case 'c':
        {
            graph->num_vertices = (size+1)*(size+1);
            graph__initialize_square(graph);
            break;
        }
    case 't':
        {
            graph->num_vertices = 6 * size * size;
            graph__initialize_triangular(graph);
	        break;
        }
    default:
        {
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void graph__display(struct graph_t * graph)
{
    size_t size = graph__size(graph);
    gsl_spmatrix* t = graph->t;
    gsl_spmatrix* o = graph->o;
    printf("Le nombre de sommet est : %ld\n",size);
    int a, b;
    printf("La matrice d'adjacence t\n");
    for(size_t i=0 ; i < size ; i++)
        {
            printf("%ld |",i);
            for(size_t j=0 ; j < size ; j++)
            {
                a = gsl_spmatrix_get(t,i,j);
                printf(" %d ",a);
            }
            printf("|\n");
        }
    printf("La matrice o du graph\n");
    for(size_t i = 0; i < 2; i++)
      {
	printf("|");
	for(size_t j=0 ; j < size ; j++)
            {
                b = gsl_spmatrix_get(o,i,j);
                printf(" %d ",b);
            }
	printf("|\n");
      }
}

void graph__print(struct graph_t *graph, char t)
{
  size_t width = graph__width(graph,t);
  gsl_spmatrix* o = graph->o;
  size_t x;
  switch(t){
  case 'c':
    {
      for(size_t j = 0; j <= width; j++)
	{
	  for(size_t i = 0; i <= width; i++)
	    {
	      x = i + j * (width + 1);
	      if(gsl_spmatrix_get(o,0,x))
		printf(ROUGE " ● " NORMAL);
	      else if(gsl_spmatrix_get(o,1,x))
		printf(BLANC " ● " NORMAL);
	      else
		printf(NORMAL " ○ ");
	    }
	  printf("\n");
	}
      break;
    }
  case 'h':
    {
      for(size_t j = 0; j <= width; j++)
	{
	  for(size_t k = 0; k < j; k++)
	    printf("  ");
	  for(size_t i = 0; i <= width; i++)
	    {
	      x = i + j * (width + 1);
	      if(gsl_spmatrix_get(o,0,x))
		printf(ROUGE " ⬢ " NORMAL);
	      else if(gsl_spmatrix_get(o,1,x))
		printf(BLANC " ⬢ " NORMAL);
	      else
		printf(NORMAL " ⬡ ");
	    }
	  printf("\n");
	}
      break;
    }
  case 't':
    {
      size_t total_line = 4*width -1;
      size_t index_end,index;
      size_t w = 0;
      for(size_t line = 0; line < total_line; line++)
	{
	  index =  index__first_in_line(graph,line);
	  if(line < width)
	    {
	      w = width - line - 1;
	      for(size_t k = 0; k < w; k++)
		printf("   ");
	      index_end = index + (2*(line+1) -1);
	      for(size_t i = index; i <= index_end; i++)
		{
		  if(i%2)
		    {
		      if(gsl_spmatrix_get(o,0,i))
			printf(ROUGE " ▶ " NORMAL);
		      else if(gsl_spmatrix_get(o,1,i))
			printf(BLANC " ▶ " NORMAL);
		      else
			printf(NORMAL " ▷ ");
		    }
		  else
		    {
		      if(gsl_spmatrix_get(o,0,i))
			printf(ROUGE " ◀ " NORMAL);
		      else if(gsl_spmatrix_get(o,1,i))
			printf(BLANC " ◀ " NORMAL);
		      else
			printf(NORMAL " ◁ ");
		    }
		}
	    }
	  else if(line > total_line - width)
	    {
	      w = line - total_line + width;
	      for(size_t k = 0; k < w; k++)
		printf("   ");
	      index_end = index + (2*(total_line-line) -1);
	      for(size_t i = index; i <= index_end; i++)
		{
		  if(i%2)
		    {
		      if(gsl_spmatrix_get(o,0,i))
			printf(ROUGE " ▶ " NORMAL);
		      else if(gsl_spmatrix_get(o,1,i))
			printf(BLANC " ▶ " NORMAL);
		      else
			printf(NORMAL " ▷ ");
		    }
		  else
		    {
		      if(gsl_spmatrix_get(o,0,i))
			printf(ROUGE " ◀ " NORMAL);
		      else if(gsl_spmatrix_get(o,1,i))
			printf(BLANC " ◀ " NORMAL);
		      else
			printf(NORMAL " ◁ ");
		    }
		}
	    }
	  else
	    {
	      index_end = index + 2*width-1;
	      for(size_t i = index; i <= index_end; i++)
		{
		  if(((line%2) && (i%2)) || ((line%2 == 0) && (i%2 == 0)))
		    {
		      if(gsl_spmatrix_get(o,0,i))
			printf(ROUGE " ▶ " NORMAL);
		      else if(gsl_spmatrix_get(o,1,i))
			printf(BLANC " ▶ " NORMAL);
		      else
			printf(NORMAL " ▷ ");
		    }
		  else
		    {
		      if(gsl_spmatrix_get(o,0,i))
			printf(ROUGE " ◀ " NORMAL);
		      else if(gsl_spmatrix_get(o,1,i))
			printf(BLANC " ◀ " NORMAL);
		      else
			printf(NORMAL " ◁ ");
		    }
		}
	    }
	  printf("\n");
	}
      break;
    }
  default:
    printf("Need Second Argument");
  }
}
