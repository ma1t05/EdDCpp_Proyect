/*
 * Libreria que define las estructuras utilizadas para representar
 * - a un contenedor de puntos
 * - a la matriz de contenedores
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "arrpoint.h"

typedef struct contenedor contenedor;
typedef struct grid grid;

struct contenedor {
  float x0,y0;
  arrdin puntos;
};

struct grid {
  int n,m;
  float xmin,xmax;
  float ymin,ymax;
  float delta;
  contenedor ***A;
  iterations cont;
};

grid* grid_create(point*,int);
point *grid_search(grid*,point*);
void grid_add_point(grid*,point*);
void grid_printf(grid*);
void grid_free(grid*);
