/*
 * Libreria que define las estructuras utilizadas para representar
 * - a un contenedor de puntos
 * - a la matriz de contenedores
 */

#include "grid.h"

contenedor *_grid_create_bin(float,float);

grid* grid_create(point* puntos,int k){
  
  int i,j,l;
  double r,s;
  double dx,dy;
  double xrange,yrange;

  grid *map;
  contenedor *bin;
  
  map = (grid*)malloc(sizeof(grid));
  //printf("Calculando n\n");
  map->n = map->m = ceil((float)k / log((float)k));
  //printf("Se obtuvo %d\n",map->n);
  map->xmin = puntos[0].x; map->xmax = puntos[0].x;
  map->ymin = puntos[0].y; map->ymax = puntos[0].y;
  for (l = 1;l < k;l++) {
    if (puntos[l].x < map->xmin) map->xmin = puntos[l].x;
    if (puntos[l].x > map->xmax) map->xmax = puntos[l].x;
    if (puntos[l].y < map->ymin) map->ymin = puntos[l].y;
    if (puntos[l].y > map->ymax) map->ymax = puntos[l].y;
  }
  //printf("Obtiene maximos y minimos\n");
  //printf("xmin: %.2f\txmax: %.2f\n",map->xmin,map->xmax);
  //printf("ymin: %.2f\tymax: %.2f\n",map->ymin,map->ymax);

  xrange = map->xmax - map->xmin;
  yrange = map->ymax - map->ymin;
  map->xmin -= 0.05 * xrange;
  map->xmax += 0.05 * xrange;
  map->ymin -= 0.05 * yrange;
  map->ymax += 0.05 * yrange;
  
  dx = (map->xmax - map->xmin) / map->n;
  dy = (map->ymax - map->ymin) / map->m;
  
  if (dx < dy) {
    map->m = ceil((map->ymax - map->ymin) / dx);
    map->delta = dx;
    dy = (dx * map->m - (map->ymax - map->ymin));
    map->ymax += dy * 0.5;
    map->ymin -= dy * 0.5;
  }
  else {
    map->n = ceil((map->xmax - map->xmin) / dy);
    map->delta = dy;
    dx = (dy * map->n - (map->xmax - map->xmin));
    map->xmax += dx * 0.5;
    map->xmin -= dx * 0.5;
  }

  //printf("Obtiene delta de %.2f\n",map->delta);
  //printf("Se utilizara una matrix %d x %d\n",map->n,map->m);
  map->A = (contenedor***)malloc(sizeof(contenedor**)*map->n);
  for (i = 0;i < map->n;i++) {
    map->A[i] = (contenedor**)malloc(sizeof(contenedor*)*map->m);
    for (j = 0;j < map->m;j++) map->A[i][j] = NULL;
  }

  //printf("Comienza a agregar puntos\n");
  for (l = 0;l < k;l++) {
    grid_add_point(map,&(puntos[l]));
  }

  map->cont.cont_comp = 0;
  map->cont.cont_dist = 0;

  return map;
}

void grid_add_point(grid *map,point *p) {
  int i,j;
  i = floor((p->x - map->xmin) / map->delta);
  j = floor((p->y - map->ymin) / map->delta);
  if (map->A[i][j] == NULL)
    map->A[i][j] = _grid_create_bin
      (map->xmin + map->delta * i /* x0 */,
       map->ymin + map->delta * j /* y0 */);
  arrdin_add(&(map->A[i][j]->puntos),p);
}

void grid_printf(grid *map){
  int i,j;
  for (i = 0;i < map->n;i++) {
    for (j = 0;j < map->m;j++) {
      if (map->A[i][j] == NULL)
	printf("0");
      else
	printf("%d",map->A[i][j]->puntos.elements);
    }
    printf("\n");
  }
}

