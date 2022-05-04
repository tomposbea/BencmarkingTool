#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

struct node* insert(struct node* trav, struct node * temp){
	if(!(trav->d)) return temp;
	if(strcmp(temp->d, trav->d)==0) return temp;
	//go left if smaller than trav, right if bigger
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

void right_rotate(struct node* temp){
	struct node* left = temp->l;
    	temp->l = left->r;
    	if (temp->l) temp->l->p = temp;
    	left->p = temp->p;
    	if (!temp->p) root_node = left;
    	else if (temp == temp->p->l) temp->p->l = left;
    	else temp->p->r = left;
    	left->r = temp;
    	temp->p = left;
}

void left_rotate(struct node* temp){
	struct node* right = temp->r;
    	temp->r = right->l;
    	if (temp->r) temp->r->p = temp;
    	right->p = temp->p;
    	if (!temp->p) root_node = right;
    	else if (temp == temp->p->l) temp->p->l = right;
    	else temp->p->r = right;
    	right->l = temp;
    	temp->p = right;
}

//recolor, rotate after insertion
void fixup(struct node* root, struct node* pt){
	struct node* parent_pt = NULL;
    	struct node* grand_parent_pt = NULL;
 
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
            else if(uncle_pt!=NULL) {
                // Case 2: pt is left child of its parent -> right-rotation required
                if (pt == parent_pt->l) {
                    right_rotate(parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->p;
                }
                // Case 3: pt is right child of its parent -> left-rotation required
                left_rotate(grand_parent_pt);
                int t = parent_pt->c;
                parent_pt->c = grand_parent_pt->c;
                grand_parent_pt->c = t;
                pt = parent_pt;
            }
        }
    }
    root->c = 0;
}

void delete_tree(struct node* trav){
	if(trav){
		delete_tree(trav->l);
		delete_tree(trav->r);
		trav=NULL;
	}
}

int search(struct node* trav, char* instring) {
	if(trav){
		search(trav->l, instring);
		if(!strcmp(trav->d, instring)) {
			return 1;
		}
		search(trav->r, instring);
	} else return 0;
}
