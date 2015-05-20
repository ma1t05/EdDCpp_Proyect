
#include "point.h"
#define ALPHA 0.25

/************************************************************
 * Realiza una evaluacion, de un subconjunto aleatorio 
 * y regresa el mas cercano encontrado
 ************************************************************/
point* random_selection(point *set,int k,point *r) {
  int i,j,l;
  l = k * ALPHA;
  j = 0;
  while (l > 0) {
    i = rand() % k;
    if (dist(r,&(set[i])) < dist(r,&(set[j]))) j = i;
  }
  return &(set[j]);
}

/************************************************************
 * Realiza una evaluacion exahustiva del conjunto completo
 * y regresa el mas cercano
 ************************************************************/
point* nearest_point(point *set,int k,point *r) {
  int i,j = 0;
  for (i = 0;i < k;i++) {
    if (dist(r,&(set[i])) < dist(r,&(set[j]))) j = i;
  }
  return &(set[j]);
}
