// Binary Search Tree: insert, search, delete, create new node, init

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../headers/defines.h"
#include "../../headers/process/binary_search_tree.h"

// initialize root node
void InitBSTree() {
        bsroot = NULL;
}

// Create a bstree element and initialize it with a char array
struct bstree *newNode(char* item) {
  // allocate space for a new bstree element, it's size is the bstree struct size
  struct bstree *temp = (struct bstree *)malloc(sizeof(struct bstree));
  
  // copy data into bstree struct, left and right substrees/children are set to NULL
  strcpy(temp->data, item);
  temp->left = temp->right = NULL;
  temp->counter = Counter;

  // return new bstree node
  return temp;
}

// search for item in tree in an inorder traversal method
// return: true - found it, false - item not in bstree
bool search_bs(struct bstree *bsroot, char* item) {
  if (bsroot != NULL) {	  
    // traverse left
    if(search_bs(bsroot->left, item)) return true;
    
    // check current node value
    if(!strcmp(bsroot->data,item)) return true;
    
    // traverse right
    if( search_bs(bsroot->right, item)) return true;
  
  } else return false;
}

// Insert a bstree element, return new element
struct bstree *insert_bs(struct bstree *bstree, char* data) {
  //create a bstree from data and return it
  if (bstree == NULL) return newNode(data); 

  // use inorder transversal to go to the right place (left if smaller, right if bigger)
  if(strcmp(data, bstree->data)<0)
    bstree->left = insert_bs(bstree->left, data);
  else
    bstree->right = insert_bs(bstree->right, data);

  // return current node
  return bstree;
}

// find and return the inorder successor
struct bstree *minValueNode(struct bstree *bstree) {
  struct bstree *current = bstree;

  // find the leftmost leaf
  while (current && current->left != NULL)
    current = current->left;

  return current;
}

// delete a bstree node
struct bstree *delete_bs(struct bstree *bsroot, char* data) {
  if (bsroot == NULL) return bsroot;

  // find the bstree to be deleted using inorder inorder
  if(strcmp(bsroot->data, data) < 0) // go left
    bsroot->left = delete_bs(bsroot->left, data);
  else if(strcmp(bsroot->data, data) > 0) // go right
    bsroot->right = delete_bs(bsroot->right, data);

  // node found
  else {

    // if the bstree has one or none children
    if (bsroot->left == NULL) { // no left child
      
      // free node and return its right chile
      struct bstree *temp = bsroot->right;
      bsroot = NULL;
      free(bsroot);
      return temp;
    
    } else if (bsroot->right == NULL) { // no right child
    
      // free node and return its left child
      struct bstree *temp = bsroot->left;
      bsroot = NULL;
      free(bsroot);
      return temp;
    
    } else {
    // if the bstree has two children replace it with inorder successor (smallest right) and delete inorder
      
      // search for smallest child
      struct bstree *temp = minValueNode(bsroot->right);
      
      // replace node with smallest child
      strcpy(bsroot->data, temp->data);

      // link right child to new node
      bsroot->right = delete_bs(bsroot->right, temp->data);
    }
  }

  // return root of new tree
  return bsroot;
}

// delete all nodes where counter matches
void delete_bst_by_counter(struct bstree *bsroot, int counter) {
	// inorder traversal of bst
	if(bsroot != NULL) {
		//traverse left
		delete_bst_by_counter(bsroot->left, counter);

		//delete node if its counter matches
		if(bsroot->counter == counter && bsroot != NULL) {
			//printf("%d:%s \t", bsroot->counter, bsroot->data);

			// find node in delete function by its unique data
			delete_bs(bsroot, bsroot->data);
		}

		//traverse right
		delete_bst_by_counter(bsroot->right, counter);
	}

}
