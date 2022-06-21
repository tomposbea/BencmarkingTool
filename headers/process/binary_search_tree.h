#include<stdbool.h>

// Binary Search Tree
struct bstree {
  char data[STRINGLEN];
  struct bstree *left, *right;
};
struct bstree *bsroot;

//binary_search_tree.c
void InitBSTree();
bool search_bs(struct bstree *bsroot, char* item);
struct bstree *insert_bs(struct bstree *bstree, char* data);
struct bstree *delete_bs(struct bstree *bsroot, char* data);
