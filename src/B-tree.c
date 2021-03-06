/* B-tree.c */

/***********************************************************************
 * Author: Luis Maltos
 * Clase: Estructura de Datos en C++
 * Implementacion de arboles B
 * Se implementa:
 *  search, create, insert, delete
***********************************************************************/

#include "B-tree.h"

B_node* _B_tree_create_node(int t);
B_node* _B_tree_find_node(B_tree *T,B_node *x,const void *key);
B_node *_B_tree_find_node2(B_tree *T,B_node *x,const void *key,void (*f_printf)(const void *key));
void *_B_tree_successor(B_tree *T,B_node *x,const void *key);
void *_B_tree_predecessor(B_tree *T,B_node *x,const void *key);
void _B_tree_remove_key(B_tree *T,B_node *x,const void *key);
void _B_tree_delete_node(B_node *x);

void free_B_node(B_node *x);
void B_tree_split_child(B_tree *T,B_node *x,int i);
void B_tree_insert_nonfull(B_tree *T,B_node *x,const void *key);
float plot(B_node *x,FILE *nodes,FILE *edges,FILE* boxes,float xo,float y);
int _sizeof_B_tree(B_tree *T,B_node *x);

B_tree* B_tree_create_tree (int t,int (*fcmp)(void *info,const void *key1,const void *key2)){
  B_tree *T;
  B_node *x;
  x = _B_tree_create_node(t);
  T = (B_tree*)malloc(sizeof(B_tree));
  T->t = t;
  T->root = x;
  T->fcmp = fcmp;
  T->info = &(T->t);
  T->size = 0;
  /* Use of count comparations */
  T->cont.cont_comp = 0;
  T->cont.cont_dist = 0;
  return T;
}

B_node* _B_tree_create_node(int t){
  int i;
  B_node *x;
  x = (B_node*)malloc(sizeof(B_node));
  x->n = 0;
  x->leaf = TRUE;
  x->key = (void**)malloc(sizeof(void*)*(2*t - 1));
  for(i = 0;i < 2*t-1;i++) 
    x->key[i] = NULL;
  x->c = (B_node**)malloc(sizeof(B_node*)*(2*t));
  for(i = 0;i < 2*t;i++) x->c[i] = NULL;
  return x;
}

B_node *B_tree_insert_key (B_tree *T,const void *key) {
  B_node *r,*s;
  r = T->root;
  if (r->n == 2 * T->t - 1) {
    s = _B_tree_create_node(T->t);
    T->root = s;
    s->leaf = FALSE;
    s->n = 0;
    s->c[0] = r;
    B_tree_split_child(T,s,0);
    r = s;
  }
  B_tree_insert_nonfull(T,r,key);
}


B_node *B_tree_find_node(B_tree *T,const void *key) {
  return _B_tree_find_node(T,T->root,key);
}

void *B_tree_predecessor(B_tree *T,const void *key){
  return _B_tree_predecessor(T,T->root,key);
}

void *B_tree_successor(B_tree *T,const void *key){
  return _B_tree_successor(T,T->root,key);
}

B_node *_B_tree_find_node(B_tree *T,B_node *x,const void *key) {
  int i = 0;
  while(i < x->n && (T->fcmp)(T->info,key,(x->key)[i]) > 0) {
    /* Comparations */ T->cont.cont_comp++;
    i++;
  }
  /* Comparations */ T->cont.cont_comp += 2;
  if (i < x->n && (T->fcmp)(T->info,key,x->key[i]) == 0)
    return x;
  if (x->leaf)
    return NULL;
  return _B_tree_find_node(T,x->c[i],key);
}

void *_B_tree_predecessor(B_tree *T,B_node *x,const void *key){
  int i = x->n - 1;
  void *predecessor;
  while (i >= 0 && (T->fcmp)(T->info,x->key[i],key) >= 0) {
    /* Comparations */ T->cont.cont_comp++;
    i--;
  }
  if (x->leaf)
    return (i >= 0 ? x->key[i] : NULL);
  predecessor = _B_tree_predecessor(T,x->c[i+1],key);
  if (predecessor == NULL && i >= 0)
    predecessor = x->key[i];
  return predecessor;
}

