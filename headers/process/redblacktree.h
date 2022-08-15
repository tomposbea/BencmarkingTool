// Structure to represent each node in a red-black tree
struct node {
    char d[STRINGLEN]; //data
    int c; // 1-red, 0-black
    struct node* p; // parent
    struct node* r; // right-child
    struct node* l; // left child
    int counter;
};

// red-black tree root node
struct node* root_node;

//redblacktree.c
struct node* insert(struct node* trav, struct node * temp);
void fixup(struct node* root, struct node* pt);
void left_rotate(struct node* temp);
void right_rotate(struct node* temp);
int search(struct node* trav, char* instring);
void delete_tree(struct node* trav);
void InitRBTree();
