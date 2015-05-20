/* B-tree.h */

/***********************************************************************
 * Author: Luis Maltos
 * Clase: Estructura de Datos en C++
 * Implementacion de arboles B
 * Se implementa:
 *  find, create, insert, remove,
***********************************************************************/

#ifndef _B_TREE_H
#define _B_TREE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct B_node B_node;
typedef struct B_tree B_tree;

struct B_tree {
  /* B-tree */
  int t;
  /* Parameter for number of keys in nodes between [t-1,2*t - 1] */
  struct B_node *root;
  /* pointer to the root node */
  int (*fcmp)(void *info, const void *key1, const void *key2);
  /* application-defined key comparison routine */
  void *info;
  /* transit pointer passed to the routine fcmp */
  int size;
  /* the tree size (the total number of nodes) */
};

struct B_node {
  /* node of a B-tree */
  int n;
  /* number of keys */
  const void *key;
  /* pointer to the node array of keys */
  short int leaf;
  /* node flag:
     0 - this node is a leaf
     1 - this node isn't a leaf */
  B_node **c;
  /* pointer to the node array of subtrees */
};

B_tree* B_tree_create_tree
(int t,int (*fcmp)(void *info,const void *key1,const void *key2));
/* create B-tree */

B_node *B_tree_insert_key(B_tree *T,const void *key);
/* insert new node into B-tree */

B_node *B_tree_find_node(B_node *x,const void *key);
/* find node in B-tree */

const void *B_tree_predecessor(B_node *x,const void *key);
/* find the predecessor node in B-tree */

const void *B_tree_successor(B_node *x,const void *key);
/* find the successor node in B-tree */

void B_tree_remove_key(B_tree *T,const void *key);
/* remove key from B-tree */

void B_tree_delete_tree(B_tree *T);
/* delete B-tree */

void gnuplot(B_tree *T);
/* plots the B-tree from gnuplot */

#endif

/* eof */
