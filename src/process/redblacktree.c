// Red and black search tree: init, insert, search, delete

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/process/redblacktree.h"

// initialize root node
void InitRBTree() {
        root_node=NULL;
}

// insert new node: temp, return root: trav
struct node* insert(struct node* trav, struct node * temp){
	// tree is empty, the new node is the root
	if(!(trav->d)) return temp;

	// new node is already in the tree
	if(strcmp(temp->d, trav->d)==0) return temp;
	
	//go left if smaller than trav, right if bigger
	//set parent of left/right node as current node
	if(strcmp(temp->d, trav->d)<0)
    	{
        	trav->l = insert(trav->l, temp);
        	trav->l->p = trav;
    	}
	else if(strcmp(temp->d, trav->d)>0)
	{
        	trav->r = insert(trav->r, temp);
        	trav->r->p = trav;
    	}
	
	//return unchange parent node
	return trav;
}

// rotate tree to the right
void right_rotate(struct node* temp){
	struct node* left = temp->l;
    	
	temp->l = left->r;	
	if (temp->l) 
		temp->l->p = temp;
    	
	left->p = temp->p;
    	if (!temp->p) 
		root_node = left;
    	else if (temp == temp->p->l) 
		temp->p->l = left;
    	else 
		temp->p->r = left;
    	
	left->r = temp;
    	temp->p = left;
}

// rotate tree to the left
void left_rotate(struct node* temp){
	struct node* right = temp->r;
    	
	temp->r = right->l;
    	if (temp->r) 
		temp->r->p = temp;
    	
	right->p = temp->p;
    	if (!temp->p) 
		root_node = right;
    	else if (temp == temp->p->l) 
		temp->p->l = right;
    	else 
		temp->p->r = right;
    	
	right->l = temp;
    	temp->p = right;
}

//recolor, rotate to right position after insertion
//color: 1-red, 0-black, new node is always red, but two adjacent nodes can't be red
void fixup(struct node* root, struct node* pt){
	struct node* parent_pt = NULL;
    	struct node* grand_parent_pt = NULL;
 
	// find parent and grandparent node of pt
    	while ((pt != root) && (pt->c != 0) && (pt->p->c == 1)){
        	parent_pt = pt->p;
        	grand_parent_pt = pt->p->p;
 
        //Case A: Parent of pt is left child of Grand-parent
        if (parent_pt == grand_parent_pt->l)
        {
            struct node* uncle_pt = grand_parent_pt->r;
            // Case 1: The uncle of pt is also red -> only Recoloring required
            if (uncle_pt != NULL && uncle_pt->c == 1)
            {
                grand_parent_pt->c = 1;
                parent_pt->c = 0;
                uncle_pt->c = 0;
                pt = grand_parent_pt;
            } else {
                //Case 2: pt is right child of its parent -> left-rotation required
                if (pt == parent_pt->r) {
                    left_rotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
 
                // Case 3: pt is left child of its parent -> right-rotation required
                right_rotate(grand_parent_pt);
                int t = parent_pt->c;
                parent_pt->c = grand_parent_pt->c;
                grand_parent_pt->c = t;
                pt = parent_pt;
            }
        }
 
        //Case B: Parent of pt is right child of Grand-parent
        else {
            struct node* uncle_pt = grand_parent_pt->l;
 
            //  Case 1: The uncle of pt is also red -> only Recoloring required
            if ((uncle_pt != NULL) && (uncle_pt->c == 1))
            {
                grand_parent_pt->c = 1;
                parent_pt->c = 0;
                uncle_pt->c = 0;
                pt = grand_parent_pt;
            }
            else { //if(uncle_pt!=NULL) {
                // Case 2: pt is left child of its parent -> right-rotation required
                if (pt == parent_pt->l) {
                    right_rotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
                // Case 3: pt is right child of its parent -> left-rotation required
                if(uncle_pt!=NULL) {
			left_rotate(grand_parent_pt);
                	int t = parent_pt->c;
                	parent_pt->c = grand_parent_pt->c;
                	grand_parent_pt->c = t;
                	pt = parent_pt;
		}
            }
        }
    }
    root->c = 0;
}

// delete tree using inorder traversal
void delete_tree(struct node* trav){
	if(trav == NULL) return;
	
	// if node has a value
	if(trav){
		// traverse to left and right child
		delete_tree(trav->l);
		delete_tree(trav->r);

		// free node
		trav=NULL;
		free(trav);
	}
}

// search for instring in tree starting from trav(root)
// return: 1 - found it, 0 - not in tree
int search(struct node* trav, char* instring) {
	if(trav){
		search(trav->l, instring); // traverse left
		
		//compare instring to current node
		if(!strcmp(trav->d, instring)) { 
			return 1;
		} 
		search(trav->r, instring); // traverse right
	} else return 0;
}
