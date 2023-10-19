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
    Node array; 
    int size; //given by the user, corresponds to K constant
    int capacity;
    int (*weight_fun)(void* a, void *b);
    void *data_of_interest;//distance of elements from this node are used as weights in the heap  
}heap, *Heap;





//Functions
Heap heap_create(void *data, void *data_of_interest,
 int (*weight_fun)(void* a, void *b), int data_size, int capacity);
void heap_insert(Heap h, void *item);
void *heap_pop(Heap h);
bool heap_update(Heap h, void *item, void *old_root); //upon return old_root contains the old root of the heap, if it was updated, else null
void *heap_find_max(Heap h);
void heap_destroy(Heap h);


//Helper functions
void heapify(Heap h);
void bubble_down(Heap h, int root);
void bubble_up(Heap h, int child);