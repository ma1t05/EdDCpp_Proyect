/*
 * Libreria que define las estructuras utilizadas para representar
 * - a un punto de demanda
 * - a un contenedor de puntos
 * - a la matriz de contenedores
 * - a un ajustador
 */

#include<stdlib.h>
#include<math.h>

struct punto {
  int x,y;
  int demanda;
};

struct ajustador {
  char status;
  struct point *base;
  struct point *posicion;
  struct point *destino;
};

struct contenedor {
  int x0,y0;
  int Dx,Dy;
  int elementos;
  struct ajustador **ajustadores;
  struct punto **puntos;
};

struct mapa {
  int n,m;
  int xmin,xmax;
  int ymin,ymax;
  int delta_x,delta_y;
  struct contenedor ***A;
};

typedef struct punto punto;
typedef struct ajustador ajustador;
typedef struct contenedor contenedor;
typedef struct mapa mapa;

void agrega_punto(contenedor*,punto*);

mapa* organiza_puntos(punto* puntos,int k){
  
  int i,j,l;
  int r,s;

  mapa *map;
  contenedor *bin;
  
  map = (mapa*)malloc(sizeof(mapa));
  map->n = map->m = ceiling((float) k / log(k));
  map->xmin = map->xmax = puntos[0].x;
  map->ymin = map->ymax = puntos[0].y;
  for (i = 1;i < k;i++) {
    if (puntos[i].x < map->xmin) map->xmin = puntos[i].x;
    if (puntos[i].x > map->xmax) map->xmax = puntos[i].x;
    if (puntos[i].y > map->ymin) map->ymin = puntos[i].y;
    if (puntos[i].y > map->ymax) map->ymax = puntos[i].y;
  }

  s = (map->xmax - map->xmin) / map->n;
  r = (map->xmax - map->xmin) % s;
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

void agrega_punto(contenedor *bin,punto *point){
  int i;
  punto **puntos;
  if (bin->elementos == 0) {
    bin->puntos = (punto**)malloc(sizeof(punto*));
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
    puntos = (punto**)malloc(sizeof(punto*)*bin->elementos*2);
    puntos[i] = point;
    for (i = 0;i < bin->elementos;i++) {
      puntos[i] = bin->puntos[i];
    }
    bin->elementos *= 2;
    bin->puntos = puntos;
  }
}
