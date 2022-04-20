#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../headers/defines.h"
#include "../headers/functions.h"
/*
struct DataItem {
   char data[STRINGLEN];
   long int key;
};

struct DataItem* hashArray[SIZE]; 
struct DataItem* dummyItem;
struct DataItem* item;
*/
long int hashCode(long int key) {
   return key % SIZE;
}

struct DataItem *search_in_hash(long int key) {
   long int hashIndex = hashCode(key);  
   
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
      if(hashArray[hashIndex]->key == key) return hashArray[hashIndex]; 
      ++hashIndex;
      hashIndex %= SIZE;
   }        
	
   return NULL;        
}

void insert_to_hash(long int key,char* data) {
   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   strcpy(item->data, data);  
   item->key = key;
   
   long int hashIndex = hashCode(key);

   //move in array until an empty or delete_from_hashd cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      ++hashIndex;
      hashIndex %= SIZE;
   }
	
   hashArray[hashIndex] = item;
}

struct DataItem* delete_from_hash(struct DataItem* item) {
   long int key = item->key;
   long int hashIndex = hashCode(key);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
      if(hashArray[hashIndex]->key == key) {
         struct DataItem* temp = hashArray[hashIndex]; 
         //assign a dummy item at delete_from_hashd position
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      ++hashIndex;
      hashIndex %= SIZE;
   }      
	
   return NULL;        
}

void empty_hash(){
    long int hashIndex=0;
    while(hashArray[hashIndex] != NULL) {
         //assign a dummy item at delete_from_hashd position
         hashArray[hashIndex] = dummyItem;
      }
      ++hashIndex;
      hashIndex %= SIZE;
}

void display_hash() {
   for(long int i = 0; i<SIZE; i++) {
      if(hashArray[i] != NULL)
         printf(" (%ld,%s)",hashArray[i]->key,hashArray[i]->data);
   }
   printf("\n");
}

long int make_key(char* val){
    long int count = 0;
    char str[STRINGLEN];
    memmove(str, val, STRINGLEN);
    for (long int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
    str[count] = '\0';

    long int newkey=atol(str);
    return newkey;
}
/*
int main() {
   dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
   strcpy(dummyItem->data,"000");  
   dummyItem->key = -1; 

   char a[10]="1 2 3", b[10]="4 6 76 43", c[10]="5 4 1";

   insert_to_hash(make_key(a), a);
   insert_to_hash(make_key(b), b);
   insert_to_hash(make_key(c), c);
   if(!search_in_hash(make_key(b))) insert_to_hash(make_key(b), b);
   else printf("Dup");

   display_hash();
   item = search_in_hash(make_key(b));
   if(item != NULL) printf("Element found, value: %s\n", item->data);
   else printf("Element not found\n");
}*/
