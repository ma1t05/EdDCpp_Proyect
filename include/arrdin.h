
#ifndef _ARRDIN_H_
#define _ARRDIN_H_

#include <stdlib.h>

struct arrdin {
  void **a;
  int elements;
  int size;
};

typedef struct arrdin arrdin;

arrdin* arrdin_create();
void* arrdin_get(arrdin*,int);
void arrdin_add(arrdin*,void*);
void arrdin_remove(arrdin*,void*);
void arrdin_free(arrdin*);
void _arrdin_grow(arrdin*);
int sizeof_arrdin(arrdin*);

#endif
