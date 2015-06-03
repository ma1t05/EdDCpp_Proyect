
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mapa.h"
#include "uniform.h"
#include "B-tree.h"
#include "grid.h"
#include "random_selection.h"

#define EPSILON 0.001

void look_for_points(cuadrante*,int);
B_tree *insert_points_B_tree(int,point*,int);
int compare_points(void*,const void*,const void*);
point *B_tree_nearest_point(B_tree*,point*);
void B_tree_printf(B_node*,int);
void B_tree_show(B_tree*,point*);
void prueba_cuadrantes(point*,int,int);
void prueba_B_trees(point*,int,int);
void prueba_grid(point*,int);

int main (int argc,char* argv[]){
  int k;
  point *p;

  // Arreglo de puntos
  //k = read_points_file_xyd("../Instancias/pmedian818.txt",&p);
  seed();
  k = (argc > 1 ? atoi(argv[1]) : 1024);
  p = point_generate_random_instance(k);
  //printf("Prueba Cuadrantes\n");
  //prueba_cuadrantes(p,k,2);
  //printf("Prueba Grid\n");
  prueba_grid(p,k);
  free(p);
  return 0;
}

/* Quadtree */
void prueba_cuadrantes(point *p,int k,int a) {
  int i,j;
  point q;
  cuadrante **c;
  point *c_ans,*e_ans;
  int times;
  double d;
  double *err_c;

  // Mapa dividido en cuadrantes
  c = (cuadrante**)malloc(sizeof(cuadrante*)*(a));
  err_c = (double*)malloc(sizeof(double)*(a));
  for (j = 0;j < a;j++) c[j] = cuadrante_crea(p,k,j+3);
  printf("Termina creacion de cuadrantes\n");

  for(times = 100;times <= 10000;times += 100) {
    for (j = 0;j < a;j++) err_c[j] = 0.0;

    for (i = 0;i < times;i++) {
      // Punto aleatorio dentro de la region
      q.x = unif(c[0]->x_min,c[0]->x_max); 
      q.y = unif(c[0]->y_min,c[0]->y_max);
    
      // Procedimiento exahustivo
      e_ans = nearest_point(p,k,&q);
      d = dist(e_ans,&q); /* Distancia Minima */
      // Busqueda por cuadrantes
      for(j = 0;j < a;j++) {
	c_ans = cuadrante_search(c[j],&q);
   
	// Calcular errores porcentuales
	if (d > 0.0)
	  err_c[j] += dist(&q,c_ans) / d - 1.0;
	else 
	  if (dist(&q,c_ans) > 0) err_c[j] += 1.0;
      }
    }
    printf("%d",times);
    for (j = 0; j < a;j++)
      printf(" %d %d %.2f",c[j]->cont.cont_comp,c[j]->cont.cont_dist,(double)c[j]->cont.cont_dist/(k*times));
    printf("\n");
  }
  free(err_c);
  for (j = 0;j < a;j++)
    cuadrante_free(c[j]);
  free(c);
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

/* B-tree usage */
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
  if (x->x - y->x > EPSILON) return 1;
  if (y->x - x->x > EPSILON) return -1;
  if (x->y - y->y > EPSILON) return 1;
  if (y->y - x->y > EPSILON) return -1;
  return 0;
}

void prueba_B_trees(point *p,int k,int a) {
  int i,j;
  int times = 100;
  double d,*err;
  point q,*ans,*e_ans;
  cuadrante *c;
  B_tree **T;
  c = cuadrante_crea(p,k,4);
  T = (B_tree**)malloc(sizeof(B_tree*)*a);
  err = (double*)malloc(sizeof(double)*a);
  for (j = 0;j < a;j++) {
    T[j] = insert_points_B_tree(k,p,j+3);
    err[j] = 0.0;
  }
  printf("Reserva memoria para arboles\n");
  for (i = 0;i < times;i++) {
    // Punto aleatorio dentro de la region
    q.x = unif(c->x_min,c->x_max); 
    q.y = unif(c->y_min,c->y_max);
    printf("Genera punto aleatorio\n");
    
    // Procedimiento exahustivo
    e_ans = nearest_point(p,k,&q);
    d = dist(e_ans,&q); /* Distancia Minima */
    printf("Encuentra NN\n");

    for (j = 0;j < a;j++) {
      ans = B_tree_nearest_point(T[j],&q);
      printf("Encontro NN para B-tree con t = %d\n",j+3);
      /* Calcula errores */
      if (d > 0.0) {
	printf("%.2f\n",dist(&q,ans) / d - 1.0);
	err[j] += dist(&q,ans) / d - 1.0;
      }
      else if (dist(&q,ans) > 0)
	err[j] += 1.0;
    }
  }
  printf("%d",times);
  for(j = 0;j < a;j++) printf(" %.2f",err[j]/times);
  printf("\n");
  free(err);
  for (j = 0;j < a;j++) free(T[j]);
  free(T);
}

