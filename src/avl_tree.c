// AVL tree implementation in C

#include <stdio.h>
#include <stdlib.h>

#include "../headers/avl_tree.h"


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
struct Node *newNode(int key, float value, int flag) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->weight = 1;
    node->value = value;
    node->flag = flag;
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
struct Node *insertNode(struct Node *node, int key, float value, int flag, int *size) {
    // Find the correct position to insertNode the node and insertNode it
    if (node == NULL){
        *size = *size + 1;
        return (newNode(key, value, flag));
    }

    if (key < node->key)
        node->left = insertNode(node->left, key, value, flag, size);
    else if (key > node->key)
        node->right = insertNode(node->right, key, value, flag, size);
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
struct Node *deleteNode(struct Node *root, int key, int *size) {
    // Find the node and delete it
    if (root == NULL){
        return root;
    }

    if (key < root->key)
        root->left = deleteNode(root->left, key, size);

    else if (key > root->key)
        root->right = deleteNode(root->right, key, size);

    else {
        *size = *size - 1;
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node *temp = root->left ? root->left : root->right;

            if (temp == NULL){
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            struct Node *temp = minValueNode(root->right);

            root->key = temp->key;
            root->value = temp->value;
            root->flag = temp->flag;

            root->right = deleteNode(root->right, temp->key, size);
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
        printf("%d (%f)\n", root->key, root->value);
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}

void printInOrder(struct Node *root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("%d (%f)\n", root->key, root->value);
        printInOrder(root->right);
    }
}

void avl_from_array(Avl_tree tree, int *array, float *weights, int size){
    for(int i = 0; i < size; i++){
        tree->root = insertNode(tree->root, array[i], weights[i], 1, &tree->size);
    }
}


void avl_to_array_helper(Avl_node node, int *array, int *array_index, int flag, int max_samples){
    // if we have reached the max number of samples, return
    if(*array_index >= max_samples){
        return;
    }
    if(node != NULL){
        avl_to_array_helper(node->left, array, array_index, flag, max_samples);
        if(node->flag == flag){
            array[*array_index] = node->key;
            *array_index = *array_index + 1;

            if(flag == true){
                node->flag = false;
            }
        }
        avl_to_array_helper(node->right, array, array_index, flag, max_samples);
    }
}

int *avl_to_array(Avl_tree tree, int *size, int flag, double sampling_rate, int maxNeighbors){
    int *array = malloc(tree->size*sizeof(int));
    int array_index = 0;
    int max_samples = (int) maxNeighbors*sampling_rate;
    avl_to_array_helper(tree->root, array, &array_index, flag, max_samples);
    *size = array_index;
    return array;
}



void avl_to_whole_array_helper(Avl_node node, int *array, int *array_index, float *weights){
    // if we have reached the max number of samples, return
    if(node != NULL){
        avl_to_whole_array_helper(node->left, array, array_index, weights);
        array[*array_index] = node->key;
        weights[*array_index] = node->value;
        *array_index = *array_index + 1;
        avl_to_whole_array_helper(node->right, array, array_index, weights);
    }
}

int *avl_to_whole_array(Avl_tree tree, int *size, float *weights){
    int *array = malloc(tree->size*sizeof(int));
    int array_index = 0;
    avl_to_whole_array_helper(tree->root, array, &array_index, weights);
    *size = array_index;
    return array;
}

void avl_insert(Avl_tree tree, int key, float value, int flag){
    tree->root = insertNode(tree->root, key, value, flag, &tree->size);
}

void avl_remove(Avl_tree tree, int key){
    tree->root = deleteNode(tree->root, key, &tree->size);
}

void avl_destroy_helper(Avl_node tree){
    if(tree != NULL){
        avl_destroy_helper(tree->left);
        avl_destroy_helper(tree->right);
        free(tree);
    }
}

void avl_destroy(Avl_tree tree){
    avl_destroy_helper(tree->root);
    free(tree);
}

Avl_tree avl_create(){
    Avl_tree tree = malloc(sizeof(avl_tree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void avl_set_flag_helper(Avl_node node,int key, int flag){
    if(node != NULL){
        if(node->key == key){
            node->flag = flag;
        }
        else if(key < node->key){
            avl_set_flag_helper(node->left, key, flag);
        }
        else{
            avl_set_flag_helper(node->right, key, flag);
        }
    }
}

void avl_set_flag(Avl_tree tree,int key, int flag){
    avl_set_flag_helper(tree->root, key, flag);
}

int avl_get_flag_helper(Avl_node node, int key){
    if(node != NULL){
        if(node->key == key){
            return node->flag;
        }
        else if(key < node->key){
            return avl_get_flag_helper(node->left, key);
        }
        else{
            return avl_get_flag_helper(node->right, key);
        }
    }
    return -1;
}

int avl_get_flag(Avl_tree tree, int key){
    return avl_get_flag_helper(tree->root, key);
}

float avl_get_weight_helper(Avl_node node, int key){
    if(node != NULL){
        if(node->key == key){
            return node->value;
        }
        else if(key < node->key){
            return avl_get_weight_helper(node->left, key);
        }
        else{
            return avl_get_weight_helper(node->right, key);
        }
    }
    return -1;
}

float avl_get_weight(Avl_tree tree, int key){
    return avl_get_weight_helper(tree->root, key);
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