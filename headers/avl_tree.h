#include <stdio.h>
#include <stdlib.h>

// Create Node
struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int weight;   //weight avl
    float value; //weight of input
    int flag;   //flag for update

};
//TODO implement flag for update
typedef struct my_avl_tree {
    struct Node *root;
    int size;

} avl_tree, *Avl_tree;

int max(int a, int b);

int weight(struct Node *N);

struct Node *newNode(int key, float value);

struct Node *rightRotate(struct Node *y);

struct Node *leftRotate(struct Node *x);

int getBalance(struct Node *N);

struct Node *insertNode(struct Node *node, int key, float value);

struct Node *minValueNode(struct Node *node);

struct Node *deleteNode(struct Node *root, int key);

void printPreOrder(struct Node *root);

void avl_destroy(Avl_tree root);

