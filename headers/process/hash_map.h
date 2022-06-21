// Structure for hash map
struct DataItem {
   char data[STRINGLEN];
   long int key;
};

struct DataItem* hashArray[SIZE];
struct DataItem* dummyItem;
struct DataItem* item;

//hash_map.c
struct DataItem *search_in_hash(long int key);
void insert_to_hash(long int key,char* data);
struct DataItem* delete_from_hash(struct DataItem* item);
void display_hash();
long int make_key(char* val);
void empty_hash();
void InitDummyItem();
