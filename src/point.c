
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "point.h"

float dist(point *a,point *b) {
  return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

int read_points_file(const char* filename,point **p){
  int k,d,i;
  float s;
  FILE *points_file;
  point *puntos;
  /* Format
     k S
     x1 y1 d1
     ...
     xk yk dk     
   */
  points_file = fopen(filename,"r");
  if (points_file == NULL) {
    printf("Error al abrir el archivo\n");
    return 0;
  }
  fscanf(points_file,"%d",&k);
  fscanf(points_file,"%f",&s);
  puntos = (point*)malloc(sizeof(point)*k);
  for (i = 0;i < k;i++) {
    fscanf(points_file,"%f",&(puntos[i].x));
    fscanf(points_file,"%f",&(puntos[i].y));
    fscanf(points_file,"%d",&d);
  }
  fclose(points_file);
  *p = puntos;
  return k;
}
