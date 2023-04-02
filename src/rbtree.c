#include "rbtree.h"
#include <stdlib.h>

test2

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->color = RBTREE_BLACK; // 루트니까 블랙으로 초기화
  p->nil  = newNode;
  p->root = newNode;
  return p;
}

void rotate_right(rbtree *t, node_t* target) 
{
  node_t* x = target->left;
  target->left = x->right;

  if (x->right != t->nil) 
  {
    x->right->parent = target;
  }

  x->parent = target->parent;
  if (target->parent == t->nil) {
    t->root = x;
  }
  else if (target == target->parent->left) {
    target->parent->left = x;
  }
  else {
    target->parent->right = x;
  }

  x->right = target;
  target->parent = x;
}

void rotate_left(rbtree *t, node_t* target) 
{
  node_t* y = target->right;
  target->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = target;
  }

  y->parent = target->parent;
  if (target->parent == t->nil) 
  {
    t->root = y;
  }
  else if (target == target->parent->left) 
  {
    target->parent->left = y;
  }
  else 
  {
    target->parent->right = y;
  }

  y->left = target;
  target->parent = y;
}

void rb_insert_fixup(rbtree *t, node_t * target) 
{
  while (target->parent->color == RBTREE_RED) 
  {
    if (target->parent == target->parent->parent->left)  // 대분류1. z.p는 조부모의 왼쪽 자식
    {

      node_t * y = target->parent->parent->right; // 삼촌 y
      if (y->color == RBTREE_RED) { // 경우 1: target의 삼촌 y가 적색
        target->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      }
      else
      {
        if (target == target->parent->right) // 경우 2: target의 삼촌 y가 흑색, target이 부모의 오른쪽 자식 -> 경우3으로 만듬
        {
          target = target->parent;
          rotate_left(t, target);
        }
        /* 경우 3: target의 삼촌 y가 흑색, target이 왼쪽 자식 */
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        rotate_right(t, target->parent->parent);
      }
    }

    else // 대분류2. z.p는 조부모의 오른쪽 자식
    {
      node_t * y = target->parent->parent->left; // 삼촌 y
      if (y->color == RBTREE_RED)  // 경우 1: target의 삼촌 y가 적색
      {
        target->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        target = target->parent->parent;
      }

      else 
      {
        if (target == target->parent->left)  // 경우 2: target의 삼촌 y가 흑색, taget이 부모의 왼쪽 자식 -> 경우3으로 만듬
        {
          target = target->parent;
          rotate_right(t, target);
        }
         /* 경우 3: target의 삼촌 y가 흑색, target이 오른쪽 자식 */
        target->parent->color = RBTREE_BLACK;
        target->parent->parent->color = RBTREE_RED;
        rotate_left(t, target->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t * z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  node_t * y = t->nil; // 부모노드 역할 
  node_t * x = t->root; // 부모를 끌고가는 역할 
  while (x != t->nil)  
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }

  z->parent = y;
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else 
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // 삽입되는 z는 적색
  rb_insert_fixup(t, z);
  
  return t->root;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* cur = t->root;
  while (cur != t->nil)
  {
    if (cur->key == key) 
      return cur;
    else if (cur->key < key) 
      cur = cur->right;
    else
      cur = cur->left;
  }
  return NULL;  
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
