
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mapa.h"
#include "uniform.h"
#include "B-tree.h"
#include "grid.h"
#include "random_selection.h"

#define EPSILON 0.001
#define STEP_SIZE 1000

void look_for_points(cuadrante*,int);
B_tree *insert_points_B_tree(int,point*,int);
int compare_points(void*,const void*,const void*);
point *B_tree_nearest_point(B_tree*,point*);
void B_tree_printf(B_node*,int);
void B_tree_show(B_tree*,point*);
void prueba_cuadrantes(point*,int,int);
void prueba_B_trees(point*,int,int);
void prueba_grid(point*,int);
void NN_test();

int main (int argc,char* argv[]){
  int k;
  point *p;

  // Arreglo de puntos
  //k = read_points_file_xyd("../Instancias/pmedian818.txt",&p);
  seed();
  NN_test();
  /*k = (argc > 1 ? atoi(argv[1]) : 1024);
  p = point_generate_random_instance(k);
  prueba_cuadrantes(p,k,5); // Hay error de aproximacion
  prueba_B_trees(p,k,5);
  prueba_grid(p,k);
  free(p); /* */
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
  double *err_c,err;
  clock_t start;
  double sec;

  // Mapa dividido en cuadrantes
  start = clock();
  c = (cuadrante**)malloc(sizeof(cuadrante*)*(a));
  err_c = (double*)malloc(sizeof(double)*(a));
  for (j = 0;j < a;j++) {
    c[j] = cuadrante_crea(p,k,j+1);
  }
  sec = ((double)(clock() - start))/CLOCKS_PER_SEC;
  printf("Termina creacion de cuadrantes en %.4f segundos\n",sec);
  for (j = 0;j < a;j++) {
    printf("Ocupando %.2f mb\n",(double)sizeof_cuadrante(c[j])/1024);
  }

  for(times = 0;times < 10000;times += STEP_SIZE) {
    for (j = 0;j < a;j++) err_c[j] = 0.0;

    for (i = 0;i < STEP_SIZE;i++) {
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
	if (d > 0.0) {
	  err = dist(&q,c_ans) / d - 1.0;
	  if (err > 0.0) 
	    printf("err = %.2f\n",err);
	  err_c[j] += err;
	}
	else if (dist(&q,c_ans) > 0) 
	  err_c[j] += 1.0;
      }
    }
    printf("%d",times+STEP_SIZE);
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
  double d,*err,e;
  point q,*ans,*e_ans;
  cuadrante *c;
  B_tree **T;
  c = cuadrante_crea(p,k,4);
  T = (B_tree**)malloc(sizeof(B_tree*)*a);
  err = (double*)malloc(sizeof(double)*a);
  for (j = 0;j < a;j++) {
    T[j] = insert_points_B_tree(k,p,j+3);
    err[j] = 0.0;
    printf("Creo un arbol B con %.2f Kb\n",(double)sizeof_B_tree(T[j])/1024);
  }
  //printf("Reserva memoria para arboles\n");
  for (i = 0;i < times;i++) {
    // Punto aleatorio dentro de la region
    q.x = unif(c->x_min,c->x_max); 
    q.y = unif(c->y_min,c->y_max);
    //printf("Genera punto aleatorio\n");
    
    // Procedimiento exahustivo
    e_ans = nearest_point(p,k,&q);
    d = dist(e_ans,&q); /* Distancia Minima */
    //printf("Encuentra NN\n");

    for (j = 0;j < a;j++) {
      ans = B_tree_nearest_point(T[j],&q);
      //printf("Encontro NN para B-tree con t = %d\n",j+3);
      /* Calcula errores */
      if (d > 0.0) {
	e = dist(ans,&q) / d - 1.0;
	if (e > 0.0) printf("%.2f\n",e);
	err[j] += e;
	if (e < 0.0) {
	  printf("Distancia minima a (%.2f,%.2f): %.2f ",q.x,q.y,d);
	  printf("desde NN %d es (%.2f,%.2f)\n",e_ans->idx,e_ans->x,e_ans->y);
	  printf("Distancia encontrada desde punto %d: %.2f (%.2f,%.2f)\n",ans->idx,dist(ans,&q),ans->x,ans->y);
	}
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

void f_print_point(const void* r) {
  point *q = (point*)r;
  printf("(%.2f,%.2f)",q->x,q->y);
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
	if (q == NULL || dist(p,r) < dist(p,q)) {
	  /* Dist */ T->cont.cont_dist += 2;
	  q = r;
	}
	x = B_tree_find_node(T,r);
	if (x == NULL) {
	  printf("*** WTF! ***\nBuscando a :");
	  f_print_point(r);
	  printf(" predecesor de ");
	  f_print_point(p);
	  printf("\n");
	  x = B_tree_find_node2(T,r,f_print_point);
	  printf("Busca nodo de q\n");
	  B_tree_find_node2(T,p,f_print_point);
	}
	if (x->leaf) {
	  for (i = 0;i < x->n;i++) {
	    if (dist(p,x->key[i]) < dist(p,q)) {
	      /* Dist */ T->cont.cont_dist++;
	      q = x->key[i];
	    }
	  }
	  r = x->key[0];
	}
	/* Dist */ T->cont.cont_dist++;
	/* Comparations */ T->cont.cont_comp++;
	if (p->x - r->x > dist(p,q))
	  r = NULL;
      }
      else x = NULL;
    }

    if (s != NULL) {
      s = B_tree_successor(T,s);
      if (s != NULL) {
	if (q == NULL || dist(p,s) < dist(p,q)) {
	  /* Dist */ T->cont.cont_dist += 2;
	  q = s;
	}
	y = B_tree_find_node(T,s);
	if (y->leaf) {
	  if (y != x) {
	    for (i = y->n;i > 0;i--) {
	      if (dist(p,y->key[i-1]) < dist(p,q)) {
		/* Dist */ T->cont.cont_dist++;
		q = y->key[i-1];
	      }
	    }
	  }
	  s = y->key[y->n - 1];
	}
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
  printf("Crea grid usando %.2f Kb\n",(double)sizeof_grid(G)/1024);
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

void NN_test() {
  int i,j,k;
  int times;
  double d;
  double nn_time;
  point *p,*q;
  point *ans,*c_ans,*b_ans,*g_ans;
  double *err_c,*err_b,err_g;
  int c_num = 5,b_num = 5;
  clock_t beginning,now;
  cuadrante **c;
  B_tree **T;
  grid *G;
  FILE *setup,*memory,*time;
  setup = fopen("setup.dat","w");
  memory  = fopen("memory.dat","w");
  time = fopen("time.dat","w");

  for (k = 100;k <= 1000;k+=100) {
    printf("Comienza for sobre k = %d\n",k);
    p = point_generate_random_instance(k);
    nn_time = 0.0;
 
    // Cuadrantes
    //printf("Crea cuadrantes\n");
    c = (cuadrante**)malloc(sizeof(cuadrante*)*(c_num));
    err_c = (double*)malloc(sizeof(double)*(c_num));
    for (j = 0;j < c_num;j++) {
      beginning = clock();
      c[j] = cuadrante_crea(p,k,j+1);
      now = clock();
      c[j]->cont.t_setup = ((double)(now - beginning))/CLOCKS_PER_SEC;
      c[j]->cont.t_search = 0.0;
      err_c[j] = 0.0;
    }

    // Arboles B
    //printf("Crea arboles-B\n");
    T = (B_tree**)malloc(sizeof(B_tree*)*b_num);
    err_b = (double*)malloc(sizeof(double)*b_num);
    for (j = 0;j < b_num;j++) {
      beginning = clock();
      T[j] = insert_points_B_tree(k,p,j+3);
      now = clock();
      T[j]->cont.t_setup = ((double)(now - beginning))/CLOCKS_PER_SEC;
      T[j]->cont.t_search = 0.0;
      err_b[j] = 0.0;
    }
  
    // Grid
    //printf("Crea Grid\n");
    beginning = clock();
    G = grid_create(p,k);
    now = clock();
    G->cont.t_setup = ((double)(now - beginning))/CLOCKS_PER_SEC;
    G->cont.t_search = 0.0;
    err_g = 0.0;
    
    fprintf(setup,"%d\t0.00",k);
    for (j = 0;j < c_num;j++)
      fprintf(setup,"\t%f",c[j]->cont.t_setup);
    for (j = 0;j < b_num;j++)
      fprintf(setup,"\t%f",T[j]->cont.t_setup);
    fprintf(setup,"\t%f\n",G->cont.t_setup);
    
    // Uso de memoria
    fprintf(memory,"%d\t0.00",k);
    for (j = 0;j < c_num;j++) {
      fprintf(memory,"\t%f",(double)sizeof_cuadrante(c[j])/1024);
    }
    for (j = 0;j < b_num;j++) {
      fprintf(memory,"\t%f",(double)sizeof_B_tree(T[j])/1024);
    }
    fprintf(memory,"\t%f\n",(double)sizeof_grid(G)/1024);

    for(times = 0;times < 10000;times += STEP_SIZE) {
      //printf("Comienza ciclo de %d a %d iteraciones\n",times - 100,times);
      // Punto aleatorio dentro de la region
      q = point_generate_random_instance(STEP_SIZE);

      // Procedimiento exahustivo
      //printf("Comienza procedimiento exahustivo\n");
      for (i = 0;i < STEP_SIZE;i++) {
	ans = nearest_point(p,k,&(q[i]));
	d = dist(ans,&(q[i])); /* Distancia Minima */
      }
      
      // Busqueda por cuadrantes
      //printf("Comienza busqueda por cuadrantes\n");
      for(j = 0;j < c_num;j++) {
	//printf("Comienza cuadrante %d\n",j+1);
	beginning = clock();
	for (i = 0;i < STEP_SIZE;i++) {
	  c_ans = cuadrante_search(c[j],&(q[i]));
	}
	now = clock();
	c[j]->cont.t_search += ((double)(now - beginning))/CLOCKS_PER_SEC;
      }

      // Busqueda en arboles-B
      //printf("Comienza busqueda por arboles B\n");
      for (j = 0;j < b_num;j++) {
	beginning = clock();
	for (i = 0;i < STEP_SIZE;i++) {
	  b_ans = B_tree_nearest_point(T[j],&(q[i]));
	}
	now = clock();
	T[j]->cont.t_search += ((double)(now - beginning))/CLOCKS_PER_SEC;
      }

      // Grid
      //printf("Comienza busqueda en Grid\n");
      beginning = clock();
      for (i = 0;i < STEP_SIZE;i++) {
	g_ans = grid_search(G,&(q[i]));
      }
      now = clock();
      G->cont.t_search += ((double)(now - beginning))/CLOCKS_PER_SEC;

      beginning = clock();
      for (i = 0;i < STEP_SIZE;i++) {
	ans = nearest_point(p,k,&(q[i]));
      }
      now = clock();
      nn_time += ((double)(now - beginning))/CLOCKS_PER_SEC;

      free(q);
      
      fprintf(time,"%d\t%d",k,times+STEP_SIZE);
      fprintf(time,"\t%f",nn_time);
      for (j = 0;j < c_num;j++)
	fprintf(time,"\t%f",c[j]->cont.t_search);
      for (j = 0;j < b_num;j++)
	fprintf(time,"\t%f",T[j]->cont.t_search);
      fprintf(time,"\t%f\n",G->cont.t_search);

    }
  
    grid_free(G);

    free(err_b);
    for (j = 0;j < b_num;j++)
      B_tree_delete_tree(T[j]);
    free(T);

    free(err_c);
    for (j = 0;j < c_num;j++)
      cuadrante_free(c[j]);
    free(c);

    free(p);
  }
  fclose(setup);
  fclose(memory);
  fclose(time);
}
