#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Create Node
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int weight;   //weight avl
    float value; //weight of input
    int flag;   //flag for update

}avl_node, *Avl_node;

typedef struct my_avl_tree {
    struct Node *root;
    int size;

} avl_tree, *Avl_tree;

int max(int a, int b);

int weight(struct Node *N);

struct Node *newNode(int key, float value, int flag);

struct Node *rightRotate(struct Node *y);

struct Node *leftRotate(struct Node *x);

int getBalance(struct Node *N);

struct Node *insertNode(struct Node *node, int key, float value, int flag, int *size);

struct Node *minValueNode(struct Node *node);

struct Node *deleteNode(struct Node *root, int key, int *size);

void printPreOrder(struct Node *root);


void avl_set_flag(Avl_tree tree,int key, int flag);
int avl_get_flag(Avl_tree tree, int key);


Avl_tree avl_create();
void avl_insert(Avl_tree tree, int key, float value, int flag);
void avl_remove(Avl_tree tree, int key);
void avl_from_array(Avl_tree root, int *array, float *weights, int size);
int *avl_to_array(Avl_tree tree, int *size, int flag, double sampling_rate, int maxNeighbors);
void avl_destroy(Avl_tree root);
float avl_get_weight(Avl_tree tree, int key);

int *avl_to_whole_array(Avl_tree tree, int *size, float *weights);