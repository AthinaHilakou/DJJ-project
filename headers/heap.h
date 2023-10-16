//An implementation of an ADT heap
/*Note: this specific heap implementation is used to represent the K nearest neighbors
of a node in a KNNG, therefore it is *not* scalable, as K
is a constant. According to the definition of a KKNG a heap of this kind contains exactly K elements.*/

//Structures 
typedef struct{
    void *item; //item of unknown data type, general implementation
    int weight; //weight of item in heap, a positive integer
}node ,*Node;

typedef struct{
    Node* items; 
    int size; //given by the user, corresponds to K constant
}heap, *Heap;


//Functions
Heap heap_create(int K, Node* items); //create a heap from node list "items" O(n) 
Node heap_update(Heap h, Node item); //inserts a new item in heap. Old root is popped and returned
Node heap_find_max(Heap h); 
void heap_destroy(Heap h);


//Helper functions
void heapify(Heap h);
void bubble_down(Heap h, int root);