point *grid_search(grid *map,point *p) {
  int a,b;
  int i,j;
  int k,l;
  int n,m;
  double d;
  contenedor *bin;
  point *ans,*aux;
  a = floor((p->x - map->xmin) / map->delta);
  b = floor((p->y - map->ymin) / map->delta);
  //printf("Comienza grid_search de (%d,%d)\n",a,b);
  
  /* Determina la primera n que deja dentro de grid */
  n = 0;
  if (a < 0) {
    n = -a;
  }
  else if (a >= map->n) {
    n = a - map->n + 1;
  }
  
  if (b < 0) {
    if (n == 0 || -b < n) n = -b;
  }
  else if (b >= map->m) {
    if (n == 0 || b - map->m + 1 < n)
      n = b - map->m + 1;
  }
  //printf("Comienza con n = %d\n",n);

  /* revisa si el punto cae dentro de un bin no vacio del grid */
  bin = (n == 0 ? map->A[a][b] : NULL);

  /* Busca la primera n que tiene bins no vacios */
  if (bin == NULL) n--;
  k = 0;
  /* Decrementamos n para comenzar con la primera que nos deja en el grid */
  while (bin == NULL) {
    n++; k = 0;
    i = a + n;
    j = b;
    if (i < map->n && j >= 0 && j < map->m && map->A[i][j] != NULL)
      bin = map->A[i][j];

    i = a;
    j = b + n;
    if (j < map->m && i >= 0 && i < map->n && map->A[i][j] != NULL)
      bin = map->A[i][j];
    
    i = a - n;
    j = b;
    if (i >= 0 && j >= 0 && j < map->m && map->A[i][j] != NULL) 
      bin = map->A[i][j];
    
    i = a;
    j = b - n;
    if (j >= 0 && i >= 0 && i < map->n && map->A[i][j] != NULL) 
      bin = map->A[i][j];

    while (bin == NULL && ++k < n) {
      i = a + n;
      if (i < map->n) {
	j = b + k;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL) 
	  bin = map->A[i][j];
	j = b - k;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL)
	  bin = map->A[i][j];
      }

      j = b + n;
      if (j < map->m) {
	i = a + k;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL)
	  bin = map->A[i][j];
	i = a - k;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL) 
	  bin = map->A[i][j];
      }
    
      i = a - n;
      if (i >= 0) {
	j = b + k;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL)
	  bin = map->A[i][j];
	j = b - k;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL)
	  bin = map->A[i][j];
      }
    
      j = b - n;
      if (j >= 0) {
	i = a + k;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL)
	  bin = map->A[i][j];
	i = a - k;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL) 
	  bin = map->A[i][j];
      }
    }

    if (bin == NULL) {
      i = a + n;
      j = b + n;
      if (i < map->n && j >= 0 && j < map->m && map->A[i][j] != NULL)
	bin = map->A[i][j];

      i = a - n;
      j = b + n;
      if (j < map->m && i >= 0 && i < map->n && map->A[i][j] != NULL)
	bin = map->A[i][j];
    
      i = a - n;
      j = b - n;
      if (i >= 0 && j >= 0 && j < map->m && map->A[i][j] != NULL)
	bin = map->A[i][j];
    
      i = a + n;
      j = b - n;
      if (j >= 0 && i >= 0 && i < map->n && map->A[i][j] != NULL)
	bin = map->A[i][j];
    }

  }
  //printf("Se detuvo con n = %d y k = %d\t",n,k);

  /* bin apunta a un contenedor no vacio de grid */
  ans = closest_point(&(bin->puntos),p);
  map->cont.cont_dist += bin->puntos.elements + 1;
  d = dist(p,ans);

  m = n;
  l = k;

  do {
    if (l == 0) {
      i = a + m;
      j = b;
      if (i < map->n && j >= 0 && j < map->m && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }

      i = a;
      j = b + m;
      if (j < map->m && i >= 0 && i < map->n && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }
    
      i = a - m;
      j = b;
      if (i >= 0 && j >= 0 && j < map->m && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }
    
      i = a;
      j = b - m;
      if (j >= 0 && i >= 0 && i < map->n && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }
    }

    if (l > 0) l--;
    while (++l < m && ((m-1)*(m-1) + (l-1)*(l-1)) * map->delta * map->delta < d*d) {

      i = a + m;
      if (i < map->n) {
	j = b + l;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
	j = b - l;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
      }

      j = b + m;
      if (j < map->m) {
	i = a + l;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
	i = a - l;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
      }
    
      i = a - m;
      if (i >= 0) {
	j = b + l;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
	j = b - l;
	if (j >= 0 && j < map->m && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
      }
    
      j = b - m;
      if (j >= 0) {
	i = a + l;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
	i = a - l;
	if (i >= 0 && i < map->n && map->A[i][j] != NULL) {
	  aux = closest_point(&(map->A[i][j]->puntos),p);
	  map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	  if (dist(p,aux) < d) {
	    d = dist(aux,p);
	    ans = aux;
	  }
	}
      }
    }

    if (l == m && 2 * (m - 1)*(m - 1) * map->delta * map->delta < d * d) {
      i = a + n;
      j = b + n;
      if (i < map->n && j >= 0 && j < map->m && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }

      i = a - n;
      j = b + n;
      if (j < map->m && i >= 0 && i < map->n && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }
    
      i = a - n;
      j = b - n;
      if (i >= 0 && j >= 0 && j < map->m && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }
    
      i = a + n;
      j = b - n;
      if (j >= 0 && i >= 0 && i < map->n && map->A[i][j] != NULL) {
	aux = closest_point(&(map->A[i][j]->puntos),p);
	map->cont.cont_dist += map->A[i][j]->puntos.elements + 1;
	if (dist(p,aux) < d) {
	  d = dist(aux,p);
	  ans = aux;
	}
      }
    }
    l = 0;
  } while ( d > map->delta * m++ );
  return ans;
  }

contenedor *_grid_create_bin(float x0,float y0){
  contenedor *bin;
  bin = (contenedor*)malloc(sizeof(contenedor));
  bin->x0 = x0;
  bin->y0 = y0;
  bin->puntos.elements = 0;
  bin->puntos.size = 1;
  bin->puntos.a = (void**)malloc(sizeof(void*));
  bin->puntos.a[0] = NULL;
  return bin;
}

void grid_free(grid *map) {
  int i,j;
  for (i = 0;i < map->n;i++) {
    for (j = 0;j < map->m;j++) {
      if (map->A[i][j] != NULL) {
	free(map->A[i][j]->puntos.a);
	free(map->A[i][j]);
      }
    }
    free(map->A[i]);
  }
  free(map->A);
  free(map);
}

int sizeof_grid(grid *map) {
  int i,j,size;

  size = sizeof(map);
  size += sizeof(contenedor**) * map->n;
  size += sizeof(contenedor*) * map->n * map->m;
  for (i = 0;i < map->n;i++) {
    for (j = 0;j < map->m;j++) {
      if (map->A[i][j] != NULL) {
	size += sizeof_arrdin(&(map->A[i][j]->puntos));
	size += sizeof(contenedor);
      }
    }
  }
  
  return size;
}
