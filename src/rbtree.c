#include "rbtree.h"
#include <stdlib.h>

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
  while (target->parent->color == RBTREE_RED) // 종료조건: 루프 종료시 target의 부모는 BLACK
  {
    if (target->parent == target->parent->parent->left)  // 대분류1. target.p는 조부모의 왼쪽 자식
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

// 자식부터 삭제해야지만 dangling pointer를 참조하는 것 방지, 따라서 postorder 사용
void postorder_delete_all_node(rbtree *t, node_t* root) {
  if (root == t->nil)
    return;
  
  if (root->left != t->nil) 
    postorder_delete_all_node(t, root->left);
  if (root->right != t->nil)
    postorder_delete_all_node(t, root->right);
  free(root);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  postorder_delete_all_node(t, t->root);

  free(t->nil); // sentinel도 메모리를 해제해줘야 함
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
  node_t *cur = t->root;
  while (cur->left != t->nil) 
  {
    cur = cur->left;
  }

  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while (cur->right != t->nil) 
  {
    cur = cur->right;
  }

  return cur;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if(u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  
  v->parent = u->parent;
}

/* erase에서 subtree의 min 찾는 역할
: BST에서 최소의 키를 가지는 원소를 찾으려면 nil만날때까지 루트로부터 왼쪽 자식을 계속 따라가기 */
node_t *tree_minimum(rbtree *t, node_t * root) 
{
  while (root->left != t->nil) 
  {
    root = root->left;
  }

  return root;
}

void rb_erase_fixup(rbtree *t, node_t* x)
{
  while (x != t->root && x->color == RBTREE_BLACK) // 루트가 아니며, doubly black인동안.
  {
    if (x == x->parent->left) // 대분류 1. fixup을 적용하는 x는 자신의 부모의 좌측자식
    {
      node_t* w = x->parent->right; // 형제

      if (w->color == RBTREE_RED) // 경우 1. 형제가 RED인경우 
      // 부모와 형제의 색을 바꾸고, 부모를 기준으로 왼쪽으로 회전한 후, doubly black기준으로 경우 2,3,4중 하나로 해결
      {
        w->color = RBTREE_BLACK;  
        x->parent->color = RBTREE_RED;
        rotate_left(t, x->parent);
        w = x->parent->right; // 새로운 형제
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) // 경우 2. 형제가 BLACK, 형제의 두 자녀 모두 BLACK
      // 형제와 나의 BLACK을 모아서 부모에게 전달(형제는 RED로 변화), 부모를 기준으로 extra black 다시 해결
      {
        w->color = RBTREE_RED;
        x = x->parent; // 부모에게 상황 위임
      }
      else {
        if (w->right->color == RBTREE_BLACK) // 경우 3. 형제가 BLACK, 형제의 우측자식이 BLACK, 좌측은 RED
        // 형제의 오른쪽 자녀를 RED로 만들어서 경우4를 적용시킬 준비
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_right(t, w);
          w = x->parent->right;
        }

        // 경우 4. 형제가 BLACK, 형제의 오른쪽 자녀가 RED
        w->color = x->parent->color;  // 형제의 색깔은 부모의 색으로
        x->parent->color = RBTREE_BLACK; // 형제의 오른쪽 자녀와 부모를 BLACK으로 
        w->right->color = RBTREE_BLACK;
        rotate_left(t, x->parent); // 부모를 기준으로 왼쪽으로 회전
        x = t->root;
      }
    }
    else // 대분류 2. fixup을 적용하는 x는 자신의 부모의 우측자식 - 대분류1과 왼쪽 오른쪽이 반대
    {
      node_t* w = x->parent->left;
      if (w->color == RBTREE_RED) 
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_right(t, x->parent);
        w = x->parent->left;
      }
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK))
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        w->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t * y = z;
  color_t y_original_color = y->color;
  node_t* x;

  if (z->left == t->nil) // 좌측이 nil(우측도 nil일수 있음)
  {
    x = z->right;
    rb_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) // 좌측자식 하나 - 위 두가지경우에는 삭제되는 색 = 삭제되는 노드의 색
  {
    x = z->left;
    rb_transplant(t, z, z->left);
  }
  else // 자식 둘 - 삭제되는 색 = 삭제되는 노드의 successor의 색
  {
    node_t * y = tree_minimum(t, z->right); // y가 successor(왼쪽서브트리에서 가장 작은 값)
    y_original_color = y->color;
    x = y->right; // 이 경우에도 successor의 자리를 대체하는 x에 대해 fixup을 돌려줘야 한다
    if (y->parent == z) 
    {
      x->parent = y;
    }
    else  /* 우측서브트리가 남기때문에(y가 z.right 서브트리의 유일 원소가 아니기에), 남은 서브트리에 대한 작업 필요 */
    {
      rb_transplant(t, y, y->right); // y의 우측자식 y자리로 옮겨주고
      y->right = z->right;  // y의 우측자식을 z의 우측서브트리로 둠
      y->right->parent = y;
    }
    rb_transplant(t, z, y); 
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) 
    rb_erase_fixup(t, x);
  
  free(z);
  return 0;
}

void inorder(const rbtree* t, node_t* root, key_t * arr, key_t * idx) 
{
  if (root != t->nil) // 센티넬을 사용하기에 nil이 더이상 값이 없음을 나타낸다
  {
    inorder(t, root->left, arr, idx);
    arr[(*idx)++] = root->key;
    inorder(t, root->right, arr, idx);
  }

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  key_t idx = 0;
  
  inorder(t, t->root, arr, &idx);

  return 0;
}
