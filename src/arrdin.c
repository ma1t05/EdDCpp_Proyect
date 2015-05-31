
#include "arrdin.h"

arrdin* arrdin_create() {
  arrdin *c;
  c = (arrdin*)malloc(sizeof(arrdin));
  c->elements = 0;
  c->size = 1;
  c->a = (void**)malloc(sizeof(void*));
  c->a[0] = NULL;
  return c;
}

void* arrdin_get(arrdin *c,int i){
  if (i >= c->size) return NULL;
  return c->a[i];
}

void arrdin_add(arrdin* c,void *e){
  int i;

  if (c->elements == c->size)
    _arrdin_grow(c);

  c->elements++;
  for (i = 0;i < c->size;i++) {
    if (c->a[i] == NULL) {
      c->a[i] = e;
      return;
    }
  }

}

void arrdin_remove(arrdin *c,void *e) {
  int i;
  for (i = 0;i < c->size;i++) {
    if (c->a[i] == e) {
      c->a[i] = NULL;
      c->elements--;
      return;
    }
  }
}

void arrdin_free(arrdin *c) {
  free(c->a);
  free(c);
}

void _arrdin_grow(arrdin* c) {
  int i;
  void **b;
  b = (void**)malloc(sizeof(void*) * c->size * 2); // Reservar mas memoria
  for (i = 0;i < c->size;i++) {
    b[i] = c->a[i]; // Copiar elementos
    b[i+c->size] = NULL;
  }
  free(c->a); // Liberar memoria
  c->a = b;
  c->size *= 2;
}
