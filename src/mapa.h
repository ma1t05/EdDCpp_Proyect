
#ifndef _MAPA_H_
#define _MAPA_H_

#include <stdlib.h>
#include "arrdin.h"
#include "point.h"
#define MAX_POINTS 4
#define TRUE 1
#define FALSE 0

struct cuadrante {
  float x_0,y_0;
  float x_min,x_max;
  float y_min,y_max;
  arrdin *points;
  int is_leaf;
  struct cuadrante *c_00,*c_01,*c_11,*c_10;
};

typedef struct cuadrante cuadrante;

/*_ _ _ _ _ _
  |    |     |
  | 10 | 11  |
  |_ _ |_ _ _|
  |    |     |
  | 00 | 01  |
  |_ _ |_ _ _|
 */

cuadrante* cuadrante_crea(float,float,float,float);
int cuadrante_add(cuadrante*,point*);
void cuadrante_divide(cuadrante*);
cuadrante* cuadrante_test(point*,int);
void cuadrante_printf(cuadrante*,int);
point* cuadrante_search(cuadrante*,point*);
point* closest_point(arrdin*,point*);
void cuadrante_free(cuadrante*);

#endif
