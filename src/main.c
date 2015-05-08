
#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include "uniform.h"

void look_for_points(cuadrante*,int);

int main (int argc,char* argv[]){
  int k;
  point *p;
  cuadrante *c;
  k = read_points_file("../Tesis/Instancias/Q_MCLP_30.txt",&p);
  c = cuadrante_test(p,k);
  //cuadrante_printf(c,0);
  look_for_points(c,10);
  cuadrante_free(c);
  free(p);
  return 0;
}

void look_for_points(cuadrante *c,int n){
  int i;
  point *p,*q;
  p = (point*)malloc(sizeof(point));
  for (i = 0;i < n;i++) {
    p->x = unif(c->x_min,c->x_max);
    p->y = unif(c->y_min,c->y_max);
    printf("Looking for the point (%f,%f)\n",p->x,p->y);
    q = cuadrante_search(c,p);
    if (q != NULL)
      printf("\tthe neares point is: (%f,%f) at distance %f\n",q->x,q->y,dist(p,q));
    else
      printf("\tno point found\n");
  }
  free(p);
}
