// Implementing Red-Black Tree in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/process/redblacktree2.h"

/*enum nodeColor {
    RED,
    BLACK
};

struct rbNode {
    char data[STRINGLEN];
    int color;
    struct rbNode* link[2];
    int counter;
};

struct rbNode* root = NULL;
*/

// Create a red-black tree
struct rbNode* createNode(char * data) {
    struct rbNode* newnode;
    newnode = (struct rbNode*)malloc(sizeof(struct rbNode));
    strncpy(newnode->data, data, STRINGLEN);
    newnode->color = RED;
    newnode->link[0] = newnode->link[1] = NULL;
    newnode->counter = Counter;
    return newnode;
}

// Insert an node
int insertion(char * data) {
    struct rbNode* stack[98], * ptr, * newnode, * xPtr, * yPtr;
    int dir[98], ht = 0, index;
    ptr = root;
    if (!root) {
        root = createNode(data);
        return 1;
    }

    stack[ht] = root;
    dir[ht++] = 0;
    while (ptr != NULL) {
        index = strcmp(ptr->data, data);
        if (index == 0) {
            return 0;
        }
        index = (index < 0) ? 1 : 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }
    stack[ht - 1]->link[index] = newnode = createNode(data);
    while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
        if (dir[ht - 2] == 0) {
            yPtr = stack[ht - 2]->link[1];
            if (yPtr != NULL && yPtr->color == RED) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            }
            else {
                if (dir[ht - 1] == 0) {
                    yPtr = stack[ht - 1];
                }
                else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[1];
                    xPtr->link[1] = yPtr->link[0];
                    yPtr->link[0] = xPtr;
                    stack[ht - 2]->link[0] = yPtr;
                }
                xPtr = stack[ht - 2];
                xPtr->color = RED;
                yPtr->color = BLACK;
                xPtr->link[0] = yPtr->link[1];
                yPtr->link[1] = xPtr;
                if (xPtr == root) {
                    root = yPtr;
                }
                else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
        else {
            yPtr = stack[ht - 2]->link[0];
            if ((yPtr != NULL) && (yPtr->color == RED)) {
                stack[ht - 2]->color = RED;
                stack[ht - 1]->color = yPtr->color = BLACK;
                ht = ht - 2;
            }
            else {
                if (dir[ht - 1] == 1) {
                    yPtr = stack[ht - 1];
                }
                else {
                    xPtr = stack[ht - 1];
                    yPtr = xPtr->link[0];
                    xPtr->link[0] = yPtr->link[1];
                    yPtr->link[1] = xPtr;
                    stack[ht - 2]->link[1] = yPtr;
                }
                xPtr = stack[ht - 2];
                yPtr->color = BLACK;
                xPtr->color = RED;
                xPtr->link[1] = yPtr->link[0];
                yPtr->link[0] = xPtr;
                if (xPtr == root) {
                    root = yPtr;
                }
                else {
                    stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                }
                break;
            }
        }
    }
    root->color = BLACK;
    return 1;
}

