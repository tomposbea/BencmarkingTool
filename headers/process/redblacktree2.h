// color of a node
enum nodeColor {
    RED,
    BLACK
};

// structure of a node
struct rbNode {
    char data[STRINGLEN];
    int color;
    struct rbNode* link[2]; //left, right child
    int counter;
};

// root node
struct rbNode* root;

// functions
struct rbNode* createNode(char * data);
int insertion(char * data);
void deletion(char* data);
void delete_rbt_by_counter(struct rbNode *rb, int counter);
