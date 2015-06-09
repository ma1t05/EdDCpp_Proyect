
#ifndef _POINT_H_
#define _POINT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct point {
  float x,y; /* x and y coords of point */
  int idx;
};

struct iterations {
  struct point *ans;
  int cont_comp; /* Cantidad de Comparaciones */
  int cont_dist; /* Cantidad de calculos de distancia */
  double t_setup; /* Tiempo de construccion */
  double t_search; /* Tiempo de busqueda */
};

typedef struct point point;
typedef struct iterations iterations;

float dist(point*,point*);
int read_points_file_xyd(const char*,point**);
point *point_generate_random_instance(int);

#endif