// Delete a node
void deletion(char* data) {
    struct rbNode* stack[98], * ptr, * xPtr, * yPtr;
    struct rbNode* pPtr, * qPtr, * rPtr;
    int dir[98], ht = 0, diff, i;
    enum nodeColor color;

    if (!root) {
        printf("Tree not available\n");
        return;
    }

    ptr = root;
    while (ptr != NULL) {
        diff = strcmp(ptr->data, data);
        if (diff == 0)
            break;
        diff = (diff < 0) ? 1 : 0;
        stack[ht] = ptr;
        dir[ht++] = diff;
        ptr = ptr->link[diff];
    }

    if (ptr != NULL) {
    if (ptr->link[1] == NULL) {
        if ((ptr == root) && (ptr->link[0] == NULL)) {
            ptr = NULL;
	    free(ptr);
            root = NULL;
        }
        else if (ptr == root) {
            root = ptr->link[0];
            ptr = NULL;
	    free(ptr);
        }
        else {
            stack[ht - 1]->link[dir[ht - 1]] = ptr->link[0];
        }
    }
    else {
        xPtr = ptr->link[1];
        if (xPtr->link[0] == NULL) {
            xPtr->link[0] = ptr->link[0];
            color = xPtr->color;
            xPtr->color = ptr->color;
            ptr->color = color;

            if (ptr == root) {
                root = xPtr;
            }
            else {
                stack[ht - 1]->link[dir[ht - 1]] = xPtr;
            }

            dir[ht] = 1;
            stack[ht++] = xPtr;
        }
        else {
            i = ht++;
            while (1) {
                dir[ht] = 0;
                stack[ht++] = xPtr;
                yPtr = xPtr->link[0];
                if (!yPtr->link[0])
                    break;
                xPtr = yPtr;
            }

            dir[i] = 1;
            stack[i] = yPtr;
            if (i > 0)
                stack[i - 1]->link[dir[i - 1]] = yPtr;

            yPtr->link[0] = ptr->link[0];

            xPtr->link[0] = yPtr->link[1];
            yPtr->link[1] = ptr->link[1];

            if (ptr == root) {
                root = yPtr;
            }

            color = yPtr->color;
            yPtr->color = ptr->color;
            ptr->color = color;
        }
    }
    }

    if (ht < 1)
        return;

    if ( ptr != NULL && ptr->color == BLACK) {
        while (1) {
            pPtr = stack[ht - 1]->link[dir[ht - 1]];
            if (pPtr && pPtr->color == RED) {
                pPtr->color = BLACK;
                break;
            }

            if (ht < 2)
                break;

            if (dir[ht - 2] == 0) {
                rPtr = stack[ht - 1]->link[1];

                if (!rPtr)
                    break;

                if (rPtr->color == RED) {
                    stack[ht - 1]->color = RED;
                    rPtr->color = BLACK;
                    stack[ht - 1]->link[1] = rPtr->link[0];
                    rPtr->link[0] = stack[ht - 1];

                    if (stack[ht - 1] == root) {
                        root = rPtr;
                    }
                    else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    dir[ht] = 0;
                    stack[ht] = stack[ht - 1];
                    stack[ht - 1] = rPtr;
                    ht++;

                    rPtr = stack[ht - 1]->link[1];
                }

		if ( rPtr != NULL) {
                if ( (!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
                    (!rPtr->link[1] || rPtr->link[1]->color == BLACK)) {
                    rPtr->color = RED;
                }
                else {
                    if (!rPtr->link[1] || rPtr->link[1]->color == BLACK) {
                        qPtr = rPtr->link[0];
                        rPtr->color = RED;
                        qPtr->color = BLACK;
                        rPtr->link[0] = qPtr->link[1];
                        qPtr->link[1] = rPtr;
                        rPtr = stack[ht - 1]->link[1] = qPtr;
                    }
                    rPtr->color = stack[ht - 1]->color;
                    stack[ht - 1]->color = BLACK;
                    rPtr->link[1]->color = BLACK;
                    stack[ht - 1]->link[1] = rPtr->link[0];
                    rPtr->link[0] = stack[ht - 1];
                    if (stack[ht - 1] == root) {
                        root = rPtr;
                    }
                    else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    break;
                }
            }
            else {
                rPtr = stack[ht - 1]->link[0];
                if (!rPtr)
                    break;

                if (rPtr->color == RED) {
                    stack[ht - 1]->color = RED;
                    rPtr->color = BLACK;
                    stack[ht - 1]->link[0] = rPtr->link[1];
                    rPtr->link[1] = stack[ht - 1];

                    if (stack[ht - 1] == root) {
                        root = rPtr;
                    }
                    else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    dir[ht] = 1;
                    stack[ht] = stack[ht - 1];
                    stack[ht - 1] = rPtr;
                    ht++;

                    rPtr = stack[ht - 1]->link[0];
                }
                if ((!rPtr->link[0] || rPtr->link[0]->color == BLACK) &&
                    (!rPtr->link[1] || rPtr->link[1]->color == BLACK)) {
                    rPtr->color = RED;
                }
                else {
                    if (!rPtr->link[0] || rPtr->link[0]->color == BLACK) {
                        qPtr = rPtr->link[1];
                        rPtr->color = RED;
                        qPtr->color = BLACK;
                        rPtr->link[1] = qPtr->link[0];
                        qPtr->link[0] = rPtr;
                        rPtr = stack[ht - 1]->link[0] = qPtr;
                    }
                    rPtr->color = stack[ht - 1]->color;
                    stack[ht - 1]->color = BLACK;
                    rPtr->link[0]->color = BLACK;
                    stack[ht - 1]->link[0] = rPtr->link[1];
                    rPtr->link[1] = stack[ht - 1];
                    if (stack[ht - 1] == root) {
                        root = rPtr;
                    }
                    else {
                        stack[ht - 2]->link[dir[ht - 2]] = rPtr;
                    }
                    break;
                }
            }
	    }
            ht--;
        }
    }
}

// delete all nodes where counter matches
void delete_rbt_by_counter(struct rbNode *rb, int counter) {
	if(rb != NULL) {
		//traverse left
		delete_rbt_by_counter(rb->link[0], counter);

		if(rb->counter == counter && rb != NULL) {
			//printf("%d:%s \t", rb->counter, rb->data);
			//printf("%d ", rb->counter);
			deletion(rb->data);
		}

		// traverse right
		delete_rbt_by_counter(rb->link[1], counter);
	}
}

