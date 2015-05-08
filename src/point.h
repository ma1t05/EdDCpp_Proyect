
#ifndef _POINT_H_
#define _POINT_H_

struct point {
  float x,y;
};

typedef struct point point;

float dist(point*,point*);
int read_points_file(const char*,point**);

#endif
