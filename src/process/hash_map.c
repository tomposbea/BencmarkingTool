// Hash map: insert, search, make key out of an array, delete item, delete hash map
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/defines.h"
#include "../../headers/process/hash_map.h"

// initialize replacer for deleted items
// key=-1, data=000
void InitDummyItem(){
        dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
        strcpy(dummyItem->data,"000");
        dummyItem->key = -1;
}

// calculate hash code based on the key of an entry
long int hashCode(long int key) {
   return key % SIZE;
}

// search in hash map based on key
struct DataItem *search_in_hash(long int key) {
   // get the index of the searched element
   long int hashIndex = hashCode(key);  
   
   //iterate through hash table
   while(hashArray[hashIndex] != NULL) {
      // return found element
      if(hashArray[hashIndex]->key == key) 
	      return hashArray[hashIndex]; 
      ++hashIndex;
      hashIndex %= SIZE;
   }        
   
   // NULL if element is not found
   return NULL;        
}

// insert new entry to hash table, known key + data
void insert_to_hash(long int key,char* data) {
   // allocate memory for the new item and initialize it with the parameters
   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   strcpy(item->data, data);  
   item->key = key;
   
   // calculate index of new entry
   long int hashIndex = hashCode(key);

   //move in array until an empty or delete_from_hashd cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      ++hashIndex;
      hashIndex %= SIZE;
   }
	
   // place new entry
   hashArray[hashIndex] = item;
}

// delete item from hash map
struct DataItem* delete_from_hash(struct DataItem* item) {
   // get key and index of item
   long int key = item->key;
   long int hashIndex = hashCode(key);

   //iterate in array until item is found
   while(hashArray[hashIndex] != NULL) {
      if(hashArray[hashIndex]->key == key) {
	 //select value of item
         struct DataItem* temp = hashArray[hashIndex]; 
         //assign a dummy item at delete_from_hashd position
         hashArray[hashIndex] = dummyItem; 
	 // return old value of item
         return temp;
      }
	
      //go to next element of hash map      
      ++hashIndex;
      hashIndex %= SIZE;
   }      
   
   // return NULL if not found
   return NULL;        
}

// delete all entries of hash map
void empty_hash(){
    // iterate through hash map
    long int hashIndex=0;
    while(hashArray[hashIndex] != NULL) {
         //assign a dummy item at delete_from_hashd position
         hashArray[hashIndex] = dummyItem;
      }
      ++hashIndex;
      hashIndex %= SIZE;
}

// print non NULL hash map entries: (key,data)
void display_hash() {
   for(long int i = 0; i<SIZE; i++) {
      if(hashArray[i] != NULL)
         printf(" (%ld,%s)",hashArray[i]->key,hashArray[i]->data);
   }
   printf("\n");
}

// make hash key out of a value, return the new key
long int make_key(char* val){
    long int count = 0;

    // str will be the new key
    char str[STRINGLEN];
    memmove(str, val, STRINGLEN);

    // delete spaces from str
    for (long int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];

    // trailing zero at the end
    str[count] = '\0';

    // convert to long int
    long int newkey=atol(str);
    return newkey;
}