void *_B_tree_successor(B_tree *T,B_node *x,const void *key){
  int i = 0;
  void *successor;
  while (i < x->n && ((T->fcmp)(T->info,key,x->key[i]) >= 0)) {
    /* Comparations */ T->cont.cont_comp++;
    i++;
  }
  if (x->leaf)
    return (i < x->n ? x->key[i] : NULL);
  successor = _B_tree_successor(T,x->c[i],key);
  if (successor == NULL && i < x->n)
    successor = x->key[i];
  return successor;
}

void B_tree_remove_key(B_tree *T,const void *key){
  _B_tree_remove_key(T,T->root,key);
}

void _B_tree_remove_key(B_tree *T,B_node *x,const void *key){
  int i = x->n - 1,j;
  const void *k;
  B_node *y,*z;
  while (i > 0 && (T->fcmp)(T->info,key,x->key[i]) < 0) {
    /* Comparations */
    T->cont.cont_comp++;
    i--;
  }
  if ((T->fcmp)(T->info,key,x->key[i]) == 0) {
    /* Comparations */
    T->cont.cont_comp++;
    if (x->leaf){ /* Case 1 */
      for(;i < x->n;i++)
	x->key[i-1] = x->key[i];
    }
    else { /* Case 2 */
      y = x->c[i];
      z = x->c[i+1];
      if (y->n >= T->t) { /* 2a */
	k = _B_tree_predecessor(T,y,key);
	x->key[i] = k;
	_B_tree_remove_key(T,y,k);
      }
      else if (z->n >= T->t) { /* 2b */
	k = _B_tree_successor(T,z,key);
	x->key[i] = k;
	_B_tree_remove_key(T,z,k);
      }
      else { /* 2c */
	i++;
	x->n--;
	for(;i < x->n;i++){
	  x->key[i-1] = x->key[i];
	  x->c[i] = x->c[i+1];
	}
	y->key[T->t] = key;
	for(i = 1;i < z->n;i++){
	  y->key[T->t + i] = z->key[i];
	  y->c[T->t + i] = z->c[i];
	  free(z->key);
	  free(z->c);
	  free(z);
	}
	_B_tree_remove_key(T,y,key);
      }
    }
  }
  else { /* Case 3 */
    /* Comparations */
    T->cont.cont_comp++;
    y = ((T->fcmp)(T->info,key,x->key[i]) < 0 ? x->c[i] : x->c[++i]);
    if (y->n < T->t) {
      if (i < x->n && x->c[i+1]->n >= T->t) {
	z = x->c[i+1]; /* Hermano con almenos t claves */
	/* Baja clave de x a y */
	y->n++;
	y->key[T->t - 1] = x->key[i];
	y->c[T->t] = z->c[0];
	/* Sube clave de hermano a x */
	x->key[i] = z->key[0];
	/* Suprimir clave 0 de hermano */
	z->c[0] = z->c[1];
	for(j = 1;j < z->n;j++){
	  z->key[j-1] = z->key[j];
	  z->c[j] = z->c[j+1];
	}
	z->n--;
      }
      else if (i > 0 && x->c[i-1]->n >= T->t) {
	z = x->c[i-1]; /* Hermano con mas de t claves */
	/* Baja clave de x a y */
	y->n++;
	y->c[T->t] = y->c[T->t - 1];
	for(j = y->n - 1;j > 0;j--){
	  y->key[j] = y->key[j-1];
	  y->c[j] = y->c[j-1];
	}
	y->key[0] = x->key[i];
	y->c[0] = z->c[z->n];
	/* Sube clave de hermano a x */
	x->key[i] = z->key[z->n-1];
	/* Suprime ultima clave a hermano */
	z->key[z->n-1] = NULL; /* No es necesario borrar valor */
	z->c[z->n] = NULL; /* No es necesario apuntar nuevamente a null */
	z->n--;
      }
      else {
	if (i < x->n)
	  z = x->c[i+1];
	else {
	  z = y;
	  y = x->c[i-1];
	}
	/* Combinar z en y */
	y->key[T->t - 1] = x->key[i];
	y->c[T->t] = z->c[0];
	for(j = 0;j < z->n;i++){
	  y->key[T->t + j] = z->key[j];
	  y->c[T->t + j + 1] = z->c[j+1];
	}
	free(z->key);
	free(z->c);
	free(z);
      }
    }
    _B_tree_remove_key(T,y,key);
  }
}

