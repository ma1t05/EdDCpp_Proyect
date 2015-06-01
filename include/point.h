
#ifndef _POINT_H_
#define _POINT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point {
  float x,y; /* x and y coords of point */
};

struct iterations {
  struct point *ans;
  int cont_comp; /* Cantidad de Comparaciones */
  int cont_dist; /* Cantidad de calculos de distancia */
};

typedef struct point point;
typedef struct iterations iterations;

float dist(point*,point*);
int read_points_file(const char*,point**);

#endif
