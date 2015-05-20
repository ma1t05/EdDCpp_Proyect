/*
 * Libreria que define las estructuras utilizadas para representar
 * - a un contenedor de puntos
 * - a la matriz de contenedores
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "point.h"
#include "grid.h"

grid* grid_create(point* puntos,int k){
  
  int i,j,l;
  int r,s;

  grid *map;
  contenedor *bin;
  
  map = (grid*)malloc(sizeof(grid));
  printf("Calculando n\n");
  map->n = map->m = ceil((float)k / log((float)k));
  printf("Se obtuvo %d\n",map->n);
  map->xmin = puntos[0].x; map->xmax = puntos[0].x;
  map->ymin = puntos[0].y; map->ymax = puntos[0].y;
  for (i = 1;i < k;i++) {
    if (puntos[i].x < map->xmin) map->xmin = puntos[i].x;
    if (puntos[i].x > map->xmax) map->xmax = puntos[i].x;
    if (puntos[i].y > map->ymin) map->ymin = puntos[i].y;
    if (puntos[i].y > map->ymax) map->ymax = puntos[i].y;
  }
  printf("Obtiene maximos y minimos\n");

  dx = (map->xmax - map->xmin) / map->n;
  dy = (map->ymax - map->ymin) / map->m;
  s = (dx > dy ? dx : dy);

  r = (map->xmax - map->xmin) % s;
  printf("%d divisiones con residuo %d\n",s,r);
  if (r > 0) {
    if (s - r > 1) {
      map->xmin -= (s - r) / 2;
      map->xmax += (s - r) / 2;
      r = (s - r) % 2;
    }
    if (r > 0)
      map->xmin--;
    map->n++;
  }
  map->delta_x = s;
  printf("Obitne delta x %d\n",s);

  s = (map->ymax - map->ymin) / map->m;
  r = (map->ymax - map->ymin) % s;
  if (r > 0) {
    if (s - r > 1) {
      map->ymin -= (s - r) / 2;
      map->ymax += (s - r) / 2;
      r = (s - r) % 2;
    }
    if (r > 0)
      map->ymin--;
    map->m++;
  }
  map->delta_y = s;
  printf("Obitne delta y %d\n",s);

  map->A = (contenedor***)malloc(sizeof(contenedor**)*map->n);
  for (i = 0;i < map->n;i++) {
    map->A[i] = (contenedor**)malloc(sizeof(contenedor*)*map->m);
    for (j = 0;j < map->m;j++) map->A[i][j] = NULL;
  }

  for (l = 0;l < k;l++) {
    i = (puntos[l].x - map->xmin) / map->delta_x;
    j = (puntos[l].y - map->ymin) / map->delta_y;
    if (map->A[i][j] == NULL) {
      bin = (contenedor*)malloc(sizeof(contenedor));
      map->A[i][j] = bin;
      bin->x0 = map->xmin + map->delta_x * i;
      bin->y0 = map->ymin + map->delta_y * j;
      bin->elementos = 0;
      bin->ajustadores = NULL;
    }
    else bin = map->A[i][j];
    agrega_punto(bin,&(puntos[i]));
  }
}

void printf_mapa(mapa *map){
  int i,j;
  for (i = 0;i < map->n;i++) {
    for (j = 0;j < map->m;j++) {
      if (map->A[i][j] == NULL)
	printf("0");
      else
	printf("%d",map->A[i][j]->elementos);
    }
    printf("\n");
  }
}

void agrega_punto(contenedor *bin,point *point){
  int i;
  point **puntos;
  if (bin->elementos == 0) {
    bin->puntos = (point**)malloc(sizeof(point*));
    bin->elementos = 1;
  }
  puntos = bin->puntos;
  for (i = 0;i < bin->elementos;i++) {
    if (puntos[i] == NULL) {
      puntos[i] = point;
      break;
    }
  }
  if (i == bin->elementos) {
    puntos = (point**)malloc(sizeof(point*)*bin->elementos*2);
    puntos[i] = point;
    for (i = 0;i < bin->elementos;i++) {
      puntos[i] = bin->puntos[i];
    }
    bin->elementos *= 2;
    bin->puntos = puntos;
  }
}
