
#include "mapa.h"

cuadrante* _cuadrante_crea(float,float,float,float,int);
point* closest_point(arrdin*,point*);
void cuadrante_divide(cuadrante*);

cuadrante* _cuadrante_crea(float x_min,float y_min,float x_max,float y_max,int max_points){
  cuadrante *c;
  c = (cuadrante*)malloc(sizeof(cuadrante));
  c->x_0 = (x_max - x_min) * 0.5;
  c->y_0 = (y_max - y_min) * 0.5;
  c->x_min = x_min;  c->x_max = x_max;
  c->y_min = y_min;  c->y_max = y_max;
  c->points = arrdin_create();
  c->is_leaf = TRUE;
  c->max_points = max_points;
  c->c_10 = NULL;  c->c_11 = NULL;
  c->c_00 = NULL;  c->c_01 = NULL;
  c->cont.cont_comp = 0;
  c->cont.cont_dist = 0;
  return c;
}

int cuadrante_add(cuadrante *c,point *p){
  int ans,ans_aux;
  if (p->x < c->x_min || p->x > c->x_max ||
      p->y < c->y_min || p->y > c->y_max) {
    return FALSE;
  }

  if (c->points->elements+1 > c->max_points && c->is_leaf)
    cuadrante_divide(c);

  arrdin_add(c->points,p); // casting to void??
  
  if (!(c->is_leaf)) {
    ans = FALSE;
    ans_aux = cuadrante_add(c->c_00,p); ans = (ans || ans_aux);
    ans_aux = cuadrante_add(c->c_01,p); ans = (ans || ans_aux);
    ans_aux = cuadrante_add(c->c_10,p); ans = (ans || ans_aux);
    ans_aux = cuadrante_add(c->c_11,p); ans = (ans || ans_aux);
    if (ans == FALSE)
      printf("(%f,%f) rejected\n",p->x,p->y);
  }

  return TRUE;
}

void cuadrante_divide(cuadrante *c) {
  float lambda = 0.6;
  float x_max,x_min,y_max,y_min;
  int i;
  point *q,*r = NULL;
  c->is_leaf = FALSE;
  x_max = (1 - lambda) * c->x_min + lambda * c->x_max;
  x_min = lambda * c->x_min + (1 - lambda) * c->x_max;
  y_max = (1 - lambda) * c->y_min + lambda * c->y_max;
  y_min = lambda * c->y_min + (1 - lambda) * c->y_max;
  c->c_00 = _cuadrante_crea(c->x_min,c->y_min,x_max,y_max,c->max_points);
  c->c_01 = _cuadrante_crea(x_min,c->y_min,c->x_max,y_max,c->max_points);
  c->c_11 = _cuadrante_crea(x_min,y_min,c->x_max,c->y_max,c->max_points);
  c->c_10 = _cuadrante_crea(c->x_min,y_min,x_max,c->y_max,c->max_points);
  for (i = 0;i < c->points->size;i++) {
    if (c->points->a[i] != NULL) {
      q = (point*)c->points->a[i];
      cuadrante_add(c->c_00,q);
      cuadrante_add(c->c_01,q);
      cuadrante_add(c->c_10,q);
      cuadrante_add(c->c_11,q);
    }
  }

}

cuadrante* cuadrante_crea(point *puntos,int k,int max_points){
  int i,add;
  cuadrante *c;
  float x_min,x_max,y_min,y_max;
  float epsilon;
  
  // Determina minimos y maximos
  x_min = puntos[0].x;  x_max = puntos[0].x;
  y_min = puntos[0].y;  y_max = puntos[0].y;
  for (i = 1;i < k;i++) {
    if (x_min > puntos[i].x) x_min = puntos[i].x;
    if (x_max < puntos[i].x) x_max = puntos[i].x;
    if (y_min > puntos[i].y) y_min = puntos[i].y;
    if (y_max < puntos[i].y) y_max = puntos[i].y;
  }

  epsilon = (x_max - x_min) * 0.05;
  x_min -= epsilon;
  x_max += epsilon;
  epsilon = (y_max - y_min) * 0.05;
  y_min -= epsilon;
  y_max += epsilon;

  c = _cuadrante_crea(x_min,y_min,x_max,y_max,max_points);
  for (i = 0;i < k;i++) {
    add = cuadrante_add(c,&(puntos[i]));
    if (!add)
      printf("(%f,%f) rechazado\n",puntos[i].x,puntos[i].y);
  }

  return c;
}

void cuadrante_printf(cuadrante* c,int depth){
  int i;
  for (i = 0;i < depth;i++) printf("\t");
  printf("%d\t{(%.2f,%.2f)->(%.2f,%.2f)}\n",c->points->elements,c->x_min,c->y_min,c->x_max,c->y_max);
  if (!(c->is_leaf)) {
    cuadrante_printf(c->c_00,depth+1);
    cuadrante_printf(c->c_10,depth+1);
    cuadrante_printf(c->c_01,depth+1);
    cuadrante_printf(c->c_11,depth+1);
  }
}

point* cuadrante_search(cuadrante* c,point *p) {
  point *r;
  double d; /* shorter distance to container limits */
  r = NULL;
  if (c->is_leaf) {
    /* Calcula dist */ c->cont.cont_dist += c->points->elements; 
    r = closest_point(c->points,p);
  }
  else {
    /* Compara */ c->cont.cont_comp++;
    if (p->x < c->x_0) {
      /* Compara */ c->cont.cont_comp++;
      d = p->x - c->x_min;
      if (d > c->c_00->x_max - p->x) d = c->c_00->x_max - p->x;
      if (p->y < c->y_0) {
	r = cuadrante_search(c->c_00,p);
	if (d > p->y - c->y_min) d = p->y - c->y_min;
	if (d > c->c_00->y_max - p->y) d = c->c_00->y_max - p->y;
      }
      else {
	r = cuadrante_search(c->c_10,p);
	if (d > c->y_max - p->y) d = c->y_max - p->y;
	if (d > p->y - c->c_10->y_min) d = p->y - c->c_10->y_min;
      }
    }
    else {
      /* Compara coordenadas */
      c->cont.cont_comp++;
      d = c->x_max - p->x;
      if (d > p->x - c->c_11->x_min) d = p->x - c->c_11->x_min;
      if (p->y < c->y_0) {
	r = cuadrante_search(c->c_01,p);
	if (d > p->y - c->y_min) d = p->y - c->y_min;
	if (d > c->c_01->y_max - p->y) d = c->c_01->y_max - p->y;
      }
      else {
	r = cuadrante_search(c->c_11,p);
	if (d > c->y_max - p->y) d = c->y_max - p->y;
	if (d > p->y - c->c_11->y_min) d = p->y - c->c_11->y_min;
      }
    }
    if (r == NULL || dist(r,p) > d) {
      /* Calcula dist contra c->points->elements */
      c->cont.cont_dist += c->points->elements; 
      r = closest_point(c->points,p);
    }
  }
  return r;
}

void cuadrante_free(cuadrante* c) {
  if (c->is_leaf) {
    cuadrante_free(c->c_00);
    cuadrante_free(c->c_01);
    cuadrante_free(c->c_11);
    cuadrante_free(c->c_10);
  }
  arrdin_free(c->points);
  free(c);
}
