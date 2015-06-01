
#include "arrpoint.h"

point* closest_point(arrdin* points,point *p){
  int i;
  point *q,*r = NULL;
  for (i = 0;i < points->size;i++) {
    if (points->a[i] != NULL) {
      q = (point*)points->a[i];
      if (r == NULL || dist(p,q) < dist(p,r)) r = q;
    }
  }  
  return r;
}
