
#include <stdlib.h>
#include <stdio.h>
#include "mapa.h"
#include "uniform.h"
#include "B-tree.h"

void look_for_points(cuadrante*,int);
B_tree *insert_points_B_tree(int,point*,int);
int compare_points(void*,const void*,const void*);
point *B_tree_nearest_point(B_tree*,point*);

int main (int argc,char* argv[]){
  int i,k;
  point *p,*q,*r,*s;
  cuadrante *c;
  int cont3 = 0,cont4 = 0;
  B_tree *T3,*T4;
  k = read_points_file("../Tesis/Instancias/Q_MCLP_30.txt",&p);
  c = cuadrante_test(p,k);
  T3 = insert_points_B_tree(k,p,3);
  T4 = insert_points_B_tree(k,p,4);
  q = (point*)malloc(sizeof(point));
  for (i = 0;i < 10;i++) {
    q->x = unif(c->x_min,c->x_max);
    q->y = unif(c->y_min,c->y_max);
    r = B_tree_nearest_point(T3,q);
    printf("\t(3) the neares point is: (%f,%f) at distance %f\n",r->x,r->y,dist(q,r));
    s = B_tree_nearest_point(T4,q);
    printf("\t(3) the neares point is: (%f,%f) at distance %f\n",s->x,s->y,dist(q,s));
  }
  free(q);
  B_tree_delete_tree(T3);
  B_tree_delete_tree(T4);
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

B_tree *insert_points_B_tree(int k,point *p,int t) {
  int i;
  B_tree *T;
  T = B_tree_create_tree(t,compare_points);
  for (i = 0;i < k;i++) 
    B_tree_insert_key(T,&(p[i]));
  return T;
}

int compare_points(void *info,const void *a,const void *b) {
  point *x,*y;
  x = (point*) a;
  y = (point*) b;
  if (x->x > y->x) return 1;
  if (x->x < y->x) return -1;
  if (x->y > y->y) return 1;
  if (x->y < y->y) return -1;
  return 0;
}

point *B_tree_nearest_point(B_tree* T,point* p) {
  int cont = 0;
  point *q,*r,*s;
  r = p;
  s = p;
  q = NULL;
  do {
    cont++;
    r = B_tree_predecessor(T,r);
    s = B_tree_successor(T,s);
    if ((q == NULL && r != NULL) || 
	dist(p,r) < dist(p,q)) q = r;
    if (s != NULL && dist(p,s) < dist(p,q)) q = s;
  } while ( (r != NULL || s != NULL) &&
	    (r != NULL && (p->x - r->x) < dist(p,q)) &&
	    (s != NULL && (s->x - p->x) < dist(p,q)));
  printf("t = %d\t%d\n",T->t,cont);
  return q;
}
