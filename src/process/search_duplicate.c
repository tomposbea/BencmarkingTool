#ifdef __linux__
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/sysinfo.h>
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>      /* Character Type Classification */
#include <inttypes.h>
#include <time.h>
#include <math.h>

#include "../../headers/defines.h"
#include "../../headers/process/binary_search_tree.h"
#include "../../headers/process/redblacktree.h"
#include "../../headers/process/hash_map.h"
#include "../../headers/process/lcm.h"
#include "../../headers/process/search_duplicate.h"
#include "../../headers/process/redblacktree2.h"

#include "../../headers/generate/generate.h"
#include "../../headers/generate/conversions.h"

#if defined(__linux__)
#include "../../headers/def_linux.h"
#else
#include "../../headers/def_windows.h"
#endif

// RB tree by Zsigmond
void* SearchInRBTree2(void* str) {
        if (NULL == str) {
                printf("OOPs. RB Tree zero parameter.\n");
                return NULL;
        }

        int ret = insertion(str);
        if (ret == 0) {
                // Duplicate found/
		tree = 1;
                found_tree++;
        } else {
		tree = 0;
	}
        return NULL;
}

// if matrix not in red and black tree, a new node is created for it
void *SearchInRBTree(void *str){
        char *instring = str;
        if(!search(root_node, instring)) {
                struct node* temp = (struct node*)malloc(sizeof(struct node));
                temp->r = NULL;
                temp->l = NULL;
                temp->p = NULL;
                int len = strlen(instring);
                strncpy(temp->d, instring, len);
                temp->c = 1;
                root_node = insert(root_node, temp);
                fixup(root_node, temp);
                tree=0;
        } else {
                tree=1;
                found_tree++;
        }
}

void InsertToTable( char *instring) {
        // Critical section
        int oldbusy = 1;            // Always check busy flag
        while (oldbusy) {
                oldbusy = AtomicExhange(&TableBusy, 1); // Read TableBusy AND write 1 to it.
                if (oldbusy) {          // Check if ours
                        // No. Someone is already in the loop.
                        while (AtomicLoad(&TableBusy)) SleepUni(0); // Table is Busy. Wait.
                        oldbusy = 1;        // Table is free, but there is a race for it. Try to catch again.
                }
        }

        // Write to table
        strncpy_s(Table[TablePtr], STRINGLEN, instring, STRINGLEN);
        // Set pointer
        TablePtr = (TablePtr + 1) % table_size;

        // Step counter.
        ProcessedCtrl++;
        // Release table
        AtomicExhange(&TableBusy, 0);
}

// searching for matrix in table, insert if not found
// found_table: number of duplicates in the table
// table: 0 - matrix not found, 1 - matrix found
void *SearchInTable(void *str){
         //char *instring = str;

         if (FindDuplicate(str) != -1) { //duplicate
                table=1;
                found_table++;
         } else { //not found
                 table=0;
                 InsertToTable(str);
         }
}

// searching for matrix in hash table, insert if not found
// found_hash: number of duplicates in the hash table
// hash: 0 - matrix not found, 1 - matrix found
void *SearchInHashTable(void *str){
        if(!search_in_hash(make_key(str))) { //not found
                insert_to_hash(make_key(str), str);
                hash=0;
        } else { //duplicate
                hash=1;
                found_hash++;
        }
}

// searching for matrix in binary search tree, insert if not found
// found_bstree: number of duplicates in the hash table
// bstree: 0 - matrix not found, 1 - matrix found
void *SearchInBSTree(void *str) {
        if(search_bs(bsroot, str)){ //found it, duplicate
                bstree = 1;
                found_bstree++;
        } else { //not found
                bsroot = insert_bs(bsroot, str);
                bstree = 0;
        }
}