point *B_tree_nearest_point(B_tree* T,point* p) {
  int i;
  point *q,*r,*s;
  B_node *x,*y;
  r = p;
  s = p;
  q = NULL;
  x = B_tree_find_node(T,p);
  if (x != NULL) return p;
  y = NULL;
  do {

    if (r != NULL) {
      r = B_tree_predecessor(T,r);
      if (r != NULL) {
	printf("r != NULL\n");
	if (q == NULL || dist(p,r) < dist(p,q)) {
	  /* Dist */ T->cont.cont_dist += 2;
	  q = r;
	  printf("\tactualiza q a r\n");
	}
	x = B_tree_find_node(T,r);
	printf("\tencuntra nodo de r %d\n",x);
	if (x == NULL) printf("*** WTF! ***\n");
	if (x->leaf) {
	  printf("\tes hoja!\n");
	  for (i = 0;i < x->n;i++) {
	    if (dist(p,x->key[i]) < dist(p,q)) {
	      /* Dist */ T->cont.cont_dist++;
	      q = x->key[i];
	      printf("\tactualiza q a x->key[%d]\n",i);
	    }
	  }
	  r = x->key[0];
	}
	/* Dist */ T->cont.cont_dist++;
	/* Comparations */ T->cont.cont_comp++;
	if (p->x - r->x > dist(p,q))
	  r = NULL;
	printf("\texit r != NULL\n");
      }
      else x = NULL;
    }

    if (s != NULL) {
      s = B_tree_successor(T,s);
      if (s != NULL) {
	printf("s != NULL\n");
	if (q == NULL || dist(p,s) < dist(p,q)) {
	  /* Dist */ T->cont.cont_dist += 2;
	  q = s;
	}
	y = B_tree_find_node(T,s);
	if (y->leaf && y != x) {
	  printf("*** x != y\n");
	  for (i = y->n;i > 0;i--) {
	    if (dist(p,y->key[i-1]) < dist(p,q)) {
	      /* Dist */ T->cont.cont_dist++;
	      q = y->key[i-1];
	    }
	  }
	}
	s = y->key[y->n - 1];
	/* Dist */ T->cont.cont_dist++;
	/* Comparations */ T->cont.cont_comp++;
	if (s->x - p->x > dist(p,q)) 
	  s = NULL;
      }
    }

  } while (r != NULL || s != NULL);
  return q;
}

void B_tree_printf(B_node* x,int depth) {
  int i;
  for (i = 0;i < depth;i++)
    printf(" ");
  printf("[");
  for (i = 0;i < x->n;i++)
    printf(" %d",(x->key[i] != NULL));
  printf(" ]\n");
  if (!x->leaf)
    for (i = 0;i <= x->n;i++)
      B_tree_printf(x->c[i],depth+1);
}

void B_tree_show(B_tree* T,point *p) {
  point *r;
  r = p;
  do {
    p = r;
    r = B_tree_predecessor(T,p);
  } while (r != NULL);
  
  printf("B-tree %d\n",T->t);
  r = p;
  do {
    printf("(%.2f,%.2f)\n",r->x,r->y);
    p = r;
    r = B_tree_successor(T,p);
  } while (r != NULL);

  printf("Orden inverso\n");
  r = p;
  do {
    printf("(%.2f,%.2f)\n",r->x,r->y);
    p = r;
    r = B_tree_predecessor(T,p);
  } while (r != NULL);
}

/* Grid usage */
void prueba_grid(point *p,int k) {
  int i;
  grid *G;
  double d,err,t_err;
  point q,*e_ans,*ans;
  G = grid_create(p,k);
  t_err = 0.0;
  
  // Punto aleatorio dentro de la region
  for (i = 0; i < 100;i++) {
    q.x = unif(G->xmin,G->xmax); 
    q.y = unif(G->ymin,G->ymax);
  
    e_ans = nearest_point(p,k,&q);
    d = dist(&q,e_ans);
    ans = grid_search(G,&q);
    err = dist(&q,ans) / d - 1.0;
    if (err > 0) printf(" err = %.2f\n",err);
    t_err += err;
  }
  printf("Uso de dist prom: %.2f con un error de %.2f\n",G->cont.cont_dist * 0.01,t_err);
  
  grid_free(G);
}