void B_tree_delete_tree(B_tree *T){
  _B_tree_delete_node(T->root);
  free(T);
}

void _B_tree_delete_node(B_node *x){
  int i;
  if (!x->leaf)
    for(i = 0;i <= x->n;i++) _B_tree_delete_node(x->c[i]);
  free(x->key);
  free(x->c);
  free(x);
}


void B_tree_split_child(B_tree *T,B_node *x,int i){
  int j;
  B_node *y,*z;
  /*printf("Comienza split child en posicion %d\n",i); */
  z = _B_tree_create_node(T->t);
  y = x->c[i];
  z->leaf = y->leaf;
  z->n = T->t - 1;
  for(j = 0;j < T->t - 1;j++)
    z->key[j] = y->key[j+T->t];
  if (!y->leaf) {
    for(j = 0;j < T->t;j++)
      z->c[j] = y->c[j+T->t];
  }
  y->n = T->t - 1;
  for(j = x->n;j > i;j--) {
    x->c[j+1] = x->c[j];
    x->key[j] = x->key[j-1];
  }
  x->c[i+1] = z;
  x->key[i] = y->key[T->t - 1];
  x->n++;
}

void B_tree_insert_nonfull(B_tree *T,B_node *x,const void *key){
  int i = x->n;
  if (x->leaf) {
    /*printf("Comienza insert nonfull en hoja\n");*/
    while (i > 0 && (T->fcmp)(T->info,key,x->key[i-1]) < 0) {
      /* Comparations */ T->cont.cont_comp++;
      x->key[i] = x->key[--i]; /* Precaucion!, validar orden de aplicacion */
    }
    x->key[i] = key;
    x->n++;
  }
  else {
    /*printf("Comienza insert nonfull en nodo\n");*/
    while (i > 0 && (T->fcmp)(T->info,key,x->key[i-1]) < 0) {
      /* Comparations */ T->cont.cont_comp++;
      i--;
    }
    if (x->c[i]->n == 2 * T->t - 1) {
      B_tree_split_child(T,x,i);
      if ((T->fcmp)(T->info,key,x->key[i]) > 0) i++;
    }
    B_tree_insert_nonfull(T,x->c[i],key);
  }
}

#define BLOCK_SIZE 50.0
#define HEIGHT 10.0
#define DELTA 3.0
#define EPSILON 0.25

float plot(B_node *x,FILE *nodes,FILE *edges,FILE* boxes,float xo,float y){
  int i;
  float xf,*z;
  if (x->leaf) {
    xo+=DELTA;
    fprintf(boxes,"%f %f %f 0.0\n",xo,y-DELTA*0.25,BLOCK_SIZE * x->n);
    fprintf(boxes,"%f %f %f 0.0\n",xo,y+DELTA*0.25,BLOCK_SIZE * x->n);
    fprintf(boxes,"%f %f 0.0 %f\n",xo,y-DELTA*0.25,DELTA * 0.5);
    xo+=BLOCK_SIZE*0.5;
    for(i = 0; i < x->n; i++) {
      fprintf(nodes,"%f %f %d\n",xo,y,x->key[i]);
      xo+= BLOCK_SIZE;
    }
    fprintf(boxes,"%f %f 0.0 %f\n",xo-BLOCK_SIZE*0.5,y-DELTA*0.25,DELTA * 0.5);
    xo+=DELTA;
  }
  else {
    z = (float*)malloc(sizeof(float)*(x->n+2));
    xf = xo;
    for(i = 0;i <= x->n;i++){
      z[i] = xf;
      xf = plot(x->c[i],nodes,edges,boxes,xf,y - HEIGHT);
    }
    z[i] = xf;
    xo = (xo + xf) * 0.5;
    xo-= BLOCK_SIZE * x->n * 0.5;
    fprintf(boxes,"%f %f %f 0.0\n",xo,y-DELTA*0.25,BLOCK_SIZE * x->n);
    fprintf(boxes,"%f %f %f 0.0\n",xo,y+DELTA*0.25,BLOCK_SIZE * x->n);
    fprintf(boxes,"%f %f 0.0 %f\n",xo,y-DELTA*0.25,DELTA*0.5);
    xo+= BLOCK_SIZE * 0.5;
    for(i = 0;i < x->n;i++){
      fprintf(nodes,"%f %f %d\n",xo,y,x->key[i]);
      xo+= BLOCK_SIZE;
    }
    xo-= BLOCK_SIZE * 0.5;
    fprintf(boxes,"%f %f 0.0 %f\n",xo,y-DELTA*0.25,DELTA * 0.5);
    xo-= BLOCK_SIZE * x->n;
    for(i = 0;i <= x->n;i++){
      fprintf(edges,"%f %f %f %f\n",xo,y,(z[i]+z[i+1])/2-xo,-HEIGHT+EPSILON);
      xo+= BLOCK_SIZE;
    }
    free(z);
    xo = xf;
  }
  return xo;
}

