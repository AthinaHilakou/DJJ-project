// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>

#include "../headers/avl_tree.h"


int max(int a, int b);

// Calculate weight
int weight(struct Node *N) {
    if (N == NULL)
        return 0;
    return N->weight;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Create a node
struct Node *newNode(int key, float value) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->weight = 1;
    node->value = value;
    return (node);
}

// Right rotate
struct Node *rightRotate(struct Node *y) {
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->weight = max(weight(y->left), weight(y->right)) + 1;
    x->weight = max(weight(x->left), weight(x->right)) + 1;

    return x;
}

// Left rotate
struct Node *leftRotate(struct Node *x) {
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->weight = max(weight(x->left), weight(x->right)) + 1;
    y->weight = max(weight(y->left), weight(y->right)) + 1;

    return y;
}

// Get the balance factor
int getBalance(struct Node *N) {
    if (N == NULL)
        return 0;
    return weight(N->left) - weight(N->right);
}

// Insert node
struct Node *insertNode(struct Node *node, int key, float value) {
    // Find the correct position to insertNode the node and insertNode it
    if (node == NULL)
        return (newNode(key, value));

    if (key < node->key)
        node->left = insertNode(node->left, key, value);
    else if (key > node->key)
        node->right = insertNode(node->right, key, value);
    else
        return node;

    // Update the balance factor of each node and
    // Balance the tree
    node->weight = 1 + max(weight(node->left),
                weight(node->right));

    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

struct Node *minValueNode(struct Node *node) {
    struct Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

// Delete a node
struct Node *deleteNode(struct Node *root, int key) {
    // Find the node and delete it
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else {
        if ((root->left == NULL) || (root->right == NULL)) {
        struct Node *temp = root->left ? root->left : root->right;

        if (temp == NULL) {
            temp = root;
            root = NULL;
        } else
            *root = *temp;
        free(temp);
        } else {
        struct Node *temp = minValueNode(root->right);

        root->key = temp->key;

        root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and
    // balance the tree
    root->weight = 1 + max(weight(root->left),
                weight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Print the tree
void printPreOrder(struct Node *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}

void avl_from_array(Avl_tree root, int *array, float *weights, int size){
    for(int i = 0; i < size; i++){
        root = insertNode(root, array[i], weights[i]);
    }
}

int *avl_to_array(Avl_tree tree, int *size){
    int *array = malloc(tree->size*sizeof(int));
    int array_index = 0;
    avl_to_array_helper(tree, array, &array_index);
    *size = tree->size;
    return array;
}

void avl_to_array_helper(Avl_tree tree, int *array, int *array_index){
    if(tree != NULL){
        avl_to_array_helper(tree->left, array, array_index);
        array[*array_index] = tree->key;
        *array_index++;
        avl_to_array_helper(tree->right, array, array_index);
    }
}


void avl_insert(Avl_tree tree, int key, float value){
    tree->root = insertNode(tree->root, key, value);
}

void avl_remove(Avl_tree tree, int key){
    tree->root = deleteNode(tree->root, key);
}



void avl_destroy(Avl_tree tree){
    if(tree != NULL){
        avl_destroy(tree->left);
        avl_destroy(tree->right);
        free(tree);
    }
}


// int main() {
//   struct Node *root = NULL;

//   root = insertNode(root, 2);
//   root = insertNode(root, 1);
//   root = insertNode(root, 7);
//   root = insertNode(root, 4);
//   root = insertNode(root, 5);
//   root = insertNode(root, 3);
//   root = insertNode(root, 8);

//   printPreOrder(root);

//   root = deleteNode(root, 3);

//   printf("\nAfter deletion: ");
//   printPreOrder(root);

//   return 0;
// }