void gnuplot(B_tree *T){
  FILE *nodes,*edges,*boxes;
  FILE *gnuPipe = popen("gnuplot","w");
  nodes = fopen("B_nodes.dat","w");
  edges = fopen("B_edges.dat","w");
  boxes = fopen("B_boxes.dat","w");
  printf("Imprimiendo\n");
  plot(T->root,nodes,edges,boxes,0.0,0.0);
  fclose(nodes);
  fclose(edges);
  fclose(boxes);

  fprintf(gnuPipe,"set term svg\n");
  fprintf(gnuPipe,"set output 'B_tree_%d.svg'\n",T->t);
  fprintf(gnuPipe,"unset key\n");
  fprintf(gnuPipe,"unset border\n");
  fprintf(gnuPipe,"unset yzeroaxis\n");
  fprintf(gnuPipe,"unset xtics\n");
  fprintf(gnuPipe,"unset ytics\n");
  fprintf(gnuPipe,"unset ztics\n");
  fprintf(gnuPipe,"plot ");
  fprintf(gnuPipe,"'B_nodes.dat' using 1:2 with points");
  fprintf(gnuPipe,", 'B_edges.dat' using 1:2:3:4 with vectors filled head linecolor rgb \"dark-blue\"");
  fprintf(gnuPipe,", 'B_boxes.dat' using 1:2:3:4 with vectors nohead linecolor rgb \"red\"");
  fprintf(gnuPipe,"\n");
  pclose(gnuPipe);
}

B_node *B_tree_find_node2(B_tree *T,const void *key,void (*f_printf)(const void *key)) {
  return _B_tree_find_node2(T,T->root,key,f_printf);
}

B_node *_B_tree_find_node2(B_tree *T,B_node *x,const void *key,void (*f_printf)(const void *key)) {
  int i = 0;
  if (x == T->root)
    printf("Start search on root\n");
  if (x->leaf)
    printf("Current node is a leaf ");
  else
    printf("Current node isn't a leaf ");
  printf(" whit %d elements\n",x->n);

  while(i < x->n) {
    f_printf((x->key)[i]);
    printf(" ");
    i++;
  }
  printf("\n");
  i = 0;

  while(i < x->n && (T->fcmp)(T->info,key,(x->key)[i]) > 0) {
    /* Comparations */ T->cont.cont_comp++;
    printf("For i = %d\t fcmp = %d\n",i,(T->fcmp)(T->info,key,(x->key)[i]));
    f_printf((x->key)[i]);
    i++;
  }
  printf("Stops on i = %d\n",i);
  /* Comparations */ T->cont.cont_comp += 2;
  if (i < x->n && (T->fcmp)(T->info,key,x->key[i]) == 0) {
    printf("Return node x = %d\n",x);
    return x;
  }
  if (x->leaf) {
    printf("return NULL\n");
    return NULL;
  }
  printf("Search in node c_i = %d\n",x->c[i]);
  return _B_tree_find_node2(T,x->c[i],key,f_printf);
}

int sizeof_B_tree(B_tree *T) {
  int size;
  size = sizeof(B_tree);
  if (T->root != NULL) {
    size += _sizeof_B_tree(T,T->root);
  }
  return size;
}

int _sizeof_B_tree(B_tree *T,B_node *x) {
  int i,size;
  if (x == NULL) 
    return 0;
  size = sizeof(B_node);
  size += sizeof(void*) * (2 * T->t - 1);
  size += sizeof(B_node*) * (2 * T->t);
  if (x->leaf == FALSE) {
    for(i = 0;i <= x->n;i++)
      size += _sizeof_B_tree(T,x->c[i]);
  }
  return size;
}

/* eof */











